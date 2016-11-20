/*
 * C object code improver-- second part
 */

#include "c2h.c"

rmove()
{
	register struct node *p;
	register char *cp;
	register int r;
	int r1, flt;

	for (p=first.forw; p!=0; p = p-&gt;forw) {
	if (debug) {
		for (r=0; r&lt;2*NREG; r++)
			if (regs[r][0])
				printf("%d: %s\n", r, regs[r]);
		printf("-\n");
	}
	flt = 0;
	switch (p-&gt;op) {

	case MOVF:
	case MOVFO:
	case MOVOF:
		flt = NREG;

	case MOV:
		if (p-&gt;subop==BYTE)
			goto badmov;
		dualop(p);
		if ((r = findrand(regs[RT1], flt)) &gt;= 0) {
			if (r == flt+isreg(regs[RT2]) &amp;&amp; p-&gt;forw-&gt;op!=CBR) {
				p-&gt;forw-&gt;back = p-&gt;back;
				p-&gt;back-&gt;forw = p-&gt;forw;
				redunm++;
				continue;
			}
		}
		repladdr(p, 0, flt);
		r = isreg(regs[RT1]);
		r1 = isreg(regs[RT2]);
		dest(regs[RT2], flt);
		if (r &gt;= 0)
			if (r1 &gt;= 0)
				savereg(r1+flt, regs[r+flt]);
			else
				savereg(r+flt, regs[RT2]);
		else
			if (r1 &gt;= 0)
				savereg(r1+flt, regs[RT1]);
			else
				setcon(regs[RT1], regs[RT2]);
		source(regs[RT1]);
		setcc(regs[RT2]);
		continue;

	case ADDF:
	case SUBF:
	case DIVF:
	case MULF:
		flt = NREG;

	case ADD:
	case SUB:
	case BIC:
	case BIS:
	case MUL:
	case DIV:
	case ASH:
	badmov:
		dualop(p);
		repladdr(p, 0, flt);
		source(regs[RT1]);
		dest(regs[RT2], flt);
		if (p-&gt;op==DIV &amp;&amp; (r = isreg(regs[RT2])&gt;=0))
			regs[r+1][0] = 0;
		ccloc[0] = 0;
		continue;

	case CLRF:
	case NEGF:
		flt = NREG;

	case CLR:
	case COM:
	case INC:
	case DEC:
	case NEG:
	case ASR:
	case ASL:
	case SXT:
		singop(p);
		dest(regs[RT1], flt);
		if (p-&gt;op==CLR &amp;&amp; flt==0)
			if ((r = isreg(regs[RT1])) &gt;= 0)
				savereg(r, "$0");
			else
				setcon("$0", regs[RT1]);
		setcc(regs[RT1]);
		continue;

	case TSTF:
		flt = NREG;

	case TST:
		singop(p);
		repladdr(p, 0, flt);
		source(regs[RT1]);
		if (equstr(regs[RT1], ccloc)) {
			p-&gt;back-&gt;forw = p-&gt;forw;
			p-&gt;forw-&gt;back = p-&gt;back;
			p = p-&gt;back;
			nrtst++;
			nchange++;
		}
		continue;

	case CMPF:
		flt = NREG;

	case CMP:
	case BIT:
		dualop(p);
		source(regs[RT1]);
		source(regs[RT2]);
		repladdr(p, 1, flt);
		ccloc[0] = 0;
		continue;

	case CBR:
	case CFCC:
		ccloc[0] = 0;
		continue;

	case JBR:
		redunbr(p);

	default:
		clearreg();
	}
	}
}

jumpsw()
{
	register struct node *p, *p1;
	register t;
	int nj;

	t = 0;
	nj = 0;
	for (p=first.forw; p!=0; p = p-&gt;forw)
		p-&gt;refc = ++t;
	for (p=first.forw; p!=0; p = p1) {
		p1 = p-&gt;forw;
		if (p-&gt;op == CBR &amp;&amp; p1-&gt;op==JBR &amp;&amp; p-&gt;ref &amp;&amp; p1-&gt;ref
		 &amp;&amp; abs(p-&gt;refc - p-&gt;ref-&gt;refc) &gt; abs(p1-&gt;refc - p1-&gt;ref-&gt;refc)) {
			p-&gt;subop = revbr[p-&gt;subop];
			t = p1-&gt;ref;
			p1-&gt;ref = p-&gt;ref;
			p-&gt;ref = t;
			t = p1-&gt;labno;
			p1-&gt;labno = p-&gt;labno;
			p-&gt;labno = t;
			nrevbr++;
			nj++;
		}
	}
	return(nj);
}

