<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/units.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/units.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/units.c>
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
#define	NDIM	10
#define	NTAB	601
char	*dfile	"/usr/lib/units";
char	*unames[NDIM];
double	getflt();
int	fperr();
struct unit
{
	double	factor;
	char	dim[NDIM];
};

struct table
{
	double	factor;
	char	dim[NDIM];
	char	*name;
} table[NTAB];
char	names[NTAB*10];
struct prefix
{
	double	factor;
	char	*pname;
} prefix[]
{
	1e-18,	"atto",
	1e-15,	"femto",
	1e-12,	"pico",
	1e-9,	"nano",
	1e-6,	"micro",
	1e-3,	"milli",
	1e-2,	"centi",
	1e-1,	"deci",
	1e1,	"deka",
	1e2,	"hecta",
	1e2,	"hecto",
	1e3,	"kilo",
	1e6,	"mega",
	1e6,	"meg",
	1e9,	"giga",
	1e12,	"tera",
	0.0,	0
};
int	ibuf[259];
int	fperrc;
int	peekc;
int	dumpflg;

main(argc, argv)
char *argv[];
{
	register i;
	register char *file;
	struct unit u1, u2;
	double f;

	if(argc&gt;1 &amp;&amp; *argv[1]=='-') {
		argc--;
		argv++;
		dumpflg++;
	}
	file = dfile;
	if(argc &gt; 1)
		file = argv[1];
	if(fopen(file, ibuf) &lt; 0) {
		printf("no table\n");
		exit();
	}
	ldfps(07600); /* interrupt on fp errors */
	signal(8, fperr);
	init();
	close(ibuf[0]);
	ibuf[0] = 0;

loop:
	fperrc = 0;
	printf("you have: ");
	if(convr(&amp;u1))
		goto loop;
	if(fperrc)
		goto fp;
loop1:
	printf("you want: ");
	if(convr(&amp;u2))
		goto loop1;
	for(i=0; i&lt;NDIM; i++)
		if(u1.dim[i] != u2.dim[i])
			goto conform;
	f = u1.factor/u2.factor;
	if(fperrc)
		goto fp;
	printf("\t* %e\n", f);
	printf("\t/ %e\n", 1./f);
	goto loop;

conform:
	if(fperrc)
		goto fp;
	printf("conformability\n");
	units(&amp;u1);
	units(&amp;u2);
	goto loop;

fp:
	printf("underflow or overflow\n");
	goto loop;
}

units(up)
struct unit *up;
{
	register struct unit *p;
	register f, i;

	p = up;
	printf("\t%e ", p-&gt;factor);
	f = 0;
	for(i=0; i&lt;NDIM; i++)
		f =| pu(p-&gt;dim[i], i, f);
	if(f&amp;1) {
		putchar('/');
		f = 0;
		for(i=0; i&lt;NDIM; i++)
			f =| pu(-p-&gt;dim[i], i, f);
	}
	putchar('\n');
}

pu(u, i, f)
{

	if(u &gt; 0) {
		if(f&amp;2)
			putchar('-');
		if(unames[i])
			printf("%s", unames[i]); else
			printf("*%c*", i+'a');
		if(u &gt; 1)
			putchar(u+'0');
			return(2);
	}
	if(u &lt; 0)
		return(1);
	return(0);
}

convr(up)
struct unit *up;
{
	register struct unit *p;
	register c;
	register char *cp;
	char name[20];
	int den, err;

	p = up;
	for(c=0; c&lt;NDIM; c++)
		p-&gt;dim[c] = 0;
	p-&gt;factor = getflt();
	if(p-&gt;factor == 0.)
		p-&gt;factor = 1.0;
	err = 0;
	den = 0;
	cp = name;

loop:
	switch(c=get()) {

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
	case '/':
	case ' ':
	case '\t':
	case '\n':
		if(cp != name) {
			*cp++ = 0;
			cp = name;
			err =| lookup(cp, p, den, c);
		}
		if(c == '/')
			den++;
		if(c == '\n')
			return(err);
		goto loop;
	}
	*cp++ = c;
	goto loop;
}

lookup(name, up, den, c)
char *name;
struct unit *up;
{
	register struct unit *p;
	register struct table *q;
	register i;
	char *cp1, *cp2;
	double e;

	p = up;
	e = 1.0;

loop:
	q = hash(name);
	if(q-&gt;name) {
		l1:
		if(den) {
			p-&gt;factor =/ q-&gt;factor*e;
			for(i=0; i&lt;NDIM; i++)
				p-&gt;dim[i] =- q-&gt;dim[i];
		} else {
			p-&gt;factor =* q-&gt;factor*e;
			for(i=0; i&lt;NDIM; i++)
				p-&gt;dim[i] =+ q-&gt;dim[i];
		}
		if(c &gt;= '2' &amp;&amp; c &lt;= '9') {
			c--;
			goto l1;
		}
		return(0);
	}
	for(i=0; cp1 = prefix[i].pname; i++) {
		cp2 = name;
		while(*cp1 == *cp2++)
			if(*cp1++ == 0) {
				cp1--;
				break;
			}
		if(*cp1 == 0) {
			e =* prefix[i].factor;
			name = cp2-1;
			goto loop;
		}
	}
	for(cp1 = name; *cp1; cp1++);
	if(cp1 &gt; name+1 &amp;&amp; *--cp1 == 's') {
		*cp1 = 0;
		goto loop;
	}
	printf("cannot recognize %s\n", name);
	return(1);
}

