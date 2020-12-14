#include <stdio.h>
#include <stdlib.h>
#include "HardCodedData.h"
#include "Factor_Utilities.h"
int main(int argc, char** argv) {

	int number = 36;
	factor_struct* factors = get_factors(number);
	print_factors(factors, number);

	//cleanup

	free(factors);
	return 0;
}