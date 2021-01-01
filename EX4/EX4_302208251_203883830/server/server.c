#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include "Player_Thread.h"
#include "Hard_Coded_Data.h"
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv) {
	//variables
	WSADATA wsa_data;
	int iresult;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	DWORD thread_id[MAX_THREADS];
	HANDLE threads[MAX_THREADS];
	Player_Thread_Params* player_params[MAX_THREADS];
	// Initialize Winsock
	iresult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iresult != 0) {
		printf("WSAStartup failed: %d\n", iresult);
		return 1;
	}

	// create address info struct hints 
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iresult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iresult != 0) {
		printf("getaddrinfo failed: %d\n", iresult);
		WSACleanup();
		return 1;
	}

	// setup server socket
	SOCKET ServerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ServerSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return -1;
	}

	// bind socket to IP address
	iresult = bind(ServerSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iresult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}

	// listen to IP adress
	printf("listening for client\n");
	if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ServerSocket);
		WSACleanup();
		return -1;
	}


	//connect clients and open threads

	SOCKET ClientSocket = INVALID_SOCKET;
	int thread_number = 0;
	while (thread_number<2) {
		player_params[thread_number] = (Player_Thread_Params*)malloc(sizeof(Player_Thread_Params));
		ClientSocket = INVALID_SOCKET;
		// Accept a client socket
		ClientSocket = accept(ServerSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ServerSocket);
			WSACleanup();
			return 1;
		}
		// Open Player Thread
		player_params[thread_number]->socket = ClientSocket;
		player_params[thread_number]->player = thread_number;
		threads[thread_number] = CreateThread(NULL,
			0,
			player_thread,
			(LPVOID)player_params[thread_number],
			0,
			&thread_id[thread_number]);
		if (threads[thread_number] == INVALID_HANDLE_VALUE) {
			printf("failed to create thread\n");
			free(player_params[thread_number]);
			return -1;
		}

		thread_number+=1;

	}


	DWORD dw_wait_result = WaitForMultipleObjects(thread_number, threads, TRUE, INFINITE);
	switch (dw_wait_result)
	{
	case WAIT_OBJECT_0:
		printf("threads ended succesfully\n");
		break;
	case WAIT_TIMEOUT:
		printf("one or more threads failed to return\n");
		return 1;
	default:
		break;
	}

	// shutdown the send half of the connection since no more data will be sent
	iresult = shutdown(ClientSocket, SD_SEND);
	if (iresult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return -1;
	}

	// Deinitialize Winsock
	closesocket(ClientSocket);
	iresult = WSACleanup();
	if (iresult != 0) {
		printf("WSACleanup failed: %d\n", iresult);
		return -1;
	}

	//cleanup
	for (int i = 0; i < thread_number; i++) {
		free(player_params[i]);
		CloseHandle(threads[i]);
	}
	return 0;
}