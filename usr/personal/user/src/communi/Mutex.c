#include "../../include/comm.h"
int CreateMutex()
{
		return CreateSemphore(1);
}

int CloseMutex(int md)
{
	return CloseSemphore(md);
}

int AcquireMutex(int md)
{
	return AcquireSemphore(md);
}

int ReleaseMutex(int md)
{
	return ReleaseSemphore(md);
}


