#include "sno.h"

/*
 *   Snobol III
 */


int	freesize;
struct node *freespace &amp;end;
struct node *freelist 0;
int	*fault -1;

mes(s) {
	sysput(strstr(s));
}

init(s, t) {
	register struct node *a, *b;

	a = strstr(s);
	b = look(a);
	delete(a);
	b-&gt;typ = t;
	return(b);
}

main(argc, argv)
char *argv[];
{
	extern fin, fout;
	register struct node *a, *b, *c;

	if(argc &gt; 1) {
		fin = open(argv[1], 0);
		if(fin &lt; 0) {
			mes("cannot open input");
			exit();
		}
	}
	fout = dup(1);
	lookf = init("f", 0);
	looks = init("s", 0);
	lookend = init("end", 0);
	lookstart = init("start", 0);
	lookdef = init("define", 0);
	lookret = init("return", 0);
	lookfret = init("freturn", 0);
	init("syspit", 3);
	init("syspot", 4);
	a = c = compile();
	while (lookend-&gt;typ != 2) {
		a-&gt;p1 = b = compile();
		a = b;
	}
	cfail = 1;
	a-&gt;p1 = 0;
	if (lookstart-&gt;typ == 2)
		c = lookstart-&gt;p2;
	while (c=execute(c));
	flush();
}

syspit() {
	extern fin;
	register struct node *b, *c, *d;
	int a;

	if ((a=getchar())=='\n')
		return(0);
	b = c = alloc();
	while(a != '\n') {
		c-&gt;p1 = d = alloc();
		c = d;
	l:
		c-&gt;ch = a;
		if(a == '\0') {
			if(fin) {
				close(fin);
				fin = 0;
				a = getchar();
				goto l;
			}
			rfail = 1;
			break;
		}
		a = getchar();
	}
	b-&gt;p2 = c;
	if(rfail) {
		delete(b);
		b = 0;
	}
	return(b);
}

syspot(string)
struct node *string;
{
	register struct node *a, *b, *s;

	s = string;
	if (s!=0) {
		a = s;
		b = s-&gt;p2;
		while(a != b) {
			a = a-&gt;p1;
			putchar(a-&gt;ch);
		}
	}
	putchar('\n');
}

strstr(s)
char s[];
{
	int c;
	register struct node *e, *f, *d;

	d = f = alloc();
	while ((c = *s++)!='\0') {
		(e=alloc())-&gt;ch = c;
		f-&gt;p1 = e;
		f = e;
	}
	d-&gt;p2 = e;
	return(d);
}

class(c) {
	switch (c) {
		case ')':  return(1);
		case '(':  return(2);
		case '\t':
		case ' ': return(3);
		case '+':  return(4);
		case '-':  return(5);
		case '*': return(6);
		case '/':  return(7);
		case '$':  return(8);
		case '"':
		case '\'': return(9);
		case '=':  return(10);
		case ',':  return(11);
	}
	return(0);
}

alloc() {
	register struct node *f;
	register int i;
	extern fout;

	if (freelist==0) {
		if (--freesize &lt; 20) {
			if ((i=sbrk(1200)) == -1) {
				flush();
				write (fout, "Out of free space\n", 18);
				exit();
			}
			freesize =+ 200;
		}
		return(freespace++);
	}
	f = freelist;
	freelist = freelist-&gt;p1;
	return(f);
}

free(pointer)
struct node *pointer;
{
	pointer-&gt;p1 = freelist;
	freelist = pointer;
}

nfree()
{
	register int i;
	register struct node *a;

	i = freesize;
	a = freelist;
	while(a) {
		a = a-&gt;p1;
		i++;
	}
	return(i);
}

look(string)
struct node *string;
{
	register struct node *i, *j, *k;

	k = 0;
	i = namelist;
	while (i) {
		j = i-&gt;p1;
		if (equal(j-&gt;p1, string) == 0)
			return(j);
		i = (k=i)-&gt;p2;
	}
	i = alloc();
	i-&gt;p2 = 0;
	if (k)
		k-&gt;p2 = i;
	else
		namelist = i;
	j = alloc();
	i-&gt;p1 = j;
	j-&gt;p1 = copy(string);
	j-&gt;p2 = 0;
	j-&gt;typ = 0;
	return(j);
}

copy(string)
struct node *string;
{
	register struct node *j, *l, *m;
	struct node *i, *k;

	if (string == 0)
		return(0);
	i = l = alloc();
	j = string;
	k = string-&gt;p2;
	while(j != k) {
		m = alloc();
		m-&gt;ch = (j=j-&gt;p1)-&gt;ch;
		l-&gt;p1 = m;
		l = m;
	}
	i-&gt;p2 = l;
	return(i);
}

