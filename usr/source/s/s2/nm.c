<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/nm.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/nm.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/nm.c>
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
int	fout;
int	cflg;
int	nflg;
int	uflg;
int	rflg	1;
int	gflg;
int	pflg;
struct	nl
{
	char	name[8];
	int	typ;
	int	*val;
} *nlp;
int	fi;
int	buf[8];
main(argc, argv)
char **argv;
{
	int n, i, j;
	int compare();

	if (--argc &gt; 0 &amp;&amp; *argv[1] == '-') {
		argv++;
		while (*++*argv) switch (**argv) {
		case 'n':
			nflg++;
			continue;

		case 'c':
			cflg++;
			continue;

		case 'g':
			gflg++;
			continue;

		case 'u':
			uflg++;
			continue;

		case 'r':
			rflg = -1;
			continue;

		case 'p':
			pflg ++;
			continue;

		default:
			continue;
		}
		argc--;
	}
	if (argc==0)
		fi = open("a.out", 0); else
		fi = open(*++argv, 0);
	if(fi &lt; 0) {
		printf("cannot open input\n");
		exit();
	}
	read(fi, buf, 020);
	if(buf[0]!=0407 &amp;&amp; buf[0]!=0410 &amp;&amp; buf[0]!=0411) {
		printf("bad format\n");
		exit();
	}
	seek(fi, buf[1], 1);		/* text */
	seek(fi, buf[2], 1);		/* data */
	if(buf[7] != 1) {
		seek(fi, buf[1], 1);
		seek(fi, buf[2], 1);	/* reloc */
	}
	n = ldiv(0, buf[4], 12);
	if(n == 0) {
		printf("no name list\n");
		exit();
	}
	nlp = sbrk(12*n);
	read(fi, nlp, n*12);
	if (pflg==0)
		qsort(nlp, n, 12, compare);
	fout = dup(1);
	close(1);
	for(i=0; i&lt;n; i++) {
		if(gflg &amp;&amp; (nlp-&gt;typ&amp;040)==0)
			goto out;
		if(cflg) {
			if(nlp-&gt;name[0] != '_')
				goto out;
			for(j=0; j&lt;7; j++)
				nlp-&gt;name[j] = nlp-&gt;name[j+1];
			nlp-&gt;name[7] = '\0';
		}
		j = nlp-&gt;typ&amp;037;
		if(j &gt; 4)
			j = 1;
		if(j==0 &amp;&amp; nlp-&gt;val)
			j = 5;
		if(uflg &amp;&amp; j!=0)
			goto out;
		if(!uflg) {
			if(j==0)
				printf("      "); else
				printo(nlp-&gt;val);
			printf("%c ", (nlp-&gt;typ&amp;040? "UATDBC":"uatdbc")[j]);
		}
		printf("%.8s\n", nlp);
	out:
		nlp++;
	}
	flush();
}

compare(p1, p2)
struct nl *p1, *p2;
{
	int a, i;

	a = 0;
	if(nflg) {
		if(p1-&gt;val &gt; p2-&gt;val) {
			a = 1;
			goto out;
		}
		if(p1-&gt;val &lt; p2-&gt;val) {
			a = -1;
			goto out;
		}
	}
	for(i=0; i&lt;8; i++)
	if(p1-&gt;name[i] != p2-&gt;name[i]) {
		if(p1-&gt;name[i] &gt; p2-&gt;name[i])
			a = 1; else
			a = -1;
		goto out;
	}
out:
	return(a*rflg);
}

printo(v)
{
	int i;

	printf("%c", v&lt;0?'1':'0');
	for(i=0; i&lt;5; i++) {
		printf("%c", ((v&gt;&gt;12)&amp;7)+'0');
		v =&lt;&lt;3;
	}
}











