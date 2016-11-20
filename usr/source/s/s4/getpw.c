<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s4/getpw.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s4/getpw.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s4/getpw.c>
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
getpw(uid, buf)
int uid;
char buf[];
{
	auto pbuf[259];
	static pwf;
	register n, c;
	register char *bp;

	if(pwf == 0)
		pwf = open("/etc/passwd", 0);
	if(pwf &lt; 0)
		return(1);
	seek(pwf, 0, 0);
	pbuf[0] = pwf;
	pbuf[1] = 0;
	pbuf[2] = 0;
	uid =&amp; 0377;

	for (;;) {
		bp = buf;
		while((c=getc(pbuf)) != '\n') {
			if(c &lt;= 0)
				return(1);
			*bp++ = c;
		}
		*bp++ = '\0';
		bp = buf;
		n = 3;
		while(--n)
		while((c = *bp++) != ':')
			if(c == '\n')
				return(1);
		while((c = *bp++) != ':') {
			if(c&lt;'0' || c&gt;'9')
				continue;
			n = n*10+c-'0';
		}
		if(n == uid)
			return(0);
	}
	return(1);
}