addsob()
{
	register struct node *p, *p1;

	for (p = &amp;first; (p1 = p-&gt;forw)!=0; p = p1) {
		if (p-&gt;op==DEC &amp;&amp; isreg(p-&gt;code)&gt;=0
		 &amp;&amp; p1-&gt;combop==(CBR|JNE&lt;&lt;8)) {
			if (p-&gt;refc &lt; p1-&gt;ref-&gt;refc)
				continue;
			if (p-&gt;refc - p1-&gt;ref-&gt;refc &gt; 50)
				continue;
			p-&gt;labno = p1-&gt;labno;
			p-&gt;combop = SOB;
			p1-&gt;forw-&gt;back = p;
			p-&gt;forw = p1-&gt;forw;
			nsob++;
		}
	}
}

abs(x)
{
	return(x&lt;0? -x: x);
}

equop(ap1, p2)
struct node *ap1, *p2;
{
	register char *cp1, *cp2;
	register struct node *p1;

	p1 = ap1;
	if (p1-&gt;combop != p2-&gt;combop)
		return(0);
	if (p1-&gt;op&gt;0 &amp;&amp; p1-&gt;op&lt;MOV)
		return(0);
	cp1 = p1-&gt;code;
	cp2 = p2-&gt;code;
	if (cp1==0 &amp;&amp; cp2==0)
		return(1);
	if (cp1==0 || cp2==0)
		return(0);
	while (*cp1 == *cp2++)
		if (*cp1++ == 0)
			return(1);
	return(0);
}

decref(ap)
{
	register struct node *p;

	p = ap;
	if (--p-&gt;refc &lt;= 0) {
		nrlab++;
		p-&gt;back-&gt;forw = p-&gt;forw;
		p-&gt;forw-&gt;back = p-&gt;back;
	}
}

nonlab(ap)
struct node *ap;
{
	register struct node *p;

	p = ap;
	while (p &amp;&amp; p-&gt;op==LABEL)
		p = p-&gt;forw;
	return(p);
}

alloc(an)
{
	register int n;
	register char *p;

	n = an;
	n++;
	n =&amp; ~01;
	if (lasta+n &gt;= lastr) {
		if (sbrk(2000) == -1) {
			write(2, "Optimizer: out of space\n", 14);
			exit(1);
		}
		lastr =+ 2000;
	}
	p = lasta;
	lasta =+ n;
	return(p);
}

clearreg()
{
	register int i;

	for (i=0; i&lt;2*NREG; i++)
		regs[i][0] = '\0';
	conloc[0] = 0;
	ccloc[0] = 0;
}

savereg(ai, as)
char *as;
{
	register char *p, *s, *sp;

	sp = p = regs[ai];
	s = as;
	if (source(s))
		return;
	while (*p++ = *s) {
		if (s[0]=='(' &amp;&amp; s[1]=='r' &amp;&amp; s[2]&lt;'5') {
			*sp = 0;
			return;
		}
		if (*s++ == ',')
			break;
	}
	*--p = '\0';
}

dest(as, flt)
char *as;
{
	register char *s;
	register int i;

	s = as;
	if ((i = isreg(s)) &gt;= 0)
		regs[i+flt][0] = 0;
	while ((i = findrand(s, flt)) &gt;= 0)
		regs[i][0] = 0;
	while (*s) {
		if ((*s=='(' &amp;&amp; (*(s+1)!='r' || *(s+2)!='5')) || *s++=='*') {
			for (i=flt; i&lt;flt+NREG; i++) {
				if (regs[i][0] != '$')
					regs[i][0] = 0;
				conloc[0] = 0;
			}
			return;
		}
	}
}

singop(ap)
struct node *ap;
{
	register char *p1, *p2;

	p1 = ap-&gt;code;
	p2 = regs[RT1];
	while (*p2++ = *p1++);
	regs[RT2][0] = 0;
}


dualop(ap)
struct node *ap;
{
	register char *p1, *p2;
	register struct node *p;

