/*	Authors - Nadav Schwalb 302208251 Shoval Ben Shushan 203883830
	Project EX1 - Son
	Description - program is called by Father with a arg string wich represents the forrest
				  calculates number of char F (fire) in string and returns as exit code
*/
#include <stdio.h>
#include <string.h>
#include "CountChars.h"
int main(int argc, char** argv) {
	printf("son called with input string %s\n", *(argv + 1));
	char* buffer;
	int burnt_count = 0;
	buffer = strupr(*(argv + 1));
	burnt_count = char_in_str(buffer, 'F');
		
	exit(burnt_count);
}