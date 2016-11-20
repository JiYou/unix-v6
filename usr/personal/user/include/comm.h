#ifndef _COMM_H_
#define _COMM_H_
#include "param.h"
/*
 * Semphore sys call
 */
int CreateSemphore(int initNum);
int CloseSemphore(int sd);
int AcquireSemphore(int sd);
int ReleaseSemphore(int sd);

/*
 * Mutex sys call
 */
int CreateMutex();
int CloseMutex(int md);
int AcquireMutex(int md);
int ReleaseMutex(int md);

/*
 * Event sys call
 */
#define 		WT_IMME					0
#define 		WT_INFINITE			-1

int CreateEvent(BOOL initStatus);
int CloseEvent(int ed);
int WaitEvent(int ed, int time);
int SetEvent(int ed);

/*
 * Message sys call
 */
int SendMessage(int pid, char *msg, int len, int time);
int ReceiveMessage(char *msg, int len, int time);

/*
 * Mail sys call
 */
int CreatePrivMbx(char *name);
int CloseMbx(int md);
int SendMail(char *name, char *mail, int len, int time);
int ReceiveMail(int mbxd, char *mail, int len, int time);

int CreateSharedMbx();
int SendSharedMail(int mbxd, char *mail, int len, int time);

int CreatePublicMbx (char *name);
int ClosePublicMbx(char *name);
int ReceivePublicMail(char *name, char *mail, int len, int time);

/*
 * Shared memory sys call
 */
#define PERMISSION_READ				1
#define PERMISSION_WRITE				2
#define PERMISSION_ALL			(PERMISSION_WRITE| PERMISSION_READ)
struct MapShm
{
	int *addr;
	int size;
};

int CreateSharedMemory(char *name, int blocks, int ownpermin, int otherpermin);
int FreeSharedMemory(int shd);
int MapSharedMemory(char *name, MapShm *ms);
int UnmapSharedMemory(int shd);
 
#endif