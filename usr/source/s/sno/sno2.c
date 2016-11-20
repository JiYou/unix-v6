<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/sno/sno2.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/sno/sno2.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/sno/sno2.c>
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
#include "sno.h"


compon() {
	register struct node *a, *b;
	register int c;
	static next;

	if (next == 0)
		schar = getc(); else
		next = 0;
	if (schar == 0) {
		(a=alloc())-&gt;typ = 0;
		return(a);
	}
	switch (class(schar-&gt;ch)) {

	case 1:
		schar-&gt;typ = 5;
		return(schar);

	case 2:
		schar-&gt;typ = 16;
		return(schar);

	case 3:
		a = schar;
		for(;;) {
			schar = getc();
			if (schar == 0) {
				a-&gt;typ = 0;
				return(a);
			}
			if (class(schar-&gt;ch) != 3)
				break;
			free(schar);
		}
		next = 1;
		a-&gt;typ = 7;
		return(a);

	case 4:
		schar-&gt;typ = 8;
		return(schar);

	case 5:
		schar-&gt;typ = 9;
		return(schar);

	case 6:
		a = schar;
		schar = getc();
		if (class(schar-&gt;ch) == 3)
			a-&gt;typ = 10; else
			a-&gt;typ = 1;
		next = 1;
		return(a);

	case 7:
		a = schar;
		schar = getc();
		if (class(schar-&gt;ch) == 3)
			a-&gt;typ = 11; else
			a-&gt;typ = 2;
		next = 1;
		return(a);

	case 8:
		schar-&gt;typ = 12;
		return(schar);

	case 9:
		c = schar-&gt;ch;
		a = getc();
		if(a == 0)
			goto lerr;
		b = schar;
		if(a-&gt;ch == c) {
			free(schar);
			a-&gt;typ = 15;
			a-&gt;p1 = 0;
			return(a);
		}
		b-&gt;p1 = a;
		for(;;) {
			schar = getc();
			if (schar == 0)
			lerr:
				writes("illegal literal string");
			if(schar-&gt;ch == c)
				break;
			a-&gt;p1 = schar;
			a = schar;
		}
		b-&gt;p2 = a;
		schar-&gt;typ = 15;
		schar-&gt;p1 = b;
		return(schar);

	case 10:
		schar-&gt;typ = 3;
		return(schar);

	case 11:
		schar-&gt;typ = 4;
		return(schar);

	}
	b = alloc();
	b-&gt;p1 = a = schar;
	schar = getc();
	while(schar!=0 &amp; !class(schar-&gt;ch)) {
		a-&gt;p1 = schar;
		a = schar;
		schar = getc();
	}
	b-&gt;p2 = a;
	next = 1;
	a = look(b);
	delete(b);
	b = alloc();
	b-&gt;typ = 14;
	b-&gt;p1 = a;
	return(b);
}

nscomp()
{
	register struct node *c;

	while((c=compon())-&gt;typ == 7)
		free(c);
	return(c);
}

push(stack) {
	register struct node *a;

	(a=alloc())-&gt;p2 = stack;
	return(a);
}

pop(stack)
struct node *stack;
{
	register struct node *a, *s;

	s = stack;
	if (s == 0)
		writes("pop");
	a = s-&gt;p2;
	free(s);
	return(a);
}

expr(start, eof, e)
struct node *e;
{
	register struct node *stack, *list, *comp;
	int operand, op, space, op1;
	struct node *a, *b, *c;
	int d;

