#include <stdio.h>

int main()
{
	char line[100];
	char code[100];
	int i = 0;
	printf("begin\n");
	fgets(line, 100, stdin);
	printf(line);

	sscanf(line, "%s %d\n", code, &i);
	printf("code: %s , i: %d\n", code, i);
	return 0;
}