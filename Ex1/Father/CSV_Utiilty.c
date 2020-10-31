#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
}

void print_char_array(char* array, int rows, int columns) {
	printf("printing array to screen\n");
	int i, j = 0;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			printf("%c,",*(array + i*columns + j));
		}
		printf("\n");
	}
}

//TODO: add capitalize array function