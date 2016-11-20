<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s5/perror.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s5/perror.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s5/perror.c>
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
/*
 * Print the error indicated
 * in the cerror cell.
 */

int	errno;
int	sys_nerr;
char	*sys_errlist[];
perror(s)
char *s;
{
	register char *c;
	register n;

	c = "Unknown error";
	if(errno &lt; sys_nerr)
		c = sys_errlist[errno];
	n = strlen(s);
	if(n) {
		write(2, s, n);
		write(2, ": ", 2);
	}
	write(2, c, strlen(c));
	write(2, "\n", 1);
}

/*
 * Return the number of bytes in a string
 */

strlen(str)
char *str;
{
	register char *s;

	s = str;
	while(*s++)
		;
	return(s-str-1);
}
//NEXEC












