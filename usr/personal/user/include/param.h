#ifndef _PARAM_H_
#define _PARAM_H_

typedef int BOOL;
#define TRUE  	1
#define FALSE  	0

//进程间通信相关错误码
#define 			EINUSE							40
#define 			ESYSPID							43		//向系统进程发送消息
#define 			ENOMBX							42		//不存在该信箱
#define 			EQUEFULL						44		//消息队列已满
#define 			EQUEEPY							45		//消息队列为空
#define 			EEXIT								46		//接收进程已退出

//线程相关错误码
#define 			ENOTHDSPC					50	//没有线程数据结构
#define  		  EINVTHDID					51		//无效的线程ID
#define 		  EINVTHDSTATE 			52		//目标线程不处于运行态
#define 			ENOTHREAD					54

#endif