 #pragma once
void forrest_next_gen(int forrest_size, char* old_forrest_array);
void matrix_extender(char* original_matrix_pt, char* new_matrix_pt, int mat_size);
int neighbours_counter(char* arrey_pointer, int i, int j, int line_size, char look_for);
int diagonal_naibors_counter(char* arrey_pointer, int i, int j, int line_size, char look_for);
int diagonal_naibors_counter(char* arrey_pointer, int i, int j, int line_size, char look_for);
void next_generation_calculator(char* new_generation_matrix, char* old_generation_matrix, int new_matrix_size);