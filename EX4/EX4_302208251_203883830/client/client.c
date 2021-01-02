#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Messages.h"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in  *sockaddr_ipv4;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	BOOL quit = FALSE;


	// Validate the parameters
	if (argc < 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	//Attempt to connect to server
	ptr = result->ai_next;
	while (TRUE) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			printf("Failed connecting to server on %s:%d\n",
				inet_ntoa(sockaddr_ipv4->sin_addr), 
				sockaddr_ipv4->sin_port);
			int answer = 0;
			printf("1. Try to reconnect\n2. Exit\n");
			scanf("%d", &answer);
			if (answer == 1) {
				ptr = ptr->ai_next;
				if (ptr == NULL) { printf("no valid connections\n"); return 1; }
				continue;
			}
			else if (answer == 2) {
				printf("terminating program\n");
				return 1;
			}
			else {
				printf("invalid choice\n");
				return 1;
			}
		}
		break;
	}
	
	printf("Connected to server on %s:%d\n",
		inet_ntoa(sockaddr_ipv4->sin_addr),
		sockaddr_ipv4->sin_port);

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}


	//// Send an initial buffer
	//strcpy(sendbuf, argv[2]);
	//iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	//if (iResult == SOCKET_ERROR) {
	//	printf("send failed with error: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	//// recieve initial response
	//iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	//if (iResult > 0) {
	//	recvbuf[iResult] = '\0';
	//	Message* message = message_parser(recvbuf);
	//	switch (handle_message(message))
	//	{
	//	case NORMAL:
	//		break;
	//	case UNKNOWN:
	//		break;
	//	case APPROVED:
	//		printf("Connected to server on %s:%d\n",
	//			inet_ntoa(sockaddr_ipv4->sin_addr),
	//			sockaddr_ipv4->sin_port);
	//		break;
	//	case DISCONECT:
	//		quit = TRUE;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//else if (iResult == 0)
	//	printf("Connection closed\n");
	//else {
	//	printf("recv failed with error: %d\n", WSAGetLastError());
	//}
	char sendbuf[DEFAULT_BUFLEN];

	//player setup
	sprintf(sendbuf, "CLIENT_REQUEST:%s\n", argv[3]);
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	//server approval and menu


	while (!quit) {
		char message[DEFAULT_BUFLEN];
		scanf("%s", message);
		strcat(message, "\n");
		if (strcmp(message, "Quit") == 0) {
			printf("quitting\n");
			quit = TRUE;
		}
		iResult = send(ConnectSocket, message, (int)strlen(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		do
		{
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				recvbuf[iResult] = '\0';
				Message* message = message_parser(recvbuf);
				switch (handle_message(message))
				{
				case NORMAL:
					break;
				case UNKNOWN:
					break;
				case APPROVED:
					break;
				case DISCONECT:
					quit = TRUE;
					break;
				default:
					break;
				}
			}
			else if (iResult == 0)
				printf("Connection closed\n");
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
			}
		} while (iResult >= recvbuflen);

	}


	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection




	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}