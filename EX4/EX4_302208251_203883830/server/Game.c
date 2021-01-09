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

GameSession* create_game_session() {
	GameSession* game_session = (GameSession*)malloc(sizeof(GameSession));
	game_session->session_mutex = CreateMutex(NULL, FALSE, NULL);
	game_session->active_players = 0;
	game_session->turn_ended = 0;
	return game_session;
}

GameSession* destroy_game_session(GameSession* game_session) {
	CloseHandle(game_session->session_mutex);
	CloseHandle(game_session->session_file);
	free(game_session);
	return NULL;
}

BOOL open_session_file(GameSession* game_session) {
	game_session->session_file = CreateFileA(GAME_SESSION_FILE_NAME,
		(GENERIC_READ | GENERIC_WRITE),
		(FILE_SHARE_READ | FILE_SHARE_WRITE),
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (game_session->session_file == INVALID_HANDLE_VALUE) {
		printf("file exists\n");
		game_session->session_file = CreateFileA(GAME_SESSION_FILE_NAME,
			(GENERIC_READ | GENERIC_WRITE),
			(FILE_SHARE_READ | FILE_SHARE_WRITE),
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		return TRUE;
	}
	if (game_session->session_file == INVALID_HANDLE_VALUE) {
		printf("failed to open game session file, error code: %d\n",GetLastError());
		return FALSE;
	}
	else {
		return TRUE;
	}
}

