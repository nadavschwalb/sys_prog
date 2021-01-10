#pragma once
#include <Windows.h>
#include <stdlib.h>
#define GAME_SESSION_FILE_NAME "GameSession.txt"


typedef struct Player {
	char* username;
	char combo[5];
	char move[5];
	int bulls;
	int cows;
}Player;

typedef struct GameSession {
	HANDLE session_file;
	HANDLE session_mutex;
	int active_players;
	char turn_ended;
	Player* player_array[2];
	HANDLE  play_events[2];
	BOOL winners[2];

}GameSession;

Player* create_player();
BOOL destroy_player(Player* player);
GameSession* create_game_session();
GameSession* destroy_game_session(GameSession* game_session);
BOOL open_session_file(GameSession* game_session);
int play_move(GameSession* game_session, int self);