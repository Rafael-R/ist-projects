# Makefile
# Projeto de Sistemas Operativos 2019/2020

SOURCES = main.c tecnicofs-client-api.c
OBJS = $(SOURCES:%.c=%.o)
CC = gcc
LD = gcc
CFLAGS = -Wall -std=gnu99 -I../ -g
TARGET = client

.PHONY: all clean

all: $(TARGET)

$(TARGET):
	$(LD) $(CFLAGS) $^ -o $@


tecnicofs-client-api.o: tecnicofs-client-api.c tecnicofs-client-api.h ../tecnicofs-api-common.h

main.o: main.c tecnicofs-client-api.h

client: ../tecnicofs-api-common.o tecnicofs-client-api.o main.o


%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

