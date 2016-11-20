#ifndef _THREAD_U_H_
#define _THREAD_U_H_

#include "proc.h"
#define USIZE					(4096/64)
#define MAX_EVENTS_PROC			4

#define  THREAD_SSIZE			1024
#define  SSIZE					4096	
/*
 * 线程上下文
 */
struct U_Thread
{
	int u_rsav[2];    /* save r5,r6 when exchanging stacks */  
  int u_fsav[25];   /* save fp registers */					
/* rsav and fsav must be first in structure */   
  char u_segflg;    /* flag for IO; user or kernel space */   
  char u_error;     /* return error code */
  char u_uid;/* effective user id */
  char u_gid;/* effective group id */
  char u_ruid;/* real user id */
  char u_rgid;/* real group id */
  int u_threadp;/* pointer to Thread structure */
  char *u_base;     /* base address for IO */ 
  char *u_count;    /* bytes remaining for IO */ //read/write to/from u_base
  char *u_offset[2];/* offset in file for IO */ 
  int *u_cdir; /* pointer to inode for current directory */  
  char u_dbuf[DIRSIZ];/* current pathname component */ 
  char *u_dirp;     /* current pointer to inode */
  struct     {/* current directory entry */    
    int	  u_ino;		//the directory u_name's inode number in the disk.
    char     u_name[DIRSIZ]; 
  } u_dent;
  int *u_pdir;/* inode of parent directory of dirp */  
  int u_ofile[NOFILE]; /* pointers to file structures of open files */
  int u_arg[5];     /* arguments to current system call */   
 
  int u_qsav[2];  /* label variable for quits & interrupts */
  int u_ssav[2];    /* label variable for swapping */
  int u_utime;/* this process user time */
  int u_stime;/* this process system time */ 
  int u_cutime[2];  /* sum of childs’ utimes */
  int u_cstime[2];  /* sum of childs’ stimes */
  int *u_ar0;/* address of users saved R0 */
  int u_prof[4];    /* profile arguments */
  char u_intflg;    /* catch intr from sys */
  char u_pad;
  char u_kernstack[802]; // 每个线程的内核栈
};

/*
 * 进程u变量，其总大小大约4K, 位于内核虚拟地址[0o140000, 0o150000]。
 */
struct 
{
	//所有线程的“u变量”
	struct U_Thread u_threads[NTHREADS_PROC];
	
	//signal是进程级的，但处理过程运行在线程上下文中
	int u_signal[NSIG];/* disposition of signals */
	
	//新创建线程默认的u/gid 
	char u_uid;/* effective user id */ 
	char u_gid;/* effective group id */
	char u_ruid;/* real user id */
	char u_rgid;/* real group id */

  int u_uisa[16];   /* prototype segmentation addresses */   
  int u_uisd[16];   /* prototype segmentation descriptors */ 
  int u_tsize;/* text size (*64) */
  int u_dsize;/* data size (*64) */
  int u_ssize;/* stack size (*64) */

//线程栈分配记录，为了提高效率，可采用链表实现。这里简单起见，采用数组。
  struct map u_stackmap[NTHREAD_PROC]; 
  int u_sep;/* flag for I and D separation */
  
	int u_osema [4];	//  记录信号量打开的句柄数组：
	int u_rsema[4];	//以及已经获得的信号量个数
	int u_oevent[MAX_EVENTS_PROC];
  int u_ombx[MBX_COUNT_PROC];
  
	struct proc *u_procp;

	int u_curthd;	//Index for current thread structure in u_threads
}u;


#endif