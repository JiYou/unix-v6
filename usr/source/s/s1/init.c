#define	tabsize	20
#define	all	p = &itab[0]; p < &itab[20]; p++
#define	ever	;;
#define	single	0173030
#define	reboot	0173040
char	shell[]	"/bin/sh";
char	minus[]	"-";
char	runc[]	"/etc/rc";
char	init[]	"/etc/init";
char	ifile[]	"/etc/ttys";
char	utmp[]	"/etc/utmp";
char	wtmpf[]	"/usr/adm/wtmp";
char	ctty[]	"/dev/tty8";
int	fi;
struct
{
	int	flag;
	int	line;
	char	coms[2];
} line;
struct	tab
{
	int	pid;
	int	line;	//终端号, ttyx中的x。
	int	comn; //终端类型
} itab[tabsize];

struct {
	char	name[8];
	char	tty;
	char	fill;
	int	time[2];
	int	wfill;
} wtmp;

main()
{
	register i;
	register struct tab *p, *q;
	int reset();

	/*
	 * if not single user,
	 * run shell sequence
	 */

	if(getcsw() != single) {//SW
		i = fork();
		if(i == 0) {//这里是子进程
			open("/", 0);
			dup(0);
			dup(0);
			execl(shell, shell, runc, 0);
			exit();
		}
		while(wait() != i);//父进程等待sh退出
		close(creat(utmp, 0644)); //清除utmp文件内容
		if ((i = open(wtmpf, 1)) >= 0) {
			seek(i, 0, 2);	//at the end of file
			wtmp.tty = '~';//从文件尾处写入:用户名+ ~(tty)+时间
			time(wtmp.time);
			write(i, &wtmp, 16);
			close(i);
		}
	}

	/*
	 * main loop for hangup signal
	 * close all files and
	 * check switches for magic values
	 */

	setexit();
	signal(1, reset);
	for(i=0; i<10; i++)
		close(i);
	switch(getcsw()) {

	case single:
	error:
		termall();
		i = fork();
		if(i == 0) {
			open(ctty, 2); //打开控制台
			dup(0);
			execl(shell, minus, 0); //期望用户在shell下编辑/etc/ttys文件，然后退出
			exit();
		}
		while(wait() != i);

	case reboot:
		termall();
		execl(init, minus, 0);
		reset();
	}

	/*
	 * open and merge in init file
	 */

	fi = open(ifile, 0);//从/etc/ttys中读入内容
	q = &itab[0];
	while(rline()) {// /etc/ttys中必须是4个字节一组排列: flag, line, comn, '\n'       ------itab最后一个
		if(line.flag == '0')			//0不运行				   flag2, line2, comn2, '\n'
			continue;
		for(all)
			if(p->line==line.line || p->line==0) {//如果找到一个空的结构或之前已运行tty-line的结构
				if(p >= q) {					//则把它替换成当前的q结构，这样可用q来记录新tty-line
					i = p->pid;
					p->pid = q->pid;
					q->pid = i;
					p->line = q->line;
					p->comn = q->comn;
					q->line = line.line;
					q->coms[0] = line.comn;	//只保留line.comn低字节，因为从/etc/ttys中读出时就是单字节
					q++;
				}
				break;
			}
	}//顺序排列从/etc/ttys中读出的内容到itab中。
	close(fi);
	if(q == &itab[0])
		goto error;
	for(; q < &itab[tabsize]; q++)//终止其他所有不是从ifile中读出的进程。
		term(q);
	for(all)
		if(p->line != 0 && p->pid == 0)
			dfork(p);
	for(ever) {
		i = wait();
		for(all)
			if(p->pid == i) {
				rmut(p);
				dfork(p);
			}
	}
}

termall()
{
	register struct tab *p;

	for(all)
		term(p);
}

term(ap)
struct tab *ap;
{
	register struct tab *p;

	p = ap;
	if(p->pid != 0) {
		rmut(p);
		kill(p->pid, 9);
	}
	p->pid = 0;
	p->line = 0;
}

rline()
{
	static char c[4];

	if(read(fi, c, 4) != 4 || c[3] != '\n')
		return(0);
	line.flag = c[0];
	line.line = c[1];
	line.comn = c[2];
	return(1);
}

dfork(ap)
struct tab *ap;
{
	register i;
	register char *tty;
	register struct tab *p;

	p = ap;
	i = fork();
	if(i == 0) {
		signal(1, 0);
		tty = "/dev/ttyx";
		tty[8] = p->line;
		chown(tty, 0);
		chmod(tty, 0622);
		open(tty, 2);
		dup(0);
		execl("etc/getty", minus, p->coms, 0);	//p->coms = &p->comn，不太严谨
		exit();
	}
	p->pid = i;
}

rmut(p)
struct tab *p;
{
	register i, f;
	static char zero[16];

	f = open(utmp, 1);
	if(f >= 0) {
		i = p->line;
		if(i >= 'a')
			i =+ '0' + 10 - 'a';
		seek(f, (i-'0')*16, 0);
		write(f, zero, 16);
		close(f);
	}
	f = open(wtmpf, 1);
	if (f >= 0) {
		wtmp.tty = p->line;
		time(wtmp.time);
		seek(f, 0, 2);
		write(f, &wtmp, 16);
		close(f);
	}
}










