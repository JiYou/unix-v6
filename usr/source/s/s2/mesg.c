<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/mesg.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/mesg.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/mesg.c>
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
 * mesg -- set current tty to accept or
 *	forbid write permission.
 *
 *	mesg [y] [n]
 *		y allow messages
 *		n forbid messages
 */

int	sbuf[40];

main(argc, argv)
char *argv[];
{
	register char *tty;

	tty = "/dev/ttyx";
	tty[8] = ttyn(1);
	if(stat(tty, sbuf) &lt; 0) {
		write(2, "cannot stat\n", 12);
		exit(1);
	}
	if(argc &lt; 2) {
		if(sbuf[2] &amp; 02)
			goto no;
		goto yes;
	}
	if(*argv[1] == 'y')
		goto yes;

no:
	if(chmod(tty, 0600) &lt; 0)
		goto bad;
	goto was;

yes:
	if(chmod(tty, 0622) &lt; 0)
		goto bad;

was:
	if(sbuf[2] &amp; 02)
		write(2, "was y\n", 6); else
		write(2, "was n\n", 6);
	exit(0);

bad:
	write(2, "cannot change mode\n", 19);
	exit(1);
}











