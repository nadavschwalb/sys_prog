//proram simulates trees burning in a forrest
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HardCodedData.h"
#include "CSV_Utility.h"
#include "Forrest.h"

int main(int argc, char** argv) {

	printf("starting Father\n");

	int forrest_size = 0;
	int generations = 0;

	char line_buffer[MAX_FORREST_SIZE * 4];
	if (strstr(*(argv + 1), ".txt") == NULL) {
		printf("Error: incorrect argument\n");
		exit(-1);
	}

	printf("opening files\n");

	FILE* forrest_input = fopen(*(argv + 1), "r");
	if (forrest_input == NULL) {
		printf("Error: no such file in directory\n");
		exit(-1);
	}

	FILE* forrest_output = fopen("output.txt", "w");
	if (forrest_input == NULL) {
		printf("Error: failed to open output file\n");
		exit(-1);
	}

	fgets(line_buffer, MAX_FORREST_SIZE, forrest_input);
	forrest_size = atoi(line_buffer);
	fgets(line_buffer, MAX_FORREST_SIZE, forrest_input);
	generations = atoi(line_buffer);

	printf("forrest size: %d, generations: %d\n", forrest_size, generations);

	char* forrest_array = (char*)malloc(forrest_size * forrest_size * sizeof(char));
	if (forrest_array == NULL) {
		printf("Error: failed to allocate memory. \n");
		exit(-1);
	}
	char_CSV_parser(forrest_input, forrest_size, forrest_size, forrest_array);
	print_char_array(forrest_array, forrest_size, forrest_size);
	arr_to_upper(forrest_array, forrest_size, forrest_size);
	print_char_array(forrest_array, forrest_size, forrest_size);
	printf("generations: %d\n", generations);
	for (int i = 0; i < generations-1; i++) {
		forrest_next_gen(forrest_size, forrest_array);
		print_char_array(forrest_array, forrest_size, forrest_size);
	}
	//TODO: loop for x generations and execute generational update of forrest
		
	//TODO: call Son with string descirbing forest

	//TODO: output to file

	//TODO: exit safely
	fclose(forrest_input);
	fclose(forrest_output);
	free(forrest_array);
	return 0;
}

