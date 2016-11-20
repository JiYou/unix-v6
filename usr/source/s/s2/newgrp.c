<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/newgrp.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/newgrp.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/newgrp.c>
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
int	gbuf[259];
int	pbuf[259];
int	ttyb[3];
char	name[10];
char	passwd[10];
char	space[1000];
int	peek;
int	pwdflg;

main(argc, argv)
char *argv[];
{
	register id;
	register char *p, *q;

	if(argc &lt; 2) {
		printf("usage: newgrp groupname\n");
		done();
	}
	if(fopen("/etc/group", gbuf) &lt; 0) {
		printf("cannot open group file\n");
		done();
	}
	do {
		field(name, gbuf);
		p = name;
		q = argv[1];
		while(*p == *q++)
			if(*p++ == 0 || p &gt;= name+8)
				goto l1;
	} while(skip(gbuf));
	printf("%s: not a valid group name\n", argv[1]);
	done();

l1:
	if(fopen("/etc/passwd", pbuf) &lt; 0) {
		printf("cannot open password file\n");
		done();
	}
	p = space;
	id = getuid() &amp; 0377;
	do {
		field(p, pbuf);
		field(name, pbuf);
		if(value(pbuf) == id) {
			if(name[0] == 0)
				pwdflg++;
			while(*p++) ;
		}
	} while(skip(pbuf));
	*p = 0;

	field(passwd, gbuf);
	id = value(gbuf);
	if(id == 1) {
		pwdflg = 0;
		goto l2;
	}
	do {
		p = space;
		field(name, gbuf);
		while(*p) {
			q = name;
			while(*p == *q++) {
				if(*p++ == 0)
					goto l2;
			}
			while(*p++) ;
		}
	} while(peek == ',');
	goto no;

l2:
	if(pwdflg &amp;&amp; passwd[0]) {
		printf("password: ");
		gtty(0, pbuf);
		pbuf[3] = pbuf[2];
		pbuf[2] =&amp; ~010;
		stty(0, pbuf);
		read(0, gbuf, 512);
		pbuf[2] = pbuf[3];
		stty(0, pbuf);
		printf("\n");
		p = name;
		q = gbuf;
		while(p &lt; name+8) {
			if(*q == '\n')
				break;
			*p++ = *q++;
		}
		*p = 0;
		p = crypt(name);
		q = passwd;
		while(q &lt; passwd+8)
			if(*p++ != *q++)
				goto no;
	}
	if(setgid(id) &lt; 0) {
		perror("setgid");
		goto no;
	}
	done();

no:
	printf("Sorry\n");
	done();
}

skip(buf)
{

	while(peek &gt; 0) {
		if(peek == '\n') {
			peek = 1;
			return(1);
		}
		peek = getc(buf);
	}
	return(0);
}

field(cp, buf)
{
	register c;
	register char *p, *q;

	p = cp;
	q = p+8;
	while((c = getc(buf)) != '\n') {
		if(c == ',' || c == ':' || c &lt;= 0)
			break;
		if(p &lt; q)
			*p++ = c;
	}
	*p = 0;
	peek = c;
}

value(buf)
{
	register n, c;

	n = 0;
	while((c = getc(buf)) &gt;= '0' &amp;&amp; c &lt;= '9')
		n = n*10 + c-'0';
	peek = c;
	return(n);
}

done()
{

	setuid(getuid());
	execl("/bin/sh", "-", 0);
	exit();
}











