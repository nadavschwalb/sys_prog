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
	Task_Thread_Params* params = (Task_Thread_Params*)lpParameter;
	DWORD dwSemaphoreGun = WaitForSingleObject(params->SemaphoreGun, INFINITE);
	switch (dwSemaphoreGun)
	{
	case WAIT_OBJECT_0:
		break;
	default:
		return FALSE;
	}
	printf("started thread id: %d\n", GetCurrentThreadId());
	
	LONG next_priority = 0;
	LPSTR task_buffer = "";
	int number = 0;
	BOOL loop_condition = TRUE;


	while (loop_condition) {
		if (!read_lock(params->queue_lock)) {
			printf("failed to read lock\n");
			return FALSE;
		}
		printf("read_lock queue thread id: %d\n", GetCurrentThreadId());
		if (Empty(params->queue)) {
			break;
		}
		next_priority = Pop(params->queue);


		printf("next priority: %lu\n",next_priority);

		if (!read_lock(params->tasks_lock)) {
			printf("failed to read lock\n");
			return FALSE;
		}
		printf("read_lock tasks thread id: %d\n", GetCurrentThreadId());
		SetFilePointer(params->task_file, next_priority, NULL, FILE_BEGIN);

		getline(params->task_file, &task_buffer);

		if (!read_release(params->tasks_lock)) {
			printf("failed to read release\n");
			return FALSE;
		}
		printf("read_release tasks thread id: %d\n", GetCurrentThreadId());

		if (!read_release(params->queue_lock)) {
			printf("failed to release read\n");
			return FALSE;
		}
		printf("read_release queue thread id: %d\n", GetCurrentThreadId());
		if (!write_lock(params->tasks_lock)) {
			printf("failed to write lock\n");
			return FALSE;
		}
		printf("write_lock tasks thread id: %d\n", GetCurrentThreadId());

		number = atoi(strtok(task_buffer, "\r"));
		factor_struct* factors = get_factors(number);
		printf("thread id: %d\n", GetCurrentThreadId());
		print_factors(factors, number);
		SetFilePointer(params->task_file,0, NULL, FILE_END);
		write_factors(params->task_file, factors, number);
		free(factors);

		if (!write_release(params->tasks_lock)) {
			printf("failed to write release\n");
			return FALSE;
		}
		printf("write_release tasks thread id: %d\n", GetCurrentThreadId());
	}


	printf("thread closed succesfuly: %lu\n", GetCurrentThreadId());
	ReleaseSemaphore(params->SemaphoreGun,1,NULL);
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