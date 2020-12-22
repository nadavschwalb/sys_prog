#pragma once
typedef struct factor_array {
	 int* array;
	 int array_len;
}factor_struct;

factor_struct* get_factors(int number);
void print_factors(factor_struct* factor_array, int number);
void write_factors(HANDLE hfile, factor_struct* factor_array, int number);