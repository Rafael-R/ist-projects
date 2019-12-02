# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCES = main.c test.c tecnicofs-client-api.c
OBJS = $(SOURCES:%.c=%.o)
CC = gcc
LD = gcc
CFLAGS = -Wall -std=gnu99 -I../ -g
TARGET = client
TESTS = test-create test-delete test-read test-success

.PHONY: all clean

all: $(TARGET) $(TESTS)

$(TARGET):
	$(LD) $(CFLAGS) $^ -o $@

$(TESTS):
	$(CC) $(CFLAGS) $^ -o $@


tecnicofs-client-api.o: tecnicofs-client-api.c tecnicofs-client-api.h

main.o: main.c

client: tecnicofs-client-api.o main.o

test-create: tecnicofs-client-api.c api-tests/client-api-test-create.c

test-delete: tecnicofs-client-api.c api-tests/client-api-test-delete.c

test-read: tecnicofs-client-api.c api-tests/client-api-test-read.c

test-success: tecnicofs-client-api.c api-tests/client-api-test-success.c


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET) $(TESTS)
