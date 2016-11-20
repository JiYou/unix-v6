<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/opr.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/opr.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/opr.c>
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
 *	opr -- off line print dispatcher
 *		chooses spooling routine appropriate 
 *		to destination
 *
 *	last entry in table isdefault
 */

char *code[] {
	"-lp",	"/lib/lpr",	/* line printer */
	0
};

main(argc, argv)
char **argv;
{
	int i, j;

	argv[argc] = 0;
	for(i=0; code[i]; i=+2)
	if(argc &gt; 1)
		for(j=0; code[i][j]==argv[1][j]; j++)
			if(code[i][j] == 0)
					execv(code[i+1], &amp;argv[1]);
	execv(code[i-1], argv);
	write(2, "can't start daemon\n", 19);
}











