/*
 *  C compiler
 */

#include "c1h.c"

max(a, b)
{
	if (a&gt;b)
		return(a);
	return(b);
}

degree(at)
struct tnode *at;
{
	register struct tnode *t, *t1;

	if ((t=at)==0 || t-&gt;op==0)
		return(0);
	if (t-&gt;op == CON)
		return(-3);
	if (t-&gt;op == AMPER)
		return(-2);
	if (t-&gt;op==ITOL &amp;&amp; (t1 = isconstant(t)) &amp;&amp; t1-&gt;value&gt;= 0)
		return(-2);
	if ((opdope[t-&gt;op] &amp; LEAF) != 0) {
		if (t-&gt;type==CHAR || t-&gt;type==FLOAT)
			return(1);
		return(0);
	}
	return(t-&gt;degree);
}

pname(ap, flag)
struct tnode *ap;
{
	register i;
	register struct tnode *p;

	p = ap;
loop:
	switch(p-&gt;op) {

	case SFCON:
	case CON:
		printf("$");
		psoct(p-&gt;value);
		return;

	case FCON:
		printf("L%d", p-&gt;value);
		return;

	case NAME:
		i = p-&gt;offset;
		if (flag==2)
			i =+ 2;
		if (i) {
			psoct(i);
			if (p-&gt;class!=OFFS)
				putchar('+');
			if (p-&gt;class==REG)
				regerr();
		}
		switch(p-&gt;class) {

		case SOFFS:
		case XOFFS:
			pbase(p);

		case OFFS:
			printf("(r%d)", p-&gt;regno);
			return;

		case EXTERN:
		case STATIC:
			pbase(p);
			return;

		case REG:
			printf("r%d", p-&gt;nloc);
			return;

		}
		error("Compiler error: pname");
		return;

	case AMPER:
		putchar('$');
		p = p-&gt;tr1;
		if (p-&gt;op==NAME &amp;&amp; p-&gt;class==REG)
			regerr();
		goto loop;

	case AUTOI:
		printf("(r%d)%c", p-&gt;nloc, flag==1?0:'+');
		return;

	case AUTOD:
		printf("%c(r%d)", flag==1?0:'-', p-&gt;nloc);
		return;

	case STAR:
		p = p-&gt;tr1;
		putchar('*');
		goto loop;

	}
	error("pname called illegally");
}

regerr()
{
	error("Illegal use of register");
}

pbase(ap)
struct tnode *ap;
{
	register struct tnode *p;

	p = ap;
	if (p-&gt;class==SOFFS || p-&gt;class==STATIC)
		printf("L%d", p-&gt;nloc);
	else
		printf("_%.8s", &amp;(p-&gt;nloc));
}

xdcalc(ap, nrleft)
struct tnode *ap;
{
	register struct tnode *p;
	register d;

	p = ap;
	d = dcalc(p, nrleft);
	if (d&lt;20 &amp;&amp; p-&gt;type==CHAR) {
		if (nrleft&gt;=1)
			d = 20;
		else
			d = 24;
	}
	return(d);
}

dcalc(ap, nrleft)
struct tnode *ap;
{
	register struct tnode *p, *p1;

	if ((p=ap)==0)
		return(0);
	switch (p-&gt;op) {

	case NAME:
		if (p-&gt;class==REG)
			return(9);

	case AMPER:
	case FCON:
	case AUTOI:
	case AUTOD:
		return(12);

	case CON:
	case SFCON:
		if (p-&gt;value==0)
			return(4);
		if (p-&gt;value==1)
			return(5);
		if (p-&gt;value &gt; 0)
			return(8);
		return(12);

	case STAR:
		p1 = p-&gt;tr1;
		if (p1-&gt;op==NAME||p1-&gt;op==CON||p1-&gt;op==AUTOI||p1-&gt;op==AUTOD)
			if (p-&gt;type!=LONG)
				return(12);
	}
	if (p-&gt;type==LONG)
		nrleft--;
	return(p-&gt;degree &lt;= nrleft? 20: 24);
}

notcompat(ap, ast, op)
struct tnode *ap;
{
	register at, st;
	register struct tnode *p;

