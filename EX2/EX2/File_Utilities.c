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




BOOL read_file(HANDLE file, char buffer[],int buffer_size) {
	BOOL success = TRUE;
	DWORD nbuff = buffer_size;
	DWORD bytes_read = 0;
	success = ReadFile(
		file,
		(void*)buffer,
		nbuff,
		&bytes_read,
		NULL
	);
	if (!success) {
		printf("read buffer failed error code: %d\n", GetLastError());
		exit(-1);
	}
	if (success && bytes_read == 0) { //EOF
		return FALSE;
	}
	else {
		//add null terminator
		buffer[bytes_read] = '\0';
		return TRUE;
	}
	
}

int write_file(HANDLE file, char buffer[], int buffer_size) {
	BOOL success = TRUE;
	DWORD nbuff = buffer_size;
	DWORD bytes_written = 0;
	success = WriteFile(
		file,
		(void*)buffer,
		nbuff,
		&bytes_written,
		NULL
	);
	if (!success) {
		printf("read line failed error code: %d\n", GetLastError());
		exit(-1);
	}
	if (success && bytes_written <= nbuff) { //written to file
		buffer[bytes_written] = '\0';
		return bytes_written;
	}
	else {
		//add null terminator
		printf("unknown file write error occered\nterminating error code: %d\n", GetLastError());
		exit(-1);
	}
}


