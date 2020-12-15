#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "HardCodedData.h"
#include "Factor_Utilities.h"
#include "File_Handler.h"
#include "Queue.h"
int main(int argc, char** argv) {
	LPCSTR tasks_file_name = argv[1];
	LPCSTR prioraty_file_name = argv[2];
	DWORD num_tasks = atoi(argv[3]);
	DWORD num_threads = atoi(argv[4]);
	int number = 36;

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
	int* task_queue = (int*)malloc(num_tasks * sizeof(int));
	if (task_queue == NULL) {
		printf("failed to allocate queue\n");
		return -1;
	}
	int* queue = (int*)malloc(num_tasks*sizeof(int));
	fill_tasks_queue(queue, task_file, prioraty_file);
	
	for (int i = 0; i < num_tasks; i++) {
		printf("%d\n", queue[i]);
	}
	factor_struct* factors = get_factors(number);
	print_factors(factors, number);

	//cleanup

	free(factors);
	CloseHandle(task_file);
	CloseHandle(prioraty_file);
	free(queue);
	return 0;
}