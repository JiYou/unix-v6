<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s7/ne.h</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s7/ne.h</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s7/ne.h>
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
int	dbg;
int	ct;
int	lp[40];
char	used[100];	/* available registers */
int	ps;	/* dflt init pt size */
int	ft;	/* dflt font */
int	first;
extern	int	fout, fin;
int	ifile;
int	linect;	/* line number in file */
int	eqline;	/* line where eqn started */
int	svargc;
char	**svargv;
int	eht[100];
int	ebase[100];
int	ewid[100];
struct { char c1; char c2; };
int	yyval;
int	*yypv;
int	yylval;
int	tht[30];
int	tbase[30];
int	ptr;
char	*nptr[50];
char *sptr[50];
int	eqnreg, eqnht, eqnbase;
int	lefteq, righteq;
int	lastchar;	/* last character read by lex */
#define	FATAL	1
int	ESC;
int	HREV;
int	HFWD;
int	SI;
int	SO;











