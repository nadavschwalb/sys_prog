#pragma once
#include <Windows.h>

typedef struct Lock {
	HANDLE Mutex;
	HANDLE RoomEmpty;
	HANDLE Turnstile;
	HANDLE SemaphoreGun;
	int readers;
}Lock;

Lock* InitializeLock();
Lock* DestroyLock(Lock* lock);
BOOL write_release(Lock* lock);
BOOL read_release(Lock* lock);
BOOL read_lock(Lock* lock);
BOOL write_lock(Lock* lock);