#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Messages.h"
#include "Hard_Coded_Data.h"
#include "Player_Thread.h"

Message* message_parser(char* message) {
	char* p_colon = NULL;
	char* token;
	Message* parsed_message = (Message*)malloc(sizeof(Message));
	parsed_message->message_type = (char*)malloc(sizeof(char)*strlen(message)+1);
	parsed_message->param_list = (char**)malloc(sizeof(char)*strlen(message));
	parsed_message->param_count = 0;
	parsed_message->response = (char*)malloc(sizeof(char)*DEFAULT_BUFLEN);
	p_colon = strchr(message, ':');
	if (p_colon == NULL) {
		strcpy(parsed_message->message_type, message);
		parsed_message->message_type[strlen(message)-1] = '\0';
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
	int last_param_len = strlen(parsed_message->param_list[parsed_message->param_count -1]);
	parsed_message->param_list[parsed_message->param_count - 1][last_param_len-1] = '\0';
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

int handle_message(Message* message, Player_Thread_Params* param) {
	if (strcmp(message->message_type, "CLIENT_REQUEST") == 0) {
		strcpy(param->game_session->player_array[param->player_number]->username, message->param_list[0]);
		strcpy(message->response, "SERVER_APPROVED\n");
	}
	else if (strcmp(message->message_type, "CLIENT_SETUP") == 0) {
		param->game_session->player_array[param->player_number]->combo = atoi(message->param_list[0]);
		printf("%s's combo: %d\n",
			param->game_session->player_array[param->player_number]->username,
			param->game_session->player_array[param->player_number]->combo);
	}
	else if (strcmp(message->message_type, "CLIENT_PLAYER_MOVE") == 0) {
		param->game_session->player_array[param->player_number]->move = atoi(message->param_list[0]);
	}
	else if (strcmp(message->message_type, "CLIENT_DISCONNECT") == 0) {
		return DISCONNECT;
	}
	else if (strcmp(message->message_type, "READY_FOR_MENU") == 0) {
		strcpy(message->response, "SERVER_MAIN_MENU\n");
	}
	else if (strcmp(message->message_type, "CLIENT_VERSUS") == 0) {
		param->game_session->player_array[param->player_number]->ready_to_play = TRUE;
		param->game_session->active_players++;
		while (param->game_session->active_players < 2) {

		}
		int other_player = param->player_number ^ 1;
		sprintf(message->response, "SERVER_INVITE:%s\n", param->game_session->player_array[other_player]->username);
		//strcpy(message->response, "SERVER_INVITE:OTHER_PLAYER\n");
	}
	else if (strcmp(message->message_type, "CLIENT_INVITE_APPROVED") == 0) {
		strcpy(message->response, "SERVER_SETUP_REQUEST\n");
	}

	
	else {
		printf("Unknown message from client\n");
		return UNKNOWN;
	}

	return NORMAL;
}