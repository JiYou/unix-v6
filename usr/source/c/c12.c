/*
 *		C compiler part 2 -- expression optimizer
 *
 */

#include "c1h.c"

optim(atree)
struct tnode *atree;
{
	register op, dope;
	int d1, d2;
	struct tnode *t;
	register struct tnode *tree;

	if ((tree=atree)==0)
		return(0);
	if ((op = tree-&gt;op)==0)
		return(tree);
	if (op==NAME &amp;&amp; tree-&gt;class==AUTO) {
		tree-&gt;class = OFFS;
		tree-&gt;regno = 5;
		tree-&gt;offset = tree-&gt;nloc;
	}
	dope = opdope[op];
	if ((dope&amp;LEAF) != 0)
		return(tree);
	if ((dope&amp;BINARY) == 0)
		return(unoptim(tree));
	/* is known to be binary */
	if (tree-&gt;type==CHAR)
		tree-&gt;type = INT;
	if ((dope&amp;COMMUTE)!=0) {
	acomm:	d1 = tree-&gt;type;
		tree = acommute(tree);
		tree-&gt;type = d1;
		/*
		 * PDP-11 special:
		 * replace a&amp;b by a NAND ~ b.
		 * This will be undone when in
		 * truth-value context.
		 */
		if (tree-&gt;op!=AND)
			return(tree);
		tree-&gt;op = NAND;
		tree-&gt;tr2 = block(1, COMPL, tree-&gt;tr2-&gt;type, 0, tree-&gt;tr2);
	}
    again:
	tree-&gt;tr1 = optim(tree-&gt;tr1);
	tree-&gt;tr2 = optim(tree-&gt;tr2);
	if ((dope&amp;RELAT) != 0) {
		if ((d1=degree(tree-&gt;tr1)) &lt; (d2=degree(tree-&gt;tr2))
		 || d1==d2 &amp;&amp; tree-&gt;tr1-&gt;op==NAME &amp;&amp; tree-&gt;tr2-&gt;op!=NAME) {
			t = tree-&gt;tr1;
			tree-&gt;tr1 = tree-&gt;tr2;
			tree-&gt;tr2 = t;
			tree-&gt;op = maprel[op-EQUAL];
		}
		if (tree-&gt;tr1-&gt;type==CHAR &amp;&amp; tree-&gt;tr2-&gt;op==CON
		 &amp;&amp; (dcalc(tree-&gt;tr1) &lt;= 12 || tree-&gt;tr1-&gt;op==STAR)
		 &amp;&amp; tree-&gt;tr2-&gt;value &lt;= 127 &amp;&amp; tree-&gt;tr2-&gt;value &gt;= 0)
			tree-&gt;tr2-&gt;type = CHAR;
	}
	d1 = max(degree(tree-&gt;tr1), islong(tree-&gt;type));
	d2 = max(degree(tree-&gt;tr2), 0);
	if (tree-&gt;tr1-&gt;type==LONG &amp;&amp; dope&amp;RELAT)
		d1 = 10;
	switch (op) {

	case LTIMES:
	case LDIV:
	case LMOD:
	case LASTIMES:
	case LASDIV:
	case LASMOD:
		tree-&gt;degree = 10;
		break;

	/*
	 * PDP-11 special:
	 * generate new =&amp;~ operator out of =&amp;
	 * by complementing the RHS.
	 */
	case ASSAND:
		op = ASSNAND;
		tree-&gt;op = op;
		tree-&gt;tr2 = block(2, COMPL, tree-&gt;tr2-&gt;type, 0, tree-&gt;tr2);
		goto again;

	case NAND:
		if (isconstant(tree-&gt;tr2) &amp;&amp; tree-&gt;tr2-&gt;value==0)
			return(tree-&gt;tr1);
		goto def;

	case CALL:
		tree-&gt;degree = 10;
		break;

	case QUEST:
	case COLON:
		tree-&gt;degree = max(d1, d2);
		break;

	case MINUS:
		if (t = isconstant(tree-&gt;tr2)) {
			tree-&gt;op = PLUS;
			if (t-&gt;type==DOUBLE)
				/* PDP-11 FP representation */
				t-&gt;value =^ 0100000;
			else
				t-&gt;value = -t-&gt;value;
			goto acomm;
		}
		goto def;

	case DIVIDE:
	case ASDIV:
	case ASTIMES:
		if (tree-&gt;tr2-&gt;op==CON &amp;&amp; tree-&gt;tr2-&gt;value==1)
			return(tree-&gt;tr1);
		if (ispow2(tree) == 0) {

		case MOD:
		case ASMOD:
			d1 =+ 2;
			d2 =+ 2;
		}
		if (tree-&gt;type==LONG)
			return(hardlongs(tree));
		goto constant;

	case LSHIFT:
	case RSHIFT:
	case ASRSH:
	case ASLSH:
		if (tree-&gt;tr2-&gt;op==CON &amp;&amp; tree-&gt;tr2-&gt;value==0)
			return(tree-&gt;tr1);
		/*
		 * PDP-11 special: turn right shifts into negative
		 * left shifts
		 */
		if (op==LSHIFT||op==ASLSH)
			goto constant;
		if (tree-&gt;tr2-&gt;op==CON &amp;&amp; tree-&gt;tr2-&gt;value==1)
			goto constant;
		op =+ (LSHIFT-RSHIFT);
		tree-&gt;op = op;
		tree-&gt;tr2 = block(1, NEG, tree-&gt;type, 0, tree-&gt;tr2);
		goto again;

	constant:
		if (tree-&gt;tr1-&gt;op==CON &amp;&amp; tree-&gt;tr2-&gt;op==CON) {
			const(op, &amp;tree-&gt;tr1-&gt;value, tree-&gt;tr2-&gt;value);
			return(tree-&gt;tr1);
		}


	def:
	default:
		tree-&gt;degree = d1==d2? d1+islong(tree-&gt;type): max(d1, d2);
		break;
	}
	return(tree);
}

