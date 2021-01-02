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
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")

DWORD WINAPI player_thread(LPVOID lpParam) {
	Player_Thread_Params* params = (Player_Thread_Params*)lpParam;
	printf("player %d thread started\n", params->player);
	int iresult = 0;
	int iSendResult = 0;
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	BOOL quit = FALSE;
	// Receive until the peer shuts down the connection


	while (!quit) {

		do {
			//DWORD dw_result = WaitForSingleObject(params->socket_mutex, INFINITE);
			//switch (dw_result)
			//{
			//case WAIT_OBJECT_0:
			//	printf("player %d connect to socket\n", params->player);
			//	break;
			//case WAIT_TIMEOUT:
			//	printf("player %d failed to receive acsess to socket\n", params->player);
			//	return 0;
			//default:
			//	break;
			//}
			iresult = recv(params->socket, recvbuf, recvbuflen, 0);
			if (iresult > 0) {

				// Echo the buffer back to the sender
				recvbuf[iresult] = '\0';
				printf("%s\n", recvbuf);
				sprintf(sendbuf, "message recieved player %d\n", params->player);
				if (strcmp(recvbuf, "Quit") == 0) {
					quit = TRUE;
					sprintf(sendbuf, "player %d quit the game\n", params->player);
				}
				
				iSendResult = send(params->socket, sendbuf, strlen(sendbuf), 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed: %d\n", WSAGetLastError());
					closesocket(params->socket);
					WSACleanup();
					return 1;
				}
				printf("Bytes sent: %d\n", iSendResult);
			}
			else if (iresult == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed: %d\n", WSAGetLastError());
				closesocket(params->socket);
				WSACleanup();
				return -1;
			}
			//printf("player %d releasing socket\n", params->player);
			//ReleaseMutex(params->socket_mutex);
		} while (iresult > 0);

	}
	
	return 0;
}