	list = alloc();
	e-&gt;p2 = list;
	stack = push(0);
	stack-&gt;typ = eof;
	operand = 0;
	space = start;
l1:
	if (space) {
		comp = space;
		space = 0;
	} else
		comp = compon();

l3:
	op = comp-&gt;typ;
	switch (op) {

	case 7:
		space = 1;
		free(comp);
		comp = compon();
		goto l3;

	case 10:
		if (space == 0) {
			comp-&gt;typ = 1;
			goto l3;
		}

	case 11:
		if (space == 0) {
			comp-&gt;typ = 2;
			goto l3;
		}

	case 8:
	case 9:
		if (operand == 0)
			writes("no operand preceding operator");
		operand = 0;
		goto l5;

	case 14:
	case 15:
		if (operand == 0) {
			operand = 1;
			goto l5;
		}
		if (space == 0)
			goto l7;
		goto l4;

	case 12:
		if (operand == 0)
			goto l5;
		if (space)
			goto l4;
	l7:
		writes("illegal juxtaposition of operands");

	case 16:
		if (operand == 0)
			goto l5;
		if (space)
			goto l4;
		b = compon();
		op = comp-&gt;typ = 13;
		if (b-&gt;typ == 5) {
			comp-&gt;p1 = 0;
			goto l10;
		}
		comp-&gt;p1 = a = alloc();
		b = expr(b, 6, a);
		while((d=b-&gt;typ) == 4) {
			a-&gt;p1 = b;
			a = b;
			b = expr(0, 6, a);
		}
		if (d != 5)
			writes("error in function");
		a-&gt;p1 = 0;
	l10:
		free(b);
		goto l6;

	l4:
		space = comp;
		op = 7;
		operand = 0;
		goto l6;
	}
	if (operand==0)
		writes("no operand at end of expression");
l5:
	space = 0;
l6:
	op1 = stack-&gt;typ;
	if (op &gt; op1) {
		stack = push(stack);
		if (op == 16)
			op = 6;
		stack-&gt;typ = op;
		stack-&gt;p1 = comp;
		goto l1;
	}
	c = stack-&gt;p1;
	stack = pop(stack);
	if (stack == 0) {
		list-&gt;typ = 0;
		return(comp);
	}
	if (op1 == 6) {
		if (op != 5)
			writes("too many ('s");
		goto l1;
	}
	if (op1 == 7)
		c = alloc();
	list-&gt;typ = op1;
	list-&gt;p2 = c-&gt;p1;
	list-&gt;p1 = c;
	list = c;
	goto l6;
}

match(start, m)
struct node *m;
{
	register struct node *list, *comp, *term;
	struct node *a;
	int b, bal;

	term = bal = 0;
	list = alloc();
	m-&gt;p2 = list;
	comp = start;
	if (!comp)
		comp = compon();
	goto l2;

l3:
	list-&gt;p1 = a = alloc();
	list = a;
l2:
	switch (comp-&gt;typ) {
	case 7:
		free(comp);
		comp = compon();
		goto l2;

	case 12:
	case 14:
	case 15:
	case 16:
		term = 0;
		comp = expr(comp, 6, list);
		list-&gt;typ = 1;
		goto l3;

	case 1:
		free(comp);
		comp = compon();
		bal = 0;
		if (comp-&gt;typ == 16) {
			bal = 1;
			free(comp);
			comp = compon();
		}
		a = alloc();
		b = comp-&gt;typ;
		if (b == 2 | b == 5 | b == 10 | b == 1)
			a-&gt;p1 = 0; else {
			comp = expr(comp, 11, a);
			a-&gt;p1 = a-&gt;p2;
		}
		if (comp-&gt;typ != 2) {
			a-&gt;p2 = 0;
		} else {
			free(comp);
			comp = expr(0, 6, a);
		}
		if (bal) {
			if (comp-&gt;typ != 5)
				goto merr;
			free(comp);
			comp = compon();
		}
		b = comp-&gt;typ;
		if (b != 1 &amp; b != 10)
			goto merr;
		list-&gt;p2 = a;
		list-&gt;typ = 2;
		a-&gt;typ = bal;
		free(comp);
		comp = compon();
		if(bal)
			term = 0; else
			term = list;
		goto l3;
	}
	if(term)
		term-&gt;typ = 3;
	list-&gt;typ = 0;
	return(comp);

merr:
	writes("unrecognized component in match");
}

