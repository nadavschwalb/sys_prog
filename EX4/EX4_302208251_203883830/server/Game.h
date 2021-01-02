#pragma once
#include <Windows.h>
#include <stdlib.h>

typedef struct Player {
	char* username;
	int combo;
	int move;
	BOOL ready_to_play;
}Player;

Player* create_player();
BOOL destroy_player(Player* player);