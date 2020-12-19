#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "HardCodedData.h"
#include "File_Handler.h"
#include "Queue.h"


Queue* InitializeQueue(int queue_size) {
	Queue* queue_struct = (Queue*)malloc(sizeof(Queue));
	queue_struct->p_queue = (LONG*)malloc((queue_size+1) * sizeof(LONG));
	queue_struct->queue_size = queue_size;
	queue_struct->head = 0;
	queue_struct->tail = 0;
	if (queue_struct == NULL || queue_struct->p_queue == NULL) {
		return NULL;
	}
	else return queue_struct;
}

LONG Top(Queue* queue) {
	if (queue->head == queue->tail) {
		printf("list empty can't get top\n");
		return -1;
	}
	else return queue->p_queue[queue->head];
}
LONG Pop(Queue* queue) {
	if (queue->head >= queue->tail) {
		printf("pop out of range\n");
		return -1;
	}
	LONG ret_val = queue->p_queue[queue->head];
	queue->head++;
	return ret_val;
}

BOOL Push(Queue* queue, LONG element) {
	if (queue->tail > queue->queue_size) {
		printf("push out of range\n");
		return FALSE;
	}
	queue->p_queue[queue->tail] = element;
	queue->tail++;
	return TRUE;
}

BOOL Empty(Queue* queue) {
	if (queue->head >= queue->tail) {
		return TRUE;
	}
	else return FALSE;
}

Queue* DestroyQueue(Queue* queue) {
	free(queue->p_queue);
	free(queue);
	return NULL;
}

void print_queue(Queue* queue) {
	printf("queue: ");
	if (Empty(queue)) {
		printf("Empty\n");
		return;
	}
	for (int i = queue->head; i < queue->tail - 1; i++) {
		printf("%ld, ", queue->p_queue[i]);
	}
	printf("%ld\n", queue->p_queue[queue->tail -1]);
}

void fill_priority_queue(Queue* queue,int num_tasks,HANDLE prioraty_file) {

	for (int i = 0; i < num_tasks; i++) {
		LONG element = get_priority(prioraty_file);
		if (element >= 0) {
			if (Push(queue, element)) {
				continue;
			}
			else return -1;
		}
		else return -1;
	}
}
 
LONG get_priority(HANDLE priority_file) {
	LPSTR priority_str = "";
	LONG priority = 0;
	if (getline(priority_file, &priority_str)) {
		return atoi(strtok(priority_str, "\r"));
	}
	else {
		printf("failed to get next priority from priority file\n");
		return -1;
	}
	free(priority_str);
}

