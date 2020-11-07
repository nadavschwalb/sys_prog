//TODO: fire solver loops through array and tests index by case 'F','G','T'

//TODO: check neighbors for fire

//TODO: check neighbors for two trees
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HardCodedData.h"
#include "CSV_Utility.h"
#include "Forrest.h"

//local function declarations

//Description - pads array with zeros to help with calculating next forrest
//Parameters- pointer to array, pointer to extened array, original array size
//Returns - void
void matrix_extender(char* original_matrix_pt, char* new_matrix_pt, int mat_size);
//Description - counts immidiate neighbors of given value
//Parameters- array pointer, indexs in array, array size, char value 
//Returns - number of "look_for" chars in immidiate neighbors
int neighbours_counter(char* arrey_pointer, int i, int j, int line_size, char look_for);
//Description - counts diagonal neighbors of given value
//Parameters- array pointer, indexs in array, array size, char value 
//Returns - number of "look_for" chars in diagonal neighbors
int diagonal_neighbors_counter(char* arrey_pointer, int i, int j, int line_size, char look_for);
//Description - calculates and updates next gen forrest array
//Parameters- new gen array, old gen array, array size
//Returns - void
void next_generation_calculator(char* new_generation_matrix, char* old_generation_matrix, int new_matrix_size);


void forrest_next_gen(int forest_size, char* forrest_array) {
	char* old_forrest_copy = (char*)malloc((forest_size+2) * (forest_size+2) * sizeof(char));
	if (old_forrest_copy == NULL) {
		printf("Error: failed to allocate memory. \n");
		exit(-1);
	}
	matrix_extender(forrest_array, old_forrest_copy, forest_size);
	next_generation_calculator(forrest_array, old_forrest_copy, forest_size);
}

void next_generation_calculator(char* new_generation_matrix, char* old_generation_matrix,int new_matrix_size) {
	int i, j;
	for (i = 1; i < new_matrix_size + 1; i++) {
		for (j = 1; j < new_matrix_size + 1; j++) {
			switch (*(old_generation_matrix + i * (new_matrix_size + 2) + j)) {
			case 'F':
				*(new_generation_matrix + (i - 1)*new_matrix_size + (j - 1)) = 'G';
				break;
			case 'T':
				if (0 < neighbours_counter(old_generation_matrix, i, j, new_matrix_size + 2, 'F')) {
					*(new_generation_matrix + (i - 1)*new_matrix_size + (j - 1)) = 'F';
				}
				else {
					*(new_generation_matrix + (i - 1)*new_matrix_size + (j - 1)) = 'T';
				}
				break;
			case 'G':
				if (1 < neighbours_counter(old_generation_matrix, i, j, new_matrix_size + 2, 'T') +
					diagonal_neighbors_counter(old_generation_matrix, i, j, new_matrix_size + 2, 'T')) {
					*(new_generation_matrix + (i - 1)*new_matrix_size + (j - 1)) = 'T';
				}
				else {
					*(new_generation_matrix + (i - 1)*new_matrix_size + (j - 1)) = 'G';
				}
				break;
			}

		}
	}
}

void matrix_extender(char* original_matrix_pt, char* new_matrix_pt ,int mat_size){
	int i, j;
	for (i = 0; i < mat_size + 2; i++)
	{
		for (j = 0; j < mat_size + 2; j++)
		{
			if (i == 0 || j == 0||i==mat_size+1||j==mat_size+1) 
			{
				*(new_matrix_pt + i * (mat_size + 2) + j) = '0';
			}
			else
			{
				*(new_matrix_pt + i * (mat_size + 2) + j) = *(original_matrix_pt + (i - 1)*mat_size + (j - 1));
			}
		}
	}
}


int neighbours_counter(char* arrey_pointer, int i, int j, int line_size, char look_for) {
	int counter = 0;
	if (*(arrey_pointer+i*(line_size)+j + 1) == look_for){
		counter += 1;
	}
	if (*(arrey_pointer + i*(line_size) +j - 1) == look_for) {
		counter += 1;
	}
	if (*(arrey_pointer+ i*(line_size)+j + line_size) == look_for) {
		counter += 1;
	}
	if (*(arrey_pointer+ i*(line_size)+j - line_size) == look_for) {
	counter += 1;
	}
	return counter;
}

int diagonal_neighbors_counter(char* arrey_pointer, int i, int j, int line_size, char look_for) {
	int counter = 0;
	if (*(arrey_pointer +i*(line_size)+j + line_size + 1) == look_for) {
		counter += 1;
	}
	if (*(arrey_pointer + i * (line_size)+j + line_size - 1) == look_for) {
		counter += 1;
	}
	if (*(arrey_pointer + i * (line_size)+j - line_size + 1) == look_for) {
		counter += 1;
	}
	if (*(arrey_pointer + i * (line_size)+j - line_size - 1) == look_for) {
		counter += 1;
	}
	return counter;
}

	