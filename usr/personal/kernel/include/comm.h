/*
 * �����ź�����ؽṹ
 */
#define MAX_SEMA				64
#define S_WANT					0x8000
#define S_ALLOC					0x4000
#define S_BITS 			    (S_WANT|S_ALLOC)
#define PSEMA					1

struct Semaphore
{
	int s;
	int ref;			//�������ü���
	int flag;
};

struct	Semaphs
{
	struct Semaphore semas[MAX_SEMA]
	int flag;
}semaphs;

/*
 * �����¼���ؽṹ
 */
#define MAX_EVENTS				64
	
#define PEVENT					1

#define EEVENTSET				42

struct Event
{
	BOOL e;
	int ref;			//�������ü���
	int flag;
};

struct	Evts
{
	struct Event events[MAX_EVENTS]
	int flag;
}evts;

/*
 * ������Ϣ������ؽṹ
 */
#define MSG_QUEUE_LEN					16		//ÿ�����̵���Ϣ���г���
#define MAX_MSG_LEN					32		//ÿ����Ϣ����󳤶�
#define MSG_BUF_COUNT					128		//��Ϣ��������
#define PMSG								1		//���̹������ȼ�

//������Ϣ����
struct Message
{
	char data[MAX_MSG_LEN];
	int len;
	struct Message *next;
}msgBuf[MSG_BUF_COUNT];

#define 	MB_WANT					0x8000
//������Ϣ�������������н��̷���ʹ�á�
struct
{
	struct Message *pHead;
	int flag;							
}mbFree;

#define M_RCV_WANT				0x8000
#define M_SEND_WANT				0x4000
#define M_BITS						(M_RCV_WANT| M_SEND_WANT)
//ÿ�����̵���Ϣ����
struct MsgQueue
{
	Message *pHead;
	Message *pTail;
	int flag;							//��־��ͬʱ��¼��������Ϣ����
};

/*
 * �ʼ�(mailbox)��ؽṹ
 */
#define MBX_LEN	_PROC					16		//ÿ�����̵���������
#define MAX_MAIL_LEN					256		//ÿ���ʼ�����󳤶�
#define MAIL_BUF_COUNT				64		//�ʼ���������
#define MBX_COUNT						64		//ϵͳ����������
#define MBX_COUNT_PROC				4		//ÿ����������ӵ�е�������

#define PMBX							1		

//�����ʼ�����
struct Mail
{
	char data[MAX_MAIL_LEN];
	int len;
	struct Mail *next;
}mailBuf[MAIL_BUF_COUNT];

//�����ʼ��������������н��̷���ʹ�á�
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

//ÿ�����̵��ʼ�����
struct MailBox
{
	Mail *pHead;
	Mail *pTail;
	char name[14];
	int flag;					//��־��ͬʱ����¼�ʼ�����
	int ref;					//�������ü��������Թ���͹���������Ч
	struct MailBox *next;
}mbxBuf[MBX_COUNT];
 
//�����ʼ��������������н��̷���ʹ�á�
struct	
{
	struct MailBox *pHead;
	int flag;
}mbxFree;

/*
 * �����ڴ�(shared memory)��ؽṹ
 */
#define SHARED_MEM_COUNT			16	//ϵͳ�й����ڴ�������
#define MEM_MAP_COUNT				64	//ӳ��ṹ�������
#define MEM_MAP_COUNT_PROC			4	//ÿ����������ӵ�еĹ����ڴ���

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

//����ӳ�乲���ڴ�Ľṹ
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
