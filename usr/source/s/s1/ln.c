<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/ln.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/ln.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/ln.c>
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
#
/*
 * ln target [ new name ]
 */

struct ibuf {
	int	inum;
	int	iflags;
	char	inl;
	char	iuid;
	int	isize;
	int	iaddr[8];
	char	*ictime[2];
	char	*imtime[2];
	int	fill;
};

#define	DIR	040000
#define	FMT	060000

main(argc, argv)
char **argv;
{
	static struct ibuf statb;
	register char *np;

	if (argc&lt;2) {
		write(1, "Usage: ln target [ newname ]\n", 29);
		exit(1);
	}
	if (argc==2) {
		np = argv[1];
		while(*np++);
		while (*--np!='/' &amp;&amp; np&gt;argv[1]);
		np++;
		argv[2] = np;
	}
	stat(argv[1], &amp;statb);
	if ((statb.iflags&amp;FMT) == DIR) {
		write(1, "No directory link\n", 18);
		exit(1);
	}
	if (link(argv[1], argv[2])&lt;0) {
		write(1, "Can't link\n", 11);
		exit(1);
	}
	exit(0);
}











