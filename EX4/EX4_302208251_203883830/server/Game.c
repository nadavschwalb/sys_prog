#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "Messages.h"
#include "Hard_Coded_Data.h"
#include "Player_Thread.h"

int calc_cows(GameSession* game_session, int self, int other);
int calc_bulls(GameSession* game_session, int self, int other);

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
	game_session->winners[0] = FALSE;
	game_session->winners[1] = FALSE;
	game_session->play_events[0] = CreateEventA(NULL, TRUE, FALSE, NULL);
	game_session->play_events[1] = CreateEventA(NULL, TRUE, FALSE, NULL);
	return game_session;
}

GameSession* destroy_game_session(GameSession* game_session) {
	CloseHandle(game_session->session_mutex);
	CloseHandle(game_session->session_file);
	CloseHandle(game_session->play_events[0]);
	CloseHandle(game_session->play_events[1]);
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

int play_move(GameSession* game_session, int self) {
	
	int other = self ^ 1;
	game_session->player_array[self]->bulls = calc_bulls(game_session, self, other);
	game_session->player_array[self]->cows = calc_cows(game_session, self, other);
	if (game_session->player_array[self]->bulls == 4) {
		game_session->winners[self] = TRUE;
	}
}

int calc_cows(GameSession* game_session,int self, int other) {
	int cows = 0;
	for (int i = 0; i < 4; i++) {
		if (strchr(game_session->player_array[other]->combo,
			game_session->player_array[self]->move[i]) != NULL 
			&& 
			game_session->player_array[self]->move[i] != 
			game_session->player_array[other]->combo[i]) 
			cows++;
	}
	return cows;
}

int calc_bulls(GameSession* game_session, int self, int other) {
	int bulls = 0;
	for (int i = 0; i < 4; i++) {
		if (game_session->player_array[self]->move[i] == 
			game_session->player_array[other]->combo[i]) 
			bulls++;
	}
	return bulls;
}
