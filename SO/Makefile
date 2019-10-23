# Makefile
# Projeto de Sistemas Operativos 2019/2020

CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../
LDFLAGS=-lm


all: tecnicofs

tecnicofs: main fs lib/bst.o lib/hash.o
	$(LD) $(CFLAGS) $(LDFLAGS) lib/bst.o lib/hash.o fs.o main-nosync.o -o tecnicofs-nosync -lpthread
	$(LD) $(CFLAGS) $(LDFLAGS) lib/bst.o lib/hash.o fs.o main-mutex.o  -o tecnicofs-mutex  -lpthread
	$(LD) $(CFLAGS) $(LDFLAGS) lib/bst.o lib/hash.o fs.o main-rwlock.o -o tecnicofs-rwlock -lpthread

main: main.c fs lib
	$(CC) $(CFLAGS)          -c main.c -o main-nosync.o
	$(CC) $(CFLAGS) -DMUTEX  -c main.c -o main-mutex.o
	$(CC) $(CFLAGS) -DRWLOCK -c main.c -o main-rwlock.o

fs: fs.c fs.h lib
	$(CC) $(CFLAGS) -o fs.o -c fs.c

lib: lib/bst.c lib/bst.h lib/hash.c lib/hash.h
	$(CC) $(CFLAGS) -o lib/bst.o  -c lib/bst.c
	$(CC) $(CFLAGS) -o lib/hash.o -c lib/hash.c

clean:
	@echo Cleaning...
	rm -f lib/*.o *.o tecnicofs-nosync tecnicofs-mutex tecnicofs-rwlock
