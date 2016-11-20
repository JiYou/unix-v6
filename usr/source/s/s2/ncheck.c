<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/ncheck.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/ncheck.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/ncheck.c>
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

char	*dargv[]
{
	"/dev/rrk2",
	"/dev/rrp0",
	0
};

#define NINODE	16*16
#include "/usr/sys/ino.h"
#include "/usr/sys/filsys.h"

struct	filsys	sblock;
struct	inode	inode[NINODE];

int	sflg;
int	aflg;
#define	NI	20
#define	NDIRS	787

int	ilist[NI] { -1};
int	fi;
struct	htab {
	int	hino;
	int	hpino;
	char	hname[14];
} htab[NDIRS];
int	nhent	10;
int	(*pass[])()	{ pass1, pass2, pass3 };
char	*lasts;
int	ino;
int	nerror;
int	nffil;
int	fout;
int	nfiles;
struct dir {
	int	ino;
	char	name[14];
};

main(argc, argv)
char **argv;
{
	register char **p;
	register int n, *lp;

	nffil = dup(1);
	if (argc == 1) {
		for (p = dargv; *p;)
			check(*p++);
		return(nerror);
	}
	while (--argc) {
		argv++;
		if (**argv=='-') switch ((*argv)[1]) {
		case 's':
			sflg++;
			continue;

		case 'a':
			aflg++;
			continue;

		case 'i':
			lp = ilist;
			while (lp &lt; &amp;ilist[NI-1] &amp;&amp; (n = number(argv[1]))) {
				*lp++ = n;
				argv++;
				argc--;
			}
			*lp++ = -1;
			continue;

		default:
			printf2("Bad flag\n");
		}
		check(*argv);
	}
	return(nerror);
}

check(file)
char *file;
{
	register i, j, pno;

	fi = open(file, 0);
	if (fi &lt; 0) {
		printf2("cannot open %s\n", file);
		return;
	}
	printf2("%s:\n", file);
	sync();
	bread(1, &amp;sblock, 512);
	nfiles = sblock.s_isize*16;
	for (i=0; i&lt;NDIRS; i++)
		htab[i].hino = 0;
	fout = nffil;
	flush();
	for (pno=0; pno&lt;3; pno++) {
		ino = 0;
		for (i=0; ino&lt;nfiles; i =+ NINODE/16) {
			bread(i+2, inode, sizeof inode);
			for (j=0; j&lt;NINODE &amp;&amp; ino&lt;nfiles; j++) {
				ino++;
				(*pass[pno])(&amp;inode[j]);
			}
		}
	}
	flush();
	fout = 1;
}

pass1(ip)
{
	if ((ip-&gt;i_mode&amp;IALLOC)==0 || (ip-&gt;i_mode&amp;IFMT)!=IFDIR)
		return;
	lookup(ino, 1);
}

pass2(ip)
struct inode *ip;
{
	register doff;
	register struct htab *hp;
	register struct dir *dp;
	int i;

	if ((ip-&gt;i_mode&amp;IALLOC)==0 || (ip-&gt;i_mode&amp;IFMT)!=IFDIR)
		return;
	doff = 0;
	while (dp = dread(ip, doff)) {
		doff =+ 16;
		if (dp-&gt;ino==0)
			continue;
		if ((hp = lookup(dp-&gt;ino, 0)) == 0)
			continue;
		if (dotname(dp))
			continue;
		hp-&gt;hpino = ino;
		for (i=0; i&lt;14; i++)
			hp-&gt;hname[i] = dp-&gt;name[i];
	}
}

pass3(ip)
struct inode *ip;
{
	register doff;
	register struct dir *dp;
	register int *ilp;

	if ((ip-&gt;i_mode&amp;IALLOC)==0 || (ip-&gt;i_mode&amp;IFMT)!=IFDIR)
		return;
	doff = 0;
	while (dp = dread(ip, doff)) {
		doff =+ 16;
		if (dp-&gt;ino==0)
			continue;
		if (aflg==0 &amp;&amp; dotname(dp))
			continue;
		for (ilp=ilist; *ilp &gt;= 0; ilp++)
			if (*ilp == dp-&gt;ino)
				break;
		if (ilp &gt; ilist &amp;&amp; *ilp!=dp-&gt;ino)
			continue;
		printf("%d	", dp-&gt;ino);
		pname(ino, 0);
		printf("/%.14s\n", dp-&gt;name);
	}
}

dotname(adp)
{
	register struct dir *dp;

	dp = adp;
	if (dp-&gt;name[0]=='.')
		if (dp-&gt;name[1]==0 || dp-&gt;name[1]=='.' &amp;&amp; dp-&gt;name[2]==0)
			return(1);
	return(0);
}

pname(i, lev)
{
	register struct htab *hp;

	if (i==1)
		return;
	if ((hp = lookup(i, 0)) == 0) {
		printf("???");
		return;
	}
	if (lev &gt; 10) {
		printf("...");
		return;
	}
	pname(hp-&gt;hpino, ++lev);
	printf("/%.14s", hp-&gt;hname);
}

lookup(i, ef)
{
	register struct htab *hp;

	for (hp = &amp;htab[i%NDIRS]; hp-&gt;hino;) {
		if (hp-&gt;hino==i)
			return(hp);
		if (++hp &gt;= &amp;htab[NDIRS])
			hp = htab;
	}
	if (ef==0)
		return(0);
	if (++nhent &gt;= NDIRS) {
		printf2("Out of core-- increase NDIRS\n");
		flush();
		exit(1);
	}
	hp-&gt;hino = i;
	return(hp);
}

dread(aip, aoff)
{
	register b, off;
	register struct inode *ip;
	static ibuf[256];
	static char buf[512];

	off = aoff;
	ip = aip;
	if ((off&amp;0777)==0) {
		if (off==0177000) {
			printf2("Monstrous directory %l\n", ino);
			return(0);
		}
		if ((ip-&gt;i_mode&amp;ILARG)==0) {
			if (off&gt;=010000 || (b = ip-&gt;i_addr[off&gt;&gt;9])==0)
				return(0);
			bread(b, buf, 512);
		} else {
			if (off==0) {
				if (ip-&gt;i_addr[0]==0)
					return(0);
				bread(ip-&gt;i_addr[0], ibuf, 512);
			}
			if ((b = ibuf[(off&gt;&gt;9)&amp;0177])==0)
				return(0);
			bread(b, buf, 512);
		}
	}
	return(&amp;buf[off&amp;0777]);
}

bread(bno, buf, cnt)
{

	seek(fi, bno, 3);
	if (read(fi, buf, cnt) != cnt) {
		printf2("read error %d\n", bno);
		exit();
	}
}

bwrite(bno, buf)
{

	seek(fi, bno, 3);
	if (write(fi, buf, 512) != 512) {
		printf2("write error %d\n", bno);
		exit();
	}
}

number(as)
char *as;
{
	register n, c;
	register char *s;

	s = as;
	n = 0;
	while ((c = *s++) &gt;= '0' &amp;&amp; c &lt;= '9') {
		n = n*10+c-'0';
	}
	return(n);
}

printf2(s, a1, a2)
{
	extern fout;

	flush();
	fout = 2;
	printf(s, a1, a2);
	fout = nffil;
	flush();
}











