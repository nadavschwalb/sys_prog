#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"
HANDLE create_new_file(LPCSTR file_path, DWORD creation_disposition) {
	 
	HANDLE hfile = CreateFileA(
		file_path,
		(GENERIC_READ | GENERIC_WRITE),
		(FILE_SHARE_READ | FILE_SHARE_WRITE),
		NULL,
		creation_disposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hfile == INVALID_HANDLE_VALUE) {
		printf("failed to open file: %s\n terminating\n", file_path);
		exit(-1);
	}

	return hfile;

	
}




void read_line(HANDLE file, char buffer[]) {
	BOOL success = 1;
	DWORD nbuff = BUFFSIZE;
	DWORD bytes_read = 0;
	success = ReadFile(
		file,
		(void*)buffer,
		nbuff,
		&bytes_read,
		NULL
	);
	if (!success) {
		printf("read line failed error code: %d\n", GetLastError());
		exit(-1);
	}
	//add null terminator
	buffer[bytes_read] = '\0';
	
}