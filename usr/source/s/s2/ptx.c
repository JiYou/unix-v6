<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/ptx.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/ptx.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/ptx.c>
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
/* permuted title index */

char	*tfil "/tmp/p.tmp";
char	*sw[] {
	"a",
	"an",
	"and",
	"as",
	"for",
	"is",
	"of",
	"on",
	"or",
	"the",
	"to",
	"up",
	0};
char	line[200];
int	ch;
int	ptflg;
int	llen	72;

main(argc, argv)
int argc;
char *argv[];
{
	extern fin, fout;
	extern onintr();
	int f;

	if(argc&gt;1 &amp;&amp; *argv[1]=='-') {
		llen = 100;
		ptflg++;
		argc--;
		argv++;
	}
	if(argc&lt;2) {
		printf("arg count\n");
		exit();
	}
	fin = open(argv[1]);
	if(fin &lt; 0) {
		printf("%s: cannot open\n", argv[1]);
		exit();
	}
	f = creat(tfil, 0600);
	if(f &lt; 0) {
		printf("cannot create %s\n", tfil);
		exit();
	}
	fout = f;
	if ((signal(2, 1) &amp; 01) ==0)
		signal(2, onintr);
	pass1();
	flush();
	close(fin);
	fin = 0;
	close(fout);
	fout = 1;
	f = fork();
	if(f &lt; 0) {
		printf("try again\n");
		exit();
	}
	if(f == 0) {
		execl("/bin/sort", "sort", "-d", "-o", tfil, tfil, 0);
		execl("/usr/bin/sort", "sort", "-d", "-o", tfil, tfil, 0);
		printf("someone moved sort\n");
		exit();
	}
	while(wait() != f);
	fin = open(tfil, 0);
	if(fin &lt; 0) {
		printf("cannot reopen %s\n", tfil);
		exit();
	}
	if (argc&gt;=3)
		f = creat(argv[2], 0666);
	else
		f = dup(1);
	if(f &lt; 0) {
		printf("%s: cannot open\n", argv[2]);
		exit();
	}
	fout = f;
	pass2();
	flush();
	onintr();
}
//rexit
onintr()
{
	unlink(tfil);
	exit();
}

pass1()
{
	int n, c, i, ll, j, k, cc, ccc;

loop:
	if ((c=getchar())=='\0')
		return;
	n = 0;
	i = 0;
	while(c!='\n' &amp;&amp; c!='\0') {
		if(c == '(')
			c = 0177;
		if(c==' ' || c=='\t') {
			i++;
			c = getchar();
			continue;
		}
		if(i) {
			i = 0;
			if(n&lt;=llen) line[n++] = ' ';
		}
		if (n&lt;=llen) line[n++] = c;
		c = getchar();
	}
	ll = n;
	line[n++] = 0;
	i = -1;
l1:
	while((cc=line[++i])==' ');
	n = i;
	j = 0;
	while(sw[j]) {
		i = n;
		k = 0;
		while ((cc=sw[j][k++])==line[i++]);
		if(cc==0 &amp;&amp; ((ccc=line[--i])==' '||ccc==0))
			goto l1;
		j++;
	}
	i = n;
	while (c=line[n++]) putchar(c);
	putchar('~');
	n = 0;
	while (n&lt;i) {
		c = line[n++];
		if (c!=' ' || n!=i)
			putchar(c);
	}
	putchar('\n');
	while((c=line[i++])!=0 &amp;&amp; c!=' ');
	--i;
	if (c) goto l1;
	goto loop;
}

pass2()
{
	int i, n, c, tilde, llen2, nbfore, nafter;


	llen2 = llen/2+6;
loop:
	if ((c=getchar())=='\0')
		return;
	n = nbfore = nafter = 0;
	tilde = -1;
	while(c!='\n' &amp;&amp; c!='\0') {
		if(c == 0177)
			c = '(';
		if (n&lt;=llen) line[n] = c;
		if (c=='~') tilde = n;
		if (tilde&gt;=0) nafter++; else nbfore++;
		n++;
		c = getchar();
	}
	if (tilde&lt;0)
		tilde = n++;
	nafter--;
	if (nbfore&gt;llen2) {
		i = tilde;
		while (nbfore &gt; llen2)
			while(line[--i]!=' ' &amp;&amp; i&gt;=0) nbfore--;
		if (i&lt;0) goto l1;
		line[tilde] = 0200;
		nafter =+ (tilde-i+2);
		tilde = i;
	}
	if (nafter &gt;= llen-llen2) {
		i = tilde;
		while(nafter-- &gt;= llen-llen2)
			while(line[++i]!=' ' &amp;&amp; i&lt;n) nafter--;
		if (i&gt;=n) goto l1;
		line[tilde] = 0200;
		nafter++;
		tilde = i;
	}
l1:
	if(!ptflg) {
		for(i=llen-llen2-nafter; i&gt;=8; i =- 8)
			putchar('\t');
		while(--i&gt;=0)
			putchar(' ');
	} else
		printf(".xx \"");
	i = tilde;
	while (++i&lt;n) p1char(line[i]);
	if(!ptflg)
		printf("  "); else
		printf("\" \"");
	i = -1;
	while(++i&lt;tilde) p1char(line[i]);
	if(ptflg)
		putchar('"');
	putchar('\n');
	goto loop;
}

p1char(c)
{
	if ((c&amp;0377) == 0200) {
		putchar('.');
		putchar('.');
		c = '.';
	}
	putchar(c);
}











