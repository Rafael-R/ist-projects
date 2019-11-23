# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCES = main.c test.c tecnicofs-client-api.c
OBJS = $(SOURCES:%.c=%.o)
CC = gcc
LD = gcc
CFLAGS = -Wall -std=gnu99 -I../ -g
TARGETS = client test

.PHONY: all clean

all: $(TARGETS)

$(TARGETS):
	$(LD) $(CFLAGS) $^ -o $@


tecnicofs-client-api.o: tecnicofs-client-api.c tecnicofs-client-api.h ../tecnicofs-api-common.h

main.o: main.c ../tecnicofs-api-common.h

test.o: test.c tecnicofs-client-api.o

client: ../tecnicofs-api-common.o tecnicofs-client-api.o main.o

test: ../tecnicofs-api-common.o tecnicofs-client-api.o test.o


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGETS)
