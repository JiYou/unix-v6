<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/bcd.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/bcd.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/bcd.c>
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
int chtab[]{
00000, /*   */
03004, /* ! */
02404, /* " */
02040, /* sharp */
02042, /* $ */
02104, /* % */
00001, /* &amp; */
03002, /* ' */
02201, /* ( */
02202, /* ) */
02102, /* * */
00005, /* + */
02044, /* , */
00002, /* - */
02041, /* . */
00014, /* / */
00004, /* 0 */
00010, /* 1 */
00020, /* 2 */
00040, /* 3 */
00100, /* 4 */
00200, /* 5 */
00400, /* 6 */
01000, /* 7 */
02000, /* 8 */
04000, /* 9 */
02200, /* : */
02402, /* ; */
02401, /* &lt; */
02204, /* = */
02400, /* &gt; */
03000, /* ? */
02100, /* at */
 011,
 021,
 041,
0101,
0201,
0401,
01001,
02001,
04001,
012,
022,
042,
0102,
0202,
0402,
01002,
02002,
02002,
024,
044,
0104,
0204,
0404,
01004,
02004,
04004,
02020, /* [ */
03001, /* \ */
02101, /* ] */
00006, /* ^ */
02024 /* _ */
};
	char s[128];
	char *sp {&amp;s[0]};
main(argc, argv)
char *argv[];
{
	char *spp;
	int i;
	int j;
	int c;
	int l;

	if (argc&lt;2) {
		puts("% ");
		while ((c=getchar())!='\0'&amp;c!='\n')
			*sp++ = c;
		*sp = 0;
		sp = &amp;s[0];
	} else
		sp = *++argv;
	puts("\n\n\n\n");
	puts(" ________________________________");
	puts("________________\n");
	spp = sp;
	while(*spp++);
	spp--;
	l = spp - sp;
	putchar('/');
	puts(sp);
	i = 49 - l;
	while(--i&gt;0) putchar(' ');
	puts("|\n");
	j = 0;
	spp = sp;
	while (j++&lt;12) {
		putchar('|');
		i = 0;
		spp = sp;
		while (i&lt;48) {
			c = *spp++ - 040;
			i++;
			if (c&gt;='a'-040) c = c - 040;
			if (i&gt;l) c = 0;
			if (c&lt;0 | c&gt;137) c = 0;
			if ((chtab[c]&gt;&gt;(j-1))&amp;1) 
				puts("[]");
			else
				putchar(j&gt;3?'0'+j-3:' ');
		}
		puts("|\n");
	}
	putchar('|');
	puts("____________");
	puts("____________________________________");
	puts("|\n");
	puts("\n\n\n\n");
}

puts(ss) char *ss; {
	int i;
	char t;
	i = 0;
	while(t = *ss++) {
		if(t &gt;= 'a' &amp;&amp; t &lt;= 'z')
			t =+ 'A'-'a';
		putchar(t);
	}
}











