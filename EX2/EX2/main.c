#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "HardCodedData.h"

int main(int argc, TCHAR* argv[]) {

	// Golbal Variables ----------------------------------------------------------------
	DWORD retval = 0;
	TCHAR file_path[BUFFSIZE] = TEXT("");
	LPCSTR* lpFilePart = { NULL };
	TCHAR file_name[BUFFSIZE] =TEXT("");
	int cypher_key = 0;
	char arg_options = '\0';

	// Check for optional args ---------------------------------------------------------
	if (argc > 3) {
		arg_options = *(strchr(argv[1],'-') + 1);
		retval = GetFullPathNameA(argv[2], BUFFSIZE, file_path, lpFilePart);
		cypher_key = atoi(argv[3]);
	}
	else {
		retval = GetFullPathNameA(argv[1], BUFFSIZE, file_path, lpFilePart);
		cypher_key = atoi(argv[2]);
	}


	// get full file path and file name ------------------------------------------------
	
	if (retval == 0) {
		printf("GetFullPath failed Error code (%d)\n", GetLastError());
	}
	else {
		TCHAR* temp;
		temp = strrchr(file_path,'\\') + 1;
		strcpy(file_name,temp);
		*(temp) = '\0';
		printf("option: %c, input file name: %s, cypher key: %d", arg_options, file_name, cypher_key);

	}


	//TODO: open input file windows API
	//TODO: open output file decrypted.txt in folder path
	//TODO: 
	return 0;
}