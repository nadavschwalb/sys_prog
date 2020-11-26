#pragma once
//Description: struct that holds the input paramaters to each new thread 
typedef struct Thread_Param
{
	HANDLE hfile_input;
	HANDLE hfile_output;
	int section_length;
	int distance_to_move;
	HANDLE ghMutex;
	int cypher_key;
	int thread_num;
}Thread_Param, *p_Thread_Param;

//Description:	a function that runs in each thread to decrypt or encrypte
//				a section of the input file
//Parameters:	a Thread_param type struct holding the function input parameters
//Returns:		DWORD TRUE if thread took ownership of mutex handle and ran correctly FALSE otherwise 
DWORD WINAPI decipher_thread(LPVOID lpParam);

