#include "../include/param.h"
#include "../include/Thread.h"

//ϵͳ�߳���ڣ�������Ҫ�����ǵ����û��߳���ں��������ں������غ����
//ExitThread�˳��̡߳�
void ThreadSysEntry(ThreadProc UserEntry, int *pParam)
{
	int exitCode = (*UserEntry)(pParam);
	ExitThread(exitCode);
}