unoptim(atree)
struct tnode *atree;
{
	register struct tnode *subtre, *tree;
	register int *p;
	double static fv;
	struct { int integer; };

	if ((tree=atree)==0)
		return(0);
	if (tree-&gt;op==CBRANCH) {
		tree-&gt;btree = optim(tree-&gt;btree);
		return(tree);
	}
	subtre = tree-&gt;tr1 = optim(tree-&gt;tr1);
	switch (tree-&gt;op) {

	case FSEL:
		tree-&gt;tr1 = block(2, RSHIFT, INT, 0, subtre,
		    block(1, CON, INT, 0, tree-&gt;bitoffs));
		tree-&gt;op = AND;
		tree-&gt;type = INT;
		tree-&gt;tr2 = block(1, CON, INT, 0, (1&lt;&lt;tree-&gt;flen)-1);
		return(optim(tree));

	case AMPER:
		if (subtre-&gt;op==STAR)
			return(subtre-&gt;tr1);
		if (subtre-&gt;op==NAME &amp;&amp; subtre-&gt;class == OFFS) {
			p = block(2, PLUS, tree-&gt;type, 1, subtre, tree);
			subtre-&gt;type = tree-&gt;type;
			tree-&gt;op = CON;
			tree-&gt;type = INT;
			tree-&gt;degree = 0;
			tree-&gt;value = subtre-&gt;offset;
			subtre-&gt;class = REG;
			subtre-&gt;nloc = subtre-&gt;regno;
			subtre-&gt;offset = 0;
			return(p);
		}
		break;

	case STAR:
		if (subtre-&gt;op==AMPER)
			return(subtre-&gt;tr1);
		if (subtre-&gt;op==NAME &amp;&amp; subtre-&gt;class==REG) {
			subtre-&gt;type = tree-&gt;type;
			subtre-&gt;class = OFFS;
			subtre-&gt;regno = subtre-&gt;nloc;
			return(subtre);
		}
		p = subtre-&gt;tr1;
		if ((subtre-&gt;op==INCAFT||subtre-&gt;op==DECBEF)&amp;&amp;tree-&gt;type!=LONG
		 &amp;&amp; p-&gt;op==NAME &amp;&amp; p-&gt;class==REG &amp;&amp; p-&gt;type==subtre-&gt;type) {
			p-&gt;type = tree-&gt;type;
			p-&gt;op = subtre-&gt;op==INCAFT? AUTOI: AUTOD;
			return(p);
		}
		if (subtre-&gt;op==PLUS &amp;&amp; p-&gt;op==NAME &amp;&amp; p-&gt;class==REG) {
			if (subtre-&gt;tr2-&gt;op==CON) {
				p-&gt;offset =+ subtre-&gt;tr2-&gt;value;
				p-&gt;class = OFFS;
				p-&gt;type = tree-&gt;type;
				p-&gt;regno = p-&gt;nloc;
				return(p);
			}
			if (subtre-&gt;tr2-&gt;op==AMPER) {
				subtre = subtre-&gt;tr2-&gt;tr1;
				subtre-&gt;class =+ XOFFS-EXTERN;
				subtre-&gt;regno = p-&gt;nloc;
				subtre-&gt;type = tree-&gt;type;
				return(subtre);
			}
		}
		break;
	case EXCLA:
		if ((opdope[subtre-&gt;op]&amp;RELAT)==0)
			break;
		tree = subtre;
		tree-&gt;op = notrel[tree-&gt;op-EQUAL];
		break;

	case NEG:
	case COMPL:
		if (tree-&gt;type==CHAR)
			tree-&gt;type = INT;
		if (tree-&gt;op == subtre-&gt;op)
			return(subtre-&gt;tr1);
		if (subtre-&gt;op==ITOL) {
			subtre-&gt;op = tree-&gt;op;
			subtre-&gt;type = INT;
			tree-&gt;op = ITOL;
		}
	}
	if (subtre-&gt;op == CON) switch(tree-&gt;op) {

	case NEG:
		subtre-&gt;value = -subtre-&gt;value;
		return(subtre);

	case COMPL:
		subtre-&gt;value = ~subtre-&gt;value;
		return(subtre);

	case ITOF:
		fv = subtre-&gt;value;
		p = &amp;fv;
		p++;
		if (*p++==0 &amp;&amp; *p++==0 &amp;&amp; *p++==0) {
			subtre-&gt;type = DOUBLE;
			subtre-&gt;value = fv.integer;
			subtre-&gt;op = SFCON;
			return(subtre);
		}
		break;
	}
	tree-&gt;degree = max(islong(tree-&gt;type), degree(subtre));
	return(tree);
}

