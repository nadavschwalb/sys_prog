#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"
#include "Queue.h"
#include "Lock.h"


Lock* InitializeLock() {
	Lock* lock = (Lock*)malloc(sizeof(Lock));
	lock->Mutex = CreateMutex(NULL, FALSE, NULL);
	lock->RoomEmpty = CreateSemaphore(NULL, 1, 1, NULL);
	lock->Turnstile = CreateMutex(NULL, FALSE, NULL);
	lock->readers = 0;
	if (lock->Mutex == INVALID_HANDLE_VALUE ||
		lock->RoomEmpty == INVALID_HANDLE_VALUE ||
		lock->Turnstile == INVALID_HANDLE_VALUE ||
		lock == NULL) {
		printf("failed to intialize lock\n");
		return NULL;
	}
	return lock;
}

BOOL read_lock(Lock* lock) {
	DWORD dwRoomEmpty;
	DWORD dwTurnstile = WaitForSingleObject(lock->Turnstile, 10);
	switch (dwTurnstile)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		printf("turnsile timeout\n");
		return FALSE;
	default:
		printf("unknown error in turnstile\n");
		return FALSE;
	}
	ReleaseMutex(lock->Turnstile);
	DWORD dwMutex = WaitForSingleObject(lock->Mutex, INFINITE);
	switch (dwMutex)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		printf("mutex timeout\n");
		return FALSE;
	default:
		printf("unknown error in mutex\n");
		return FALSE;
	}
	lock->readers++;
	if (lock->readers == 1) {
		dwRoomEmpty = WaitForSingleObject(lock->RoomEmpty, INFINITE);
		switch (dwRoomEmpty)
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			printf("RoomEmpty timeout\n");
			return FALSE;
		default:
			printf("unknown error in RoomEmpty\n");
			return FALSE;
		}
	}
	ReleaseMutex(lock->Mutex);
	return TRUE;
}


BOOL read_release(Lock* lock) {
	DWORD dwMutex = WaitForSingleObject(lock->Mutex, INFINITE);
	switch (dwMutex)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		printf("mutex timeout\n");
		return FALSE;
	default:
		printf("unknown error in mutex\n");
		return FALSE;
	}
	lock->readers--;
	if (lock->readers == 0) {
		ReleaseSemaphore(lock->RoomEmpty, 1, NULL);
	}
	ReleaseMutex(lock->Mutex);
	return TRUE;
}

BOOL write_lock(Lock* lock) {
	DWORD dwTurnstile = WaitForSingleObject(lock->Turnstile, INFINITE);
	switch (dwTurnstile)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		printf("turnsile timeout\n");
		return FALSE;
	default:
		printf("unknown error in turnstile\n");
		return FALSE;
	}
	DWORD dwRoomEmpty = WaitForSingleObject(lock->RoomEmpty, INFINITE);
	switch (dwRoomEmpty)
	{
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT:
		printf("RoomEmpty timeout\n");
		return FALSE;
	default:
		printf("unknown error in RoomEmpty\n");
		return FALSE;
	}
	return TRUE;
}

BOOL write_release(Lock* lock) {
	ReleaseMutex(lock->Turnstile);
	ReleaseSemaphore(lock->RoomEmpty, 1, NULL);
	return TRUE;
}

Lock* DestroyLock(Lock* lock) {
	CloseHandle(lock->Mutex);
	CloseHandle(lock->Turnstile);
	CloseHandle(lock->RoomEmpty);
	free(lock);
	return NULL;
}