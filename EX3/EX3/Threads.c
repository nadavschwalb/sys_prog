#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "Factor_Utilities.h"
#include "File_Handler.h"
#include "Queue.h"
#include "Lock.h"
#include "HardCodedData.h"
#include "Threads.h"

BOOL WINAPI task_thread(LPVOID lpParameter) {
	printf("started thread id: %d\n", GetCurrentThreadId());
	Task_Thread_Params* params = (Task_Thread_Params*)lpParameter;
	LONG next_priority = 0;
	LPSTR task_buffer = "";
	int number = 0;

	while (!Empty(params->queue)) {
		DWORD lock_status = WaitForSingleObject(params->lock->read_lock, 50);
		printf("received mutex: %d\n", GetCurrentThreadId());
		switch (lock_status)
		{
		case WAIT_OBJECT_0:
			if (Empty(params->queue)) return TRUE;
			next_priority = Pop(params->queue);
			SetFilePointer(params->task_file, next_priority, NULL, FILE_BEGIN);

			if (getline(params->task_file, &task_buffer)) {
				number = atoi(strtok(task_buffer, "\r"));
				factor_struct* factors = get_factors(number);
				printf("thread id: %d\n", GetCurrentThreadId());
				print_factors(factors, number);
				free(factors);
			}
			else {
				printf("failed to read task file\n");
				return FALSE;
			}
			break;
		case WAIT_TIMEOUT:
			return FALSE;
		default:
			return FALSE;
		}
		printf("release mutex id: %d\n", GetCurrentThreadId());
		ReleaseMutex(params->lock->read_lock);
	}
	
	
	
	return TRUE;
}

HANDLE create_thread(Task_Thread_Params* params, LPDWORD lpThreadId) {
	HANDLE new_thread = CreateThread(NULL, 0, task_thread, (LPVOID)params, 0, lpThreadId);
	if (new_thread == NULL) {
		printf("failed to create thread");
		return NULL;
	}
	else return new_thread;
}