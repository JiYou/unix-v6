<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/ps.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/ps.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/ps.c>
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
#

/*
 *	ps - process status
 *	examine and print certain things about processes
 */

#include "/usr/sys/param.h"
#include "/usr/sys/proc.h"
#include "/usr/sys/tty.h"
#include "/usr/sys/user.h"

struct {
	char name[8];
	int  type;
	char  *value;
} nl[3];

struct proc proc[NPROC];
struct tty tty;
struct user u;
int	lflg;
int	kflg;
int	xflg;
int	tflg;
int	aflg;
int	mem;
int	swap;

int	stbuf[257];
int	ndev;
char	devc[65];
int	devl[65];
int	devt[65];
char	*coref;
struct ibuf {
	char	idevmin, idevmaj;
	int	inum;
	int	iflags;
	char	inl;
	char	iuid;
	char	igid;
	char	isize0;
	int	isize;
	int	iaddr[8];
	char	*ictime[2];
	char	*imtime[2];
	int	fill;
};
int	obuf[259];


main(argc, argv)
char **argv;
{
	struct proc *p;
	int n, b;
	int i, c, mtty;
	char *ap;
	int uid, puid;

	obuf[0] = 1;
	if (argc&gt;1) {
		ap = argv[1];
		while (*ap) switch (*ap++) {
		case 'a':
			aflg++;
			break;

		case 't':
			tflg++;
			break;

		case 'x':
			xflg++;
			break;

		case 'l':
			lflg++;
			break;

		case 'k':
			kflg++;
			break;

		}
	}

	if(chdir("/dev") &lt; 0) {
		printf("cannot change to /dev\n");
		done();
	}
	setup(&amp;nl[0], "_proc");
	setup(&amp;nl[1], "_swapdev");
	nlist(argc&gt;2? argv[2]:"/unix", nl);
	if (nl[0].type==0) {
		printf("No namelist\n");
		return;
	}
	coref = "/dev/mem";
	if(kflg)
		coref = "/usr/sys/core";
	if ((mem = open(coref, 0)) &lt; 0) {
		printf("No mem\n");
		done();
	}
	seek(mem, nl[1].value, 0);
	read(mem, &amp;nl[1].value, 2);
	seek(mem, nl[0].value, 0);
	read(mem, proc, sizeof proc);
	getdev();
	mtty = ttyn(0);
	uid = getuid() &amp; 0377;
	if(lflg)
	printf("TTY F S UID   PID PRI ADDR  SZ  WCHAN COMMAND\n"); else
		printf("TTY  PID COMMAND\n");
	for (i=0; i&lt;NPROC; i++) {
		if (proc[i].p_stat==0)
			continue;
		if (proc[i].p_ttyp==0) {
			if (xflg==0)
				continue;
			c = '?';
		} else {
			for(c=0; c&lt;ndev; c++)
			if(devt[c] == proc[i].p_ttyp) {
				c = devc[c];
				goto out;
			}
			seek(mem, proc[i].p_ttyp, 0);
			read(mem, &amp;tty, sizeof tty);
			for(c=0; c&lt;ndev; c++)
			if(devl[c] == tty.t_dev) {
				devt[c] = proc[i].p_ttyp;
				c = devc[c];
				goto out;
			}
			c = '?';
		out:;
		}
		puid = proc[i].p_uid &amp; 0377;
		if (uid != puid &amp;&amp; aflg==0)
			continue;
		if (lflg || c!=mtty)
			printf("%c:", c);
		else
			printf("  ");
		if (lflg) {
			printf("%3o %c%4d", proc[i].p_flag,
				"0SWRIZT"[proc[i].p_stat], puid);
		}
		printf("%6l", proc[i].p_pid);
		if (lflg) {
			printf("%4d%5o%4d", proc[i].p_pri, proc[i].p_addr,
				(proc[i].p_size+7)&gt;&gt;3);
			if (proc[i].p_wchan)
				printf("%7o", proc[i].p_wchan); else
				printf("       ");
		}
		if (proc[i].p_stat==5)
			printf(" &lt;defunct&gt;");
		else
			prcom(i);
		printf("\n");
	}
	done();
}

getdev()
{
	register struct { int dir_ino; char dir_n[14]; } *p;
	register i, c;
	int f;
	char dbuf[512];
	int sbuf[20];

	f = open("/dev");
	if(f &lt; 0) {
		printf("cannot open /dev\n");
		done();
	}
	swap = -1;
	c = 0;

loop:
	i = read(f, dbuf, 512);
	if(i &lt;= 0) {
		close(f);
		if(swap &lt; 0) {
			printf("no swap device\n");
			done();
		}
		ndev = c;
		return;
	}
	while(i &lt; 512)
		dbuf[i++] = 0;
	for(p = dbuf; p &lt; dbuf+512; p++) {
		if(p-&gt;dir_ino == 0)
			continue;
		if(p-&gt;dir_n[0] == 't' &amp;&amp;
		   p-&gt;dir_n[1] == 't' &amp;&amp;
		   p-&gt;dir_n[2] == 'y' &amp;&amp;
		   p-&gt;dir_n[4] == 0 &amp;&amp;
		   p-&gt;dir_n[3] != 0) {
			if(stat(p-&gt;dir_n, sbuf) &lt; 0)
				continue;
			devc[c] = p-&gt;dir_n[3];
			devl[c] = sbuf-&gt;iaddr[0];
			c++;
			continue;
		}
		if(swap &gt;= 0)
			continue;
		if(stat(p-&gt;dir_n, sbuf) &lt; 0)
			continue;
		if((sbuf-&gt;iflags &amp; 060000) != 060000)
			continue;
		if(sbuf-&gt;iaddr[0] == nl[1].value)
			swap = open(p-&gt;dir_n, 0);
	}
	goto loop;
}

setup(p, s)
char *p, *s;
{
	while (*p++ = *s++);
}

prcom(i)
{
	int baddr, laddr, mf;
	register int *ip;
	register char *cp, *cp1;
	int c, nbad;

	baddr = 0;
	laddr = 0;
	if (proc[i].p_flag&amp;SLOAD) {
		laddr = proc[i].p_addr;
		mf = mem;
	} else {
		baddr = proc[i].p_addr;
		mf = swap;
	}
	laddr =+ proc[i].p_size - 8;
	baddr =+ laddr&gt;&gt;3;
	laddr = (laddr&amp;07)&lt;&lt;6;
	seek(mf, baddr, 3);
	seek(mf, laddr, 1);
	if (read(mf, stbuf, 512) != 512)
		return(0);
	for (ip = &amp;stbuf[256]; ip &gt; &amp;stbuf[0];) {
		if (*--ip == -1) {
			cp = ip+1;
			if (*cp==0)
				cp++;
			nbad = 0;
			for (cp1 = cp; cp1 &lt; &amp;stbuf[256]; cp1++) {
				c = *cp1;
				if (c==0)
					*cp1 = ' ';
				else if (c &lt; ' ' || c &gt; 0176) {
					if (++nbad &gt;= 5) {
						*cp1++ = ' ';
						break;
					}
					*cp1 = '?';
				}
			}
			while (*--cp1==' ')
				*cp1 = 0;
			printf(lflg?" %.16s":" %.64s", cp);
			return(1);
		}
	}
	return(0);
}

done()
{

	fflush(obuf);
	exit();
}

putchar(c)
{

	putc(c, obuf);
}











