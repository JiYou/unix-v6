<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/ls.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/ls.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/ls.c>
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
 * list file or directory
 */

struct {
	int	fdes;
	int	nleft;
	char	*nextc;
	char	buff[512];
} inf;

struct ibuf {
	int	idev;
	int	inum;
	int	iflags;
	char	inl;
	char	iuid;
	char	igid;
	char	isize0;
	int	isize;
	int	iaddr[8];
	char	*iatime[2];
	char	*imtime[2];
};

struct lbuf {
	char	lname[15];
	int	lnum;
	int	lflags;
	char	lnl;
	char	luid;
	char	lgid;
	char	lsize0;
	int	lsize;
	char	*lmtime[2];
};

struct lbufx {
	char	*namep;
};

int	aflg, dflg, lflg, sflg, tflg, uflg, iflg, fflg, gflg;
int	fout;
int	rflg	1;
char	*year;
int	flags;
int	uidfil	-1;
int	lastuid	-1;
char	tbuf[16];
int	tblocks;
int	statreq;
struct	lbuf	*lastp	&amp;end;
struct	lbuf	*rlastp	&amp;end;
char	*dotp	".";

#define	IFMT	060000
#define	DIR	0100000
#define	CHR	020000
#define	BLK	040000
#define	ISARG	01000
#define	LARGE	010000
#define	STXT	010000
#define	SUID	04000
#define	SGID	02000
#define	ROWN	0400
#define	WOWN	0200
#define	XOWN	0100
#define	RGRP	040
#define	WGRP	020
#define	XGRP	010
#define	ROTH	04
#define	WOTH	02
#define	XOTH	01
#define	RSTXT	01000

main(argc, argv)
char **argv;
{
	int i, j;
	extern struct lbuf end;
	register struct lbuf *ep, *ep1;
	register struct lbuf *slastp;
	struct lbuf lb;
	int t;
	int compar();

	fout = dup(1);
	time(lb.lmtime);
	year = lb.lmtime[0] - 245; /* 6 months ago */
	if (--argc &gt; 0 &amp;&amp; *argv[1] == '-') {
		argv++;
		while (*++*argv) switch (**argv) {
		case 'a':
			aflg++;
			continue;

		case 's':
			sflg++;
			statreq++;
			continue;

		case 'd':
			dflg++;
			continue;

		case 'g':
			gflg++;
			continue;

		case 'l':
			lflg++;
			statreq++;
			continue;

		case 'r':
			rflg = -1;
			continue;

		case 't':
			tflg++;
			statreq++;
			continue;

		case 'u':
			uflg++;
			continue;

		case 'i':
			iflg++;
			continue;

		case 'f':
			fflg++;
			continue;

		default:
			continue;
		}
		argc--;
	}
	if (fflg) {
		aflg++;
		lflg = 0;
		sflg = 0;
		tflg = 0;
		statreq = 0;
	}
	if(lflg) {
		t = "/etc/passwd";
		if(gflg)
			t = "/etc/group";
		uidfil = open(t, 0);
	}
	if (argc==0) {
		argc++;
		argv = &amp;dotp - 1;
	}
	for (i=0; i &lt; argc; i++) {
		if ((ep = gstat(*++argv, 1))==0)
			continue;
		ep-&gt;namep = *argv;
		ep-&gt;lflags =| ISARG;
	}
	qsort(&amp;end, lastp - &amp;end, sizeof *lastp, compar);
	slastp = lastp;
	for (ep = &amp;end; ep&lt;slastp; ep++) {
		if (ep-&gt;lflags&amp;DIR &amp;&amp; dflg==0 || fflg) {
			if (argc&gt;1)
				printf("\n%s:\n", ep-&gt;namep);
			lastp = slastp;
			readdir(ep-&gt;namep);
			if (fflg==0)
				qsort(slastp,lastp - slastp,sizeof *lastp,compar);
			if (statreq)
				printf("total %d\n", tblocks);
			for (ep1=slastp; ep1&lt;lastp; ep1++)
				pentry(ep1);
		} else 
			pentry(ep);
	}
	flush();
}

pentry(ap)
struct lbuf *ap;
{
	struct { char dminor, dmajor;};
	register t;
	register struct lbuf *p;
	register char *cp;

