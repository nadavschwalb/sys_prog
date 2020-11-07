/*	Authors - Nadav Schwalb 302208251 Shoval Ben Shushan 203883830
	Project EX1 - Father
	Description - program reads input file with square forrest of char type and number of gens to run
				  runs given generations and calls son to calculate number of burnt trees in the forrest
				  in each generation.
				  outputs each generation to "output.txt"
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include "HardCodedData.h"
#include "CSV_Utility.h"
#include "Forrest.h"
#include "Process_Generator.h"

int main(int argc, char** argv) {

	printf("starting Father\n");

	int forrest_size = 0;
	int generations = 0;
	int burned_count = 0;

	char line_buffer[MAX_FORREST_SIZE * 4];
	if (strstr(*(argv + 1), ".txt") == NULL) {
		printf("Error: incorrect argument\n");
		exit(-1);
	}

	printf("opening files\n");
 
	//open input and output files, closed on cleanup
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

	//get forrest size and number of generations to run
	fgets(line_buffer, MAX_FORREST_SIZE, forrest_input);
	forrest_size = atoi(line_buffer);
	fgets(line_buffer, MAX_FORREST_SIZE, forrest_input);
	generations = atoi(line_buffer);
	printf("forrest size: %d, generations: %d\n", forrest_size, generations);

	// create forrest array - memory is freed on cleanup
	char* forrest_array = (char*)malloc(forrest_size * forrest_size * sizeof(char) + 1);
	if (forrest_array == NULL) {
		printf("Error: failed to allocate memory. \n");
		exit(-1);
	}

	//parse input into array and convert to uppercase 
	char_CSV_parser(forrest_input, forrest_size, forrest_size, forrest_array);
	arr_to_upper(forrest_array, forrest_size, forrest_size);

	//generation 0
	print_char_array(forrest_array, forrest_size, forrest_size ,"current forrest");
	burned_count = CreateProcessSimpleMain(forrest_array);
	if (burned_count < 0) exit(burned_count); // son proccess creation failed return exitcode -1
	printf("generation: %d, burnt trees: %d\n", 1, burned_count);;
	fprintf_s(forrest_output, "%s - %d\n", forrest_array, burned_count);

	// generations 1 through n
	for (int i = 0; i < generations-1; i++) {
		forrest_next_gen(forrest_size, forrest_array);
		print_char_array(forrest_array, forrest_size, forrest_size, "current forrest");
		burned_count = CreateProcessSimpleMain(forrest_array);
		if (burned_count < 0) exit(burned_count); // son proccess creation failed return exitcode -1
		printf("generation: %d, burnt trees: %d\n",i+2,burned_count);
		fprintf_s(forrest_output, "%s - %d\n", forrest_array, burned_count);
	}

		


	//exit safely and cleanup
	fclose(forrest_input);
	fclose(forrest_output);
	free(forrest_array);
	printf("program ended succesfuly\n");
	return 0;
}