struct acl {
	int nextl;
	int nextn;
	struct tnode *nlist[20];
	struct tnode *llist[21];
};

acommute(atree)
{
	struct acl acl;
	int d, i, op, flt;
	register struct tnode *t1, **t2, *tree;
	struct tnode *t;

	acl.nextl = 0;
	acl.nextn = 0;
	tree = atree;
	op = tree-&gt;op;
	flt = isfloat(tree);
	insert(op, tree, &amp;acl);
	acl.nextl--;
	t2 = &amp;acl.llist[acl.nextl];
	if (!flt) {
		/* put constants together */
		for (i=acl.nextl;i&gt;0&amp;&amp;t2[0]-&gt;op==CON&amp;&amp;t2[-1]-&gt;op==CON;i--) {
			acl.nextl--;
			t2--;
			const(op, &amp;t2[0]-&gt;value, t2[1]-&gt;value);
		}
	}
	if (op==PLUS || op==OR) {
		/* toss out "+0" */
		if (acl.nextl&gt;0 &amp;&amp; (t1 = isconstant(*t2)) &amp;&amp; t1-&gt;value==0) {
			acl.nextl--;
			t2--;
		}
		if (acl.nextl &lt;= 0)
			return(*t2);
		/* subsume constant in "&amp;x+c" */
		if (op==PLUS &amp;&amp; t2[0]-&gt;op==CON &amp;&amp; t2[-1]-&gt;op==AMPER) {
			t2--;
			t2[0]-&gt;tr1-&gt;offset =+ t2[1]-&gt;value;
			acl.nextl--;
		}
	} else if (op==TIMES || op==AND) {
		t1 = acl.llist[acl.nextl];
		if (t1-&gt;op==CON) {
			if (t1-&gt;value==0)
				return(t1);
			if (op==TIMES &amp;&amp; t1-&gt;value==1 &amp;&amp; acl.nextl&gt;0)
				if (--acl.nextl &lt;= 0)
					return(acl.llist[0]);
		}
	}
	if (op==PLUS &amp;&amp; !flt)
		distrib(&amp;acl);
	tree = *(t2 = &amp;acl.llist[0]);
	d = max(degree(tree), islong(tree-&gt;type));
	if (op==TIMES &amp;&amp; !flt)
		d++;
	for (i=0; i&lt;acl.nextl; i++) {
		t1 = acl.nlist[i];
		t1-&gt;tr2 = t = *++t2;
		t1-&gt;degree = d = d==degree(t)? d+islong(t1-&gt;type): max(d, degree(t));
		t1-&gt;tr1 = tree;
		tree = t1;
		if (tree-&gt;type==LONG) {
			if (tree-&gt;op==TIMES)
				tree = hardlongs(tree);
			else if (tree-&gt;op==PLUS &amp;&amp; (t = isconstant(tree-&gt;tr1))
			       &amp;&amp; t-&gt;value &lt; 0) {
				tree-&gt;op = MINUS;
				t-&gt;value = - t-&gt;value;
				t = tree-&gt;tr1;
				tree-&gt;tr1 = tree-&gt;tr2;
				tree-&gt;tr2 = t;
			}
		}
	}
	if (tree-&gt;op==TIMES &amp;&amp; ispow2(tree))
		tree-&gt;degree = max(degree(tree-&gt;tr1), islong(tree-&gt;type));
	return(tree);
}

