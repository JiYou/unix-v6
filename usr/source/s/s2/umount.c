<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/umount.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/umount.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/umount.c>
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
#define	NMOUNT	16
#define	NAMSIZ	32

struct mtab {
	char	file[NAMSIZ];
	char	spec[NAMSIZ];
} mtab[NMOUNT];

main(argc, argv)
char **argv;
{
	register struct mtab *mp;
	register char *p1, *p2;
	int mf;

	sync();
	mf = open("/etc/mtab", 0);
	read(mf, mtab, NMOUNT*2*NAMSIZ);
	if(argc != 2) {
		printf("arg count\n");
		return;
	}
	if (umount(argv[1]) &lt; 0) {
		perror("umount");
		return;
	}
	p1 = argv[1];
	while(*p1++)
		;
	p1--;
	while(*--p1 == '/')
		*p1 = '\0';
	while(p1 &gt; argv[1] &amp;&amp; *--p1 != '/')
		;
	if(*p1 == '/')
		p1++;
	argv[1] = p1;
	for (mp = mtab; mp &lt; &amp;mtab[NMOUNT]; mp++) {
		p1 = argv[1];
		p2 = &amp;mp-&gt;spec[0];
		while (*p1++ == *p2)
			if (*p2++ == 0) {
				for (p1 = mp-&gt;file; p1 &lt; &amp;mp-&gt;file[NAMSIZ*2];)
					*p1++ = 0;
				mp = &amp;mtab[NMOUNT];
				while ((--mp)-&gt;file[0] == 0);
				mf = creat("/etc/mtab", 0644);
				write(mf, mtab, (mp-mtab+1)*2*NAMSIZ);
				return;
			}
	}
	printf("%s not in mount table\n", argv[1]);
}











