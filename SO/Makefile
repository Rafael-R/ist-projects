# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCE = tecnicofs-api-common.c
OBJ = $(SOURCE:%.c=%.o)
CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -g
LDFLAGS=-lm -pthread
TARGETS= client server

.PHONY: all clean

all: tecnicofs-api-common.o
	(cd client; make all)
	(cd server; make all)

tecnicofs-api-common.o: tecnicofs-api-common.c tecnicofs-api-common.h

%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)
	(cd client; make clean)
	(cd server; make clean)