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
	fs->nextINumber = 0;
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
	int inumber = 0;
	node* searchNode = search(fs->bstRoot, name);
	if ( searchNode ) {
		inumber = searchNode->inumber;
	}
	sync_unlock(&(fs->bstLock));
	return inumber;
}

int create_file(tecnicofs* fs, uid_t client, char *name, int inumber, int permissions) {
	int status = lookup_file(fs, name);
	if (status != 0) {
		return TECNICOFS_ERROR_FILE_ALREADY_EXISTS;
	} else {
		sync_wrlock(&(fs->bstLock));
		permission owner = permissions % 10;
		permission others = permissions / 10;
		fs->bstRoot = insert(fs->bstRoot, name, inumber);
		inode_create(client, owner, others);
		sync_unlock(&(fs->bstLock));
		return 0;
	}
}

int delete_file(tecnicofs* fs, uid_t client, char *name) {
	int iNumber = lookup_file(fs, name);
	if (iNumber != 0) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		uid_t owner;
		inode_get(iNumber, &owner, NULL, NULL, NULL, 0);
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
	if (oldINumber == 0) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		uid_t owner;
		inode_get(oldINumber, &owner, NULL, NULL, NULL, 0);
		if (owner != client) {
			return TECNICOFS_ERROR_PERMISSION_DENIED;
		} else {
			int newINumber = lookup_file(fs, newName);
			if (newINumber != 0) {
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

int open_file(tecnicofs* fs, uid_t client, char *name, int mode, temp_file_t* temp) {
	int iNumber = lookup_file(fs, name);
	if (iNumber != 0) {
		return TECNICOFS_ERROR_FILE_NOT_FOUND;
	} else {
		uid_t owner;
		permission ownerPerm, othersPerm;
		inode_get(iNumber, &owner, &ownerPerm, &othersPerm, &(temp->fileContent), 100);
		if ((owner == client && mode != ownerPerm) || (mode != othersPerm)) {
			return TECNICOFS_ERROR_PERMISSION_DENIED;
		} else {
			temp->mode = mode;
			return 0;
		}
	}
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	print_tree(fp, fs->bstRoot);
}
