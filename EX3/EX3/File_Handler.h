#pragma once
#include <Windows.h>
HANDLE open_file(LPCSTR filename);
BOOL getline(HANDLE file, LPSTR* line_buffer);