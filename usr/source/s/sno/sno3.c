<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/sno/sno3.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/sno/sno3.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/sno/sno3.c>
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
 * sno3
 */


bextend(str, last)
struct node *str, *last;
{
	register struct node *a, *s;
	register int b;
	int c, d;

	s = str;
	if ((c = s-&gt;p1) == 0)
		goto bad;
	b = d = 0;
	a = s-&gt;p2;
	if(a == 0) {
		a = c;
		goto eb2;
	}
eb1:
	if (a == last)
		goto bad;
	a = a-&gt;p1;
eb2:
	d++;
	c = class(a-&gt;ch);
	if (c == 1) { /* rp */
		if (b == 0)
			goto bad;
		b--;
		goto eb3;
	}
	if (c == 2) { /* lp */
		b++;
		goto eb1;
	}
eb3:
	if (b == 0) {
		s-&gt;p2= a;
		return(d);
	}
	goto eb1;
bad:
	return(0);
}

ubextend(str, last)
struct node *str, *last;
{
	register struct node *a, *b, *s;

	s = str;
	a = s-&gt;p1;
	if(a == 0)
		goto bad;
	b = s-&gt;p2;
	if(b == 0)
		goto good;
	if (b == last)
		goto bad;
	a = b-&gt;p1;
good:
	s-&gt;p2 = a;
	return(1);
bad:
	return(0);
}

search(arg, r)
struct node *arg, *r;
{
	struct node *list, *back, *str,
		*etc, *next, *last, *base, *e;
	register struct node *a, *b, *var;
	int c, d;

	a = arg-&gt;p2;
	list = base = alloc();
	last = next = 0;
	goto badv1;
badvanc:
	a = a-&gt;p1;
	if (a-&gt;typ == 0) {
		list-&gt;p1 = 0;
		if (rfail == 1) {
			a = 0;
			goto fail;
		}
		list = base;
		if (r == 0)
			next = last = 0; else {
			next = r-&gt;p1;
			last = r-&gt;p2;
		}
		goto adv1;
	}
	b = alloc();
	list-&gt;p1 = b;
	list = b;
badv1:
	list-&gt;p2 = back = alloc();
	back-&gt;p1 = last;
	b = a-&gt;p2;
	c = a-&gt;typ;
	list-&gt;typ = c;
	if (c &lt; 2) {
		back-&gt;p2 = eval(b, 1);
		goto badvanc;
	}
	last = list;
	str = alloc();
	etc = alloc();
	back-&gt;p2 = var = alloc();
	var-&gt;typ = b-&gt;typ;
	var-&gt;p1 = str;
	var-&gt;p2 = etc;
	e = b-&gt;p1;
	if (e == 0)
		etc-&gt;p1 = 0; else
		etc-&gt;p1 = eval(e, 0);
	e = b-&gt;p2;
	if (e == 0)
		etc-&gt;p2 = 0; else {
		e = eval(e, 1);
		etc-&gt;p2 = strbin(e);
		delete(e);
	}
	goto badvanc;

retard:
	a = back-&gt;p1;
	if (a == 0) {
		rfail = 1;
		goto fail;
	}
	list = a;
	back = list-&gt;p2;
	var = back-&gt;p2;
	str = var-&gt;p1;
	etc = var-&gt;p2;
	if (etc-&gt;p2)
		goto retard;
	if (var-&gt;typ == 1) {
		if (bextend(str, last) == 0)
			goto retard;
		goto adv0;
	}
	if (ubextend(str, last) == 0)
		goto retard;
adv0:
	a = str-&gt;p2;
adv01:
	if (a == last)
		next = 0; else
		next = a-&gt;p1;
advanc:
	a = list-&gt;p1;
	if (a == 0) {
		a = alloc();
		if (r == 0) {
			a-&gt;p1 = a-&gt;p2 = 0;
			goto fail;
		}
		b = r-&gt;p1;
		a-&gt;p1 = b;
		if (next == 0) {
			a-&gt;p2 = r-&gt;p2;
			goto fail;
		}
		while(1) {
			e = b-&gt;p1;
			if (e == next) {
				a-&gt;p2 = b;
				goto fail;
			}
			b = e;
		}
	}
	list = a;
adv1:
	back = list-&gt;p2;
	var = back-&gt;p2;
	d = list-&gt;typ;
	if(d &lt; 2) {
		if (var == 0)
			goto advanc;
		if (next == 0)
			goto retard;
		a = next;
		b = var-&gt;p1;
		e = var-&gt;p2;
		while(1) {
			if (a-&gt;ch != b-&gt;ch)
				goto retard;
			if (b == e)
				goto adv01;
			if (a == last)
				goto retard;
			a = a-&gt;p1;
			b = b-&gt;p1;
		}
	}
	str = var-&gt;p1;
	etc = var-&gt;p2;
	str-&gt;p1 = next;
	str-&gt;p2 = 0;
	c = etc-&gt;p2;
	if (var-&gt;typ == 1) {
		d = bextend(str, last);
		if (d == 0)
			goto retard;
		if (c == 0)
			goto adv0;
		while(1) {
			c =- d;
			if (c == 0)
				goto adv0;
			if (c &lt; 0)
				goto retard;
			d = bextend(str, last);
			if (d == 0)
				goto retard;
		}
	}
	if (c == 0) {
		if(d==3 &amp; next!=0) {
			str-&gt;p2 = last;
			goto adv0;
		}
		goto advanc;
	}
	while(c--)
		if (ubextend(str, last) == 0)
			goto retard;
	goto adv0;

fail:
	list = base;
	goto f1;
fadv:
	free(back);
	b = list-&gt;p1;
	free(list);
	if (b == 0)
		return(a);
	list = b;
f1:
	back = list-&gt;p2;
	var = back-&gt;p2;
	if (list-&gt;typ &lt; 2) {
		delete(var);
		goto fadv;
	}
	str = var-&gt;p1;
	etc = var-&gt;p2;
	if (a != 0 &amp; etc-&gt;p1 != 0) {
		if (str-&gt;p2 == 0) {
			free(str);
			str = 0;
		}
		assign(etc-&gt;p1, copy(str));
	}
	if (str)
		free(str);
	free(etc);
	free(var);
	goto fadv;
}