distrib(list)
struct acl *list;
{
/*
 * Find a list member of the form c1c2*x such
 * that c1c2 divides no other such constant, is divided by
 * at least one other (say in the form c1*y), and which has
 * fewest divisors. Reduce this pair to c1*(y+c2*x)
 * and iterate until no reductions occur.
 */
	register struct tnode **p1, **p2;
	struct tnode *t;
	int ndmaj, ndmin;
	struct tnode **dividend, **divisor;
	struct tnode **maxnod, **mindiv;

    loop:
	maxnod = &amp;list-&gt;llist[list-&gt;nextl];
	ndmaj = 1000;
	dividend = 0;
	for (p1 = list-&gt;llist; p1 &lt;= maxnod; p1++) {
		if ((*p1)-&gt;op!=TIMES || (*p1)-&gt;tr2-&gt;op!=CON)
			continue;
		ndmin = 0;
		for (p2 = list-&gt;llist; p2 &lt;= maxnod; p2++) {
			if (p1==p2 || (*p2)-&gt;op!=TIMES || (*p2)-&gt;tr2-&gt;op!=CON)
				continue;
			if ((*p1)-&gt;tr2-&gt;value == (*p2)-&gt;tr2-&gt;value) {
				(*p2)-&gt;tr2 = (*p1)-&gt;tr1;
				(*p2)-&gt;op = PLUS;
				(*p1)-&gt;tr1 = (*p2);
				*p1 = optim(*p1);
				squash(p2, maxnod);
				list-&gt;nextl--;
				goto loop;
			}
			if (((*p2)-&gt;tr2-&gt;value % (*p1)-&gt;tr2-&gt;value) == 0)
				goto contmaj;
			if (((*p1)-&gt;tr2-&gt;value % (*p2)-&gt;tr2-&gt;value) == 0) {
				ndmin++;
				mindiv = p2;
			}
		}
		if (ndmin &gt; 0 &amp;&amp; ndmin &lt; ndmaj) {
			ndmaj = ndmin;
			dividend = p1;
			divisor = mindiv;
		}
    contmaj:;
	}
	if (dividend==0)
		return;
	t = list-&gt;nlist[--list-&gt;nextn];
	p1 = dividend;
	p2 = divisor;
	t-&gt;op = PLUS;
	t-&gt;type = (*p1)-&gt;type;
	t-&gt;tr1 = (*p1);
	t-&gt;tr2 = (*p2)-&gt;tr1;
	(*p1)-&gt;tr2-&gt;value =/ (*p2)-&gt;tr2-&gt;value;
	(*p2)-&gt;tr1 = t;
	t = optim(*p2);
	if (p1 &lt; p2) {
		*p1 = t;
		squash(p2, maxnod);
	} else {
		*p2 = t;
		squash(p1, maxnod);
	}
	list-&gt;nextl--;
	goto loop;
}

squash(p, maxp)
struct tnode **p, **maxp;
{
	register struct tnode **np;

	for (np = p; np &lt; maxp; np++)
		*np = *(np+1);
}

