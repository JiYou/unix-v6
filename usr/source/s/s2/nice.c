<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/nice.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/nice.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/nice.c>
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
/* nice */
int	nicarg	4;
char	*args[100];
char	string[10000];

main(argc, argv)
int argc;
char *argv[];
{
	int i;
	register char **argp, *strp, *p;

	if(argc &gt; 1 &amp;&amp; argv[1][0] == '-') {
		nicarg = atoi(&amp;argv[1][1]);
		argc--;
		argv++;
	}
	if(argc &lt; 2) {
		printf("usage: nice [ -n ] command\n");
		exit(1);
	}
	argc--;
	argv++;
	argp = args;
	strp = string;
	for (i=0; i&lt;9; i++)
		*strp++ = "/usr/bin/"[i];
	for(i=0; i&lt;argc; i++) {
		*argp++ = strp;
		p = *argv++;
		while(*strp++ = *p++);
	}
	*argp = 0;
	nice(nicarg);
	execv(string+9, args);
	execv(string+4, args);
	execv(string, args);
	printf("%s not found\n", string+9);
	exit(1);
}











