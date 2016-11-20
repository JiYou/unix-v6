main(argc, argv)
char *argv[];
{
	char *arg;
	int fflg, rflg;

	fflg = 0;
	rflg = 0;
	while(--argc &gt; 0) {

		arg = *++argv;
		if(arg[0] == '-') {
			if(arg[1] == 'f') {
				fflg++;
				continue;
			}
			if(arg[1] == 'r') {
				rflg++;
				continue;
			}
		}

		rm(arg, fflg, rflg);
	}
}

struct stbuf	{
	int dev;
	int inum;
	int mode;
	char nlink;
	char uid;
	char gid;
	char siz0;
	char siz1;
	int addr[8];
	int adate[2];
	int mdate[2];
	};

rm(arg, fflg, rflg)
char arg[];
{
	char *p;
	int buf[20];
	int i, b;

	if(stat(arg, buf)) {
		printf("%s: non existent\n", arg);
		return;
	}
	if((buf-&gt;mode &amp; 060000) == 040000)	{
		if(rflg) {
			i = fork();
			if(i == -1) {
				printf("%s: try again\n", arg);
				return;
			}
			if(i) {
				while(wait() != i);
				return;
			}
			if(chdir(arg)) {
				printf("%s: cannot chdir\n", arg);
				exit();
			}
			p = 0;
			execl("/etc/glob", "glob", "rm", "-r",
				fflg? "-f": "*", fflg? "*": p, 0);
			printf("%s: no glob\n", arg);
			exit();
		}
		printf("%s: directory\n", arg);
		return;
	}

	if(!fflg) {

		if((getuid()&amp;0377) == buf-&gt;uid)
			b = 0200; else
			b = 2;
		if((buf-&gt;mode &amp; b) == 0 &amp;&amp; ttyn(0) != 'x') {
			printf("%s: %o mode ", arg, buf-&gt;mode);
			i = b = getchar();
			i = b;
			while(b != '\n' &amp;&amp; b != '\0')
				b = getchar();
			if(i != 'y')
				return;
		}
	}
	if(unlink(arg))
		printf("%s: not removed\n", arg);
}

putchar(c)
{
	write(1, &amp;c, 1);
}

getchar()
{
	char c;

	if(read(0, &amp;c, 1) != 1) return(0);
	return(c);
}











