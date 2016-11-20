<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s7/ne4.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s7/ne4.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s7/ne4.c>
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
# define	SIGPIPE	13	/* nroff has stopped reading */

int	ESC	033;
int	HREV	'8';
int	HFWD	'9';
int	SI	017;
int	SO	016;
int	ESCOUT	033;
int	HFWDOUT	'9';
int	HREVOUT	'8';
int	BKSPOUT	'\b';
int	FWDOUT	'~';

char	in[400];	/* input buffer */
int	exit();

main(argc,argv) int argc; char *argv[];{
	int i, type;
	flush();
	first = 0;
	lefteq = righteq = '\0';
	signal(SIGPIPE, &amp;exit);
	setfile(argc,argv);
	while( (type=getline(in)) != '\0' ){
		eqline = linect;
		if( in[0]=='.' &amp;&amp; in[1]=='E' &amp;&amp; in[2]=='Q' ){
			for( i=11; i&lt;100; used[i++]=0 );
			printf(".tr ~\n");
			printf("%s",in);
			init();
			yyparse();
			if( eqnreg&gt;0 )
				printf(".ne %d\n.rs\n'sp %d\n\\*(%d\n'sp %d\n",
					(eqnht+1)/2, (eqnht-eqnbase-2)/2,eqnreg,eqnbase/2);
			printf(".EN");
			if( lastchar == '\0' ){
				putchar('\n');
				break;
			}
			if( putchar(lastchar) != '\n' )
				while( putchar(getc()) != '\n' );
			flush();
		} else if( type != lefteq )
			printf("%s",in);
		else
			inline();
	}
	putchar('\0');
	flush();
	exit();
}

getline(s) char *s; {
	char c;
	while((*s++=c=getc())!='\n' &amp;&amp; c!='\0' &amp;&amp; c!=lefteq );
	if( c==lefteq )
		s--;
	*s++ = '\0';
	return(c);
}

inline() {
	int i,j,ds[20],t;
	i =  -1;
	do{
		if( i&gt;=17 ){
			while((j=getline(in))!='\n' &amp;&amp; j!='\0');
			error(!FATAL,"missing right delim (?) at %.20s",in);
			break;
		}
		ds[++i] = oalloc();
		printf(".ds %d \"%s\n", ds[i], in);
		init();
		yyparse();
		if( eqnreg &gt; 0 )
			ds[++i] = eqnreg;
	} while( (t=getline(in)) == lefteq );
	ds[++i] = oalloc();
	printf(".ds %d \"%s", ds[i], in);
	for( j=0; j&lt;=i; j++){
		printf("\\*(%d", ds[j]);
		ofree(ds[j]);
	}
	putchar('\n');
	flush();
}

putout(p1) int p1; {
	if(dbg)printf(".\tanswer &lt;- S%d\n",p1);
	eqnht = eht[p1];
	eqnbase = ebase[p1];
	eqnreg = p1;
}

abs(v) int v; {
	return( v&gt;0 ? v : -v );
}

max(i,j) int i,j; {
	return( i&gt;j ? i : j );
}

oalloc(){
	int i;
	for( i=11; i&lt;100; i++)
		if( used[i]++ == 0 ) return(i);
	error( FATAL, "no strings left", i);
}

ofree(n) int n; {
	used[n] = 0;
}

setfile(argc, argv) int argc; char *argv[]; {
	svargc = --argc;
	svargv = argv;
	while( svargc &gt; 0 &amp;&amp; svargv[1][0] == '-'){
		switch( svargv[1][1] ){

		case 'd': lefteq=svargv[1][2]; righteq=svargv[1][3]; break;
		case 's': break;
		case 'f': break;
		default:
			dbg = 1;
			ESCOUT = 'E';
			HFWDOUT = 'F';
			HREVOUT = 'R';
			BKSPOUT = 'B';
			FWDOUT = 'S';
		}
		svargc--;
		svargv++;
	}
	if( svargc == 0 )
		fin = dup(0);
	else if( (fin = open(svargv[1], 0)) &lt; 0)
		error( FATAL,"can't open file %s", argv[1]);
	ptr = 0;
	fout = dup(1);
	ifile = 1;
	linect = 1;
}

yyerror(){;}

int	gsize	10;
int	gfont	'I';

init(){
	ct = 0;
	first++;
}

error(fatal, s1, s2) int fatal; char *s1, *s2; {
	int sfout;
	printf("NEQN ERROR HERE");
	flush(fout);
	sfout = fout;
	fout = 2;
	if( fatal&gt;0 )
		printf("fatal error: ");
	printf(s1,s2);
	printf(" file %s, between lines %d and %d\n",
		 svargv[ifile], eqline, linect);
	flush(2);
	fout = sfout;
	if( fatal &gt; 0 )
		exit(1);
}

down(n) int n; {
	int c;
	if( n&lt;= 0 )
		c = HREVOUT;
	else
		c = HFWDOUT;
	n = abs(n);
	while( n-- &gt; 0 ){
		putchar(ESCOUT); putchar(c);
	}
}

up(n) int n; {
	int c;
	if( n&lt;= 0 )
		c = HFWDOUT;
	else
		c = HREVOUT;
	n = abs(n);
	while( n-- &gt; 0 ){
		putchar(ESCOUT); putchar(HREVOUT);
	}
}

fwd(n) int n; {
	int c,i;
	c = n&lt;0 ? BKSPOUT : FWDOUT;
	n = abs(n);
	while( n-- &gt; 0 )
		putchar(c);
}

back(n) int n; {
	int c,i;
	c = n&gt;0 ? BKSPOUT : FWDOUT;
	n = abs(n);
	while( n-- &gt; 0 )
		putchar(c);
}

line(n) int n; {
	while( n-- &gt; 0 )
		putchar('_');
}











