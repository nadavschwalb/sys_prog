#pragma once
#include <Windows.h>

typedef struct Queue {
	LONG* p_queue;
	int queue_size;
	int head;
	int tail;
}Queue;

Queue* InitializeQueue(int queue_size);
LONG Top(Queue* queue);
LONG Pop(Queue* queue);
BOOL Push(Queue* queue, LONG element);
BOOL Empty(Queue* queue);
Queue* DestroyQueue(Queue* queue);
void print_queue(Queue* queue);
LONG get_priority(HANDLE priority_file);