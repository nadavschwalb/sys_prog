#pragma once
#include <Windows.h>
#include "Factor_Utilities.h"
#include "File_Handler.h"
#include "Queue.h"
#include "Lock.h"
#include "HardCodedData.h"
#include "Threads.h"
typedef struct Task_Thread_Params {
	HANDLE task_file;
	Queue* queue;
	Lock* lock;
	
}Task_Thread_Params;

HANDLE create_thread(Task_Thread_Params* params, LPDWORD lpThreadId);