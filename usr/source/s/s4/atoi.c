<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s4/atoi.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s4/atoi.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s4/atoi.c>
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
atoi(ap)
char *ap;
{
	register n, c;
	register char *p;
	int f;

	p = ap;
	n = 0;
	f = 0;
loop:
	while(*p == ' ' || *p == '	')
		p++;
	if(*p == '-') {
		f++;
		p++;
		goto loop;
	}
	while(*p &gt;= '0' &amp;&amp; *p &lt;= '9')
		n = n*10 + *p++ - '0';
	if(f)
		n = -n;
	return(n);
}











