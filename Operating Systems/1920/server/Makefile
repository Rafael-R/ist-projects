# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCES = main.c fs.c sync.c lib/bst.c lib/inodes.c
OBJS = $(SOURCES:%.c=%.o)
CC = gcc
LD = gcc
CFLAGS = -Wall -std=gnu99 -I../ -g
LDFLAGS = -lm -pthread
TARGET = tecnicofs-rwlock

.PHONY: all clean

all: $(TARGET)

$(TARGET):
	$(LD) $(CFLAGS) $^ -o $@ $(LDFLAGS)


### RWLOCK ###
lib/bst.o: CFLAGS+=-DRWLOCK
lib/bst.o: lib/bst.c lib/bst.h

lib/inodes.o: CFLAGS+=-DRWLOCK
lib/inodes.o: lib/inodes.c lib/inodes.h ../tecnicofs-api-constants.h

sync.o: CFLAGS+=-DRWLOCK
sync.o: sync.c sync.h

fs.o: CFLAGS+=-DRWLOCK
fs.o: fs.c fs.h lib/bst.h lib/inodes.h sync.h

main.o: CFLAGS+=-DRWLOCK
main.o: main.c fs.h sync.h

tecnicofs-rwlock: lib/bst.o lib/inodes.o sync.o fs.o main.o


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET)
