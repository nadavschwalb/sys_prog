HANDLE create_new_file(LPCSTR file_path, DWORD creation_disposition);
BOOL read_file(HANDLE file, char buffer[], int buffer_size);
int write_file(HANDLE file, char buffer[], int buffer_size);