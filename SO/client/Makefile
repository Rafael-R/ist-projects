# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCE = ../tecnicofs-api-common.c
OBJ = $(SOURCE:%.c=%.o)
CC   = gcc
LD   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -g
LDFLAGS=-lm -pthread

.PHONY: all clean

all: client

client: $(OBJ)
	$(LD) $(CFLAGS) $(SOURCE) main.c -o client

clean:
	rm -f client

