#pragma once
#define DISCONNECT -1
#define UNKNOWN -2
#define NORMAL -3

#include "Player_Thread.h"
#include <Windows.h>
#include "Game.h"

typedef struct Message {
	char* message_type;
	char** param_list;
	int param_count;
	char* response;
}Message;

Message* message_parser(char* message);
BOOL destroy_message(Message* message);
void print_message(Message* message);
int handle_message(Message* message, Player_Thread_Params* param);