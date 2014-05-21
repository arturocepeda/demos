
//////////////////////////////////////////////////////////////////
//
//  Arturo Cepeda Pérez
//  Game Engine
//
//  Core
//
//  --- GEThreads.h ---
//
//////////////////////////////////////////////////////////////////


#pragma once


//
//  Windows
//
#if defined(__WIN32__)


#include <windows.h>


#define GEThread  HANDLE
#define GEThreadFunction(FunctionName)  DWORD WINAPI FunctionName(LPVOID pData)

#define GECreateThread(Thread,FunctionName,DataPointer)  Thread = CreateThread(NULL, 0, FunctionName, DataPointer, 0, NULL)
#define GEWaitForThread(Thread)  WaitForSingleObject(Thread, INFINITE)
#define GECloseThread(Thread)  CloseHandle(Thread)


#define GEMutex  CRITICAL_SECTION

#define GEInitMutex(Mutex)  InitializeCriticalSection(&Mutex)
#define GELockMutex(Mutex)  EnterCriticalSection(&Mutex)
#define GEUnlockMutex(Mutex)  LeaveCriticalSection(&Mutex)
#define GEDestroyMutex(Mutex)  DeleteCriticalSection(&Mutex)


void wait(int milliseconds) { Sleep(milliseconds); }



//
//  Unix
//
#else


#include <pthread.h>
#include <unistd.h>


#define GEThread  pthread_t
#define GEThreadFunction(FunctionName)  void* FunctionName(void* pData)

#define GECreateThread(Thread,FunctionName,DataPointer)  pthread_create(&Thread, NULL, FunctionName, DataPointer)
#define GEWaitForThread(Thread)  pthread_join(Thread, NULL)
#define GECloseThread(Thread)


#define GEMutex  pthread_mutex_t

#define GEInitMutex(Mutex)  pthread_mutex_init(&Mutex, NULL)
#define GELockMutex(Mutex)  pthread_mutex_lock(&Mutex)
#define GEUnlockMutex(Mutex)  pthread_mutex_unlock(&Mutex)
#define GEDestroyMutex(Mutex)  pthread_mutex_destroy(&Mutex)


void wait(int milliseconds) { usleep(milliseconds * 1000); }


#endif
