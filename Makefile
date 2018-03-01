all: master player

master: message.h 8characters_master.c
	gcc 8characters_master.c -o master

player: message.h 8characters_player.c
	gcc 8characters_player.c -o player