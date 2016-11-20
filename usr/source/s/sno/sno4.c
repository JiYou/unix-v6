<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/sno/sno4.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/sno/sno4.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/sno/sno4.c>
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
/*
 * sno4
 */


and(ptr)
struct node *ptr;
{
	register struct node *a, *p;

	p = ptr;
	a = p-&gt;p1;
	if (p-&gt;typ == 0) {
		switch (a-&gt;typ) {
		case0:
		case 0:
			a-&gt;typ = 1;
		case 1:
			goto l1;
		case 3:
			flush();
			return(syspit());
		case 5:
			a = a-&gt;p2-&gt;p1;
			goto l1;
		case 6:
			return(binstr(nfree()));
		}
		writes("attempt to take an illegal value");
		goto case0;
	l1:
		a = copy(a-&gt;p2);
	}
	return(a);
}

eval(e, t)
struct node *e;
{
	struct node *list, *a2, *a3, *a4, *a3base;
	register struct node *a1, *stack, *op;

	if (rfail == 1)
		return(0);
	stack = 0;
	list = e;
	goto l1;
advanc:
	list = list-&gt;p1;
l1:
	op = list-&gt;typ;
	switch (op) {
		default:
		case 0:
		if (t == 1) {
			a1 = and(stack);
			goto e1;
		}
		if (stack-&gt;typ == 1)
			writes("attempt to store in a value");
		a1 = stack-&gt;p1;
	e1:
		stack = pop(stack);
		if (stack)
			writes("phase error");
		return(a1);
	case 12:
		a1 = and(stack);
		stack-&gt;p1 = look(a1);
		delete(a1);
		stack-&gt;typ = 0;
		goto advanc;
	case 13:
		if (stack-&gt;typ)
			writes("illegal function");
		a1 = stack-&gt;p1;
		if (a1-&gt;typ!=5)
			writes("illegal function");
		a1 = a1-&gt;p2;
		op = a1-&gt;p1;
		a3base = a3 = alloc();
		a3-&gt;p2 = op-&gt;p2;
		op-&gt;p2 = 0;
		a1 = a1-&gt;p2;
		a2 = list-&gt;p2;
	f1:
		if (a1!=0 &amp; a2!=0)
			goto f2;
		if (a1!=a2)
			writes("parameters do not match");
		op = op-&gt;p1;
		goto f3;
	f2:
		a3-&gt;p1 = a4 = alloc();
		a3 = a4;
		a3-&gt;p2 = and(a1);
		assign(a1-&gt;p1, eval(a2-&gt;p2, 1));/* recursive */
		a1 = a1-&gt;p2;
		a2 = a2-&gt;p1;
		goto f1;
	f3:
		op = execute(op); /* recursive */
		if (op)
			goto f3;
		a1 = stack-&gt;p1-&gt;p2;
		op = a1-&gt;p1;
		a3 = a3base;
		stack-&gt;p1 = op-&gt;p2;
		stack-&gt;typ = 1;
		op-&gt;p2 = a3-&gt;p2;
	f4:
		a4 = a3-&gt;p1;
		free(a3);
		a3 = a4;
		a1 = a1-&gt;p2;
		if (a1 == 0)
			goto advanc;
		assign(a1-&gt;p1, a3-&gt;p2);
		goto f4;
	case 11:
	case 10:
	case 9:
	case 8:
	case 7:
		a1 = and(stack);
		stack = pop(stack);
		a2 = and(stack);
		a3 = doop(op, a2, a1);
		delete(a1);
		delete(a2);
		stack-&gt;p1 = a3;
		stack-&gt;typ = 1;
		goto advanc;
	case 15:
		a1 = copy(list-&gt;p2);
		a2 = 1;
		goto l3;
	case 14:
		a1 = list-&gt;p2;
		a2 = 0;
	l3:
		stack = push(stack);
		stack-&gt;p1 = a1;
		stack-&gt;typ = a2;
		goto advanc;
	}
}

doop(op, arg1, arg2)
{
	register int a1, a2;

	a1 = arg1;
	a2 = arg2;
	switch (op) {

	case 11:
		return(div(a1, a2));
	case 10:
		return(mult(a1, a2));
	case 8:
		return(add(a1, a2));
	case 9:
		return(sub(a1, a2));
	case 7:
		return(cat(a1, a2));
	}
	return(0);
}

execute(e)
struct node *e;
{
	register struct node *r, *b, *c;
	struct node *m, *ca, *d, *a;

	r = e-&gt;p2;
	lc = e-&gt;ch;
	switch (e-&gt;typ) {
	case 0: /*  r g */
		a = r-&gt;p1;
		delete(eval(r-&gt;p2, 1));
		goto xsuc;
	case 1: /*  r m g */
		m = r-&gt;p1;
		a = m-&gt;p1;
		b = eval(r-&gt;p2, 1);
		c = search(m, b);
		delete(b);
		if (c == 0)
			goto xfail;
		free(c);
		goto xsuc;
	case 2: /*  r a g */
		ca = r-&gt;p1;
		a = ca-&gt;p1;
		b = eval(r-&gt;p2, 0);
		assign(b, eval(ca-&gt;p2, 1));
		goto xsuc;
	case 3: /*  r m a g */
		m = r-&gt;p1;
		ca = m-&gt;p1;
		a = ca-&gt;p1;
		b = eval(r-&gt;p2, 0);
		d = search(m, b-&gt;p2);
		if (d == 0)
			goto xfail;
		c = eval(ca-&gt;p2, 1);
		if (d-&gt;p1 == 0) {
			free(d);
			assign(b, cat(c, b-&gt;p2));
			delete(c);
			goto xsuc;
		}
		if (d-&gt;p2 == b-&gt;p2-&gt;p2) {
			assign(b, c);
			free(d);
			goto xsuc;
		}
		(r=alloc())-&gt;p1 = d-&gt;p2-&gt;p1;
		r-&gt;p2 = b-&gt;p2-&gt;p2;
		assign(b, cat(c, r));
		free(d);
		free(r);
		delete(c);
		goto xsuc;
	}
xsuc:
	if (rfail)
		goto xfail;
	b = a-&gt;p1;
	goto xboth;
xfail:
	rfail = 0;
	b = a-&gt;p2;
xboth:
	if (b == 0) {
		return(e-&gt;p1);
	}
	b = eval(b, 0);
	if (b == lookret)
		return(0);
	if (b == lookfret) {
		rfail = 1;
		return(0);
	}
	if (b-&gt;typ!=2)
		writes("attempt to transfer to non-label");
	return(b-&gt;p2);
}

assign(adr, val)
struct node *adr, *val;
{
	register struct node *a, *addr, *value;

	addr = adr;
	value = val;
	if (rfail == 1) {
		delete(value);
		return;
	}
	switch (addr-&gt;typ) {
	default:
		writes("attempt to make an illegal assignment");
	case 0:
		addr-&gt;typ = 1;
	case 1:
		delete(addr-&gt;p2);
		addr-&gt;p2 = value;
		return;
	case 4:
		sysput(value);
		return;
	case 5:
		a = addr-&gt;p2-&gt;p1;
		delete(a-&gt;p2);
		a-&gt;p2 = value;
		return;
	}
}











