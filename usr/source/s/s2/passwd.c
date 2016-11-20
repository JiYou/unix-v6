<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/passwd.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/passwd.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/passwd.c>
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
 * enter a password in the password file
 * this program should be suid with owner
 * with an owner with write permission on /etc/passwd
 */
char	*tfile	{ "/tmp/ptmp" };
char	*pfile	{ "/etc/passwd" };
int	tbuf[259];
int	pbuf[259];

main(argc, argv)
char *argv[];
{
	register u, c;
	register char *p;

	if(argc != 3) {
		write(2, "Usage: passwd user password\n", 28);
		goto bex;
	}
	signal(1, 1);
	signal(2, 1);
	signal(3, 1);

	if(stat(tfile, tbuf+20) &gt;= 0) {
		write(2, "Temporary file busy -- try again\n", 33);
		goto bex;
	}
	tbuf[0] = creat(tfile, 0600);
	if(tbuf[0] &lt; 0) {
		write(2, "Cannot create temporary file\n", 29);
		goto bex;
	}
	pbuf[0] = open(pfile, 0);
	if(pbuf[0] &lt; 0) {
		write(2, "Cannot open /etc/passwd\n", 24);
		goto out;
	}
	goto l1;

/*
 * skip to beginning of next line
 */

skip:
	while(c != '\n') {
		if(c &lt; 0)
			goto ill;
		c = getc(pbuf);
		putc(c, tbuf);
	}

/*
 * compare user names
 */

l1:
	c = getc(pbuf);
	putc(c, tbuf);
	if(c &lt; 0) {
		write(2, "User name not found in password file\n", 37);
		goto out;
	}
	p = argv[1];
	while(c != ':') {
		if(*p++ != c)
			goto skip;
		c = getc(pbuf);
		putc(c, tbuf);
	}
	if(*p)
		goto skip;
/*
 * skip old password
 */
	do {
		c = getc(pbuf);
		if(c &lt; 0)
			goto ill;
	} while(c != ':');

/*
 * copy in new password
 */
	p = argv[2];
	for(c=0; c&lt;9; c++)
		if(*p++ == 0)
			break;
	*--p = 0;
	if(p != argv[2])
		p = crypt(argv[2]);
	while(*p)
		putc(*p++, tbuf);
	putc(':', tbuf);

/*
 * validate uid
 */

	u = 0;
	do {
		c = getc(pbuf);
		putc(c, tbuf);
		if(c &gt;= '0' &amp;&amp; c &lt;= '9')
			u = u*10 + c-'0';
		if(c &lt; 0)
			goto ill;
	} while(c != ':');
	c = getuid() &amp; 0377;
	if(c != 0 &amp;&amp; c != u) {
		write(2, "Permission denied\n", 18);
		goto out;
	}

/*
 * copy out and back
 */

	for(;;) {
		c = getc(pbuf);
		if(c &lt; 0) {
			fflush(tbuf);
			close(pbuf[0]);
			close(tbuf[0]);
			tbuf[0] = open(tfile, 0);
			if(tbuf[0] &lt; 0) {
				write(2, "Urk\n", 4);
				goto out;
			}
			pbuf[0] = creat(pfile, 0644);
			if(pbuf[0] &lt; 0) {
				write(2, "Cannot create /etc/passwd\n", 26);
				goto out;
			}
			while((c = read(tbuf[0], tbuf+1, 512)) &gt; 0)
				write(pbuf[0], tbuf+1, c);
			unlink(tfile);
			exit(0);
		}
		putc(c, tbuf);
	}

ill:
	write(2, "Password file illformed\n", 24);

out:
	unlink(tfile);

bex:
	exit(1);
}











