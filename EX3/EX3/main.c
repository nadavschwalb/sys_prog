#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "HardCodedData.h"
#include "Factor_Utilities.h"
#include "File_Handler.h"
#include "Queue.h"
#include "Lock.h"
#include "Threads.h"
int main(int argc, char** argv) {
	LPCSTR tasks_file_name = argv[1];
	LPCSTR prioraty_file_name = argv[2];
	DWORD num_tasks = atoi(argv[3]);
	DWORD num_threads = atoi(argv[4]);
	HANDLE* thread_array;
	Task_Thread_Params* task_params = (Task_Thread_Params*)malloc(sizeof(Task_Thread_Params));
	DWORD* thread_id_array;

	HANDLE task_file = open_file(tasks_file_name);
	if (task_file == INVALID_HANDLE_VALUE) { 
		printf("failed to open file: error code %d\n", GetLastError()); 
		return GetLastError();
	}
	HANDLE prioraty_file = open_file(prioraty_file_name);
	if (task_file == INVALID_HANDLE_VALUE) {
		printf("failed to open file: error code %d\n", GetLastError());
		return GetLastError();
	}
	Queue* priority_queue = InitializeQueue(num_tasks-1);
	if (priority_queue == NULL) {
		printf("failed to allocate queue\n");
		return -1;
	}
	
	for (DWORD i = 0; i < num_tasks; i++) {
		LONG element = get_priority(prioraty_file);
		if (element >= 0) {
			if (Push(priority_queue, element)) {
				continue;
			}
			else return -1;
		}
		else return -1;
	}

	print_queue(priority_queue);

	//create mutex
	HANDLE ghMutex = CreateMutexA(NULL, FALSE, NULL);
	if (ghMutex == NULL) {
		printf("failed to create mutex\n");
		return -1;
	}

	thread_array = (HANDLE*)malloc(num_threads * sizeof(HANDLE));
	thread_id_array = (DWORD*)malloc(num_threads * sizeof(DWORD));
	task_params->lock = InitializeLock(num_threads);
	task_params->lock->read_lock = ghMutex;
	task_params->queue = priority_queue;
	task_params->task_file = task_file;



	//create threads
	for (DWORD i = 0; i < num_threads; i++) {
		thread_array[i] = create_thread(task_params, &thread_id_array[i]);
	}
	
	WaitForMultipleObjects(num_threads, thread_array, TRUE, INFINITE);
	//cleanup
	for (DWORD i = 0; i < num_threads; i++) {
		CloseHandle(thread_array[i]);
	}
	free(thread_array);
	free(thread_id_array);
	CloseHandle(task_file);
	CloseHandle(prioraty_file);
	task_params->lock = DestroyLock(task_params->lock);
	priority_queue = DestroyQueue(priority_queue);
	free(task_params);
	return 0;
}