	p = ap;
	if (p-&gt;lnum == -1)
		return;
	if (iflg)
		printf("%5d ", p-&gt;lnum);
	if (lflg) {
		pmode(p-&gt;lflags);
		printf("%2d ", p-&gt;lnl);
		t = p-&gt;luid;
		if(gflg)
			t = p-&gt;lgid;
		t =&amp; 0377;
		if (getname(t, tbuf)==0)
			printf("%-6.6s", tbuf);
		else
			printf("%-6d", t);
		if (p-&gt;lflags &amp; (BLK|CHR))
			printf("%3d,%3d", p-&gt;lsize.dmajor&amp;0377,
			    p-&gt;lsize.dminor&amp;0377);
		else
			printf("%7s", locv(p-&gt;lsize0, p-&gt;lsize));
		cp = ctime(p-&gt;lmtime);
		if(p-&gt;lmtime[0] &lt; year)
			printf(" %-7.7s %-4.4s ", cp+4, cp+20); else
			printf(" %-12.12s ", cp+4);
	} else if (sflg)
		printf("%4d ", nblock(p-&gt;lsize0, p-&gt;lsize));
	if (p-&gt;lflags&amp;ISARG)
		printf("%s\n", p-&gt;namep);
	else
		printf("%.14s\n", p-&gt;lname);
}

getname(uid, buf)
int uid;
char buf[];
{
	int j, c, n, i;

	if (uid==lastuid)
		return(0);
	inf.fdes = uidfil;
	seek(inf.fdes, 0, 0);
	inf.nleft = 0;
	lastuid = -1;
	do {
		i = 0;
		j = 0;
		n = 0;
		while((c=getc(&amp;inf)) != '\n') {
			if (c&lt;0)
				return(-1);
			if (c==':') {
				j++;
				c = '0';
			}
			if (j==0)
				buf[i++] = c;
			if (j==2)
				n = n*10 + c - '0';
		}
	} while (n != uid);
	buf[i++] = '\0';
	lastuid = uid;
	return(0);
}

nblock(size0, size)
char *size0, *size;
{
	register int n;

	n = ldiv(size0, size, 512);
	if (size&amp;0777)
		n++;
	if (n&gt;8)
		n =+ (n+255)/256;
	return(n);
}

int	m0[] { 3, DIR, 'd', BLK, 'b', CHR, 'c', '-'};
int	m1[] { 1, ROWN, 'r', '-' };
int	m2[] { 1, WOWN, 'w', '-' };
int	m3[] { 2, SUID, 's', XOWN, 'x', '-' };
int	m4[] { 1, RGRP, 'r', '-' };
int	m5[] { 1, WGRP, 'w', '-' };
int	m6[] { 2, SGID, 's', XGRP, 'x', '-' };
int	m7[] { 1, ROTH, 'r', '-' };
int	m8[] { 1, WOTH, 'w', '-' };
int	m9[] { 1, XOTH, 'x', '-' };
int	m10[] { 1, STXT, 't', ' ' };

int	*m[] { m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10};

pmode(aflag)
{
	register int **mp;

	flags = aflag;
	for (mp = &amp;m[0]; mp &lt; &amp;m[11];)
		select(*mp++);
}

select(pairp)
int *pairp;
{
	register int n, *ap;

	ap = pairp;
	n = *ap++;
	while (--n&gt;=0 &amp;&amp; (flags&amp;*ap++)==0)
		ap++;
	putchar(*ap);
}

makename(dir, file)
char *dir, *file;
{
	static char dfile[100];
	register char *dp, *fp;
	register int i;

	dp = dfile;
	fp = dir;
	while (*fp)
		*dp++ = *fp++;
	*dp++ = '/';
	fp = file;
	for (i=0; i&lt;14; i++)
		*dp++ = *fp++;
	*dp = 0;
	return(dfile);
}

