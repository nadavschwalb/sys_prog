#pragma once
//Description: creates new file or opens existing file
//Parameters: path to file, creation disposition see WIN32 API for CreateFileA
//Returns: handle to file created or opened
HANDLE create_new_file(LPCSTR file_path, DWORD creation_disposition);
//Description: function reads to file an open file
//Parameters: file HANDLE, read buffer, buffer size
//Returns: if succeded returns TRUE else returns FALSE
BOOL read_file(HANDLE file, char buffer[], int buffer_size);
//Description: function writes to file an open file
//Parameters: file HANDLE, write buffer, buffer size
//Returns: if succeded returns number of bytes written
int write_file(HANDLE file, char buffer[], int buffer_size);