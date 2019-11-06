#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int obtainNewInumber(tecnicofs* fs) {
	int newInumber = ++(fs->nextINumber);
	return newInumber;
}

tecnicofs* new_tecnicofs(int numberBuckets){
	tecnicofs* fs = malloc(sizeof(tecnicofs));
	if (!fs) {
		perror("failed to allocate tecnicofs");
		exit(EXIT_FAILURE);
	}
	fs->buckets = numberBuckets;
	fs->bstRoots = (node**) malloc(sizeof(node*) * numberBuckets);
	fs->bstLocks = (LOCK_TYPE*) malloc(sizeof(LOCK_TYPE) * numberBuckets);
	for (int i = 0; i < numberBuckets; i++) 	{
		fs->bstRoots[i] = NULL;
		sync_init(&(fs->bstLocks[i]));
	}
	fs->nextINumber = 0;
 	return fs;
}

void free_tecnicofs(tecnicofs* fs){
	for (int i = 0; i < fs->buckets; i++) 	{
		free_tree(fs->bstRoots[i]);
		sync_destroy(&(fs->bstLocks[i]));
	}
	free(fs->bstRoots);
	free(fs->bstLocks);
	free(fs);
}

void create(tecnicofs* fs, char *name, int inumber){
	int index = hash(name, fs->buckets);
	sync_wrlock(&(fs->bstLocks[index]));
	fs->bstRoots[index] = insert(fs->bstRoots[index], name, inumber);
	sync_unlock(&(fs->bstLocks[index]));
}

void delete(tecnicofs* fs, char *name){
	int index = hash(name, fs->buckets);
	sync_wrlock(&(fs->bstLocks[index]));
	fs->bstRoots[index] = remove_item(fs->bstRoots[index], name);
	sync_unlock(&(fs->bstLocks[index]));
}

int lookup(tecnicofs* fs, char *name){
	int index = hash(name, fs->buckets);
	sync_rdlock(&(fs->bstLocks[index]));
	int inumber = 0;
	node* searchNode = search(fs->bstRoots[index], name);
	if ( searchNode ) {
		inumber = searchNode->inumber;
	}
	sync_unlock(&(fs->bstLocks[index]));
	return inumber;
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	for (size_t i = 0; i < fs->buckets; i++) 	{
		print_tree(fp, fs->bstRoots[i]);
	}
}