	p = ap;
	at = p-&gt;type;
	st = ast;
	if (st==0)		/* word, byte */
		return(at&gt;CHAR &amp;&amp; at&lt;PTR);
	if (st==1)		/* word */
		return(at&gt;INT &amp;&amp; at&lt;PTR);
	st =- 2;
	if ((at&amp;(~(TYPE+XTYPE))) != 0)
		at = 020;
	if ((at&amp;(~TYPE)) != 0)
		at = at&amp;TYPE | 020;
	if (st==FLOAT &amp;&amp; at==DOUBLE)
		at = FLOAT;
	if (p-&gt;op==NAME &amp;&amp; p-&gt;class==REG &amp;&amp; op==ASSIGN &amp;&amp; st==CHAR)
		return(0);
	return(st != at);
}

prins(op, c, itable)
struct instab *itable;
{
	register struct instab *insp;
	register char *ip;

	for (insp=itable; insp-&gt;op != 0; insp++) {
		if (insp-&gt;op == op) {
			ip = c? insp-&gt;str2: insp-&gt;str1;
			if (ip==0)
				break;
			printf("%s", ip);
			return;
		}
	}
	error("No match' for op %d", op);
}

collcon(ap)
struct tnode *ap;
{
	register op;
	register struct tnode *p;

	p = ap;
	if (p-&gt;op==STAR)
		p = p-&gt;tr1;
	if (p-&gt;op==PLUS) {
		op = p-&gt;tr2-&gt;op;
		if (op==CON || op==AMPER)
			return(1);
	}
	return(0);
}

isfloat(at)
struct tnode *at;
{
	register struct tnode *t;

	t = at;
	if ((opdope[t-&gt;op]&amp;RELAT)!=0)
		t = t-&gt;tr1;
	if (t-&gt;type==FLOAT || t-&gt;type==DOUBLE) {
		nfloat = 1;
		return('f');
	}
	return(0);
}

oddreg(t, areg)
struct tnode *t;
{
	register reg;

	reg = areg;
	if (!isfloat(t))
		switch(t-&gt;op) {
		case DIVIDE:
		case MOD:
		case ASDIV:
		case ASMOD:
			reg++;

		case TIMES:
		case ASTIMES:
			return(reg|1);
		}
	return(reg);
}

arlength(t)
{
	if (t&gt;=PTR)
		return(2);
	switch(t) {

	case INT:
	case CHAR:
		return(2);

	case LONG:
		return(4);

	case FLOAT:
	case DOUBLE:
		return(8);
	}
	return(1024);
}

/*
 * Strings for switch code.
 */

char	dirsw[] {"\
cmp	r0,$%o\n\
jhi	L%d\n\
asl	r0\n\
jmp	*L%d(r0)\n\
.data\n\
L%d:\
" };

char	simpsw[] {"\
mov	$L%d,r1\n\
mov	r0,L%d\n\
L%d:cmp	r0,(r1)+\n\
jne	L%d\n\
jmp	*L%d-L%d(r1)\n\
.data\n\
L%d:\
"};

char	hashsw[] {"\
mov	r0,r1\n\
clr	r0\n\
div	$%o,r0\n\
asl	r1\n\
add	$L%d,r1\n\
mov	r0,*(r1)+\n\
mov	(r1)+,r1\n\
L%d:cmp	r0,-(r1)\n\
jne	L%d\n\
jmp	*L%d-L%d(r1)\n\
.data\n\
L%d:\
"};

pswitch(afp, alp, deflab)
struct swtab *afp, *alp;
{
	int tlab, ncase, i, j, tabs, worst, best, range;
	register struct swtab *swp, *fp, *lp;
	int poctab[swsiz];

