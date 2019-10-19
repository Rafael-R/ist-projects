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
	fs->bstRoots = malloc(sizeof(node) * numberBuckets);
	fs->buckets = numberBuckets;
	for (size_t i = 0; i < fs->buckets; i++) 	{
		fs->bstRoots[i] = NULL;
	}
	fs->nextINumber = 0;
 	return fs;
}

void free_tecnicofs(tecnicofs* fs){
	for (size_t i = 0; i < fs->buckets; i++) 	{
		free_tree(fs->bstRoots[i]);
	}
	free(fs);
}

void create(tecnicofs* fs, char *name, int inumber){
	int index = hash(name, fs->buckets);
	fs->bstRoots[index] = insert(fs->bstRoots[index], name, inumber);
}

void delete(tecnicofs* fs, char *name){
	int index = hash(name, fs->buckets);
	fs->bstRoots[index] = remove_item(fs->bstRoots[index], name);
}

int lookup(tecnicofs* fs, char *name){
	int index = hash(name, fs->buckets);
	node* searchNode = search(fs->bstRoots[index], name);
	if ( searchNode ) return searchNode->inumber;
	return 0;
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	for (size_t i = 0; i < fs->buckets; i++) 	{
		print_tree(fp, fs->bstRoots[i]);
	}
}
