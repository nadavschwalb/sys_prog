
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Messages.h"
#include "Hard_Coded_Data.h"



Message* message_parser(char* message) {
	char* p_colon = NULL;
	char* token;
	Message* parsed_message = (Message*)malloc(sizeof(Message));
	parsed_message->message_type = (char*)malloc(sizeof(char)*strlen(message) + 1);
	parsed_message->param_list = (char**)malloc(sizeof(char)*strlen(message));
	parsed_message->response = (char*)malloc(sizeof(char)*DEFAULT_BUFLEN);
	parsed_message->param_count = 0;
	p_colon = strchr(message, ':');
	if (p_colon == NULL) {
		strcpy(parsed_message->message_type, message);
		parsed_message->message_type[strlen(message) - 1] = '\0';
		return parsed_message;
	}
	strncpy(parsed_message->message_type, message, (int)(p_colon - message));
	parsed_message->message_type[p_colon - message] = '\0';
	p_colon += 1;
	token = strtok(p_colon, ";");

	do
	{
		*(parsed_message->param_list + parsed_message->param_count) =
			(char*)malloc(sizeof(char)*strlen(message));
		strcpy(*(parsed_message->param_list + parsed_message->param_count), token);
		token = strtok(NULL, ";");
		parsed_message->param_count++;
	} while (token != NULL);
	int last_param_len = strlen(parsed_message->param_list[parsed_message->param_count - 1]);
	parsed_message->param_list[parsed_message->param_count - 1][last_param_len - 1] = '\0';
	return parsed_message;
}

BOOL destroy_message(Message* message) {

	for (int i = 0; i < message->param_count; i++) {
		free(message->param_list[i]);
	}
	free(message->param_list);
	free(message->message_type);
	free(message->response);
	free(message);
	return TRUE;
}

void print_message(Message* message) {
	printf("message type: %s\n", message->message_type);
	printf("params:\n");
	for (int i = 0; i < message->param_count; i++) {
		printf("\t%s\n", message->param_list[i]);
	}
}

int handle_message(Message* message) {
	if (strcmp(message->message_type, "SERVER_MAIN_MENU") == 0) {
		printf("Choose what to do next:\n1. Play against another client\n2. Quit\n");
		int answer = 0;
		scanf("%d", &answer);
		if(answer == 1) strcpy(message->response, "CLIENT_VERSUS\n");
		else return DISCONNECT;
	}
	else if (strcmp(message->message_type, "SERVER_APPROVED") == 0) {
		strcpy(message->response, "READY_FOR_MENU\n");
	}
	else if (strcmp(message->message_type, "SERVER_DENIED") == 0) {
		printf("server denied\n");
		return DISCONNECT;
	}
	else if (strcmp(message->message_type, "SERVER_INVITE") == 0) {
		printf("Game is on!\n your opponents name is: %s\n", message->param_list[0]);
		strcpy(message->response, "CLIENT_INVITE_APPROVED\n");
	}
	else if (strcmp(message->message_type, "SERVER_SETUP_REQUEST") == 0) {
		printf("Choose your 4 digits:");
		int answer = 0;
		scanf("%d", &answer);
		sprintf(message->response, "CLIENT_SETUP:%d\n", answer);
	}
	else if (strcmp(message->message_type, "SERVER_PLAYER_MOVE_REQUEST") == 0) {
		printf("Choose your guess:");
		int answer = 0;
		scanf("%d", &answer);
		sprintf(message->response, "CLIENT_PLAYER_MOVE:%d\n", answer);
	}

	else {
		printf("Unknown message from server\n%s\n",message->message_type);
		return UNKNOWN;
	}

	return NORMAL;
}