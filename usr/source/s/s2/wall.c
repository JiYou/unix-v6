<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/wall.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/wall.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/wall.c>
Find at most <select name="num">
<option value="5">5</option>
<option value="10">10</option>
<option value="20">20</option>
<option value="50">50</option>
<option value="100">100</option>
<option value="500">500</option></select>
related files. <input type="submit" value="Search"><br>
<input type="checkbox" name="incl" value="1">
including files from this version of Unix.
</form><hr>
<pre>
char	mesg[3000];
int	msize;
struct
{
	char	name[8];
	char	tty[2];
	int	time[2];
	int	junk;
} utmp[50];

main(argc, argv)
char *argv[];
{
	register i, *p;
	int f;

	f = open("/etc/utmp", 0);
	if(f &lt; 0) {
		printf("utmp?\n");
		exit();
	}
	read(f, utmp, sizeof utmp);
	close(f);
	f = 0;
	if(argc &gt;= 2) {
		f = open(argv[1], 0);
		if(f &lt; 0) {
			printf("%s?\n", argv[1]);
			exit();
		}
	}
	while((i = read(f, &amp;mesg[msize], sizeof mesg - msize)) &gt; 0)
		msize =+ i;
	close(f);
	for(i=0; i&lt;sizeof utmp/sizeof utmp[0]; i++) {
		p = &amp;utmp[i];
		if(p-&gt;tty[0] == 0)
			continue;
		sleep(1);
		sendmes(p-&gt;tty[0]);
	}
}

sendmes(tty)
{
	register i;
	register char *s;

	i = fork();
	if(i == -1) {
		printf("try again\n");
		return;
	}
	if(i)
		return;
	s = "/dev/ttyx";
	s[8] = tty;
	i = open(s, 1);
	if(i &lt; 0) {
		printf("cannot open tty%c\n", tty);
		exit();
	}
	close(1);
	dup(i);
	printf("Broadcast Message ...\n\n");
	write(1, mesg, msize);
	exit();
}











