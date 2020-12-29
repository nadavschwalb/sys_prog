#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv) {
	WSADATA wsa_data;
	int iresult;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

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



	SOCKET ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ServerSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}



	char recvbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Receive until the peer shuts down the connection
	do {

		iresult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iresult > 0) {
			printf("Bytes received: %d\n", iresult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iresult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iresult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return -1;
		}

	} while (iresult > 0);

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

	return 0;
}