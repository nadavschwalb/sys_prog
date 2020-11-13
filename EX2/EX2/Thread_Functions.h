

typedef struct Thread_Param
{
	HANDLE hfile_input;
	HANDLE hfile_output;
	int section_length;
	LONG distance_to_move;
	HANDLE ghMutex;
	int cypher_key;
}Thread_Param, *p_Thread_Param;

DWORD WINAPI decipher_thread(LPVOID lpParam);

