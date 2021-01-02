#pragma once
#define DISCONECT -1
#define UNKNOWN -2
#define NORMAL -3
#define APPROVED -4

#include <Windows.h>


typedef struct Message {
	char* message_type;
	char** param_list;
	int param_count;
	char* response;
}Message;

Message* message_parser(char* message);
BOOL destroy_message(Message* message);
void print_message(Message* message);
