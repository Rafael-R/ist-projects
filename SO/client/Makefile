# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCES = main.c test.c tecnicofs-client-api.c
OBJS = $(SOURCES:%.c=%.o)
CC = gcc
LD = gcc
CFLAGS = -Wall -std=gnu99 -I../ -g
TARGET = client
TESTS = create delete read success

.PHONY: all clean

all: $(TARGET) $(TESTS)

$(TARGET):
	$(LD) $(CFLAGS) $^ -o $@


tecnicofs-client-api.o: tecnicofs-client-api.c tecnicofs-client-api.h

main.o: main.c

client: tecnicofs-client-api.o main.o

create:
	$(CC) $(CFLAGS) tecnicofs-client-api.c api-tests/client-api-test-create.c -o $@

delete:
	$(CC) $(CFLAGS) tecnicofs-client-api.c api-tests/client-api-test-delete.c -o $@

read:
	$(CC) $(CFLAGS) tecnicofs-client-api.c api-tests/client-api-test-read.c -o $@

success:
	$(CC) $(CFLAGS) tecnicofs-client-api.c api-tests/client-api-test-success.c -o $@


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) $(TESTS)
