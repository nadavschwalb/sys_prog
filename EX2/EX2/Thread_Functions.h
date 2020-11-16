

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

DWORD WINAPI decipher_thread(LPVOID lpParam);

