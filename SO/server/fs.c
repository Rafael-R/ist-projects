#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int obtainNewINumber(tecnicofs* fs) {
	int newINumber = ++(fs->nextINumber);
	return newINumber;
}

tecnicofs* new_tecnicofs(){
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

void free_tecnicofs(tecnicofs* fs){
	free_tree(fs->bstRoot);
	sync_destroy(&(fs->bstLock));
	inode_table_destroy();
	free(fs);
}

int lookup_file(tecnicofs* fs, char *name){
	sync_rdlock(&(fs->bstLock));
	int iNumber = -1;
	node* searchNode = search(fs->bstRoot, name);
	if (searchNode) {
		iNumber = searchNode->inumber;
	}
	sync_unlock(&(fs->bstLock));
	return iNumber;
}

int create_file(tecnicofs* fs, uid_t client, char *name, int iNumber, int permissions) {
	int status = lookup_file(fs, name);
	if (status != -1) {
		return TECNICOFS_ERROR_FILE_ALREADY_EXISTS;
	} else {
		sync_wrlock(&(fs->bstLock));
		permission ownerPerm = permissions / 10;
		permission othersPerm = permissions % 10;
		fs->bstRoot = insert(fs->bstRoot, name, iNumber);
		inode_create(client, ownerPerm, othersPerm);
		sync_unlock(&(fs->bstLock));
		return 0;
	}
}

int delete_file(tecnicofs* fs, uid_t client, char *name) {
	int iNumber = lookup_file(fs, name);
	if (iNumber == -1) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		uid_t owner;
		inode_get(iNumber, &owner, NULL, NULL, NULL, 0); //TODO ?
		if (owner != client) {
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

int rename_file(tecnicofs* fs, uid_t client, char *oldName, char *newName) {
	int oldINumber = lookup_file(fs, oldName);
	if (oldINumber == -1) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		uid_t owner;
		inode_get(oldINumber, &owner, NULL, NULL, NULL, 0);
		if (owner != client) {
			return TECNICOFS_ERROR_PERMISSION_DENIED;
		} else {
			int newINumber = lookup_file(fs, newName);
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

int open_file(tecnicofs* fs, uid_t client, char *name, int mode, tempfile_t files[]) {
	int iNumber = lookup_file(fs, name);
	if (iNumber == -1) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		int fd = -1;
		for (int i = 0; i < MAX_OPEN_FILES; i++) {
			if (files[i].iNumber == 0) {
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

int close_file(tempfile_t files[], int fd) {
	if (files[fd].iNumber) {
		files[fd].iNumber = -1;
		files[fd].mode = NONE;
		return 0;
	} else {
		return TECNICOFS_ERROR_FILE_NOT_OPEN;
	}
}

int read_file(tempfile_t files[], int fd, char* buffer, int len) {
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

int write_file(tempfile_t files[], int fd, char* buffer) {
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

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	print_tree(fp, fs->bstRoot);
}
