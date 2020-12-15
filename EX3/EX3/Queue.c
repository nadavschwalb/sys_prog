#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "HardCodedData.h"
#include "File_Handler.h"
void fill_tasks_queue(int* queue, HANDLE tasks_file, HANDLE prioraty_file) {
	LPSTR priority_str = "";
	LONG distance_to_move = 0;
	LONG next_number = 0;
	int i = 0;
	while (getline(prioraty_file, &priority_str)) {
		distance_to_move = atoi(strtok(priority_str,"\r"));
		if (SetFilePointer(tasks_file, distance_to_move, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
			printf("failed to set file pointer\n");
		}
		else {
			getline(tasks_file, &priority_str);
			next_number = atoi(strtok(priority_str, "\r"));
			queue[i] = next_number;
			i++;
		}
		
	}
	free(priority_str);
}


