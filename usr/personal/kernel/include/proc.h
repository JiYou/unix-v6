#include "Thread.h"
// ÿ������������߳����������ڴ�ռ��������¡����ڴ�ռ��㹻���ɵ�Ϊ�����ֵ��
#define NTHREAD_PROC		4 	

#define SZOMB    			5  /* process being terminated */

//proc�ṹ��Ϊ��
struct     proc
{
  char p_stat;
	char p_flag;
	char p_nice; /* nice for scheduling */
	char p_time; /* resendial time for scheduler */
  char p_sig;  /* signal number sent to this process */
  char p_uid;  /* user id, used to direct tty signals */ 
  int p_ttyp; /* controlling tty */
  int p_pid;  /* unique process id */
  int p_ppid; /* process id of parent */
  int p_addr; /* address of swappable image */
  int p_size; /* size of swappable image (*64 bytes) */
  int *p_textp;/* pointer to text structure */
  struct Thread *p_threads[NTHREAD_PROC];
  int p_threadNum;	//���߳���
  int p_actThreadNum; //�����ŵ��߳���
  int *p_thdsysentry; /* pointer to sys entry ThreadSysEntry*/
  struct MsgQueue msgQue;
  struct  SharedMemory  *p_shm[MEM_MAP_COUNT_PROC];
	struct MemMap *p_mmap[MEM_MAP_COUNT_PROC];
} proc[NPROC];
