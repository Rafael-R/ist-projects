# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2019-20

CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../
LDFLAGS=-lm

# A phony target is one that is not really the name of a file
# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY: all clean run

all: tecnicofs

tecnicofs: lib/bst.o fs.o main.o
	$(LD) $(CFLAGS) $(LDFLAGS)          lib/bst.o fs.o main-nosync.o -o tecnicofs-nosync -lpthread
	$(LD) $(CFLAGS) $(LDFLAGS) -DMUTEX  lib/bst.o fs.o main-mutex.o  -o tecnicofs-mutex  -lpthread
	$(LD) $(CFLAGS) $(LDFLAGS) -DRWLOCK lib/bst.o fs.o main-rwlock.o -o tecnicofs-rwlock -lpthread

lib/bst.o: lib/bst.c lib/bst.h
	$(CC) $(CFLAGS) -o lib/bst.o -c lib/bst.c

fs.o: fs.c fs.h lib/bst.h
	$(CC) $(CFLAGS) -o fs.o -c fs.c

main.o: main.c fs.h lib/bst.h
	$(CC) $(CFLAGS)          -c main.c -o main-nosync.o
	$(CC) $(CFLAGS) -DMUTEX  -c main.c -o main-mutex.o
	$(CC) $(CFLAGS) -DRWLOCK -c main.c -o main-rwlock.o

clean:
	@echo Cleaning...
	rm -f lib/*.o *.o tecnicofs-nosync tecnicofs-mutex tecnicofs-rwlock
