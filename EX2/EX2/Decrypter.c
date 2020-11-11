//TODO: function that recieves string and returns decypted string

//TODO: dycription utility functions
#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"
void dycript_string(char str[], int key) {
	int i = 0;
	short temp;
	while (str[i] != '\0') {
		if (str[i] <= 'z' && str[i] >= 'a') {
			temp = (str[i] - 'a' - key) % 26;
			str[i] = (temp < 0) ? 'a' + temp + 26 : 'a' + temp;
			
		}
		else if (str[i] <= 'Z' && str[i] >= 'A') {
			temp = (str[i] - 'A' - key) % 26;
			str[i] = (temp < 0) ? 'A' + temp + 26 : 'A' + temp;
		}
		else if (str[i] <= '9' && str[i] >= '0') {
			temp = (str[i] - '0' - key) % 10;
			str[i] = (temp < 0) ? '0' + temp + 10 : '0' + temp;
		}
		i++;
	}
}