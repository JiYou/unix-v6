<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/goto.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/goto.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/goto.c>
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
int	offset	0;

main(argc, argv)
char *argv[];
{
	extern fin;
	char line[64];

	if (argc&lt;2 || ttyn(0)!='x') {
		write(1, "goto error\n", 11);
		seek(0, 0, 2);
		return;
	}
	seek(0, 0, 0);
	fin = dup(0);

loop:
	if (getlin(line)) {
		write(1, "label not found\n", 16);
		return;
		}
	if (compar(line, argv[1])) goto loop;
	seek(0, offset, 0);
}

getlin(s)
char s[];
{
	int ch, i;

	i = 0;
l:
	if ((ch=getc())=='\0') return(1);
	if (ch!=':') {
		while(ch!='\n' &amp;&amp; ch!='\0')
			ch = getc();
		goto l;
		}
	while ((ch=getc())==' ');
	while (ch!=' ' &amp;&amp; ch!='\n' &amp;&amp; ch!='\0') {
		s[i++] = ch;
		ch = getc();
		}
	while(ch != '\n')
		ch = getc();
	s[i] = '\0';
	return(0);
}

compar(s1, s2)
char s1[], s2[];
{
	int c, i;

	i = 0;
l:
	if(s1[i] != s2[i]) return(1);
	if (s1[i++] == '\0') return(0);
	goto l;
}

getc()
{

	offset++;
	return(getchar());
}











