# Makefile, exercício 1
# Projeto de Sistemas Operativos 2019/2020

CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../
LDFLAGS=-lm


all: tecnicofs

tecnicofs: lib/bst.o fs.o main.o
	$(LD) $(CFLAGS) $(LDFLAGS) lib/bst.o fs.o main-nosync.o -o tecnicofs-nosync -lpthread
	$(LD) $(CFLAGS) $(LDFLAGS) lib/bst.o fs.o main-mutex.o  -o tecnicofs-mutex  -lpthread
	$(LD) $(CFLAGS) $(LDFLAGS) lib/bst.o fs.o main-rwlock.o -o tecnicofs-rwlock -lpthread

main.o: main.c fs.h lib/bst.h
	$(CC) $(CFLAGS)          -c main.c -o main-nosync.o
	$(CC) $(CFLAGS) -DMUTEX  -c main.c -o main-mutex.o
	$(CC) $(CFLAGS) -DRWLOCK -c main.c -o main-rwlock.o

fs.o: fs.c fs.h lib/bst.h
	$(CC) $(CFLAGS) -o fs.o -c fs.c

lib/bst.o: lib/bst.c lib/bst.h
	$(CC) $(CFLAGS) -o lib/bst.o -c lib/bst.c

clean:
	@echo Cleaning...
	rm -f lib/*.o *.o tecnicofs-nosync tecnicofs-mutex tecnicofs-rwlock
