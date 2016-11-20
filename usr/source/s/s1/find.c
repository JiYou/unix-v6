<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/find.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/find.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/find.c>
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
/* find -- find files in a pathname.
	Use of find is documented in /usr/man/man1/find.1 .

	In addition, find has a secret first arg "+" which
	causes each file name to be printed along with a period
	if the predicates succeed.
 */
int randlast;
char *pathname;
int verbose;
struct anode {
	int (*F)();
	struct anode *L, *R;
} node[100];
int nn;  /* number of nodes */
char *fname, *path;
int now[2];
int ap, ac;
char **av;

struct ibuf {
	int	idev;
	int	inum;
	int	iflags;
	char	inl;
	char	iuid;
	char	igid;
	char	isize0;
	char	*isize;
	int	iaddr[8];
	int	iatime[2];
	int	imtime[2];
} statb;

main(argc,argv) char *argv[]; {

struct anode *exlist;
int find();

	time(&amp;now);
	ac = argc; av = argv; ap = 2;
	pathname = argv[1];
	if(compstr(argv[1],"+")==0) {
		verbose++;
		ap++;
		pathname = argv[2];
	} else verbose = 0; 
	argv[argc] = 0;
	if(argc&lt;3) {
		printf("Insufficient args\n");
		exit(9);
	}
	if(!(exlist = exp())) { /* parse and compile the arguments */
		printf("Odd usage\n");
		exit(9);
	}
	if(ap&lt;argc) {
		printf("Missing conjunction\n");
		exit(9);
	}
	descend(pathname,'f',find,exlist); /* to find files that match  */
}

/* compile time functions:  priority is  exp()&lt;e1()&lt;e2()&lt;e3()  */

struct anode *exp() { /* parse -o ... */
	int or();
	int p1;
	char *na;
	p1 = e1() /* get left operand */ ;
	if(compstr(na=nxtarg(),"-o")==0) {
		randlast--;
		return(mk(&amp;or,p1,exp()));
	}
	else if(*na!=0) --ap;
	return(p1);
}
struct anode *e1() { /* parse -a */
	int and();
	int p1;
	char *na;
	p1 = e2();
	if(compstr(na=nxtarg(),"-a")==0) {
		randlast--;
		return(mk(&amp;and,p1,e1()));
	}
	else if(*na!=0) --ap;
	return(p1);
}
struct anode *e2() { /* parse not (!) */
	int not();
	char *na;
	if(randlast) {
		printf("operand follows operand.\n");
		exit(9);
	}
	randlast++;
	if(compstr(na=nxtarg(),"!")==0)
		return(mk(&amp;not,e3(),0));
	else if(*na!=0) --ap;
	return(e3());
}
struct anode *e3() { /* parse parens and predicates */
	int exeq(), ok(), glob(),  mtime(), atime(), user(),
		group(), size(), perm(), links(), print(),
		type();
	int p1, i;
	char *a, *b, s;
	a = nxtarg();
	if(compstr(a,"(")==0) {
		randlast--;
		p1 = exp();
		a = nxtarg();
		if(compstr(a,")")!=0) goto err;
		return(p1);
	}
	else if(compstr(a,"-print")==0) {
		return(mk(&amp;print,0,0));
	}
	b = nxtarg();
	s = *b;
	if(s=='+') b++;
	if(compstr(a,"-name")==0)
		return(mk(&amp;glob,b,0));
	else if(compstr(a,"-mtime")==0)
		return(mk(&amp;mtime,atoi(b),s));
	else if(compstr(a,"-atime")==0)
		return(mk(&amp;atime,atoi(b),s));
	else if(compstr(a,"-user")==0) {
		if((i=getunum(b)) == -1) {
			printf("Cannot find user \"%s\"\n",b);
			exit(9);
		}
		return(mk(&amp;user,i,s));
	}
	else if(compstr(a,"-group")==0)
		return(mk(&amp;group,atoi(b),s));
	else if(compstr(a,"-size")==0)
		return(mk(&amp;size,atoi(b),s));
	else if(compstr(a,"-links")==0)
		return(mk(&amp;links,atoi(b),s));
	else if(compstr(a,"-perm")==0) {
		for(i=0; *b ; ++b) {
			if(*b=='-') continue;
			i =&lt;&lt; 3;
			i = i + (*b - '0');
		}
		return(mk(&amp;perm,i,s));
	}
	else if(compstr(a,"-type")==0) {
		i = s=='d' ? 040000 :
		    s=='b' ? 060000 :
		    s=='c' ? 020000 :
		    000000;
		return(mk(&amp;type,i,0));
	}
	else if (compstr(a,"-exec")==0) {
		i = ap - 1;
		while(compstr(nxtarg(),";")!=0);
		return(mk(&amp;exeq,i,0));
	}
	else if (compstr(a,"-ok")==0) {
		i = ap - 1;
		while(compstr(nxtarg(),";")!=0);
		return(mk(&amp;ok,i,0));
	}
	err: printf("Bad option: \"%s\" \"%s\"\n",a,b);
	exit(9);
}
struct anode *mk(f,l,r) struct anode *l,*r; { /*make an expression node*/
	node[nn].F = f;
	node[nn].L = l;
	node[nn].R = r;
	return(&amp;(node[nn++]));
}

