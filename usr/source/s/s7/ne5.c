<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s7/ne5.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s7/ne5.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s7/ne5.c>
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
#include "ne.h"

int	markpos;

mark(n) int n; {
	if( n )
		markpos = ewid[n];
	else {
		yyval = oalloc();
		printf(".ds %d \"\n", yyval);
		ebase[yyval] = ewid[yyval] = markpos = 0;
		eht[yyval] = 2;
	}
	if(dbg)printf(".\tmark %d as %d\n", n, markpos);
}

lineup(n) int n; {
	if( n ) {
		printf(".ds %d \"", n);
		fwd(markpos-ewid[n]);
		printf("\\*(%d\n", n);
		ewid[n] = markpos;
	}
	else {
		yyval = oalloc();
		printf(".ds %d \"", yyval);
		fwd(markpos);
		printf("\n");
		ebase[yyval] = 0;
		eht[yyval] = 2;
		ewid[yyval] = markpos;
	}
	if(dbg)printf(".\tlineup %d at %d\n", n, markpos);
}











