#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "CSV_Utility.h"
#include "HardCodedData.h"

void char_CSV_parser(FILE* fp, int rows, int columns, char* array) {
	char line_buffer[MAX_INPUT_STRING_LENGTH];
	int i = 0;
	int j = 0;
	int k = 0;
	while (fgets(line_buffer, columns*3, fp) != NULL) {
		while (*(line_buffer + k) != '\n' && *(line_buffer + k) != '\0') {
			if (*(line_buffer + k) == ',') k++;
			else {
				*(array + i * columns + j) = *(line_buffer + k);
				j++;
				k++;
			}
			
		}
		i++;
		k = 0;
		j = 0;
	}
	*(array + rows * columns) = '\0';
}

void print_char_array(char* array, int rows, int columns ,const char* array_name) {
	printf("%s \n",array_name);
	int i, j = 0;
	for (i = 0; i < rows - 1; i++) {
		for (j = 0; j < columns - 1; j++) {
			printf("%c,",*(array + i*columns + j));
		}
		printf("%c\n", *(array + i * columns + j + 1));
	}
	for (j = 0; j < columns - 1; j++) {
		printf("%c,", *(array + i * columns + j));
	}
	printf("%c\n", *(array + i * columns + j));
	
}


void arr_to_upper(char* array,int row, int column ) {
	int i, j = 0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			*(array + i * column + j) = toupper(*(array + i * column + j));
		}
	}
}