nxtarg() { /* get next arg from command line */
	if(ap&gt;=ac) return("");
	return(av[ap++]);
}

find(exlist,fullname) /* execute predicat list with current file */
struct anode *exlist;
char *fullname;
{
register int i;
	path = fullname;
	if(verbose) printf("%s",path);
	for(i=0;fullname[i];++i)
		if(fullname[i]=='/') fname = &amp;fullname[i+1];
	i = (*exlist-&gt;F)(exlist);
	if(verbose)
		if(i) printf(".\n");
		else printf("\n");
}

/* execution time functions */
and(p) struct anode *p; {
	return(((*p-&gt;L-&gt;F)(p-&gt;L)) &amp;&amp; ((*p-&gt;R-&gt;F)(p-&gt;R))?1:0);
}
or(p) struct anode *p; {
	 return(((*p-&gt;L-&gt;F)(p-&gt;L)) || ((*p-&gt;R-&gt;F)(p-&gt;R))?1:0);
}
not(p) struct anode *p; {
	return( !((*p-&gt;L-&gt;F)(p-&gt;L)));
}
glob(p) struct { int f; char *pat; } *p;  {
	return(gmatch(fname,p-&gt;pat));
}
print() {
	printf("%s\n",path);
	return(1);
}
mtime(p) struct { int f, t, s; } *p;  {
	return(scomp((now[0]-statb.imtime[0])*3/4,p-&gt;t,p-&gt;s));
}
atime(p) struct { int f, t, s; } *p;  {
	return(scomp((now[0]-statb.iatime[0])*3/4,p-&gt;t,p-&gt;s));
}
user(p) struct { int f, u, s; } *p;  {
	return(scomp(statb.iuid,p-&gt;u,p-&gt;s));
}
group(p) struct { int f, u; } *p;  {
	return(p-&gt;u == statb.igid);
}
links(p) struct { int f, link, s; } *p;  {
	return(scomp(statb.inl,p-&gt;link,p-&gt;s));
}
size(p) struct { int f, sz, s; } *p;  {
	register int i;
	i = statb.isize0 &lt;&lt; 7;
	i = i | ((statb.isize&gt;&gt;9) &amp; 0177);
	return(scomp(i,p-&gt;sz,p-&gt;s));
}
perm(p) struct { int f, per, s; } *p;  {
int i;
	i = (p-&gt;s=='-') ? p-&gt;per : 03777; /* '-' means only arg bits */
	return((statb.iflags &amp; i &amp; 017777) == p-&gt;per);
}
type(p) struct { int f, per, s; } *p; {
	return((statb.iflags&amp;060000)==p-&gt;per);
}
exeq(p) struct { int f, com; } *p; {
	return(doex(p-&gt;com));
}
ok(p) struct { int f, com; } *p; {
	char c;  int yes;
	yes = 0;
	printf("%s ... %s ...? ",av[p-&gt;com],path);
	if((c=getchar())=='y') yes = 1;
	while(c!='\n') c = getchar();
	if(yes) return(doex(p-&gt;com));
	return(0);
}

/* support functions */
scomp(a,b,s) char s; { /* funny signed compare */
	if(s == '+')
		return(a &gt; b);
	if(s == '-')
		return(a &lt; (b * -1));
	return(a == b);
}
doex(com) {
	int ccode;
	int np, i, c;
	char *nargv[50], *ncom, *na;

	ccode = np = 0;
	while (na=av[com++]) {
		if(compstr(na,";")==0) break;
		if(compstr(na,"{}")==0) nargv[np++] = path;
		else nargv[np++] = na;
	}
	nargv[np] = 0;
	if (np==0) return(9);
	if(fork()) /*parent*/ wait(&amp;ccode);
	else { /*child*/
		execv(nargv[0], nargv, np);
		i = 0;
		ncom = "/usr/bin/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		while(c=nargv[0][i])  {
			ncom[9+i++] = c;
		}
		ncom[9+i] = '\0';
		execv(ncom+4, nargv, np);
		execv(ncom, nargv, np);
		exit(9);
	}
	return(ccode ? 0:1);
}