	p = ap;
	p1 = p-&gt;code;
	p2 = regs[RT1];
	while (*p1 &amp;&amp; *p1!=',')
		*p2++ = *p1++;
	*p2++ = 0;
	p2 = regs[RT2];
	*p2 = 0;
	if (*p1++ !=',')
		return;
	while (*p2++ = *p1++);
}

findrand(as, flt)
char *as;
{
	register int i;
	for (i = flt; i&lt;NREG+flt; i++) {
		if (equstr(regs[i], as))
			return(i);
	}
	return(-1);
}

isreg(as)
char *as;
{
	register char *s;

	s = as;
	if (s[0]=='r' &amp;&amp; s[1]&gt;='0' &amp;&amp; s[1]&lt;='4' &amp;&amp; s[2]==0)
		return(s[1]-'0');
	return(-1);
}

check()
{
	register struct node *p, *lp;

	lp = &amp;first;
	for (p=first.forw; p!=0; p = p-&gt;forw) {
		if (p-&gt;back != lp)
			abort();
		lp = p;
	}
}

source(ap)
char *ap;
{
	register char *p1, *p2;

	p1 = ap;
	p2 = p1;
	if (*p1==0)
		return(0);
	while (*p2++);
	if (*p1=='-' &amp;&amp; *(p1+1)=='('
	 || *p1=='*' &amp;&amp; *(p1+1)=='-' &amp;&amp; *(p1+2)=='('
	 || *(p2-2)=='+') {
		while (*p1 &amp;&amp; *p1++!='r');
		if (*p1&gt;='0' &amp;&amp; *p1&lt;='4')
			regs[*p1 - '0'][0] = 0;
		return(1);
	}
	return(0);
}

repladdr(p, f, flt)
struct node *p;
{
	register r;
	int r1;
	register char *p1, *p2;
	static char rt1[50], rt2[50];

	if (f)
		r1 = findrand(regs[RT2], flt);
	else
		r1 = -1;
	r = findrand(regs[RT1], flt);
	if (r1 &gt;= NREG)
		r1 =- NREG;
	if (r &gt;= NREG)
		r =- NREG;
	if (r&gt;=0 || r1&gt;=0) {
		p2 = regs[RT1];
		for (p1 = rt1; *p1++ = *p2++;);
		if (regs[RT2][0]) {
			p1 = rt2;
			*p1++ = ',';
			for (p2 = regs[RT2]; *p1++ = *p2++;);
		} else
			rt2[0] = 0;
		if (r&gt;=0) {
			rt1[0] = 'r';
			rt1[1] = r + '0';
			rt1[2] = 0;
			nsaddr++;
		}
		if (r1&gt;=0) {
			rt2[1] = 'r';
			rt2[2] = r1 + '0';
			rt2[3] = 0;
			nsaddr++;
		}
		p-&gt;code = copy(rt1, rt2);
	}
}

movedat()
{
	register struct node *p1, *p2;
	struct node *p3;
	register seg;
	struct node data;
	struct node *datp;

	if (first.forw == 0)
		return;
	datp = &amp;data;
	for (p1 = first.forw; p1!=0; p1 = p1-&gt;forw) {
		if (p1-&gt;op == DATA) {
			p2 = p1-&gt;forw;
			while (p2 &amp;&amp; p2-&gt;op!=TEXT)
				p2 = p2-&gt;forw;
			if (p2==0)
				break;
			p3 = p1-&gt;back;
			p1-&gt;back-&gt;forw = p2-&gt;forw;
			p2-&gt;forw-&gt;back = p3;
			p2-&gt;forw = 0;
			datp-&gt;forw = p1;
			p1-&gt;back = datp;
			p1 = p3;
			datp = p2;
		}
	}
	if (data.forw) {
		datp-&gt;forw = first.forw;
		first.forw-&gt;back = datp;
		data.forw-&gt;back = &amp;first;
		first.forw = data.forw;
	}
	seg = -1;
	for (p1 = first.forw; p1!=0; p1 = p1-&gt;forw) {
		if (p1-&gt;op==TEXT||p1-&gt;op==DATA||p1-&gt;op==BSS) {
			if (p1-&gt;op == seg || p1-&gt;forw&amp;&amp;p1-&gt;forw-&gt;op==seg) {
				p1-&gt;back-&gt;forw = p1-&gt;forw;
				p1-&gt;forw-&gt;back = p1-&gt;back;
				p1 = p1-&gt;back;
				continue;
			}
			seg = p1-&gt;op;
		}
	}
}

