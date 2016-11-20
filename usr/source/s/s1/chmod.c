<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/chmod.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/chmod.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/chmod.c>
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
main(argc, argv)
char **argv;
{
	register i, m;
	register char *c;
	int count;

	if(argc &lt; 3) {
		printf("arg count\n");
		exit(1);
	}
	c = argv[1];
	m = 0;
	for(m=0; *c; c++) {
		if(*c &lt; '0' || *c &gt; '7') {
			printf("bad mode\n");
			exit(1);
		}
		m = (m&lt;&lt;3) | *c - '0';
	}
	for(i=2; i&lt;argc; i++)
		if(chmod(argv[i], m) &lt; 0) {
			count++;
			perror(argv[i]);
		}
	exit(count);
}











