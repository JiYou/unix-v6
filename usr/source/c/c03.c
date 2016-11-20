
/*
 * C compiler, phase 1
 *
 *
 * Handles processing of declarations,
 * except for top-level processing of
 * externals.
 */

#include "c0h.c"

/*
 * Process a sequence of declaration statements
 */
declist(sclass)
{
	register sc, elsize, offset;
	int type;

	offset = 0;
	sc = sclass;
	while ((elsize = getkeywords(&amp;sclass, &amp;type)) != -1) {
		offset = declare(sclass, type, offset, elsize);
		sclass = sc;
	}
	return(offset+align(INT, offset, 0));
}

/*
 * Read the keywords introducing a declaration statement
 */
getkeywords(scptr, tptr)
int *scptr, *tptr;
{
	register skw, tkw, longf;
	int o, elsize, isadecl, ismos;

	isadecl = 0;
	longf = 0;
	tkw = -1;
	skw = *scptr;
	elsize = 0;
	ismos = skw==MOS;
	for (;;) {
		mosflg = ismos;
		switch ((o=symbol())==KEYW? cval: -1) {

		case AUTO:
		case STATIC:
		case EXTERN:
		case REG:
			if (skw &amp;&amp; skw!=cval)
				error("Conflict in storage class");
			skw = cval;
			break;
	
		case LONG:
			longf++;
			break;

		case STRUCT:
			o = STRUCT;
			elsize = strdec(&amp;o, ismos);
			cval = o;
		case INT:
		case CHAR:
		case FLOAT:
		case DOUBLE:
			if (tkw&gt;=0)
				error("Type clash");
			tkw = cval;
			break;
	
		default:
			peeksym = o;
			if (isadecl==0)
				return(-1);
			if (tkw&lt;0)
				tkw = INT;
			if (skw==0)
				skw = AUTO;
			if (longf) {
				if (tkw==FLOAT)
					tkw = DOUBLE;
				else if (tkw==INT)
					tkw = LONG;
				else
					error("Misplaced 'long'");
			}
			*scptr = skw;
			*tptr = tkw;
			return(elsize);
		}
		isadecl++;
	}
}

/*
 * Process a structure declaration; a subroutine
 * of getkeywords.
 */
strdec(tkwp, mosf)
int *tkwp;
{
	register elsize, o;
	register struct hshtab *ssym;
	int savebits;
	struct hshtab *ds;

	mosflg = 1;
	ssym = 0;
	if ((o=symbol())==NAME) {
		ssym = csym;
		if (ssym-&gt;hclass==0) {
			ssym-&gt;hclass = STRTAG;
			ssym-&gt;lenp = dimp;
			chkdim();
			dimtab[dimp++] = 0;
		}
		if (ssym-&gt;hclass != STRTAG)
			redec();
		mosflg = mosf;
		o = symbol();
	}
	mosflg = 0;
	if (o != LBRACE) {
		if (ssym==0) {
		syntax:
			decsyn(o);
			return(0);
		}
		if (ssym-&gt;hclass!=STRTAG)
			error("Bad structure name");
		if ((elsize = dimtab[ssym-&gt;lenp&amp;0377])==0) {
			*tkwp = RSTRUCT;
			elsize = ssym;
		}
		peeksym = o;
	} else {
		ds = defsym;
		mosflg = 0;
		savebits = bitoffs;
		bitoffs = 0;
		elsize = declist(MOS);
		bitoffs = savebits;
		defsym = ds;
		if ((o = symbol()) != RBRACE)
			goto syntax;
		if (ssym) {
			if (dimtab[ssym-&gt;lenp&amp;0377])
				error("%.8s redeclared", ssym-&gt;name);
			dimtab[ssym-&gt;lenp&amp;0377] = elsize;
		}
	}
	return(elsize);
}

/*
 * Check that the dimension table has not overflowed
 */
chkdim()
{
	if (dimp &gt;= dimsiz) {
		error("Dimension/struct table overflow");
		exit(1);
	}
}

/*
 * Process a comma-separated list of declarators
 */
declare(askw, tkw, offset, elsize)
{
	register int o;
	register int skw;

	skw = askw;
	do {
		offset =+ decl1(skw, tkw, offset, elsize);
	} while ((o=symbol()) == COMMA);
	if (o==SEMI || o==RPARN &amp;&amp; skw==ARG1)
		return(offset);
	decsyn(o);
}

/*
 * Process a single declarator
 */
