/*
 * This table is the switch used to transfer
 * to the appropriate routine for processing a system call.
 * Each row contains the number of arguments expected
 * and a pointer to the routine.
 */
int	sysent[]
{
	0, &nullsys,			/*  0 = indir */
	0, &rexit,			/*  1 = exit */
	0, &fork,			/*  2 = fork */
	2, &read,			/*  3 = read */
	2, &write,			/*  4 = write */
	2, &open,			/*  5 = open */
	0, &close,			/*  6 = close */
	0, &wait,			/*  7 = wait */
	2, &creat,			/*  8 = creat */
	2, &link,			/*  9 = link */
	1, &unlink,			/* 10 = unlink */
	2, &exec,			/* 11 = exec */
	1, &chdir,			/* 12 = chdir */
	0, &gtime,			/* 13 = time */
	3, &mknod,			/* 14 = mknod */
	2, &chmod,			/* 15 = chmod */
	2, &chown,			/* 16 = chown */
	1, &sbreak,			/* 17 = break */
	2, &stat,			/* 18 = stat */
	2, &seek,			/* 19 = seek */
	0, &getpid,			/* 20 = getpid */
	3, &smount,			/* 21 = mount */
	1, &sumount,			/* 22 = umount */
	0, &setuid,			/* 23 = setuid */
	0, &getuid,			/* 24 = getuid */
	0, &stime,			/* 25 = stime */
	3, &ptrace,			/* 26 = ptrace */
	0, &nosys,			/* 27 = x */
	1, &fstat,			/* 28 = fstat */
	0, &nosys,			/* 29 = x */
	1, &nullsys,			/* 30 = smdate; inoperative */
	1, &stty,			/* 31 = stty */
	1, &gtty,			/* 32 = gtty */
	0, &nosys,			/* 33 = x */
	0, &nice,			/* 34 = nice */
	0, &sslep,			/* 35 = sleep */
	0, &sync,			/* 36 = sync */
	1, &kill,			/* 37 = kill */
	0, &getswit,			/* 38 = switch */
	0, &nosys,			/* 39 = x */
	0, &nosys,			/* 40 = x */
	0, &dup,			/* 41 = dup */
	0, &pipe,			/* 42 = pipe */
	1, &times,			/* 43 = times */
	4, &profil,			/* 44 = prof */
	0, &nosys,			/* 45 = tiu */
	0, &setgid,			/* 46 = setgid */
	0, &getgid,			/* 47 = getgid */
	2, &ssig,			/* 48 = sig */
	0, &nosys,			/* 49 = x */
	0, &CreateSemaphore,			/* 50 = x */
	0, &CloseSemphore,			/* 51 = x */
	0, &AcquireSemphore,			/* 52 = x */
	0, &ReleaseSemphore,			/* 53 = x */
	0, &CreateEvent,			/* 54 = x */
	0, &CloseEvent,			/* 55 = x */
	1, &WaitEvent,			/* 56 = x */
	0, &SetEvent,			/* 57 = x */
	3, &SendMessage,			/* 58 = x */
	3, &ReceiveMessage,			/* 59 = x */
	1, &CreatePrivMbx,			/* 60 = x */
	0, &CloseMbx,			/* 61 = x */
	4, &SendMail,			/* 62 = x */
	3, &ReceiveMail,				/* 63 = x */
	0, &CreateSharedMbx,			/* 64 = x */
	4, &SendSharedMail,			/* 65 = x */
	1, &CreatePublicMbx,			/* 66 = x */
	1, &ClosePublicMbx,			/* 67 = x */
	3, &ReceivePublicMail,			/* 68 = x */
	4, &CreateSharedMemory,			/* 69 = x */
	0, &FreeSharedMemory,			/* 70 = x */
	2, &MapSharedMemory,			/* 71 = x */
	0, &UnmapSharedMemory,			/* 72 = x */
	7, &CreateThread,			/* 73 = x */
	0, &ExitThread,			/* 74 = x */
	0, &gettid,			/* 75 = x */
	0, &SuspendThread,			/* 76 = x */
	0, &ResumeThread,			/* 77 = x */
	1, &TermThread,				/* 78 = x */
	1, &GetThreadID,			/* 79 = x */
	2, &GetThreadName,	/* 80 = x */
	0, &nosys,			/* 81 = x */
	0, &nosys,			/* 82 = x */
	0, &nosys				/* 83 = x */
};