char fin[518];
getunum(s) char *s; { /* find username in /etc/passwd &amp; return num. */
int i;
char str[20], *sp, c;
	i = -1;
	fin[0] = open("/etc/passwd",0);
	while(c = getchar()) {
		if(c=='\n') {
			sp = str;
			while((*sp = getchar()) != ':')
				if(! *sp++) goto RET;
			*sp = '\0';
			if(compstr(str,s)==0) {
				while((c=getchar()) != ':')
					if(! c) goto RET;
				sp = str;
				while((*sp = getchar()) != ':') sp++;
				*sp = '\0';
				i = atoi(str);
				break;
			}
		}
	}
	RET:
	close(fin);
	fin[0] = 0;
	return(i);
}

compstr(s1,s2) char s1[], s2[]; {   /* compare strings: */
register char *c1, *c2;
	c1 = s1;  c2 = s2;
	while(*c1 == *c2)
		if(*c1++ == '\0')
			return(0); /* s1 == s2 */
		else c2++;
	return(*c1 &gt; *c2 ? 1 : -1);
}

int descend(name,goal,func,arg)
int (*func)();
char *name, goal;
{
	int dir /* open directory */, offset /* in directory */;
	int dsize, top;
	struct {
		int	dinode;
		char	dname[14];
	} dentry[32];
	register int i, j, k;
	char aname[128];

	if(stat(name,&amp;statb)&lt;0) {
		printf("--bad status %s\n",name);
		return(0);
	}
/*
	if((statb.iflags&amp;060000)!=040000){ /*not a directory*/
/*
		if(goal=='f'||goal=='b') /* search goal for files */
/*
			(*func)(arg,name);
		return(1);
	} else  if(goal=='d' || goal=='b') /* search goal is directories */
/*
			(*func)(arg,name);
*/
	(*func)(arg,name);
	if((statb.iflags&amp;060000)!=040000)
		return(1);

	top = statb.isize;
	for(offset=0 ; offset &lt; top ; offset =+ 512) { /* each block */
		dsize = 512&lt;(top-offset) ? 512 : (top-offset);
		if((dir=open(name,0))&lt;0) {
			printf("--cannot open %s\n",name);
			return(0);
		}
		if(offset) seek(dir,offset,0);
		if(read(dir,&amp;dentry,dsize)&lt;0) {
			printf("--cannot read %s\n",name);
			return(0);
		}
		close(dir);
		for(i = 0; i &lt; (dsize&gt;&gt;4); ++i) { /* each dir. entry */
			if(dentry[i].dinode==0 ||
				compstr(dentry[i].dname,".")==0 ||
				compstr(dentry[i].dname,"..")==0)
				continue;
			if (dentry[i].dinode == -1) break;
			for(j=0;aname[j]=name[j];++j);
			if(aname[j-1]!='/') aname[j++] = '/';
			for(k=0; (aname[j++]=dentry[i].dname[k]) &amp;&amp;
				k&lt;13; ++k);
			aname[j] = '\0';
			if(descend(aname,goal,func,arg)==0)
				printf("--%s\n",name);
		}
	}
	return(1);
}

gmatch(s, p) /* string match as in glob */
char *s, *p; {
	if (*s=='.' &amp;&amp; *p!='.') return(0);
	return(amatch(s, p));
}

amatch(s, p)
char *s, *p;
{
	register int cc, scc, k;
	int c, lc;

	scc = *s;
	lc = 077777;
	switch (c = *p) {

	case '[':
		k = 0;
		while (cc = *++p) {
			switch (cc) {

			case ']':
				if (k)
					return(amatch(++s, ++p));
				else
					return(0);

			case '-':
				k =| lc &lt;= scc &amp; scc &lt;= (cc=p[1]);
			}
			if (scc==(lc=cc)) k++;
		}
		return(0);

	case '?':
	caseq:
		if(scc) return(amatch(++s, ++p));
		return(0);
	case '*':
		return(umatch(s, ++p));
	case 0:
		return(!scc);
	}
	if (c==scc) goto caseq;
	return(0);
}

umatch(s, p)
char *s, *p;
{
	if(*p==0) return(1);
	while(*s)
		if (amatch(s++,p)) return(1);
	return(0);
}











