#include <stdio.h>
#include <Windows.h>
#include "HardCodedData.h"
#include "File_Utilities.h"
#include "Decrypter.h"
#include "Thread_Functions.h"

DWORD WINAPI decipher_thread(LPVOID lpParam) {
	p_Thread_Param p_parameters = (p_Thread_Param)lpParam;
	printf("\n-------------\nthread opened: %d\n",p_parameters->thread_num);
	DWORD dwWaitResult;
	
	// request ownership of mutex object 
	dwWaitResult = WaitForSingleObject(p_parameters->ghMutex,INFINITE);

	switch (dwWaitResult)
	{
	// Thread got ownership of mutex object
	case WAIT_OBJECT_0:
		__try {
			//change_file_pointer(distance_to_move);
			printf("\n-------------\nmutex released thread writing: %d\n", p_parameters->thread_num);
			if (SetFilePointer(p_parameters->hfile_input, p_parameters->distance_to_move, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
				printf("failed to set new pointer in thread\n");
				exit(GetLastError());
			}
			if (SetFilePointer(p_parameters->hfile_output, p_parameters->distance_to_move, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
				printf("failed to set new pointer in thread\n");
				exit(GetLastError());
			}
			decipher_section(p_parameters->hfile_input, p_parameters->hfile_output, p_parameters->section_length, p_parameters->cypher_key);
		}

		__finally {
			// Release ownership of the mutex object
			if (!ReleaseMutex(p_parameters->ghMutex)) {
				printf("failed to release mutex object\n");
				exit(GetLastError());
			}
		}
	case WAIT_ABANDONED:
		return FALSE;
	default:
		break;
	}
	printf("\n-------------\nthread finnished resume program: %d\n", p_parameters->thread_num);
	return TRUE;
}