#pragma 
//Description - parses CSV style array into a 2D array of char type
//Parameters- pointer to FILE input, number of rows and colums of array, char* to allocaded array
//Returns - void
void char_CSV_parser(FILE* fp, int rows, int columns, char* array);
//Description - prints array of char's to stdout
//Parameters- char* to array, array size (row,column) array name
//Returns - void
void print_char_array(char* array, int rows, int columns, const char* array_name);
//Description - converts all items in char array to uppercase
//Parameters- char* to array, array size (row,column) 
//Returns - void
void arr_to_upper(char* array, int row, int column); 
