#ifndef _THREAD_H_
#define _THREAD_H_

//系统中最大线程数
#define           NTHREAD                                        256

 /* stat codes */

 #define SSLEEP    1  /* sleeping on high priority */
 #define SWAIT    2  /* sleeping on low priority */
 #define SRUN     3  /* running */
 #define SIDL     4  /* process/thread being created */
#define SSTOP    6  /* thread being traced */

 /* flag codes */

#define SLOAD   01                 /* in core */
 #define SSYS   02                 /* system thread/process */
 #define SLOCK   04               /* process cannot be swapped */ 
 #define SSWAP   010       /* process is being swapped out */
 #define STRC    020                /* thread is being traced */
 #define SWTED   040       /* another tracing flag */
#define STERM	100		//线程要被终止

typedef  int (*ThreadProc)(int *pParam);

/*
 * 线程处理结构
 */
struct Thread
{
	char t_stat;
  char t_flag;
  char t_pri;  /* priority, negative is high */
  char t_time; /* resident time for scheduling */
  char t_cpu;  /* cpu usage for scheduling */
  char t_nice; /* nice for scheduling */
  int t_tid;  /* unique thread id */
  int t_ttyp; /* controlling tty */
  int t_index; /* internal index from 0 to NTHREAD_PROC-1 */
  int t_pid; /* process id of parent */
  int t_ustackaddr; /* address of stack in userspace */
  int t_ustacksize; /* size of userstack (*64 bytes) */
  int t_wchan;/* event thread is awaiting */
  char t_name[12];
  ThreadProc t_entry;/* pointer to user entry */
  int *t_param; /* param passed to thread proc */
  struct proc *t_proc; /*parent process pointer */
}Threads[NTHREAD];

#endif