equal(string1, string2)
struct node *string1, *string2;
{
	register struct node *i, *j, *k;
	struct node *l;
	int n, m;

	if (string1==0) {
		if (string2==0)
			return(0);
		return(-1);
	}
	if (string2==0)
		return(1);
	i = string1;
	j = string1-&gt;p2;
	k = string2;
	l = string2-&gt;p2;
	for(;;) {
		m = (i=i-&gt;p1)-&gt;ch;
		n = (k=k-&gt;p1)-&gt;ch;
		if (m&gt;n)
			return(1);
		if (m&lt;n)
			return(-1);
		if (i==j) {
			if (k==l)
				return(0);
			return(-1);
		}
		if (k==l)
			return(1);
	}
}

strbin(string)
struct node *string;
{
	int n, m, sign;
	register struct node *p, *q, *s;

	s = string;
	n = 0;
	if (s==0)
		return(0);
	p = s-&gt;p1;
	q = s-&gt;p2;
	sign = 1;
	if (class(p-&gt;ch)==5) { /* minus */
		sign = -1;
		if (p==q)
			return(0);
		p = p-&gt;p1;
	}
loop:
	m = p-&gt;ch - '0';
	if (m&gt;9 | m&lt;0)
		writes("bad integer string");
	n = n * 10 + m;
	if (p==q)
		return(n*sign);
	p = p-&gt;p1;
	goto loop;
}

binstr(binary) {
	int n, sign;
	register struct node *m, *p, *q;

	n = binary;
	p = alloc();
	q = alloc();
	sign = 1;
	if (binary&lt;0) {
		sign = -1;
		n = -binary;
	}
	p-&gt;p2 = q;
loop:
	q-&gt;ch = n%10+'0';
	n = n / 10;
	if (n==0) {
		if (sign&lt;0) {
			m = alloc();
			m-&gt;p1 = q;
			q = m;
			q-&gt;ch = '-';
		}
		p-&gt;p1 = q;
		return(p);
	}
	m = alloc();
	m-&gt;p1 = q;
	q = m;
	goto loop;
}

add(string1, string2) {
	return(binstr(strbin(string1) + strbin(string2)));
}

sub(string1, string2) {
	return(binstr(strbin(string1) - strbin(string2)));
}

mult(string1, string2) {
	return(binstr(strbin(string1) * strbin(string2)));
}

div(string1, string2) {
	return(binstr(strbin(string1) / strbin(string2)));
}

cat(string1, string2) 
struct node *string1, *string2;
{
	register struct node *a, *b;

	if (string1==0)
		return(copy(string2));
	if (string2==0)
		return(copy(string1));
	a = copy(string1);
	b = copy(string2);
	a-&gt;p2-&gt;p1 = b-&gt;p1;
	a-&gt;p2 = b-&gt;p2;
	free(b);
	return(a);
}

dcat(a,b)
struct node *a, *b;
{
	register struct node *c;

	c = cat(a,b);
	delete(a);
	delete(b);
	return(c);
}

delete(string)
struct node *string;
{
	register struct node *a, *b, *c;

	if (string==0)
		return;
	a = string;
	b = string-&gt;p2;
	while(a != b) {
		c = a-&gt;p1;
		free(a);
		a = c;
	}
	free(a);
}

sysput(string) {
	syspot(string);
	delete(string);
}

dump()
{
	dump1(namelist);
}

dump1(base)
struct node *base;
{
	register struct node *b, *c, *e;
	struct node *d;

	while (base) {
		b = base-&gt;p1;
		c = binstr(b-&gt;typ);
		d = strstr("  ");
		e = dcat(c, d);
		sysput(cat(e, b-&gt;p1));
		delete(e);
		if (b-&gt;typ==1) {
			c = strstr("   ");
			sysput(cat(c, b-&gt;p2));
			delete(c);
		}
		base = base-&gt;p2;
	}
}

writes(s) {

	sysput(dcat(binstr(lc),dcat(strstr("\t"),strstr(s))));
	flush();
	if (cfail) {
		dump();
		flush();
		exit();
	}
	while(getc());
	while (compile());
	flush();
	exit();
}

getc() {
	register struct node *a;
	static struct node *line;
	static linflg;

	while (line==0) {
		line = syspit();
		if(rfail) {
			cfail++;
			writes("eof on input");
		}
		lc++;
	}
	if (linflg) {
		line = 0;
		linflg = 0;
		return(0);
	}
	a = line-&gt;p1;
	if (a==line-&gt;p2) {
		free(line);
		linflg++;
	} else
		line-&gt;p1 = a-&gt;p1;
	return(a);
}