	fp = afp;
	lp = alp;
	if (fp==lp) {
		printf("jbr	L%d\n", deflab);
		return;
	}
	tlab = isn++;
	if (sort(fp, lp))
		return;
	ncase = lp-fp;
	lp--;
	range = lp-&gt;swval - fp-&gt;swval;
	/* direct switch */
	if (range&gt;0 &amp;&amp; range &lt;= 3*ncase) {
		if (fp-&gt;swval)
			printf("sub	$%o,r0\n", fp-&gt;swval);
		printf(dirsw, range, deflab, isn, isn);
		isn++;
		for (i=fp-&gt;swval; i&lt;=lp-&gt;swval; i++) {
			if (i==fp-&gt;swval) {
				printf("L%d\n", fp-&gt;swlab);
				fp++;
			} else
				printf("L%d\n", deflab);
		}
		goto esw;
	}
	/* simple switch */
	if (ncase&lt;8) {
		i = isn++;
		j = isn++;
		printf(simpsw, i, j, isn, isn, j, i, i);
		isn++;
		for (; fp&lt;=lp; fp++)
			printf("%o\n", fp-&gt;swval);
		printf("L%d:..\n", j);
		for (fp = afp; fp&lt;=lp; fp++)
			printf("L%d\n", fp-&gt;swlab);
		printf("L%d\n", deflab);
		goto esw;
	}
	/* hash switch */
	best = 077777;
	for (i=ncase/4; i&lt;=ncase/2; i++) {
		for (j=0; j&lt;i; j++)
			poctab[j] = 0;
		for (swp=fp; swp&lt;=lp; swp++)
			poctab[lrem(0, swp-&gt;swval, i)]++;
		worst = 0;
		for (j=0; j&lt;i; j++)
			if (poctab[j]&gt;worst)
				worst = poctab[j];
		if (i*worst &lt; best) {
			tabs = i;
			best = i*worst;
		}
	}
	i = isn++;
	printf(hashsw, tabs, isn, i, i, isn+tabs+1, isn+1, isn);
	isn++;
	for (i=0; i&lt;=tabs; i++)
		printf("L%d\n", isn+i);
	for (i=0; i&lt;tabs; i++) {
		printf("L%d:..\n", isn++);
		for (swp=fp; swp&lt;=lp; swp++)
			if (lrem(0, swp-&gt;swval, tabs) == i)
				printf("%o\n", ldiv(0, swp-&gt;swval, tabs));
	}
	printf("L%d:", isn++);
	for (i=0; i&lt;tabs; i++) {
		printf("L%d\n", deflab);
		for (swp=fp; swp&lt;=lp; swp++)
			if (lrem(0, swp-&gt;swval, tabs) == i)
				printf("L%d\n", swp-&gt;swlab);
	}
esw:
	printf(".text\n");
}

sort(afp, alp)
struct swtab *afp, *alp;
{
	register struct swtab *cp, *fp, *lp;
	int intch, t;

	fp = afp;
	lp = alp;
	while (fp &lt; --lp) {
		intch = 0;
		for (cp=fp; cp&lt;lp; cp++) {
			if (cp-&gt;swval == cp[1].swval) {
				error("Duplicate case (%d)", cp-&gt;swval);
				return(1);
			}
			if (cp-&gt;swval &gt; cp[1].swval) {
				intch++;
				t = cp-&gt;swval;
				cp-&gt;swval = cp[1].swval;
				cp[1].swval = t;
				t = cp-&gt;swlab;
				cp-&gt;swlab = cp[1].swlab;
				cp[1].swlab = t;
			}
		}
		if (intch==0)
			break;
	}
	return(0);
}

ispow2(atree)
{
	register int d;
	register struct tnode *tree;

	tree = atree;
	if (!isfloat(tree) &amp;&amp; tree-&gt;tr2-&gt;op==CON) {
		d = tree-&gt;tr2-&gt;value;
		if (d&gt;1 &amp;&amp; (d&amp;(d-1))==0)
			return(d);
	}
	return(0);
}

pow2(atree)
struct tnode *atree;
{
	register int d, i;
	register struct tnode *tree;

	tree = atree;
	if (d = ispow2(tree)) {
		for (i=0; (d=&gt;&gt;1)!=0; i++);
		tree-&gt;tr2-&gt;value = i;
		d = tree-&gt;op;
		tree-&gt;op = d==TIMES? LSHIFT:
			  (d==DIVIDE? RSHIFT:
			  (d==ASTIMES? ASLSH: ASRSH));
		tree = optim(tree);
	}
	return(tree);
}

cbranch(atree, albl, cond, areg)
struct tnode *atree;
{
	int l1, op;
	register lbl, reg;
	register struct tnode *tree;