redunbr(ap)
struct node *ap;
{
	register struct node *p, *p1;
	register char *ap1;
	char *ap2;

	if ((p1 = p-&gt;ref) == 0)
		return;
	p1 = nonlab(p1);
	if (p1-&gt;op==TST) {
		singop(p1);
		savereg(RT2, "$0");
	} else if (p1-&gt;op==CMP)
		dualop(p1);
	else
		return;
	if (p1-&gt;forw-&gt;op!=CBR)
		return;
	ap1 = findcon(RT1);
	ap2 = findcon(RT2);
	p1 = p1-&gt;forw;
	if (compare(p1-&gt;subop, ap1, ap2)) {
		nredunj++;
		nchange++;
		decref(p-&gt;ref);
		p-&gt;ref = p1-&gt;ref;
		p-&gt;labno = p1-&gt;labno;
		p-&gt;ref-&gt;refc++;
	}
}

findcon(i)
{
	register char *p;
	register r;

	p = regs[i];
	if (*p=='$')
		return(p);
	if ((r = isreg(p)) &gt;= 0)
		return(regs[r]);
	if (equstr(p, conloc))
		return(conval);
	return(p);
}

compare(op, acp1, acp2)
char *acp1, *acp2;
{
	register char *cp1, *cp2;
	register n1;
	int n2;
	struct { int i;};

	cp1 = acp1;
	cp2 = acp2;
	if (*cp1++ != '$' || *cp2++ != '$')
		return(0);
	n1 = 0;
	while (*cp2 &gt;= '0' &amp;&amp; *cp2 &lt;= '7') {
		n1 =&lt;&lt; 3;
		n1 =+ *cp2++ - '0';
	}
	n2 = n1;
	n1 = 0;
	while (*cp1 &gt;= '0' &amp;&amp; *cp1 &lt;= '7') {
		n1 =&lt;&lt; 3;
		n1 =+ *cp1++ - '0';
	}
	if (*cp1=='+')
		cp1++;
	if (*cp2=='+')
		cp2++;
	do {
		if (*cp1++ != *cp2)
			return(0);
	} while (*cp2++);
	cp1 = n1;
	cp2 = n2;
	switch(op) {

	case JEQ:
		return(cp1 == cp2);
	case JNE:
		return(cp1 != cp2);
	case JLE:
		return(cp1.i &lt;= cp2.i);
	case JGE:
		return(cp1.i &gt;= cp2.i);
	case JLT:
		return(cp1.i &lt; cp2.i);
	case JGT:
		return(cp1.i &gt; cp2.i);
	case JLO:
		return(cp1 &lt; cp2);
	case JHI:
		return(cp1 &gt; cp2);
	case JLOS:
		return(cp1 &lt;= cp2);
	case JHIS:
		return(cp1 &gt;= cp2);
	}
	return(0);
}

setcon(ar1, ar2)
char *ar1, *ar2;
{
	register char *cl, *cv, *p;

	cl = ar2;
	cv = ar1;
	if (*cv != '$')
		return;
	if (!natural(cl))
		return;
	p = conloc;
	while (*p++ = *cl++);
	p = conval;
	while (*p++ = *cv++);
}

equstr(ap1, ap2)
char *ap1, *ap2;
{
	char *p1, *p2;

	p1 = ap1;
	p2 = ap2;
	do {
		if (*p1++ != *p2)
			return(0);
	} while (*p2++);
	return(1);
}

setcc(ap)
char *ap;
{
	register char *p, *p1;

	p = ap;
	if (!natural(p)) {
		ccloc[0] = 0;
		return;
	}
	p1 = ccloc;
	while (*p1++ = *p++);
}

natural(ap)
char *ap;
{
	register char *p;

	p = ap;
	if (*p=='*' || *p=='(' || *p=='-'&amp;&amp;*(p+1)=='(')
		return(0);
	while (*p++);
	p--;
	if (*--p == '+' || *p ==')' &amp;&amp; *--p != '5')
		return(0);
	return(1);
}











