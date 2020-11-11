#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "HardCodedData.h"
#include "File_Utilities.h"
#include "Decrypter.h"

int main(int argc, TCHAR* argv[]) {

	// Golbal Variables ----------------------------------------------------------------
	DWORD retval = 0;
	TCHAR dir_path[BUFFSIZE] = TEXT("");
	LPCSTR* lpFilePart = { NULL };
	TCHAR file_name[BUFFSIZE] =TEXT("");
	int cypher_key = 0;
	char arg_options = '\0';
	HANDLE input_file;
	HANDLE output_file;
	DWORD64 input_file_size = 0; 

	// Check for optional args ---------------------------------------------------------
	if (argc > 3) {
		arg_options = *(strchr(argv[1],'-') + 1);
		retval = GetFullPathNameA(argv[2], BUFFSIZE, dir_path, lpFilePart);
		
		cypher_key = atoi(argv[3]);
	}
	else {
		retval = GetFullPathNameA(argv[1], BUFFSIZE, dir_path, lpFilePart);
		cypher_key = atoi(argv[2]);
	}

	//open input file and get file size ------------------------------------------------
	input_file = create_new_file(dir_path,OPEN_EXISTING);
	input_file_size = GetFileSize(input_file, NULL);
	char line_buffer[BUFFSIZE + 1];
	// get full file path and file name ------------------------------------------------
	
	if (retval == 0) {
		printf("GetFullPath failed Error code (%d)\n", GetLastError());
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

	//loop through file ( this code will go in the thread function)--------------------
	while (read_file(input_file, line_buffer,BUFFSIZE)) {
		printf("%s", line_buffer);
		dycript_string(line_buffer, cypher_key);
		write_file(output_file, line_buffer, BUFFSIZE);
		
	}

	//cleanup -------------------------------------------------------------------------
	CloseHandle(output_file);
	CloseHandle(input_file);
	
	return 0;
}