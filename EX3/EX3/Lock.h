#pragma once
#include <Windows.h>

typedef struct Lock {
	HANDLE read_lock;
	HANDLE write_lock;
	HANDLE queue_lock;
}Lock;

Lock* InitializeLock(int max_thread_count);
Lock* DestroyLock(Lock* lock);
DWORD read_lock(Lock* lock, DWORD timeout);