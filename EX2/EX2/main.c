//Authors: Nadav Schwalb, Shoval Ben Shushan
//Project: Ceaser
//Description: programs that receives an input file, cypher key number of threads
//			   and encryption/decryption flag. it decrypts or encrypts the file using
//			   a the ceaser cypher on n separate threads

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include "HardCodedData.h"
#include "File_Utilities.h"
#include "Thread_Functions.h" 


int main(int argc, TCHAR* argv[]) {

	// Golbal Variables ----------------------------------------------------------------
	DWORD retval = 0;
	TCHAR dir_path[BUFFSIZE] = TEXT("");
	TCHAR file_name[BUFFSIZE] =TEXT("");
	int cypher_key = 0;
	int thread_count = 0;
	char arg_options = '\0';
	HANDLE input_file;
	HANDLE output_file;
	int input_file_size = 0; 
	HANDLE thread_array[MAX_THREAD_COUNT];
	p_Thread_Param thread_param_array[MAX_THREAD_COUNT];
	DWORD thread_ID;
	HANDLE ghMutex;
	time_t begin =0, end = 0  , execution_time = 0;

	// Start run time extra feature
	time(&begin);

	// Check for optional args ---------------------------------------------------------
	if (argc == 5) {
		retval = GetFullPathNameA(argv[1], BUFFSIZE, dir_path, NULL);
		cypher_key = atoi(argv[2]);
		thread_count = atoi(argv[3]);
		arg_options = (char)*(strchr(argv[4], '-') + 1);
		if (arg_options != 'd' && arg_options != 'e') {
			printf("please enter -d to decrypt and -e to encrypt\n");
			return -1;
		}

	// Set cypher key for decryption or encryption
		cypher_key = (arg_options == 'd') ? cypher_key : -cypher_key;
	}
	else if(argc < 5) {
		printf("error: too few arguments\n");
		return -1;
	}
	else {
		printf("error: too many arguments\n");
		return -1;
	}
	//open input file and get file size ------------------------------------------------
	input_file = create_new_file(dir_path,OPEN_EXISTING);
	if (input_file == INVALID_HANDLE_VALUE) {
		printf("failed to open file: %s\n terminating\n", dir_path);
		return -1;
	}
	input_file_size = GetFileSize(input_file, NULL);
	// get full file path and file name ------------------------------------------------
	if (retval == 0) {
		printf("GetFullPath failed Error code (%d)\n", GetLastError());
		return -1;
	}
	else {
		TCHAR* temp;
		temp = strrchr(dir_path,'\\') + 1;
		strcpy(file_name,temp);
		*(temp) = '\0';
		printf("option: %c, input file name: %s, cypher key: %d\n", arg_options, file_name, cypher_key);

	}

	//open output file relative to input file -----------------------------------------
	LPCSTR output_file_name = (LPCSTR)strcat(dir_path, "decrypted.txt");
	output_file = create_new_file(output_file_name,CREATE_ALWAYS);
	if (output_file == INVALID_HANDLE_VALUE) {
		printf("failed to open file: %s\n terminating\n", strcat(dir_path, "decrypted.txt"));
		return -1;
	}

	//split program into threads ------------------------------------------------------

	//create mutual exclution object
	ghMutex = CreateMutex(NULL, FALSE, NULL);
	if (ghMutex == NULL) {
		printf("CreateMutex error: %d\n", GetLastError());
		CloseHandle(ghMutex);
		CloseHandle(output_file);
		CloseHandle(input_file);
		exit(GetLastError());
	}
	// Create worker threads 
	for (int i = 0; i < thread_count; i++) {
	//allocate memory for thread parameter structs
		thread_param_array[i] = (p_Thread_Param)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Thread_Param));
		if (thread_param_array[i] == NULL) {
			printf("failed to allocate memory for thread parameter struct: thread number %d\n", i);
			CloseHandle(ghMutex);
			CloseHandle(output_file);
			CloseHandle(input_file);
			exit(GetLastError());
		}
		

	//set thread parameters
		thread_param_array[i]->hfile_input = input_file;
		thread_param_array[i]->hfile_output = output_file;
		thread_param_array[i]->distance_to_move = (input_file_size / thread_count)*i;
		thread_param_array[i]->section_length = (i < thread_count-1) ? 
												input_file_size / thread_count : 
												input_file_size / thread_count + input_file_size % thread_count;
		thread_param_array[i]->ghMutex = ghMutex;
		thread_param_array[i]->cypher_key = cypher_key;
		thread_param_array[i]->thread_num = i;

	//create new thread
		thread_array[i] = CreateThread(
							NULL,
							0,
							(LPTHREAD_START_ROUTINE)decipher_thread,
							thread_param_array[i],
							0,
							&thread_ID);
		printf("\n------------------\ncreated thread [%d] with thread ID: %d\n", i, thread_ID);
	//check thread was created
		if (thread_array[i] == NULL) {
			printf("failed to create thread number %d\n", i);
			CloseHandle(ghMutex);
			CloseHandle(output_file);
			CloseHandle(input_file);
			exit(GetLastError());
		}
	}

	//wait for all threads to finish ------------------------------------------------------
	DWORD dwthread_finished = WaitForMultipleObjects(thread_count, thread_array, TRUE, MAX_WAIT_THREAD);
	switch (dwthread_finished)
	{
	case WAIT_OBJECT_0:
		printf("all thread returned successfuly\n");
		break;
	case WAIT_TIMEOUT:
		printf("thread timed out\n");
		break;
	case WAIT_ABANDONED_0:
		printf("at least one thread was abandoned\n");
		break;
	default:
		printf("wait error: %d\n", GetLastError());
		break;
	}


	//cleanup -------------------------------------------------------------------------
	//close all threads and mutex and free heap safely
	for (int i = 0; i < thread_count; i++) {
		printf("\n------------------\nclosing thread [%d] with thread ID: %d\n", i, thread_ID);
		CloseHandle(thread_array[i]);
		HeapFree(thread_param_array[i], 0, NULL);
	}
	CloseHandle(ghMutex);
	CloseHandle(output_file);
	CloseHandle(input_file);

	//calculate execution time
	time(&begin);
	execution_time = end - begin;
	printf("\n--------------------\nexecution time: %d\n", (int)&execution_time);
	
	return 0;
}