	lbl = albl;
	reg = areg;
	if ((tree=atree)==0)
		return;
	switch(tree-&gt;op) {

	case LOGAND:
		if (cond) {
			cbranch(tree-&gt;tr1, l1=isn++, 0, reg);
			cbranch(tree-&gt;tr2, lbl, 1, reg);
			label(l1);
		} else {
			cbranch(tree-&gt;tr1, lbl, 0, reg);
			cbranch(tree-&gt;tr2, lbl, 0, reg);
		}
		return;

	case LOGOR:
		if (cond) {
			cbranch(tree-&gt;tr1, lbl, 1, reg);
			cbranch(tree-&gt;tr2, lbl, 1, reg);
		} else {
			cbranch(tree-&gt;tr1, l1=isn++, 1, reg);
			cbranch(tree-&gt;tr2, lbl, 0, reg);
			label(l1);
		}
		return;

	case EXCLA:
		cbranch(tree-&gt;tr1, lbl, !cond, reg);
		return;

	case COMMA:
		rcexpr(tree-&gt;tr1, efftab, reg);
		tree = tree-&gt;tr2;
		break;
	}
	op = tree-&gt;op;
	if (tree-&gt;type==LONG || opdope[op]&amp;RELAT&amp;&amp;tree-&gt;tr1-&gt;type==LONG) {
		if (tree-&gt;type!=LONG) {
			tree-&gt;op = MINUS;
			tree-&gt;type = LONG;
			tree = optim(tree);
		} else
			op = NEQUAL;
		rcexpr(tree, regtab, 0);
		printf("ashc	$0,r0\n");
		branch(lbl, op, !cond);
		return;
	}
	rcexpr(tree, cctab, reg);
	op = tree-&gt;op;
	if ((opdope[op]&amp;RELAT)==0)
		op = NEQUAL;
	else {
		l1 = tree-&gt;tr2-&gt;op;
	 	if ((l1==CON || l1==SFCON) &amp;&amp; tree-&gt;tr2-&gt;value==0)
			op =+ 200;		/* special for ptr tests */
		else
			op = maprel[op-EQUAL];
	}
	if (isfloat(tree))
		printf("cfcc\n");
	branch(lbl, op, !cond);
}

branch(lbl, aop, c)
{
	register op;

	if(op=aop)
		prins(op, c, branchtab);
	else
		printf("jbr");
	printf("\tL%d\n", lbl);
}

label(l)
{
	printf("L%d:", l);
}

popstk(a)
{
	switch(a) {

	case 0:
		return;

	case 2:
		printf("tst	(sp)+\n");
		return;

	case 4:
		printf("cmp	(sp)+,(sp)+\n");
		return;
	}
	printf("add	$%o,sp\n", a);
}

error(s, p1, p2, p3, p4, p5, p6)
{
	register f;
	extern fout;

	nerror++;
	flush();
	f = fout;
	fout = 1;
	printf("%d: ", line);
	printf(s, p1, p2, p3, p4, p5, p6);
	putchar('\n');
	flush();
	fout = f;
}

psoct(an)
{
	register int n, sign;

	sign = 0;
	if ((n = an) &lt; 0) {
		n = -n;
		sign = '-';
	}
	printf("%c%o", sign, n);
}

/*
 * Read in an intermediate file.
 */
