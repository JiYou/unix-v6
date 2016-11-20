<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s7/ne3.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s7/ne3.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s7/ne3.c>
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
# include "ne.h"

char *res[] {
	"&gt;=", "&lt;=", "!=",
	"+-", "==", "cdot", "CDOT",
	"times", "TIMES",
	"SIGMA", "pi", "PI",
	"alpha", "beta", "gamma", "GAMMA", "delta", "epsilon", "omega",
	"DELTA", "LAMBDA", "PHI", "OMEGA",
	"lambda", "mu", "nu", "theta", "rho", "sigma", "tau", "phi",
	"INF", "INFINITY",
	"inf", "infinity",
	"partial", "PARTIAL",
	"zeta", "eta", "iota", "kappa", "xi", "omicron", "upsilon",
	"chi", "psi", "THETA", "XI", "UPSILON", "PSI",
	"del", "DEL",
	"nothing", "NOTHING",
	"approx", "APPROX",
	0};
char *restran[] {
	"&gt;\b_", "&lt;\b_", "/\b=",
	"+\b_", "=\b_", "8.9", "8.9",
	"x", "x",
	"R", "J", "P",
	"A", "B", "\\e", "G", "D", "S", "C",
	"W", "E", "F", "Z",
	"L", "M", "@", "T", "K", "Y", "I", "U",
	"o", "o", "o", "o",
	"]", "]",
	"Q", "N", "i", "k", "X", "o", "u",
	"X", "V", "O", "X", "U", "H",
	"[", "[",
	"", "",
	"~\b=", "~\b=",
	0};

int	csp;
int	psp;
#define	CSSIZE	400
char	cs[420];

text(t,p1) int t; char *p1; {
	int i,j,c;
	int w;
	yyval = oalloc();
	ebase[yyval] = 0;
	eht[yyval] = 2;	/* ht in 1/2 spaces */
	if( t=='q' )
		j = p1;
	else if ( t == '~' )
		j = &amp;"~";
	else if ( t == '^' )
		j = &amp;"";
	else if ( t == '\t' )
		j = &amp;"\\t";
	else if( (i=lookup(p1,res))&gt;=0 )
		j = restran[i];
	else {
		for( csp=psp=0; (c=p1[psp++])!='\0'; ){
			trans(c,p1);
			if( csp&gt;CSSIZE )
				error(FATAL,"converted token %.20s... too long",p1);
		}
		cs[csp] = '\0';
		j = cs;
	}
	ewid[yyval] = width(j);
	if(dbg)printf(".\t%ctext: S%d &lt;- %s; b=%d,h=%d,w=%d\n",
		t, yyval, j, ebase[yyval], eht[yyval], ewid[yyval]);
	printf(".ds %d \"%s\n",yyval,j);
}

width(s) char *s; {
	int c,w;
	w = 0;
	while( (c = *s++) != '\0' ){
		if( c == '\b' || c == 033 )
			w--;
		else if ( c == '\\' &amp;&amp; *s == '\\' );
		else if ( c == '\\' &amp;&amp; *s == 'e' );
		else if ( c &gt;= 040 )
			w++;
	}
	return(w);
}

trans(c,p1) int c; char *p1; {
	switch( c){
	case '&gt;': case '&lt;': case '=':
		if( p1[psp]=='=' ){	/* look ahead for == &lt;= &gt;= */
			cs[csp++] = c; cs[csp++] = '\b'; cs[csp++] = '_';
			psp++;
		} else 
			cs[csp++] = c;
		break;
	case '\\':	/* troff - pass 2 or 3 more chars */
		cs[csp++] = c; cs[csp++] = c = p1[psp++]; cs[csp++] = p1[psp++];
		if( c=='(' ) cs[csp++] = p1[psp++];
		break;
	default:
		cs[csp++] = c; break;
	}
}











