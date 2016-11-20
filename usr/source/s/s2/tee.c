<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/tee.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/tee.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/tee.c>
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
int open[9] { 1 };
int n 1;
int t 0;

char in[512];

char out[512];

main(argc,argv)
char **argv;
{
	int register r,w,p;
	struct { int x1[2], type, x2[15]; } buf;
	fstat(1,&amp;buf);
	t = (buf.type&amp;060000)==020000;
	while(argc--&gt;1) {
		open[n++] = creat(argv[1],0666);
		if(stat(argv[1],&amp;buf)&gt;=0)
			if((buf.type&amp;060000)==020000)
				t++;
		argv++;
	}
	r = w = 0;
	for(;;) {
		for(p=0;p&lt;512;) {
			if(r&gt;=w) {
				if(t&gt;0&amp;&amp;p&gt;0) break;
				w = read(0,in,512);
				r = 0;
				if(w&lt;=0) {
					stash(p);
					return;
				}
			}
			out[p++] = in[r++];
		}
		stash(p);
	}
}

stash(p)
{
	int k;
	int i;
	int d;
	d = t ? 10 : p;
	for(i=0; i&lt;p; i=+d)
		for(k=0;k&lt;n;k++)
			write(open[k], out+i, d&lt;p-i?d:p-i);
}