const(op, vp, av)
int *vp;
{
	register int v;

	v = av;
	switch (op) {

	case PLUS:
		*vp =+ v;
		return;

	case TIMES:
		*vp =* v;
		return;

	case AND:
		*vp =&amp; v;
		return;

	case OR:
		*vp =| v;
		return;

	case EXOR:
		*vp =^ v;
		return;

	case DIVIDE:
	case MOD:
		if (v==0)
			error("Divide check");
		else
			if (op==DIVIDE)
				*vp =/ v;
			else
				*vp =% v;
		return;

	case RSHIFT:
		*vp =&gt;&gt; v;
		return;

	case LSHIFT:
		*vp =&lt;&lt; v;
		return;

	case NAND:
		*vp =&amp; ~ v;
		return;
	}
	error("C error: const");
}

insert(op, atree, alist)
struct acl *alist;
{
	register d;
	register struct acl *list;
	register struct tnode *tree;
	int d1, i;
	struct tnode *t;

	tree = atree;
	list = alist;
	if (tree-&gt;op == op) {
	ins:	list-&gt;nlist[list-&gt;nextn++] = tree;
		insert(op, tree-&gt;tr1, list);
		insert(op, tree-&gt;tr2, list);
		return;
	}
	tree = optim(tree);
	if (tree-&gt;op == op)
		goto ins;
	if (!isfloat(tree)) {
		/* c1*(x+c2) -&gt; c1*x+c1*c2 */
		if ((tree-&gt;op==TIMES||tree-&gt;op==LSHIFT)
		  &amp;&amp; tree-&gt;tr2-&gt;op==CON &amp;&amp; tree-&gt;tr2-&gt;value&gt;0
		  &amp;&amp; tree-&gt;tr1-&gt;op==PLUS &amp;&amp; tree-&gt;tr1-&gt;tr2-&gt;op==CON) {
			d = tree-&gt;tr2-&gt;value;
			if (tree-&gt;op==TIMES)
				tree-&gt;tr2-&gt;value =* tree-&gt;tr1-&gt;tr2-&gt;value;
			else
				tree-&gt;tr2-&gt;value = tree-&gt;tr1-&gt;tr2-&gt;value &lt;&lt; d;
			tree-&gt;tr1-&gt;tr2-&gt;value = d;
			tree-&gt;tr1-&gt;op = tree-&gt;op;
			tree-&gt;op = PLUS;
			if (op==PLUS)
				goto ins;
		}
	}
	d = degree(tree);
	for (i=0; i&lt;list-&gt;nextl; i++) {
		if ((d1=degree(list-&gt;llist[i]))&lt;d) {
			t = list-&gt;llist[i];
			list-&gt;llist[i] = tree;
			tree = t;
			d = d1;
		}
	}
	list-&gt;llist[list-&gt;nextl++] = tree;
}

block(an, args)
{
	register int *p;
	int *oldp;
	register *argp, n;

	oldp = p = spacep;
	n = an+3;
	argp = &amp;args;
	do
		*p++ = *argp++;
	while (--n);
	if (p &gt;= &amp;treespace[ossiz]) {
		error("Exp. ov. pass 2");
		exit(1);
	}
	spacep = p;
	return(oldp);
}

islong(t)
{
	if (t==LONG)
		return(2);
	return(1);
}

isconstant(at)
struct tnode *at;
{
	register struct tnode *t;

	t = at;
	if (t-&gt;op==CON || t-&gt;op==SFCON)
		return(t);
	if (t-&gt;op==ITOL &amp;&amp; t-&gt;tr1-&gt;op==CON)
		return(t-&gt;tr1);
	return(0);
}

hardlongs(at)
struct tnode *at;
{
	register struct tnode *t;

	t = at;
	switch(t-&gt;op) {

	case TIMES:
	case DIVIDE:
	case MOD:
		t-&gt;op =+ LTIMES-TIMES;
		break;

	case ASTIMES:
	case ASDIV:
	case ASMOD:
		t-&gt;op =+ LASTIMES-ASTIMES;
		t-&gt;tr1 = block(1, AMPER, LONG+PTR, 0, t-&gt;tr1);
		break;

	default:
		return(t);
	}
	return(optim(t));
}











