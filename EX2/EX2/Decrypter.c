//TODO: function that recieves string and returns decypted string

//TODO: dycription utility functions
#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"
#include "File_Utilities.h"
void decipher_string(char str[], int key) {
	int i = 0;
	short temp;
	while (str[i] != '\0') {
		if (str[i] <= 'z' && str[i] >= 'a') {
			temp = (str[i] - 'a' - key) % 26;
			str[i] = (temp < 0) ? 'a' + temp + 26 : 'a' + temp;
			
		}
		else if (str[i] <= 'Z' && str[i] >= 'A') {
			temp = (str[i] - 'A' - key) % 26;
			str[i] = (temp < 0) ? 'A' + temp + 26 : 'A' + temp;
		}
		else if (str[i] <= '9' && str[i] >= '0') {
			temp = (str[i] - '0' - key) % 10;
			str[i] = (temp < 0) ? '0' + temp + 10 : '0' + temp;
		}
		i++;
	}
}

void decipher_section(HANDLE hfile_input,HANDLE hfile_output, int section_length, int cypher_key) {
	int bytes_written = 0;
	int bytes_to_read_write = 0;
	char line_buffer[BUFFSIZE + 1];
	
	while ( bytes_written < section_length) {
		bytes_to_read_write = (section_length - bytes_written < BUFFSIZE) ? section_length - bytes_written : BUFFSIZE;
		if (!read_file(hfile_input, line_buffer, bytes_to_read_write)) break;
		printf("%s", line_buffer);
		decipher_string(line_buffer, cypher_key);
		bytes_written += write_file(hfile_output, line_buffer, bytes_to_read_write);
	}
	if (section_length == bytes_written) return;
	else {
		printf("\nsection not completed\n");
		exit(GetLastError());
	}
}

