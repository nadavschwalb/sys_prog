//TODO: fire solver loops through array and tests index by case 'F','G','T'

//TODO: check neighbors for fire

//TODO: check neighbors for two trees
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HardCodedData.h"
#include "CSV_Utility.h"
#include "Forrest.h"

void forrest_next_gen(int forest_size, char* forrest_array) {
	int i, j = 0;
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
					diagonal_naibors_counter(old_generation_matrix, i, j, new_matrix_size + 2, 'T')) {
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

void matrix_extender(char* original_matrix_pt, char* new_matrix_pt ,int mat_size)
{
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

	//TODO: create for input

//int	z = 0;
//
//	for (i = 0; i < forrest_size; i++) {
//		for (j = 0 ; j < forrest_size; j++) {
//			printf("%c\n", *(next_generation_forrest_array + i * forrest_size + j));
//			switch (*(old_forrest_array + i * forrest_size + j))
//			{
//			case 'T':
//				if (neighbours_counter((old_forrest_array + i * forrest_size + j), i, j, forrest_size, 'F') > 0) {
//					*(next_generation_forrest_array + i * forrest_size + j) = 'F';
//				}
//				else
//				{
//					*(next_generation_forrest_array + i * forrest_size + j) = 'T';
//				}
//				break;
//			case 'G':
//				if (neighbours_counter((old_forrest_array + i * forrest_size + j), i, j, forrest_size, 'F') + diagonal_naibors_counter((old_forrest_array + i * forrest_size + j), i, j, forrest_size, 'F') > 1)
//				{
//					*(next_generation_forrest_array + i * forrest_size + j) = 'T';
//				}
//				else
//				{
//					*(next_generation_forrest_array + i * forrest_size + j) = 'G';
//				}
//				break;
//			case 'F':
//				*(next_generation_forrest_array +i*forrest_size +j) = 'G';
//				break;
//			}
//			z++;
//			printf("%d\n", z);
//			printf("%c\n", *(next_generation_forrest_array + i * forrest_size + j));
//		}
//	}
//	return next_generation_forrest_array;
//}
//
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

int diagonal_naibors_counter(char* arrey_pointer, int i, int j, int line_size, char look_for) {
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

	