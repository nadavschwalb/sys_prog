#pragma once
//#include <winsock2.h>
#include <Windows.h>
#include "Game.h"
typedef struct Player_Thread_Params {
	SOCKET socket;
	int player_number;
	HANDLE socket_mutex;
	GameSession* game_session;
}Player_Thread_Params;

DWORD WINAPI player_thread(LPVOID lpParam);