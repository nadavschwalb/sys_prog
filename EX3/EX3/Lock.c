#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"
#include "Queue.h"
#include "Lock.h"


Lock* InitializeLock(int max_thread_count) {
	Lock* lock = (Lock*)malloc(sizeof(Lock));
	lock->read_lock = CreateMutexA(NULL,FALSE,NULL);
	lock->write_lock = CreateMutexA(NULL, FALSE, NULL);
	lock->queue_lock = CreateMutexA(NULL, FALSE, NULL);
	if (lock->read_lock == NULL) {
		printf("failed to create lock\n");
		return NULL;
	}
	return lock;
}

Lock* DestroyLock(Lock* lock) {
	CloseHandle(lock->read_lock);
	CloseHandle(lock->write_lock);
	CloseHandle(lock->queue_lock);
	free(lock);
	return NULL;
}

DWORD read_lock(Lock* lock, DWORD timeout) {
	DWORD write_status = WaitForSingleObject(lock->write_lock, timeout);

	switch (write_status)
	{
	case WAIT_OBJECT_0:
		return WaitForSingleObject(lock->read_lock, timeout);
	case WAIT_TIMEOUT:
		return WAIT_TIMEOUT;
	default:
		return WAIT_TIMEOUT;
	}
}

DWORD read_release(Lock* lock, DWORD timeout) {
	return 0;
}

