<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/split.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/split.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/split.c>
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
int	count	1000;
int	fnumber;
int	ibuf[259];
int	obuf[259];
char	fname[100];
char	*ifil;
char	*ofil;

main(argc, argv)
char *argv[];
{
	register i, c, f;

	for(i=1; i&lt;argc; i++)
	if(argv[i][0] == '-')
	switch(argv[i][1]) {

	case '\0':
		ifil = 1;
		continue;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		count = number(argv[i]+1);
		continue;

	} else
	if(ifil)
		ofil = argv[i]; else
		ifil = argv[i];
	if(ifil != 0 &amp;&amp; ifil != 1)
		if(fopen(ifil, ibuf) &lt; 0) {
			write(2, "cannot open input\n", 18);
			exit();
		}
	if(ofil == 0)
		ofil = "x";

loop:
	f = 1;
	for(i=0; i&lt;count; i++)
	do {
		c = getc(ibuf);
		if(c &lt; 0) {
			if(f == 0)
				fflush(obuf);
			exit();
		}
		if(f) {
			for(f=0; ofil[f]; f++)
				fname[f] = ofil[f];
			fname[f++] = fnumber/26 + 'a';
			fname[f++] = fnumber%26 + 'a';
			fname[f] = '\0';
			fnumber++;
			if(fcreat(fname, obuf) &lt; 0) {
				write(2, "Cannot create output\n", 20);
				exit();
			}
			f = 0;
		}
		putc(c, obuf);
	} while(c != '\n');
	fflush(obuf);
	close(obuf[0]);
	goto loop;
}

number(str)
char *str;
{
	register n;
	register char *s;

	n = 0;
	for(s = str; *s; s++)
		if(*s&gt;='0' &amp;&amp; *s&lt;='9')
			n = n*10 + *s-'0';
	return(n);
}











