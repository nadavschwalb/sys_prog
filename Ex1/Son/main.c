// program counts the number of burnt tree's in the forrest by counting the number of F's in the input string
#include <stdio.h>
#include <string.h>
#include "CountChars.h"
int main(int argc, char** argv) {
	
	char* buffer;
	int burnt_count = 0;
	buffer = strupr(*(argv + 1));
	printf("input string: %s\n", buffer);
	burnt_count = char_in_str(buffer, 'F');
	printf("Burnt trees: %d\n", burnt_count);
		
	exit(burnt_count);
}