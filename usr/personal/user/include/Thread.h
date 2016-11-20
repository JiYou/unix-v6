#ifndef _THREAD_H_
#define _THREAD_H_

#define THREAD_RUN						1		//�̳߳�ʼ��������״̬
#define THREAD_SUSPEND				2		//�̳߳�ʼ���ڹ���״̬
#define  SUSPPRI							(-2)		//�̹߳������ȼ�

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