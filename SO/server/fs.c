#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int obtainNewINumber(tecnicofs* fs) {
	int newINumber = ++(fs->nextINumber);
	return newINumber;
}

tecnicofs* newTecnicoFS(){
	tecnicofs* fs = malloc(sizeof(tecnicofs));
	if (!fs) {
		perror("failed to allocate tecnicofs");
		exit(EXIT_FAILURE);
	}
	fs->bstRoot = NULL;
	sync_init(&(fs->bstLock));
	inode_table_init();
	fs->nextINumber = -1;
 	return fs;
}

void freeTecnicoFS(tecnicofs* fs){
	free_tree(fs->bstRoot);
	sync_destroy(&(fs->bstLock));
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
	int status = lookupFile(fs, name);
	if (status != -1) {
		return TECNICOFS_ERROR_FILE_ALREADY_EXISTS;
	} else {
		sync_wrlock(&(fs->bstLock));
		permission ownerPerm = permissions / 10;
		permission othersPerm = permissions % 10;
		fs->bstRoot = insert(fs->bstRoot, name, obtainNewINumber(fs));
		inode_create(client, ownerPerm, othersPerm);
		sync_unlock(&(fs->bstLock));
		return 0;
	}
}

int deleteFile(tecnicofs* fs, uid_t client, char *name) {
	int iNumber = lookupFile(fs, name);
	if (iNumber == -1) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		uid_t owner;
		inode_get(iNumber, &owner, NULL, NULL, NULL, 0); //TODO ?
		if (client != owner) {
			return TECNICOFS_ERROR_PERMISSION_DENIED;
		} else {
			sync_wrlock(&(fs->bstLock));
			fs->bstRoot = remove_item(fs->bstRoot, name);
			inode_delete(iNumber);
			sync_unlock(&(fs->bstLock));
			return 0;
		}
	}
}

int renameFile(tecnicofs* fs, uid_t client, char *oldName, char *newName) {
	int oldINumber = lookupFile(fs, oldName);
	if (oldINumber == -1) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		uid_t owner;
		inode_get(oldINumber, &owner, NULL, NULL, NULL, 0);
		if (owner != client) {
			return TECNICOFS_ERROR_PERMISSION_DENIED;
		} else {
			int newINumber = lookupFile(fs, newName);
			if (newINumber != -1) {
				return TECNICOFS_ERROR_FILE_ALREADY_EXISTS;
			} else {
				sync_wrlock(&(fs->bstLock));
				fs->bstRoot = remove_item(fs->bstRoot, oldName);
				fs->bstRoot = insert(fs->bstRoot, newName, oldINumber);
				sync_unlock(&(fs->bstLock));
				return 0;
			}
		}
	}
}

int openFile(tecnicofs* fs, uid_t client, char *name, int mode, tempfile_t files[]) {
	int iNumber = lookupFile(fs, name);
	if (iNumber == -1) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		int fd = -1;
		for (int i = MAX_OPEN_FILES; i > -1; --i) {
			if (files[i].iNumber == -1) {
				fd = i;
			}
		}
		if (fd == -1) {
			return TECNICOFS_ERROR_MAXED_OPEN_FILES;
		} else {
			uid_t owner;
			permission ownerPerm, othersPerm;
			inode_get(iNumber, &owner, &ownerPerm, &othersPerm, NULL, 0);
			if ((client == owner && mode == ownerPerm) ||
					(client != owner && mode == othersPerm)) {
				files[fd].iNumber = iNumber;
				files[fd].mode = mode;
				return fd;
			} else {
				return TECNICOFS_ERROR_PERMISSION_DENIED;
			}
		}
	}
}

int closeFile(tempfile_t files[], int fd) {
	if (files[fd].iNumber != -1) {
		files[fd].iNumber = -1;
		files[fd].mode = NONE;
		return 0;
	} else {
		return TECNICOFS_ERROR_FILE_NOT_OPEN;
	}
}

int readFile(tempfile_t files[], int fd, char* buffer, int len) {
	if (files[fd].iNumber) {
		return TECNICOFS_ERROR_FILE_NOT_OPEN;
	} else if (files[fd].mode != READ && files[fd].mode != RW) {
		return TECNICOFS_ERROR_INVALID_MODE;
	} else {
		int status = inode_get(files[fd].iNumber, NULL, NULL, NULL, buffer, len);
		if (status == -1) {
			return TECNICOFS_ERROR_OTHER;
		} else {
			return status;
		}
	}
}

int writeFile(tempfile_t files[], int fd, char* buffer) {
	if (files[fd].iNumber) {
		return TECNICOFS_ERROR_FILE_NOT_OPEN;
	} else if (files[fd].mode != WRITE && files[fd].mode != RW) {
		return TECNICOFS_ERROR_INVALID_MODE;
	} else {
		int status = inode_set(files[fd].iNumber, buffer, strlen(buffer));
		if (status == -1) {
			return TECNICOFS_ERROR_OTHER;
		} else {
			return status;
		}
	}
}

void printTecnicoFS(FILE * fp, tecnicofs *fs){
	print_tree(fp, fs->bstRoot);
}
