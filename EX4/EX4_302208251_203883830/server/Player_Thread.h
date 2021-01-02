#include <winsock2.h>
#include <Windows.h>

typedef struct Player_Thread_Params {
	SOCKET socket;
	int player;
	HANDLE socket_mutex;
}Player_Thread_Params;

DWORD WINAPI player_thread(LPVOID lpParam);