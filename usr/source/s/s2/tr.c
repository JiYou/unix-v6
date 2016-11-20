<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/tr.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/tr.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/tr.c>
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
int dflag 0;
int sflag 0;
int cflag 0;
int save 0;
char code[256];
char squeez[256];
char vect[256];
struct string { int last, max, rep; char *p; } string1, string2;
int inbuf[259];

main(argc,argv)
char **argv;
{
	int i, j;
	int c, d;
	char *compl;
	extern fout;

	string1.last = string2.last = 0;
	string1.max = string2.max = 0;
	string1.rep = string2.rep = 0;
	string1.p = string2.p = "";

	if(--argc&gt;0) {
		argv++;
		if(*argv[0]=='-'&amp;&amp;argv[0][1]!=0) {
			while(*++argv[0])
				switch(*argv[0]) {
				case 'c':
					cflag++;
					continue;
				case 'd':
					dflag++;
					continue;
				case 's':
					sflag++;
					continue;
				}
			argc--;
			argv++;
		}
	}
	if(argc&gt;0) string1.p = argv[0];
	if(argc&gt;1) string2.p = argv[1];
	for(i=0; i&lt;256; i++)
		code[i] = vect[i] = 0;
	if(cflag) {
		while(c = next(&amp;string1))
			vect[c&amp;0377] = 1;
		j = 0;
		for(i=1; i&lt;256; i++)
			if(vect[i]==0) vect[j++] = i;
		vect[j] = 0;
		compl = vect;
	}
	for(i=0; i&lt;256; i++)
		squeez[i] = 0;
	for(;;){
		if(cflag) c = *compl++;
		else c = next(&amp;string1);
		if(c==0) break;
		d = next(&amp;string2);
		if(d==0) d = c;
		code[c&amp;0377] = d;
		squeez[d&amp;0377] = 1;
	}
	while(d = next(&amp;string2))
		squeez[d&amp;0377] = 1;
	squeez[0] = 1;
	for(i=0;i&lt;256;i++) {
		if(code[i]==0) code[i] = i;
		else if(dflag) code[i] = 0;
	}

	inbuf[0] = 0;
	fout = dup(1);
	close(1);
	while((c=getc(inbuf)) &gt;=0 ) {
		if(c == 0) continue;
		if(c = code[c&amp;0377]&amp;0377)
			if(!sflag || c!=save || !squeez[c&amp;0377])
				putchar(save = c);
	}
	flush();
}

next(s)
struct string *s;
{
	int a, b, c, n;
	int base;

	if(--s-&gt;rep &gt; 0) return(s-&gt;last);
	if(s-&gt;last &lt; s-&gt;max) return(++s-&gt;last);
	if(*s-&gt;p=='[') {
		nextc(s);
		s-&gt;last = a = nextc(s);
		s-&gt;max = 0;
		switch(nextc(s)) {
		case '-':
			b = nextc(s);
			if(b&lt;a || *s-&gt;p++!=']')
				goto error;
			s-&gt;max = b;
			return(a);
		case '*':
			base = (*s-&gt;p=='0')?8:10;
			n = 0;
			while((c = *s-&gt;p)&gt;='0' &amp;&amp; c&lt;'0'+base) {
				n = base*n + c - '0';
				s-&gt;p++;
			}
			if(*s-&gt;p++!=']') goto error;
			if(n==0) n = 1000;
			s-&gt;rep = n;
			return(a);
		default:
		error:
			write(1,"Bad string\n",11);
			exit();
		}
	}
	return(nextc(s));
}

nextc(s)
struct string *s;
{
	int c, i, n;

	c = *s-&gt;p++;
	if(c=='\\') {
		i = n = 0;
		while(i&lt;3 &amp;&amp; (c = *s-&gt;p)&gt;='0' &amp;&amp; c&lt;='7') {
			n = n*8 + c - '0';
			i++;
			s-&gt;p++;
		}
		if(i&gt;0) c = n;
		else c = *s-&gt;p++;
	}
	if(c==0) *--s-&gt;p = 0;
	return(c&amp;0377);
}











