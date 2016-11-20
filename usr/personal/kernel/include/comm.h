/*
 * 进程信号量相关结构
 */
#define MAX_SEMA				64
#define S_WANT					0x8000
#define S_ALLOC					0x4000
#define S_BITS 			    (S_WANT|S_ALLOC)
#define PSEMA					1

struct Semaphore
{
	int s;
	int ref;			//进程引用计数
	int flag;
};

struct	Semaphs
{
	struct Semaphore semas[MAX_SEMA]
	int flag;
}semaphs;

/*
 * 进程事件相关结构
 */
#define MAX_EVENTS				64
	
#define PEVENT					1

#define EEVENTSET				42

struct Event
{
	BOOL e;
	int ref;			//进程引用计数
	int flag;
};

struct	Evts
{
	struct Event events[MAX_EVENTS]
	int flag;
}evts;

/*
 * 进程消息队列相关结构
 */
#define MSG_QUEUE_LEN					16		//每个进程的消息队列长度
#define MAX_MSG_LEN					32		//每个消息的最大长度
#define MSG_BUF_COUNT					128		//消息缓存总数
#define PMSG								1		//进程挂起优先级

//所有消息缓存
struct Message
{
	char data[MAX_MSG_LEN];
	int len;
	struct Message *next;
}msgBuf[MSG_BUF_COUNT];

#define 	MB_WANT					0x8000
//空闲消息缓存链表，供所有进程分配使用。
struct
{
	struct Message *pHead;
	int flag;							
}mbFree;

#define M_RCV_WANT				0x8000
#define M_SEND_WANT				0x4000
#define M_BITS						(M_RCV_WANT| M_SEND_WANT)
//每个进程的消息队列
struct MsgQueue
{
	Message *pHead;
	Message *pTail;
	int flag;							//标志。同时记录队列中消息个数
};

/*
 * 邮件(mailbox)相关结构
 */
#define MBX_LEN	_PROC					16		//每个进程的信箱容量
#define MAX_MAIL_LEN					256		//每封邮件的最大长度
#define MAIL_BUF_COUNT				64		//邮件缓存总数
#define MBX_COUNT						64		//系统中信箱总数
#define MBX_COUNT_PROC				4		//每个进程最大可拥有的信箱数

#define PMBX							1		

//所有邮件缓存
struct Mail
{
	char data[MAX_MAIL_LEN];
	int len;
	struct Mail *next;
}mailBuf[MAIL_BUF_COUNT];

//空闲邮件缓存链表，供所有进程分配使用。
struct
{
	struct Mail *pHead;
	int flag;
}mailFree;

#define MBX_RCV_WANT				0x8000
#define MBX_SEND_WANT			0x4000
#define  MBX_PRIVATE				0x2000
#define  MBX_SHARED				0x1000
#define  MBX_PUBLIC					0x3000

#define  MBX_BITS			(MBX_RCV_WANT| MBX_SEND_WANT|MBX_ PUBLIC)

//每个进程的邮件队列
struct MailBox
{
	Mail *pHead;
	Mail *pTail;
	char name[14];
	int flag;					//标志，同时还记录邮件个数
	int ref;					//进程引用计数，仅对共享和公共信箱有效
	struct MailBox *next;
}mbxBuf[MBX_COUNT];
 
//空闲邮件缓存链表，供所有进程分配使用。
struct	
{
	struct MailBox *pHead;
	int flag;
}mbxFree;

/*
 * 共享内存(shared memory)相关结构
 */
#define SHARED_MEM_COUNT			16	//系统中共享内存的最大数
#define MEM_MAP_COUNT				64	//映射结构的最大数
#define MEM_MAP_COUNT_PROC			4	//每个进程最大可拥有的共享内存数

#define SM_ALLOC						0x8000
#define SM_LOAD							0x4000
#define PHY_MEM_BLOCK_SIZE			64
struct SharedMemory
{
	char *phyAddr;
	int blocks;
	int ref;
	int refmem;
	int flag;
	char name[14];
	proc *pOwner;
	int permOwn;
	int permOthers;
}shMem[SHARED_MEM_COUNT];

//进程映射共享内存的结构
struct MemMap
{
	SharedMemory *pSM;
	int virtAddr;
	int flag;
}memMap[MEM_MAP_COUNT];

struct MapShm
{
	int *addr;
	int size;
};
