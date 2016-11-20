#include "../include/param.h"
#include "../include/Thread.h"

//系统线程入口，它的主要作用是调用用户线程入口函数，并在函数返回后调用
//ExitThread退出线程。
void ThreadSysEntry(ThreadProc UserEntry, int *pParam)
{
	int exitCode = (*UserEntry)(pParam);
	ExitThread(exitCode);
}