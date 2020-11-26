#pragma once
//Description:	decrypts or encrypts section of the input file
//				into the output file
//Parameters:	input and output file handles, section length, cypher key - positive for decyrtion 
//																			negavtive for encryption													
//Returns:		void, reads and writes from existing files
void decipher_section(HANDLE hfile_input, HANDLE hfile_output, int section_length, int cypher_key);