equal(s1, s2)
char *s1, *s2;
{
	register char *c1, *c2;

	c1 = s1;
	c2 = s2;
	while(*c1++ == *c2)
		if(*c2++ == 0)
			return(1);
	return(0);
}

init()
{
	register char *cp;
	register struct table *tp, *lp;
	int c, i, f, t;
	char *np;

	cp = names;
	for(i=0; i&lt;NDIM; i++) {
		np = cp;
		*cp++ = '*';
		*cp++ = i+'a';
		*cp++ = '*';
		*cp++ = 0;
		lp = hash(np);
		lp-&gt;name = np;
		lp-&gt;factor = 1.0;
		lp-&gt;dim[i] = 1;
	}
	lp = hash("");
	lp-&gt;name = cp-1;
	lp-&gt;factor = 1.0;

l0:
	c = get();
	if(c == 0) {
		printf("%l units; %l bytes\n\n", i, cp-names);
		if(dumpflg)
		for(tp = &amp;table[0]; tp &lt; &amp;table[NTAB]; tp++) {
			if(tp-&gt;name == 0)
				continue;
			printf("%s", tp-&gt;name);
			units(tp);
		}
		return;
	}
	if(c == '/')
		while(c != '\n')
			c = get();
	if(c == '\n')
		goto l0;
	np = cp;
	while(c != ' ' &amp;&amp; c != '\t') {
		*cp++ = c;
		c = get();
		if(c == '\n') {
			*cp++ = 0;
			tp = hash(np);
			if(tp-&gt;name)
				goto redef;
			tp-&gt;name = np;
			tp-&gt;factor = lp-&gt;factor;
			for(c=0; c&lt;NDIM; c++)
				tp-&gt;dim[c] = lp-&gt;dim[c];
			i++;
			goto l0;
		}
	}
	*cp++ = 0;
	lp = hash(np);
	if(lp-&gt;name)
		goto redef;
	convr(lp);
	lp-&gt;name = np;
	f = 0;
	i++;
	if(lp-&gt;factor != 1.0)
		goto l0;
	for(c=0; c&lt;NDIM; c++) {
		t = lp-&gt;dim[c];
		if(t&gt;1 || (f&gt;0 &amp;&amp; t!=0))
			goto l0;
		if(f==0 &amp;&amp; t==1) {
			if(unames[c])
				goto l0;
			f = c+1;
		}
	}
	if(f&gt;0)
		unames[f-1] = np;
	goto l0;

redef:
	printf("redefination %s\n", np);
	goto l0;
}

double
getflt()
{
	register c, i, dp;
	double d, e;
	int f;

	d = 0.;
	dp = 0;
	do
		c = get();
	while(c == ' ' || c == '\t');

l1:
	if(c &gt;= '0' &amp;&amp; c &lt;= '9') {
		d = d*10. + c-'0';
		if(dp)
			dp++;
		c = get();
		goto l1;
	}
	if(c == '.') {
		dp++;
		c = get();
		goto l1;
	}
	if(dp)
		dp--;
	if(c == '+' || c == '-') {
		f = 0;
		if(c == '-')
			f++;
		i = 0;
		c = get();
		while(c &gt;= '0' &amp;&amp; c &lt;= '9') {
			i = i*10 + c-'0';
			c = get();
		}
		if(f)
			i = -i;
		dp =- i;
	}
	e = 1.;
	i = dp;
	if(i &lt; 0)
		i = -i;
	while(i--)
		e =* 10.;
	if(dp &lt; 0)
		d =* e; else
		d =/ e;
	if(c == '|')
		return(d/getflt());
	peekc = c;
	return(d);
}

get()
{
	register c;

	if(c=peekc) {
		peekc = 0;
		return(c);
	}
	c = getc(ibuf);
	if(c &lt;= 0) {
		if(ibuf[0])
			return(0);
		printf("\n");
		exit();
	}
	return(c);
}

hash(name)
char *name;
{
	register struct table *tp;
	register char *np;
	register h;

	h = 0;
	np = name;
	while(*np)
		h = h*57 + *np++ - '0';
	h = lrem(0, h, NTAB);
	tp = &amp;table[h];
l0:
	if(tp-&gt;name == 0)
		return(tp);
	if(equal(name, tp-&gt;name))
		return(tp);
	tp++;
	if(tp &gt;= &amp;table[NTAB])
		tp = table;
	goto l0;
}

fperr()
{

	signal(8, fperr);
	fperrc++;
}