readdir(dir)
char *dir;
{
	static struct {
		int	dinode;
		char	dname[14];
	} dentry;
	register char *p;
	register int j;
	register struct lbuf *ep;

	if (fopen(dir, &amp;inf) &lt; 0) {
		printf("%s unreadable\n", dir);
		return;
	}
	tblocks = 0;//printf
	for(;;) {
		p = &amp;dentry;
		for (j=0; j&lt;16; j++)
			*p++ = getc(&amp;inf);
		if (dentry.dinode==0
		 || aflg==0 &amp;&amp; dentry.dname[0]=='.')
			continue;
		if (dentry.dinode == -1)
			break;
		ep = gstat(makename(dir, dentry.dname), 0);
		if (ep-&gt;lnum != -1)
			ep-&gt;lnum = dentry.dinode;
		for (j=0; j&lt;14; j++)
			ep-&gt;lname[j] = dentry.dname[j];
	}
	close(inf.fdes);
}

gstat(file, argfl)
char *file;
{
	struct ibuf statb;
	register struct lbuf *rep;

	if (lastp+1 &gt;= rlastp) {
		sbrk(512);
		rlastp.idev =+ 512;
	}
	rep = lastp;
	lastp++;
	rep-&gt;lflags = 0;
	rep-&gt;lnum = 0;
	if (argfl || statreq) {
		if (stat(file, &amp;statb)&lt;0) {
			printf("%s not found\n", file);
			statb.inum = -1;
			statb.isize0 = 0;
			statb.isize = 0;
			statb.iflags = 0;
			if (argfl) {
				lastp--;
				return(0);
			}
		}
		rep-&gt;lnum = statb.inum;
		statb.iflags =&amp; ~DIR;
		if ((statb.iflags&amp;IFMT) == 060000) {
			statb.iflags =&amp; ~020000;
		} else if ((statb.iflags&amp;IFMT)==040000) {
			statb.iflags =&amp; ~IFMT;
			statb.iflags =| DIR;
		}
		statb.iflags =&amp; ~ LARGE;
		if (statb.iflags &amp; RSTXT)
			statb.iflags =| STXT;
		statb.iflags =&amp; ~ RSTXT;
		rep-&gt;lflags = statb.iflags;
		rep-&gt;luid = statb.iuid;
		rep-&gt;lgid = statb.igid;
		rep-&gt;lnl = statb.inl;
		rep-&gt;lsize0 = statb.isize0;
		rep-&gt;lsize = statb.isize;
		if (rep-&gt;lflags &amp; (BLK|CHR) &amp;&amp; lflg)
			rep-&gt;lsize = statb.iaddr[0];
		rep-&gt;lmtime[0] = statb.imtime[0];
		rep-&gt;lmtime[1] = statb.imtime[1];
		if(uflg) {
			rep-&gt;lmtime[0] = statb.iatime[0];
			rep-&gt;lmtime[1] = statb.iatime[1];
		}
		tblocks =+ nblock(statb.isize0, statb.isize);
	}
	return(rep);
}

compar(ap1, ap2)
struct lbuf *ap1, *ap2;
{
	register struct lbuf *p1, *p2;
	register int i;
	int j;
	struct { char *charp;};

	p1 = ap1;
	p2 = ap2;
	if (dflg==0) {
		if ((p1-&gt;lflags&amp;(DIR|ISARG)) == (DIR|ISARG)) {
			if ((p2-&gt;lflags&amp;(DIR|ISARG)) != (DIR|ISARG))
				return(1);
		} else {
			if ((p2-&gt;lflags&amp;(DIR|ISARG)) == (DIR|ISARG))
				return(-1);
		}
	}
	if (tflg) {
		i = 0;
		if (p2-&gt;lmtime[0] &gt; p1-&gt;lmtime[0])
			i++;
		else if (p2-&gt;lmtime[0] &lt; p1-&gt;lmtime[0])
			i--;
		else if (p2-&gt;lmtime[1] &gt; p1-&gt;lmtime[1])
			i++;
		else if (p2-&gt;lmtime[1] &lt; p1-&gt;lmtime[1])
			i--;
		return(i*rflg);
	}
	if (p1-&gt;lflags&amp;ISARG)
		p1 = p1-&gt;namep;
	else
		p1 = p1-&gt;lname;
	if (p2-&gt;lflags&amp;ISARG)
		p2 = p2-&gt;namep;
	else
		p2 = p2-&gt;lname;
	for (;;)
		if ((j = *p1.charp++ - *p2.charp++) || p1.charp[-1]==0)
			return(rflg*j);
	return(0);
}











