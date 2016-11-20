#include "Thread.h"
// 每个进程内最大线程数，由于内存空间限制所致。如内存空间足够，可调为更大的值。
#define NTHREAD_PROC		4 	

#define SZOMB    			5  /* process being terminated */

//proc结构改为：
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
  int p_threadNum;	//总线程数
  int p_actThreadNum; //运行着的线程数
  int *p_thdsysentry; /* pointer to sys entry ThreadSysEntry*/
  struct MsgQueue msgQue;
  struct  SharedMemory  *p_shm[MEM_MAP_COUNT_PROC];
	struct MemMap *p_mmap[MEM_MAP_COUNT_PROC];
} proc[NPROC];
