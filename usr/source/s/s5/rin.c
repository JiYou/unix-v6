<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s5/rin.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s5/rin.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s5/rin.c>
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
rin()
{
	double d;
	register n, c, f;

	d = 0.;
	f = 0;
	n = 0;
loop:
	c = getchar();
	if(c == '\0')
		exit();
	if(c == '-') {
		f++;
		goto loop;
	}
	if(c == '.') {
		n++;
		goto loop;
	}
	if(c&gt;='0' &amp;&amp; c&lt;='9') {
		if(n)
			n++;
		d = d*10.+c-'0';
		goto loop;
	}
	if(f)
		d = -d;
	for(f=1; f&lt;n; f++)
		d =/ 10.;
	n = d;
	return(n);
}