getree()
{
	struct tnode *expstack[20];
	register struct tnode **sp;
	register t, op;
	static char s[9];
	struct swtab *swp;

	spacep = treespace;
	sp = expstack;
	for (;;) {
		if (sp &gt;= &amp;expstack[20])
			error("Stack botch");
		op = getw(ascbuf);
		if ((op&amp;0177400) != 0177000) {
			error("Intermediate file error");
			exit(1);
		}
		switch(op =&amp; 0377) {

	case EOF:
		return;

	case BDATA:
		printf(".byte ");
		seq(',');
		break;

	case WDATA:
		seq(';');
		break;

	case PROG:
		printf(".text\n");
		break;

	case DATA:
		printf(".data\n");
		break;

	case BSS:
		printf(".bss\n");
		break;

	case SYMDEF:
		printf(".globl	_%s\n", outname(s));
		break;

	case RETRN:
		printf("jmp	cret\n");
		break;

	case CSPACE:
		t = outname(s);
		printf(".comm	_%s,%o\n", t, getw(ascbuf));
		break;

	case SSPACE:
		printf(".=.+%o\n", getw(ascbuf));
		break;

	case EVEN:
		printf(".even\n");
		break;

	case SAVE:
		printf("jsr	r5,csv\n");
		t = getw(ascbuf)-6;
		if (t==2)
			printf("tst	-(sp)\n");
		else if (t &gt; 2)
			printf("sub	$%o,sp\n", t);
		break;

	case PROFIL:
		t = getw(ascbuf);
		printf("mov	$L%d,r0\njsr	pc,mcount\n", t);
		printf(".bss\nL%d:.=.+2\n.text\n", t);
		break;

	case SNAME:
		t = outname(s);
		printf("~%s=L%d\n", t, getw(ascbuf));
		break;

	case ANAME:
		t = outname(s);
		printf("~%s=%o\n", t, getw(ascbuf));
		break;

	case RNAME:
		t = outname(s);
		printf("~%s=r%d\n", t, getw(ascbuf));
		break;

	case SWIT:
		t = getw(ascbuf);
		line = getw(ascbuf);
		swp = treespace;
		while (swp-&gt;swlab = getw(ascbuf)) {
			swp-&gt;swval = getw(ascbuf);
			swp++;
		}
		pswitch(treespace, swp, t);
		break;

	case EXPR:
		line = getw(ascbuf);
		if (sp != &amp;expstack[1]) {
			error("Expression input botch\n");
			exit(1);
		}
		nstack = 0;
		rcexpr(optim(*--sp), efftab, 0);
		spacep = treespace;
		break;

	case NAME:
		t = getw(ascbuf);
		if (t==EXTERN) {
			t = getw(ascbuf);
			*sp = block(6, NAME, t, 0, EXTERN, 0, 0,0,0,0);
			outname(&amp;(*sp)-&gt;nloc);
			sp++;
			break;
		}
		*sp = block(3, NAME, 0, 0, t, 0, 0);
		(*sp)-&gt;type = getw(ascbuf);
		(*sp)-&gt;nloc = getw(ascbuf);
		sp++;
		break;

	case CON:
	case SFCON:
	case FCON:
		t = getw(ascbuf);
		*sp++ = block(1, op, t, 0, getw(ascbuf));
		break;

	case FSEL:
		t = getw(ascbuf);
		sp[-1] = block(2, op, t, 0, sp[-1], getw(ascbuf));
		break;

	case NULL:
		*sp++ = block(0, 0, 0, 0);
		break;

	case CBRANCH:
		t = getw(ascbuf);
		sp[-1] = block(1, CBRANCH, sp[-1], t, getw(ascbuf));
		break;

	case LABEL:
		label(getw(ascbuf));
		break;

	case NLABEL:
		printf("_%s:\n", outname(s));
		break;

	case RLABEL:
		t = outname(s);
		printf("_%s:\n~~%s:\n", t, t);
		break;

	case BRANCH:
		branch(getw(ascbuf), 0);
		break;

	case SETREG:
		nreg = getw(ascbuf)-1;
		break;

	default:
		if (opdope[op]&amp;BINARY) {
			if (sp &lt; &amp;expstack[1]) {
				error("Binary expression botch");
				exit(1);
			}
			t = *--sp;
			*sp++ = block(2, op, getw(ascbuf), 0, *--sp, t);
		} else {
			sp[-1] = block(1, op, getw(ascbuf), 0, sp[-1]);
		}
		break;
	}
	}
}

outname(s)
{
	register char *p, c;
	register n;

	p = s;
	n = 0;
	while (c = getc(ascbuf)) {
		*p++ = c;
		n++;
	}
	while (n++ &lt; 8)
		*p++ = 0;
	return(s);
}

seq(c)
{
	register o;

	if (getw(ascbuf) == 0)
		return;
	for (;;) {
		printf("%o", getw(ascbuf));
		if ((o = getw(ascbuf)) != 1)
			break;
		printf("%c", c);
	}
	printf("\n");
}











