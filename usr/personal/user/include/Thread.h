#ifndef _THREAD_H_
#define _THREAD_H_

#define THREAD_RUN						1		//线程初始处于运行状态
#define THREAD_SUSPEND				2		//线程初始处于挂起状态
#define  SUSPPRI							(-2)		//线程挂起优先级

typedef  int (*ThreadProc)(int *pParam);
int CreateThread(char *name, char cPriority, int iStatus, ThreadProc entry, int *pParam, int iStackSize);
int ExitThread(int exitCode);
int gettid();
int SuspendThread(int tid);
int ResumeThread(int tid);
int TerminateThread (int tid);
int GetThreadID(char *name);
int GetThreadName(int tid, char *name);

#endif