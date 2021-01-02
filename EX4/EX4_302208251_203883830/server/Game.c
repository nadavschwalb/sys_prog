#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "Messages.h"
#include "Hard_Coded_Data.h"

Player* create_player() {
	Player* player = (Player*)malloc(sizeof(Player));
	player->username = (char*)malloc(sizeof(char)*DEFAULT_BUFLEN);
	return player;
}

BOOL destroy_player(Player* player) {
	free(player->username);
	free(player);
	return TRUE;
}