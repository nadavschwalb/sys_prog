// program counts the number of burnt tree's in the forrest by counting the number of F's in the input string
#include <stdio.h>
#include <string.h>
#include "CountChars.h"
int main(int argc, char** argv) {
	
	char* buffer;
	int burnt_count = 0;
	buffer = strupr(*(argv + 1));
	burnt_count = char_in_str(buffer, 'F');
		
	exit(burnt_count);
}