#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "HardCodedData.h"

char read_c(HANDLE hFile);

HANDLE open_file(LPCSTR filename) {

	HANDLE file = CreateFileA(filename,
							 (GENERIC_READ | GENERIC_WRITE),
							 (FILE_SHARE_READ|FILE_SHARE_WRITE),
							 NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL,
							 NULL);
	return file;
}

BOOL getline(HANDLE file, LPSTR* line_buffer) {
	unsigned int buffer_len = ARRAY_LEN;
	unsigned int i = 0;
	CHAR c =' ';
	LPSTR buffer = (LPSTR)malloc(buffer_len * sizeof(char));
	do
	{
		
		c = read_c(file);
		if ((int)c == EOF) {
			buffer[i] = '\0';
			*line_buffer = buffer;
			return FALSE;
		}
		if (i >= buffer_len) {
			buffer_len *= 2;
			buffer = (LPSTR)realloc(buffer, buffer_len * sizeof(char));
		}
		buffer[i] = c;
		i++;

	} while (c != '\n');
	buffer[i] = '\0';
	*line_buffer = buffer;
	return TRUE;
}

BOOL writeline(HANDLE hfile, LPSTR str) {
	DWORD bytes_to_write = 1;
	int i = 0;
	while (*str != '\0'){
		if (WriteFile(hfile, str, bytes_to_write, NULL, NULL)) {
			str += 1;
		}
		else {
			printf("failed to write to file");
			return FALSE;
		}
	}
	return TRUE;
}

char read_c(HANDLE hFile) {
	CHAR lpBuffer[1];
	DWORD nBytesRead = 0;
	BOOL read_status = ReadFile(hFile,
								lpBuffer,
								1,
								&nBytesRead,
								NULL
								);
	if (read_status && nBytesRead>0) {
		return (char)lpBuffer[0];
	}
	else if (read_status && nBytesRead == 0) return EOF;
	else {
		printf("failed to read file\n");
		return EOF;
	}
}