decl1(askw, tkw, offset, elsize)
{
	int t1, chkoff, a;
	register int type, skw;
	register struct hshtab *dsym;

	skw = askw;
	chkoff = 0;
	mosflg = skw==MOS;
	if ((peeksym=symbol())==SEMI || peeksym==RPARN)
		return(0);
	/*
	 * Filler field
	 */
	if (peeksym==COLON &amp;&amp; skw==MOS) {
		peeksym = -1;
		t1 = conexp();
		elsize = align(tkw, offset, t1);
		bitoffs =+ t1;
		return(elsize);
	}
	if ((t1=getype()) &lt; 0)
		goto syntax;
	type = 0;
	do
		type = type&lt;&lt;TYLEN | (t1 &amp; XTYPE);
	while (((t1=&gt;&gt;TYLEN) &amp; XTYPE)!=0);
	type =| tkw;
	dsym = defsym;
	if (!(dsym-&gt;hclass==0
	   || (skw==ARG &amp;&amp; dsym-&gt;hclass==ARG1)
	   || (skw==EXTERN &amp;&amp; dsym-&gt;hclass==EXTERN &amp;&amp; dsym-&gt;htype==type)))
		if (skw==MOS &amp;&amp; dsym-&gt;hclass==MOS &amp;&amp; dsym-&gt;htype==type)
			chkoff = 1;
		else {
			redec();
			goto syntax;
		}
	dsym-&gt;htype = type;
	if (skw)
		dsym-&gt;hclass = skw;
	if (skw==ARG1) {
		if (paraml==0)
			paraml = dsym;
		else
			parame-&gt;hoffset = dsym;
		parame = dsym;
	}
	if (elsize &amp;&amp; ((type&amp;TYPE)==RSTRUCT || (type&amp;TYPE)==STRUCT)) {
		dsym-&gt;lenp = dimp;
		chkdim();
		dimtab[dimp++] = elsize;
	}
	elsize = 0;
	if (skw==MOS) {
		elsize = length(dsym);
		t1 = 0;
		if ((peeksym = symbol())==COLON) {
			elsize = 0;
			peeksym = -1;
			t1 = conexp();
			dsym-&gt;hflag =| FFIELD;
		}
		a = align(type, offset, t1);
		elsize =+ a;
		offset =+ a;
		if (t1) {
			if (chkoff &amp;&amp; (dsym-&gt;bitoffs!=bitoffs
		 	 || dsym-&gt;flen!=t1))
				redec();
			dsym-&gt;bitoffs = bitoffs;
			dsym-&gt;flen = t1;
			bitoffs =+ t1;
		}
		if (chkoff &amp;&amp; dsym-&gt;hoffset != offset)
			redec();
		dsym-&gt;hoffset = offset;
	}
	if ((dsym-&gt;htype&amp;XTYPE)==FUNC) {
		if (dsym-&gt;hclass!=EXTERN &amp;&amp; dsym-&gt;hclass!=AUTO)
			error("Bad function");
		dsym-&gt;hclass = EXTERN;
	}
	if (dsym-&gt;hclass==AUTO) {
		autolen =+ rlength(dsym);
		dsym-&gt;hoffset = -autolen;
	} else if (dsym-&gt;hclass==STATIC) {
		dsym-&gt;hoffset = isn;
		outcode("BBNBNB", BSS, LABEL, isn++,
		    SSPACE, rlength(dsym), PROG);
	} else if (dsym-&gt;hclass==REG) {
		if ((type&amp;TYPE)&gt;CHAR &amp;&amp; (type&amp;XTYPE)==0
		 || (type&amp;XTYPE)&gt;PTR || regvar&lt;3)
			error("Bad register %o", type);
		dsym-&gt;hoffset = --regvar;
	}
syntax:
	return(elsize);
}

/*
 * Read a declarator and get the implied type
 */
getype()
{
	register int o, type;
	register struct hshtab *ds;

	switch(o=symbol()) {

	case TIMES:
		return(getype()&lt;&lt;TYLEN | PTR);

	case LPARN:
		type = getype();
		if ((o=symbol()) != RPARN)
			goto syntax;
		goto getf;

	case NAME:
		defsym = ds = csym;
		type = 0;
		ds-&gt;ssp = dimp;
	getf:
		switch(o=symbol()) {

		case LPARN:
			if (xdflg) {
				xdflg = 0;
				ds = defsym;
				declare(ARG1, 0, 0, 0);
				defsym = ds;
				xdflg++;
			} else
				if ((o=symbol()) != RPARN)
					goto syntax;
			type = type&lt;&lt;TYLEN | FUNC;
			goto getf;

		case LBRACK:
			chkdim();
			if ((o=symbol()) != RBRACK) {
				peeksym = o;
				cval = conexp();
				for (o=ds-&gt;ssp&amp;0377; o&lt;dimp; o++)
					dimtab[o] =* cval;
				dimtab[dimp++] = cval;
				if ((o=symbol())!=RBRACK)
					goto syntax;
			} else
				dimtab[dimp++] = 1;
			type = type&lt;&lt;TYLEN | ARRAY;
			goto getf;
		}
		peeksym = o;
		return(type);
	}
syntax:
	decsyn(o);
	return(-1);
}

/*
 * Enforce alignment restrictions in structures,
 * including bit-field considerations.
 */
align(type, offset, aflen)
{
	register a, t, flen;
	char *ftl;

	flen = aflen;
	a = offset;
	t = type;
	ftl = "Field too long";
	if (flen==0 &amp;&amp; bitoffs) {
		a =+ (bitoffs-1) / NBPC;
		bitoffs = 0;
	}
	while ((t&amp;XTYPE)==ARRAY)
		t = decref(t);
	if (t!=CHAR) {
		a = (a+ALIGN) &amp; ~ALIGN;
		if (a&gt;offset)
			bitoffs = 0;
	}
	if (flen) {
		if (type==INT) {
			if (flen &gt; NBPW)
				error(ftl);
			if (flen+bitoffs &gt; NBPW) {
				bitoffs = 0;
				a =+ NCPW;
			}
		} else if (type==CHAR) {
			if (flen &gt; NBPC)
				error(ftl);
			if (flen+bitoffs &gt; NCPW) {
				bitoffs = 0;
				a =+ 1;
			}
		} else
			error("Bad type for field");
	}
	return(a-offset);
}

/*
 * Complain about syntax error in declaration
 */
decsyn(o)
{
	error("Declaration syntax");
	errflush(o);
}

/*
 * Complain about a redeclaration
 */
redec()
{
	error("%.8s redeclared", defsym-&gt;name);
}











