<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s7/ne6.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s7/ne6.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s7/ne6.c>
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

column(type, p1, p2) int type, p1, p2; {
	int i, n;
	lp[p1] = ct - p1 - 1;
	if( dbg ){
		printf(".\t%c column of", type);
		for( i=p1+1; i&lt;ct; i++ )
			printf(" S%d", lp[i]);
		printf(", rows=%d\n",lp[p1]);
	}
	lp[ct++] = type;
}

matrix(p1,p2) int p1, p2; {
	int nrow, ncol, i, j, k, hb, b, val[100], db;
	int w;
	char *space;
	space = "\\ \\ ";
	nrow = lp[p1];	/* disaster if rows inconsistent */
	ncol = 0;
	for( i=p1; i&lt;ct; i =+ lp[i]+2 ){
		ncol++;
		if(dbg)printf(".\tcolct=%d\n",lp[i]);
	}
	for( k=1; k&lt;=nrow; k++ ) {
		hb = b = 0;
		j = p1 + k;
		for( i=0; i&lt;ncol; i++ ) {
			hb = max(hb, eht[lp[j]]-ebase[lp[j]]);
			b = max(b, ebase[lp[j]]);
			j =+ nrow + 2;
		}
		if(dbg)printf(".\trow %d: b=%d, hb=%d\n", k, b, hb);
		j = p1 + k;
		for( i=0; i&lt;ncol; i++ ) {
			ebase[lp[j]] = b;
			eht[lp[j]] = b + hb;
			j =+ nrow + 2;
		}
	}
	j = p1;
	w = 0;
	for( i=0; i&lt;ncol; i++ ) {
		lpile(lp[j+lp[j]+1], j+1, j+lp[j]+1);
		val[i] = yyval;
		w =+ ewid[yyval];
		j =+ nrow + 2;
	}
	yyval = oalloc();
	eht[yyval] = eht[val[0]];
	ebase[yyval] = ebase[val[0]];
	ewid[yyval] = w + 2 * (ncol-1);	/* 2 = width(space) */
	if(dbg)printf(".\tmatrix S%d: r=%d, c=%d, h=%d, b=%d, w=%d\n",
		yyval,nrow,ncol,eht[yyval],ebase[yyval], ewid[yyval]);
	printf(".ds %d \"", yyval);
	for( i=0; i&lt;ncol; i++ )  {
		printf("\\*(%d%s", val[i], i==ncol-1 ? "" : space);
		ofree(val[i]);
	}
	printf("\n");
	ct = p1;
}











