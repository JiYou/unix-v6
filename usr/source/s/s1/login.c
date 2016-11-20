
/*
 * login [ name ]
 */

struct {
	char	name[8];
	char	tty;
	char	ifill;
	int	time[2];
	int	ufill;
} utmp;

struct {
	int	speeds;
	char	erase, kill;
	int	tflags;
} ttyb;

struct {
	int	junk[5];
	int	size;
	int	more[12];
} statb;

char	*ttyx;

#define	ECHO	010

main(argc, argv)
char **argv;
{
	char pbuf[128];
	register char *namep, *np;
	char pwbuf[9];
	int t, sflags, f, c, uid, gid;

	signal(3, 1);
	signal(2, 1);
	nice(0);	//提高优先级
	ttyx = "/dev/ttyx";
	if ((utmp.tty=ttyn(0)) == 'x') {//在/dev下面找不到当前0号句柄文件对应的设备文件"tty*"
		write(1, "Sorry.\n", 7);
		exit();
	}
	ttyx[8] = utmp.tty;	//ttyx = "/dev/tty*"
	gtty(0, &ttyb);
	ttyb.erase = '#';
	ttyb.kill = '@';
	stty(0, &ttyb);
    loop:
	namep = utmp.name;
	if (argc>1) {
		np = argv[1];
		while (namep<utmp.name+8 && *np)
			*namep++ = *np++;
		argc = 0;
	} else {	//只有1个或0个参数,则不包含用户名，所以提示输入用户名
		write(1, "Name: ", 7);
		while ((c = getchar()) != '\n') {
			if (c <= 0)
				exit();
			if (namep < utmp.name+8)
				*namep++ = c;
		}
	}
	while (namep < utmp.name+8)
		*namep++ = ' ';	//如果输入用户名少于8个，则剩余用' '填满
//返回后pbuf应该是用户在/etc/passwd中所在行数据,
//"用户名:加密后的密码:用户ID:组ID:用户所使用(home)目录:shell程序路径"
//如果shell程序为空，则使用默认"/bin/sh"，运行后代替当前进程
	if (getpwentry(utmp.name, pbuf))
		goto bad;
	np = colon(pbuf);//把:变成'\0'，且使np指向密码
	if (*np!=':') {
		sflags = ttyb.tflags;
		ttyb.tflags =& ~ ECHO;	//输入密码时不回显，当然也可改成输出*
		stty(0, &ttyb);
		write(1, "Password: ", 10);
		namep = pwbuf;
		while ((c=getchar()) != '\n') {//fin=0，所以读取标准输入
			if (c <= 0)
				exit();
			if (namep<pwbuf+8)	//如果用户输入的密码超过8个字符，则后续字符忽略
				*namep++ = c;
		}
		*namep++ = '\0';
		ttyb.tflags = sflags;
		stty(0, &ttyb);
		write(1, "\n", 1);
		namep = crypt(pwbuf);
		while (*namep++ == *np++);
		if (*--namep!='\0' || *--np!=':')
			goto bad;
	}
	np = colon(np);
	uid = 0;
	while (*np != ':')
		uid = uid*10 + *np++ - '0';
	np++;
	gid = 0;
	while (*np != ':')
		gid = gid*10 + *np++ - '0';
	np++;
	np = colon(np);
	namep = np;
	np = colon(np);
	if (chdir(namep)<0) {
		write(1, "No directory\n", 13);
		goto loop;
	}
	time(utmp.time);
	if ((f = open("/etc/utmp", 1)) >= 0) {
		t = utmp.tty;
		if (t>='a')
			t =- 'a' - (10+'0');
		seek(f, (t-'0')*16, 0);
		write(f, &utmp, 16);
		close(f);
	}
	if ((f = open("/usr/adm/wtmp", 1)) >= 0) {
		seek(f, 0, 2);
		write(f, &utmp, 16);
		close(f);
	}
	if ((f = open("/etc/motd", 0)) >= 0) {
		while(read(f, &t, 1) > 0)
			write(1, &t, 1);
		close(f);
	}
	if(stat(".mail", &statb) >= 0 && statb.size)
		write(1, "You have mail.\n", 15);
	chown(ttyx, uid);
	setgid(gid);
	setuid(uid);
	if (*np == '\0')
		np = "/bin/sh";
	execl(np, "-", 0);
	write(1, "No shell.\n", 9);
	exit();
bad:
	write(1, "Login incorrect.\n", 17);
	goto loop;
}

getpwentry(name, buf)
char *name, *buf;
{
	extern fin;
	int fi, r, c;
	register char *gnp, *rnp;

	fi = fin;
	r = 1;
	if((fin = open("/etc/passwd", 0)) < 0)
		goto ret;
loop:
	gnp = name;
	rnp = buf;
	while((c=getchar()) != '\n') {	//getchr.s
		if(c <= 0)
			goto ret;
		*rnp++ = c;
	}
	*rnp++ = '\0';
	rnp = buf;
	while (*gnp++ == *rnp++);
	//第一个判断成立表明至少name[7]不是' '，而且它不和rnp[7]相等
	//第二个判断成立表明buf[]和它第一个不同的字符不是:
	//不管何种情况，用户名和:前的名称不等，则继续下一行查找
	//所以/etc/passwd的每一行应该是"用户名:密码"
	if ((*--gnp!=' ' && gnp<name+8) || *--rnp!=':')	
		goto loop;
	r = 0;
ret:
	close(fin);
	fin = 0;
	(&fin)[1] = 0;
	(&fin)[2] = 0;//fout	?
	return(r);
}

colon(p)
char *p;
{
	register char *rp;

	rp = p;
	while (*rp != ':') {
		if (*rp++ == '\0') {
			write(1, "Bad /etc/passwd\n", 16);
			exit();
		}
	}
	*rp++ = '\0';
	return(rp);
}











