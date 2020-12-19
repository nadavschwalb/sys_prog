#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "HardCodedData.h"
#include "Factor_Utilities.h"
factor_struct* get_factors(int number) {
	int max_array_size = ARRAY_LEN;
	factor_struct* factor_array = (factor_struct*)malloc(sizeof(factor_struct));
	factor_array->array = (unsigned int*)malloc(sizeof(int)*max_array_size);
	factor_array->array_len = 0;
	while (number % 2 == 0) {
		if (factor_array->array_len >= max_array_size) {
			max_array_size *= 2;
			factor_array->array = (unsigned int*)realloc(factor_array->array, max_array_size);
		}
		number = number / 2;
		factor_array->array[factor_array->array_len] = 2;
		factor_array->array_len += 1;
	
	}
	int i = 3;
	while (i <= (int)sqrt(number)) {
		while (number % i == 0) {
			if (factor_array->array_len >= max_array_size) {
				max_array_size *= 2;
				factor_array->array = (unsigned int*)realloc(factor_array->array, max_array_size);
			}
			number = number / i;
			factor_array->array[factor_array->array_len] = i;
			factor_array->array_len += 1;
		}
		i += 2;
	}
	
	if (number > 2) {
		if (factor_array->array_len >= max_array_size) {
			max_array_size *= 2;
			factor_array->array = (unsigned int*)realloc(factor_array->array, max_array_size);
		}
		factor_array->array[factor_array->array_len] = number;
		factor_array->array_len += 1;
	}
	return factor_array;
}


void print_factors(factor_struct* factor_array, int number) {
	printf("the prime factors of %d are: ", number);
	int i = 0;
	for (i = 0; i < factor_array->array_len -1; i++) {
		printf("%d, ", factor_array->array[i]);
	}
	printf("%d\r\n", factor_array->array[i]);
}


