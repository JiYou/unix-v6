<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s7/nelex.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s7/nelex.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s7/nelex.c>
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
#include "ne.h"
#include "y.tab.c"

char *cntrl[] {
	"sup", "super", "sub",
	".EN",
	"sum", "from", "to",
	"hat", "dot", "dotdot", "bar", "tilde", "under",
	"prod", "int", "integral", "union", "inter",
	"pile", "lpile", "cpile", "rpile", "over", "sqrt",
	"above", "size", "font", "roman", "italic", "bold",
	"left", "right",
	"delim", "DELIM",
	"DEFINE","define",
	".gsize", ".GSIZE", "gsize", "GSIZE", "gfont", "GFONT",
	"HAT", "DOT", "DOTDOT", "BAR", "TILDE", "UNDER",
	"PROD", "INT", "INTEGRAL", "UNION", "INTER",
	"SUM", "FROM", "TO",
	"SUP", "SUPER", "SUB", "PILE", "LPILE", "CPILE", "RPILE", "OVER", "SQRT",
	"ABOVE", "SIZE", "FONT", "ROMAN", "ITALIC", "BOLD",
	"LEFT", "RIGHT", 
	"up", "UP", "down", "DOWN", "fwd", "FWD", "back", "BACK",
	"mark", "MARK", "lineup", "LINEUP",
	"matrix", "lcol", "ccol", "rcol", "col",
	0};


int icntrl[]{
	SUPER, SUPER, SUB,
	0, /* EOF */
	SUM, FROM, TO,
	HAT, DOT, DOTDOT, BAR, TILDE, UNDER,
	PROD, INT, INT, UNION, INTER,
	PILE, LPILE, CPILE, RPILE, OVER, SQRT,
	ABOVE, SIZE, FONT, ROMAN, ITALIC, BOLD,
	LEFT, RIGHT,
	DELIM, DELIM,
	DEFINE, DEFINE,
	GSIZE, GSIZE, GSIZE, GSIZE, GFONT, GFONT,
	HAT, DOT, DOTDOT, BAR, TILDE, UNDER,
	PROD, INT, INT, UNION, INTER,
	SUM, FROM, TO,
	SUPER, SUPER, SUB, PILE, LPILE, CPILE, RPILE, OVER, SQRT,
	ABOVE, SIZE, FONT, ROMAN, ITALIC, BOLD,
	LEFT, RIGHT,
	UP, UP, DOWN, DOWN, FWD, FWD, BACK, BACK,
	MARK, MARK, LINEUP, LINEUP,
	MATRIX, LCOL, CCOL, RCOL, COL,
	0};

int	peek	-1;
#define	SSIZE	400
char	token[SSIZE];
int	sp;
int speek[10];
char *swt[10];
int sw -1;

getc(){
  loop:
	if(sw &gt;= 0){
		lastchar = (peek&lt;0) ? *swt[sw]++ : peek;
		peek = -1;
		if(lastchar != '\0')return(lastchar);
		peek = speek[sw--];
		return(' ');
		}
	lastchar = (peek&lt;0) ? getchar() : peek;
	if( lastchar=='\n' )
		linect++;
	peek = -1;
	if( lastchar!= '\0' )
		return(lastchar);
	if( ++ifile &gt; svargc ){
		peek = '\0';
		return('\0');
	}
	close(fin);
	linect = 1;
	if( (fin=open(svargv[ifile],0)) &gt;= 0 )
		goto loop;
	error(FATAL,"can't open file %s\n", svargv[ifile]);
}

