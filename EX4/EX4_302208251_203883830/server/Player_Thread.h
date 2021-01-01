#include <winsock2.h>

typedef struct Player_Thread_Params {
	SOCKET socket;
	int player;
}Player_Thread_Params;

DWORD WINAPI player_thread(LPVOID lpParam);