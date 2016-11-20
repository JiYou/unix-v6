
/*
 * C compiler
 *
 *
 */

#include "c0h.c"

/*
 * Reduce the degree-of-reference by one.
 * e.g. turn "ptr-to-int" into "int".
 */
decref(at)
{
	register t;

	t = at;
	if ((t &amp; ~TYPE) == 0) {
		error("Illegal indirection");
		return(t);
	}
	return((t&gt;&gt;TYLEN) &amp; ~TYPE | t&amp;TYPE);
}

/*
 * Increase the degree of reference by
 * one; e.g. turn "int" to "ptr-to-int".
 */
incref(t)
{
	return(((t&amp;~TYPE)&lt;&lt;TYLEN) | (t&amp;TYPE) | PTR);
}

/*
 * Make a tree that causes a branch to lbl
 * if the tree's value is non-zero together with the cond.
 */
cbranch(tree, lbl, cond)
struct tnode *tree;
{
	rcexpr(block(1,CBRANCH,tree,lbl,cond));
}

/*
 * Write out a tree.
 */
rcexpr(tree)
struct tnode *tree;
{

	treeout(tree);
	outcode("BN", EXPR, line);
}

treeout(atree)
struct tnode *atree;
{
	register struct tnode *tree;

	if ((tree = atree) == 0)
		return;
	switch(tree-&gt;op) {

	case 0:
		outcode("B", NULL);
		return;

	case NAME:
		outcode("BNN", NAME, tree-&gt;class, tree-&gt;type);
		if (tree-&gt;class==EXTERN)
			outcode("S", tree-&gt;nname);
		else
			outcode("N", tree-&gt;nloc);
		return;

	case CON:
	case FCON:
	case SFCON:
		outcode("BNN", tree-&gt;op, tree-&gt;type, tree-&gt;value);
		return;

	case FSEL:
		treeout(tree-&gt;tr1);
		outcode("BNN", tree-&gt;op, tree-&gt;type, tree-&gt;tr2);
		return;

	case CBRANCH:
		treeout(tree-&gt;btree);
		outcode("BNN", tree-&gt;op, tree-&gt;lbl, tree-&gt;cond);
		return;

	default:
		treeout(tree-&gt;tr1);
		if (opdope[tree-&gt;op]&amp;BINARY)
			treeout(tree-&gt;tr2);
		outcode("BN", tree-&gt;op, tree-&gt;type);
		return;
	}
}

/*
 * Generate a branch
 */
branch(lab) {
	outcode("BN", BRANCH, lab);
}

/*
 * Generate a label
 */
label(l) {
	outcode("BN", LABEL, l);
}

/*
 * ap is a tree node whose type
 * is some kind of pointer; return the size of the object
 * to which the pointer points.
 */
plength(ap)
struct tname *ap;
{
	register t, l;
	register struct tname *p;

	p = ap;
	if (p==0 || ((t=p-&gt;type)&amp;~TYPE) == 0)		/* not a reference */
		return(1);
	p-&gt;type = decref(t);
	l = length(p);
	p-&gt;type = t;
	return(l);
}

/*
 * return the number of bytes in the object
 * whose tree node is acs.
 */
length(acs)
struct tnode *acs;
{
	register t, n;
	register struct tnode *cs;

	cs = acs;
	t = cs-&gt;type;
	n = 1;
	while ((t&amp;XTYPE) == ARRAY) {
		t = decref(t);
		n = dimtab[cs-&gt;ssp&amp;0377];
	}
	if ((t&amp;~TYPE)==FUNC)
		return(0);
	if (t&gt;=PTR)
		return(2*n);
	switch(t&amp;TYPE) {

	case INT:
		return(2*n);

	case CHAR:
		return(n);

	case FLOAT:
	case LONG:
		return(4*n);

	case DOUBLE:
		return(8*n);

	case STRUCT:
		return(n * dimtab[cs-&gt;lenp&amp;0377]);

	case RSTRUCT:
		error("Bad structure");
		return(0);
	}
	error("Compiler error (length)");
}

/*
 * The number of bytes in an object, rounded up to a word.
 */
rlength(cs)
struct tnode *cs;
{
	return((length(cs)+ALIGN) &amp; ~ALIGN);
}

/*
 * After an "if (...) goto", look to see if the transfer
 * is to a simple label.
 */
simplegoto()
{
	register struct hshtab *csp;

	if ((peeksym=symbol())==NAME &amp;&amp; nextchar()==';') {
		csp = csym;
		if (csp-&gt;hclass==0 &amp;&amp; csp-&gt;htype==0) {
			csp-&gt;htype = ARRAY;
			if (csp-&gt;hoffset==0)
				csp-&gt;hoffset = isn++;
		}
		if ((csp-&gt;hclass==0||csp-&gt;hclass==STATIC)
		 &amp;&amp;  csp-&gt;htype==ARRAY) {
			peeksym = -1;
			return(csp-&gt;hoffset);
		}
	}
	return(0);
}

/*
 * Return the next non-white-space character
 */
nextchar()
{
	while (spnextchar()==' ')
		peekc = 0;
	return(peekc);
}

/*
 * Return the next character, translating all white space
 * to blank and handling line-ends.
 */
spnextchar()
{
	register c;

	if ((c = peekc)==0)
		c = getchar();
	if (c=='\t')
		c = ' ';
	else if (c=='\n') {
		c = ' ';
		if (inhdr==0)
			line++;
		inhdr = 0;
	} else if (c=='\001') {	/* SOH, insert marker */
		inhdr++;
		c = ' ';
	}
	peekc = c;
	return(c);
}

/*
 * is a break or continue legal?
 */
chconbrk(l)
{
	if (l==0)
		error("Break/continue error");
}

/*
 * The goto statement.
 */
dogoto()
{
	register struct tnode *np;

	*cp++ = tree();
	build(STAR);
	chkw(np = *--cp, -1);
	rcexpr(block(1,JUMP,0,0,np));
}

/*
 * The return statement, which has to convert
 * the returned object to the function's type.
 */
doret()
{
	register struct tnode *t;

	if (nextchar() != ';') {
		t = tree();
		*cp++ = &amp;funcblk;
		*cp++ = t;
		build(ASSIGN);
		cp[-1] = cp[-1]-&gt;tr2;
		build(RFORCE);
		rcexpr(*--cp);
	}
	branch(retlab);
}

/*
 * write out a character to the usual output
 * or to the string file
 */
putchar(c)
{
	write(1, &amp;c, 1);
}

outcode(s, a)
char *s;
{
	register char *sp;
	register *ap, *bufp;
	int n;
	char *np;

	bufp = obuf;
	if (strflg)
		bufp = sbuf;
	ap = &amp;a;
	for (;;) switch(*s++) {
	case 'B':
		putw(*ap++ | (0376&lt;&lt;8), bufp);
		continue;

	case 'N':
		putw(*ap++, bufp);
		continue;

	case 'S':
		np = *ap++;
		n = ncps;
		while (n-- &amp;&amp; *np) {
			putc(*np++, bufp);
		}
		putc(0, bufp);
		continue;

	case '1':
		putw(1, bufp);
		continue;

	case '0':
		putw(0, bufp);
		continue;

	case '\0':
		return;
	}
	error("Botch in outcode");
}











