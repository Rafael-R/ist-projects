#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


tecnicofs* newTecnicoFS(){
	tecnicofs* fs = malloc(sizeof(tecnicofs));
	if (!fs) {
		perror("failed to allocate tecnicofs");
		exit(EXIT_FAILURE);
	}
	fs->bstRoot = NULL;
	sync_init(&(fs->bstLock));
	sync_init(&(fs->openFilesLock));
	inode_table_init();
	fs->nextINumber = -1;
 	return fs;
}

void freeTecnicoFS(tecnicofs* fs){
	free_tree(fs->bstRoot);
	sync_destroy(&(fs->bstLock));
	sync_destroy(&(fs->openFilesLock));
	inode_table_destroy();
	free(fs);
}

int lookupFile(tecnicofs* fs, char *name){
	sync_rdlock(&(fs->bstLock));
	int iNumber = -1;
	node* searchNode = search(fs->bstRoot, name);
	if (searchNode) {
		iNumber = searchNode->inumber;
	}
	sync_unlock(&(fs->bstLock));
	return iNumber;
}

int createFile(tecnicofs* fs, uid_t client, char *name, int permissions) {
	int iNumber = lookupFile(fs, name);
	if (iNumber == -1) {
		sync_wrlock(&(fs->bstLock));
		permission ownerPerm = permissions / 10;
		permission othersPerm = permissions % 10;
		iNumber = inode_create(client, ownerPerm, othersPerm);
		fs->bstRoot = insert(fs->bstRoot, name, iNumber);
		sync_unlock(&(fs->bstLock));
		return 0;
	} else { return TECNICOFS_ERROR_FILE_ALREADY_EXISTS; }
}

int deleteFile(tecnicofs* fs, uid_t client, char *name) {
	int iNumber = lookupFile(fs, name);
	if (iNumber != -1) {
		sync_rdlock(&(fs->openFilesLock));
		if (fs->openFiles[iNumber] == 0) {
			uid_t owner;
			inode_get(iNumber, &owner, NULL, NULL, NULL, 0);
			if (client == owner) {
				sync_wrlock(&(fs->bstLock));
				fs->bstRoot = remove_item(fs->bstRoot, name);
				inode_delete(iNumber);
				sync_unlock(&(fs->bstLock));
				sync_unlock(&(fs->openFilesLock));
				return 0;
			} else {
				sync_unlock(&(fs->openFilesLock));
				return TECNICOFS_ERROR_PERMISSION_DENIED; }
		} else {
			sync_unlock(&(fs->openFilesLock));
			return TECNICOFS_ERROR_FILE_IS_OPEN;
		}
	} else { return TECNICOFS_ERROR_FILE_NOT_FOUND; }
}

int renameFile(tecnicofs* fs, uid_t client, char *oldName, char *newName) {
	int oldINumber = lookupFile(fs, oldName);
	if (oldINumber != -1) {
		uid_t owner;
		inode_get(oldINumber, &owner, NULL, NULL, NULL, 0);
		if (client == owner) {
			int newINumber = lookupFile(fs, newName);
			if (newINumber == -1) {
				sync_wrlock(&(fs->bstLock));
				fs->bstRoot = remove_item(fs->bstRoot, oldName);
				fs->bstRoot = insert(fs->bstRoot, newName, oldINumber);
				sync_unlock(&(fs->bstLock));
				return 0;
			} else { return TECNICOFS_ERROR_FILE_ALREADY_EXISTS; }
		} else { return TECNICOFS_ERROR_PERMISSION_DENIED; }
	} else { return TECNICOFS_ERROR_FILE_NOT_FOUND; }
}

int openFile(tecnicofs* fs, tempfile_t files[], uid_t client, char *name, int mode) {
	int iNumber = lookupFile(fs, name);
	if (iNumber != -1) {
		int fd = -1;
		for (int i = MAX_OPEN_FILES; i > -1; --i) {
			if (files[i].iNumber == -1) {
				fd = i;
			}
		}
		if (fd != -1) {
			uid_t owner;
			permission ownerPerm, othersPerm;
			inode_get(iNumber, &owner, &ownerPerm, &othersPerm, NULL, 0);
			if ((client == owner && ownerPerm == RW && mode > NONE) ||
				(client == owner && ownerPerm == mode) ||
				(client != owner && othersPerm == RW && mode > NONE) ||
				(client != owner && othersPerm == mode)) {
				files[fd].iNumber = iNumber;
				files[fd].mode = mode;
				sync_wrlock(&(fs->openFilesLock));
				fs->openFiles[iNumber]++;
				sync_unlock(&(fs->openFilesLock));
				return fd;
			} else { return TECNICOFS_ERROR_PERMISSION_DENIED; }
		} else { return TECNICOFS_ERROR_MAXED_OPEN_FILES; }
	} else { return TECNICOFS_ERROR_FILE_NOT_FOUND; }
}

int closeFile(tecnicofs* fs, tempfile_t files[], int fd) {
	if (files[fd].iNumber != -1) {
		sync_wrlock(&(fs->openFilesLock));
		fs->openFiles[files[fd].iNumber]--;
		sync_unlock(&(fs->openFilesLock));
		files[fd].iNumber = -1;
		files[fd].mode = NONE;
		return 0;
	} else { return TECNICOFS_ERROR_FILE_NOT_OPEN; }
}

int readFile(tempfile_t files[], int fd, char* buffer, int len) {
	if (files[fd].iNumber != -1) {
		if (files[fd].mode == RW || files[fd].mode == READ) {
			int status = inode_get(files[fd].iNumber, NULL, NULL, NULL, buffer, len);
			if (status != -1) {
				return status;
			} else { return TECNICOFS_ERROR_OTHER; }
		} else { return TECNICOFS_ERROR_INVALID_MODE; }
	} else { return TECNICOFS_ERROR_FILE_NOT_OPEN; }
}

int writeFile(tempfile_t files[], int fd, char* buffer) {
	if (files[fd].iNumber != -1) {
		if (files[fd].mode == RW || files[fd].mode == WRITE) {
			int status = inode_set(files[fd].iNumber, buffer, strlen(buffer));
			if (status != -1) {
				return status;
			} else { return TECNICOFS_ERROR_OTHER; }
		} else { return TECNICOFS_ERROR_INVALID_MODE; }
	} else { return TECNICOFS_ERROR_FILE_NOT_OPEN; }
}

void printTecnicoFS(FILE * fp, tecnicofs *fs){
	print_tree(fp, fs->bstRoot);
}
