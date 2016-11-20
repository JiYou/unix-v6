<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/sleep.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/sleep.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/sleep.c>
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
	int c, n;
	char *s;

	n = 0;
	if(argc &lt; 2) {
		printf("arg count\n");
		exit();
	}
	s = argv[1];
	while(c = *s++) {
		if(c&lt;'0' || c&gt;'9') {
			printf("bad character\n");
			exit();
		}
		n = n*10 + c - '0';
	}
	sleep(n);
}