compile() {
	register struct node *b, *comp;
	struct node *r, *l, *xs, *xf, *g;
	register int a;
	int m, t, as;

	m = l = as = xs = xf = t = 0;
	comp = compon();
	a = comp-&gt;typ;
	if (a == 14) {
		l = comp-&gt;p1;
		free(comp);
		comp = compon();
		a = comp-&gt;typ;
	}
	if (a != 7)
		writes("no space beginning statement");
	free(comp);
	if (l == lookdef)
		goto def;
	comp = expr(0, 11, r=alloc());
	a = comp-&gt;typ;
	if (a == 0)
		goto asmble;
	if (a == 2)
		goto xfer;
	if (a == 3)
		goto assig;
	m = alloc();
	comp = match(comp, m);
	a = comp-&gt;typ;
	if (a == 0)
		goto asmble;
	if (a == 2)
		goto xfer;
	if (a == 3)
		goto assig;
	writes("unrecognized component in match");

assig:
	free(comp);
	comp = expr(0, 6, as=alloc());
	a = comp-&gt;typ;
	if (a == 0)
		goto asmble;
	if (a == 2)
		goto xfer;
	writes("unrecognized component in assignment");

xfer:
	free(comp);
	comp = compon();
	a = comp-&gt;typ;
	if (a == 16)
		goto xboth;
	if (a == 0) {
		if (xs!=0 | xf!=0)
			goto asmble;
		goto xerr;
	}
	if (a != 14)
		goto xerr;
	b = comp-&gt;p1;
	free(comp);
	if (b == looks)
		goto xsuc;
	if (b == lookf)
		goto xfail;

xerr:
	writes("unrecognized component in goto");

xboth:
	free(comp);
	xs = alloc();
	xf = alloc();
	comp = expr(0, 6, xs);
	if (comp-&gt;typ != 5)
		goto xerr;
	xf-&gt;p2 = xs-&gt;p2;
	comp = compon();
	if (comp-&gt;typ != 0)
		goto xerr;
	goto asmble;

xsuc:
	if(xs)
		goto xerr;
	comp = compon();
	if (comp-&gt;typ != 16)
		goto xerr;
	comp = expr(0, 6, xs=alloc());
	if (comp-&gt;typ != 5)
		goto xerr;
	goto xfer;

xfail:
	if (xf)
		goto xerr;
	comp = compon();
	if (comp-&gt;typ != 16)
		goto xerr;
	comp = expr(0, 6, xf=alloc());
	if (comp-&gt;typ != 5)
		goto xerr;
	goto xfer;

asmble:
	if(l) {
		if (l-&gt;typ)
			writes("name doubly defined");
		l-&gt;p2 = comp;
		l-&gt;typ = 2; /* type label;*/
	}
	comp-&gt;p2 = r;
	if (m) {
		t++;
		r-&gt;p1 = m;
		r = m;
	}
	if (as) {
		t =+ 2;
		r-&gt;p1 = as;
		r = as;
	}
	(g=alloc())-&gt;p1 = 0;
	if (xs) {
		g-&gt;p1 = xs-&gt;p2;
		free(xs);
	}
	g-&gt;p2 = 0;
	if (xf) {
		g-&gt;p2 = xf-&gt;p2;
		free(xf);
	}
	r-&gt;p1 = g;
	comp-&gt;typ = t;
	comp-&gt;ch = lc;
	return(comp);

def:
	r = nscomp();
	if (r-&gt;typ != 14)
		goto derr;
	l = r-&gt;p1;
	if (l-&gt;typ)
		writes("name doubly defined");
	l-&gt;typ = 5; /*type function;*/
	a = r;
	l-&gt;p2 = a;
	r = nscomp();
	l = r;
	a-&gt;p1 = l;
	if (r-&gt;typ == 0)
		goto d4;
	if (r-&gt;typ != 16)
		goto derr;

d2:
	r = nscomp();
	if (r-&gt;typ != 14)
		goto derr;
	a-&gt;p2 = r;
	r-&gt;typ = 0;
	a = r;
	r = nscomp();
	if (r-&gt;typ == 4) {
		free(r);
		goto d2;
	}
	if (r-&gt;typ != 5)
		goto derr;
	free(r);
	if ((r=compon())-&gt;typ != 0)
		goto derr;
	free(r);

d4:
	r = compile();
	a-&gt;p2 = 0;
	l-&gt;p1 = r;
	l-&gt;p2 = 0;
	return(r);

derr:
	writes("illegal component in define");
}











