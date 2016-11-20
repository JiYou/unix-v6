<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/who.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/who.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/who.c>
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
/*
 * who
 */

int	fout;
int	buf[256];

main(argc, argv)
char **argv;
{
	char *s, *cbuf;
	int n, fi, i;
	int tty;
	struct {
		char name[8];
		char tty;
		char pad1;
		int time[2];
		char pad2[2];
	} *p;

	s = "/etc/utmp";
	if(argc == 2)
		s = argv[1];
	fi = open(s, 0);
	if(fi &lt; 0) {
		write("cannot open wtmp\n", 17);
		exit();
	}
	fout = dup(1);
	close(1);
	if (argc==3)
		tty = ttyn(0);

loop:
	n = read(fi, buf, 512);
	if(n == 0) {
		flush();
		if (argc==3)
			write(fout, "Nobody.\n", 8);
		exit();
	}

	p = &amp;buf;
	for(p = &amp;buf; (n =- 16)&gt;=0; p++) {
		if (argc==3 &amp;&amp; tty!=p-&gt;tty)
			continue;
		if(p-&gt;name[0] == '\0' &amp;&amp; argc==1)
			continue;
		for(i=0; i&lt;8; i++) {
			if(p-&gt;name[i] == '\0')
				p-&gt;name[i] = ' ';
			putchar(p-&gt;name[i]);
		}
		for(i=0; i&lt;3; i++)
			putchar("tty"[i]);
		putchar(p-&gt;tty);
		cbuf = ctime(p-&gt;time);
		for(i=3; i&lt;16; i++)
			putchar(cbuf[i]);
		putchar('\n');
		if (argc==3) {
			flush();
			exit();
		}
	}
	goto loop;
}











