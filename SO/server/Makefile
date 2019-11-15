# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCES = main.c fs.c sync.c lib/bst.c lib/hash.c
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -g
LDFLAGS=-lm -pthread
TARGET = tecnicofs-rwlock

.PHONY: all clean

all: $(TARGET)

$(TARGET):
	$(LD) $(CFLAGS) $^ -o $@ $(LDFLAGS)


### RWLOCK ###
lib/bst.o: CFLAGS+=-DRWLOCK
lib/bst.o: lib/bst.c lib/bst.h

lib/hash.o: CFLAGS+=-DRWLOCK
lib/hash.o: lib/hash.c lib/hash.h

fs.o: CFLAGS+=-DRWLOCK
fs.o: fs.c fs.h lib/bst.h lib/hash.h

sync.o: CFLAGS+=-DRWLOCK
sync.o: sync.c sync.h

main.o: CFLAGS+=-DRWLOCK
main.o: main.c ../tecnicofs-api-common.h fs.h lib/bst.h lib/hash.h sync.h

tecnicofs-rwlock: lib/bst.o lib/hash.o fs.o sync.o ../tecnicofs-api-common.o main.o


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET)
