//counts number of char c in string str

 int char_in_str(char* str, char c) {
	int char_count = 0;
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c) char_count++;
		i++;
	}
	return char_count;
}