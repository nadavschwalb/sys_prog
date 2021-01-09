#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include "Player_Thread.h"
#include "Hard_Coded_Data.h"
#include "Game.h"
#include "Messages.h"
#pragma comment(lib, "Ws2_32.lib")

DWORD WINAPI player_thread(LPVOID lpParam) {
	Player_Thread_Params* params = (Player_Thread_Params*)lpParam;
	printf("player %d thread started\n", params->player_number);
	int iresult = 0;
	int iSendResult = 0;
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	BOOL quit = FALSE;
	

	if (params->player_number > 1) {
		printf("too many players\n");
		const char* denial_response = "SERVER_DENIED\n";
		iSendResult = send(params->socket, denial_response, strlen(denial_response), 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(params->socket);
			WSACleanup();
			return 1;
		}
		return 0;
	}

	Player* player = create_player();
	params->game_session->player_array[params->player_number] = player;
	if (open_session_file(params->game_session)) {
		printf("game session file opened\n");
	}


	while (!quit) {

		do {

			iresult = recv(params->socket, recvbuf, recvbuflen, 0);
			if (iresult > 0) {

				// recieve and handle messages
				recvbuf[iresult] = '\0';
				printf("%s\n", recvbuf);
				Message* message = message_parser(recvbuf);
				switch (handle_message(message,params))
				{
				case NORMAL:
					break;
				case UNKNOWN:
					sprintf(message->response, "Unknown request from server\n");
					break;
				case DISCONNECT:
					quit = TRUE;
					sprintf(message->response, "player %d quit the game\n", params->player_number);
					break;
				default:
					break;
				}
				
				iSendResult = send(params->socket, message->response, strlen(message->response), 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed: %d\n", WSAGetLastError());
					closesocket(params->socket);
					WSACleanup();
					return 1;
				}
			}
			else if (iresult == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed: %d\n", WSAGetLastError());
				closesocket(params->socket);
				WSACleanup();
				return -1;
			}
		} while (iresult > 0);

	}
	destroy_player(player);
	
	return 0;
}