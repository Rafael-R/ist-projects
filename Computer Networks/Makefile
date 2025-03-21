# Makefile

CC=g++
DEPS = aux/networking.h aux/auxiliar.h

all: GS player

GS: server/GS.cpp server/game_protocol.h $(DEPS)
	$(CC) server/GS.cpp     -o GS

player: client/player.cpp client/commands.h $(DEPS)
	$(CC) client/player.cpp -o player

clean: 
	rm GS player client/data/*
