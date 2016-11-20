<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s5/qsort.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s5/qsort.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s5/qsort.c>
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

int	(*qscmp)();
int	qses;

qsort(a, n, es, fc)
char *a;
int n, es;
int (*fc)();
{
	qscmp = fc;
	qses = es;
	qs1(a, a+n*es);
}

qs1(a, l)
char *a, *l;
{
	register char *i, *j, *es;
	char *lp, *hp;
	int n, c;


	es = qses;

start:
	if((n=l-a) &lt;= es)
		return;


	n = ((n/(2*es))*es) &amp; 077777;
	hp = lp = a+n;
	i = a;
	j = l-es;


	for(;;) {
		if(i &lt; lp) {
			if((c = (*qscmp)(i, lp)) == 0) {
				qsexc(i, lp =- es);
				continue;
			}
			if(c &lt; 0) {
				i =+ es;
				continue;
			}
		}

loop:
		if(j &gt; hp) {
			if((c = (*qscmp)(hp, j)) == 0) {
				qsexc(hp =+ es, j);
				goto loop;
			}
			if(c &gt; 0) {
				if(i == lp) {
					qstexc(i, hp =+ es, j);
					i = lp =+ es;
					goto loop;
				}
				qsexc(i, j);
				j =- es;
				i =+ es;
				continue;
			}
			j =- es;
			goto loop;
		}


		if(i == lp) {
			if(lp-a &gt;= l-hp) {
				qs1(hp+es, l);
				l = lp;
			} else {
				qs1(a, lp);
				a = hp+es;
			}
			goto start;
		}


		qstexc(j, lp =- es, i);
		j = hp =- es;
	}
}

qsexc(i, j)
char *i, *j;
{
	register char *ri, *rj, c;
	int n;

	n = qses;
	ri = i;
	rj = j;
	do {
		c = *ri;
		*ri++ = *rj;
		*rj++ = c;
	} while(--n);
}

qstexc(i, j, k)
char *i, *j, *k;
{
	register char *ri, *rj, *rk;
	char	c;
	int	n;

	n = qses;
	ri = i;
	rj = j;
	rk = k;
	do {
		c = *ri;
		*ri++ = *rk;
		*rk++ = *rj;
		*rj++ = c;
	} while(--n);
}