yylex(){
	int c, type;
  beg:
	while( (c=getc())==' ' || c=='\n');
	yylval=c;
	switch(c){

	case '\0':
		return('\0');
	case '~':
		return(SPACE);
	case '^':
		return(THIN);
	case '\t':
		return(TAB);
	case '{':
		return(MQ);
	case '}':
		return(MQ1);
	case '"':
		for(sp=0; (c=getc())!='"'; ){
			if(c !='\\')token[sp++]=c;
			else { if((c=getc())!= '"')token[sp++]='\\';
				token[sp++] = c; }
			if( sp&gt;=SSIZE )
				error(FATAL,"quoted string %.20s... too long", token);
		}
		token[sp]='\0';
		yylval= &amp;token[0];
		return(QTEXT);
	}
	if( c==righteq )
		return('\0');

	getstr(token, c);
	if((type = lookup(token,nptr)) &gt;= 0){
		if(sw &gt;= 9)
			error(FATAL,"definitions nested &gt; 9", sw);
		swt[++sw] = sptr[type];
		speek[sw] = peek;
		peek = -1;
		goto beg;
		}
	type = lookup(token,cntrl);
	if( type &lt; 0 )
		return(CONTIG);
	if( icntrl[type]==DEFINE ) {
		define();
		goto beg;
	}
	else if( icntrl[type]==DELIM ) {
		delim();
		goto beg;
	}
	else if( icntrl[type]==GSIZE ){
		globsize();
		goto beg;
	}
	else if( icntrl[type]==GFONT ) {
		globfont();
		goto beg;
	}
	else
		return( icntrl[type] );
}

getstr(s,c) char *s, c; {
	for (sp=0; c!=' ' &amp;&amp; c!='\t' &amp;&amp; c!='\n' &amp;&amp; c!='{' &amp;&amp; c!='}'
		&amp;&amp; c!='"' &amp;&amp; c!='~' &amp;&amp; c!='^' &amp;&amp; c!=righteq; ) {
		if(c == '\\') if((c = getc()) != '"')s[sp++] = '\\';
		s[sp++] = c;
		if( sp&gt;=SSIZE )
			error(FATAL,"token %.20s... too long",s);
		c = getc();
		}
	if( c=='{' || c=='}' || c=='"' || c=='~' || c=='^' || c=='\t' || c==righteq )
		peek = c;
	s[sp]='\0';
	yylval = s;
}

lookup(str,tbl) char *str; char *tbl[]; {
	register i,j, r;
	for(i=0; tbl[i]!=0; i++){ /* table of tbl wds */
		for( j=0; (r=tbl[i][j])==str[j] &amp;&amp; r!='\0'; j++);
		if( r == str[j] )
			return(i);
	}
	return( -1 );
}

cstr(s,quote) char *s; int quote; {
	int del,c,i;
	while((del=getc()) == ' ' || del == '\t' || del == '\n');
	if(quote)
		for(i=0; (c=getc()) != del;)
			s[i++] = c;
	else {
		s[0] = del;
		for(i=1; (c=getc())!=' ' &amp;&amp; c!= '\t' &amp;&amp; c!='\n';)
			s[i++]=c;
	}
	s[i] = '\0';
	return(s);
}

define() {
	char *alloc();
	int i, c;
	while( (c=getc())==' ' || c=='\n' );
	getstr(token,c);
	if((i = lookup(token,nptr)) &gt;= 0){
		yyval = i;
		free(sptr[yyval]);
	} else {
		yyval = ptr++;
		for(i=0; token[i] != '\0'; i++);
		nptr[yyval] = alloc(i+1);
		for(i=0; nptr[yyval][i]=token[i]; i++);
	}
	if(dbg)printf(".\tdefine %s\n",nptr[yyval]);
	cstr(token,1);
	for(i=0; token[i] != '\0'; i++);
	sptr[yyval] = alloc(i+1);
	for(i=0; sptr[yyval][i] = token[i]; i++);
	if(dbg)printf(".\tname %s defined as %s\n",nptr[yyval],sptr[yyval]);
}

delim() {
	char *s;
	yyval = eqnreg = 0;
	cstr(token,0);
	lefteq = token[0];
	righteq = token[1];
	if( (lefteq == 'o' &amp;&amp; righteq == 'f') || (lefteq == 'O' &amp;&amp; righteq == 'F') )
		lefteq = righteq = '\0';
}

globsize() {
	extern int gsize;
	int c;
	while( (c=getc())==' ' || c=='\n' );
	getstr(token,c);
	gsize = numb(token);
	yyval = eqnreg = 0;
}

globfont() {
	extern int gfont;
	while( (gfont=getc())==' ' || gfont=='\n' );
	yyval = eqnreg = 0;
}











