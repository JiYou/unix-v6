<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/tbl.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/tbl.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/tbl.c>
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
#
# define ever (;;)
# define MAXCHS 2000
main(argc,argv)
	char *argv[];
{
char line[200];
extern int cin;
extern char *cspace;
	cspace = getvec(MAXCHS + 100);
for ever
{
	if (argc&gt;1)
	{
	argc--;
	argv++;
	if (match(*argv, "-ms"))
		*argv = "/usr/lib/tmac.s";
	cin = copen(*argv, 'r');
	if (cin &lt; 0)
		{
		printf(2,"where is input file %s\n",*argv);
		cexit();
		}
	}
while (gets(line))
	if (match(line, ".TS"))
		tableput();
	else
		puts(line);
if (argc &lt;= 1) break;
}
cexit();
}
# define MAXCOL 35
# define MAXLIN 220
# define ONELINE 250
char *tabentry[MAXLIN][MAXCOL];
char extra[MAXCHS];
char *cspace, *cstore;
char *exstore extra;
int sep[MAXCOL], dwide[MAXCOL];
char *instead[MAXLIN];
int newtab[MAXLIN];
char *style[MAXCOL];
tableput()
{
/* read input, write output, make tables on the way */
char st[ONELINE], *format;
int ilin, nlin, icol, ncol, k, ch, ws, cs, ns;
int maxchnge, ct;
printf(".TS\n");
/* get command line */
cstore = cspace;
exstore = extra;
ncol = 0;
for (ilin=0; ilin&lt;MAXLIN; ilin++)
	newtab[ilin]=0;
gets (st);
for (k=0; k&lt;ONELINE &amp;&amp; st[k] != '\0'; k++)
	if (!space(st[k]))
		{
		style[ncol] = st+k;
		dwide[ncol] =0;
		for(; letter(st[k]); k++)
			{
			if ((st[k]=='n' || st[k] == 'N') &amp;&amp;
				!dwide[ncol])
				{
				dwide[ncol]=1;
				sep[ncol++] = 0;
				style[ncol] = style[ncol-1];
				dwide [ncol] = 0;
				}
			}
		if (digit(st[k]))
			sep[ncol] = numb(st+k);
		else
			sep[ncol] = 3;
		ncol++;
		while (digit(st[k]))
			st[k++] = '\0';
		if (st[k] == '\0')
			break;
		st[k] = '\0';
		}
/* now get input lines */
for (nlin=0; gets(cstore) &amp;&amp; !match(cstore, ".TE"); nlin++)
	{
	if (cstore[0] == '.' &amp;&amp; letter(cstore[1]))
		{
		instead[nlin] = cstore;
		while (*cstore) cstore++;
		}
	else instead[nlin] = 0;
	for (icol = 0; icol &lt;ncol; icol++)
		{
		tabentry[nlin][icol] = cstore;
		for(; (ch= *cstore) != '\0' &amp;&amp; ch != '\t'; cstore++)
				;
		*cstore++ = '\0';
		if (dwide[icol] )
			if (broken(style[icol],nlin))
				{
				tabentry[nlin][icol+1]=maknew(tabentry[nlin][icol]);
				icol++;
				if (tabentry[nlin][icol] ==0)
					newtab[nlin]=newtab[nlin+1]=1;
				}
			else
				tabentry[nlin][++icol] = 0;
		while (span(style[icol+1],nlin) )
			tabentry[nlin][++icol] = "";
		if (ch == '\0') break;
		}
	while (++icol &lt;ncol)
		tabentry[nlin][icol] = "";
	while (*cstore != '\0')
		 *cstore++;
	if (cstore-cspace &gt; MAXCHS)
		cstore = cspace  = getvec(MAXCHS+100);
	}
/* find longest command string */
for (icol=maxchnge=0; icol&lt;ncol; icol++)
	if (slen(style[icol]) &gt;maxchnge)
		maxchnge = slen(style[icol]);
/* set tab stops */
printf(".nr 49 0\n");
for (icol = 0; icol&lt;ncol; icol++)
	{
	ct = 1 + (icol&gt;0 ? sep[icol-1] : 0);
	printf(".nr %2d 0\n", icol+50);
	for (ilin=0; ilin &lt; nlin; ilin++)
		{
		if (icol&gt;0 &amp;&amp; dwide[icol-1]&gt;0 &amp;&amp; tabentry[ilin][icol]==0)
			{
			printf(".nr 48 \\n(%2d+\\w'%s'+%dn\n",
				icol+48, tabentry[ilin][icol-1], sep[icol-1]);
			printf(".if \\n(48-\\n(%2d .nr %2d \\n(48\n",
				icol+50,icol+50);
			}
		if ( !span(style[icol],ilin) &amp;&amp; /* not part of span */
		   (dwide[icol] == 0 || tabentry[ilin][icol+1]!= 0)
				/* not a double item */
		     &amp;&amp; (!span(style[icol+1],ilin) || icol==ncol))
			{
			printf(".nr 47 \\n(%2d+\\w'%s'+%dn\n",
			icol+49,tabentry[ilin][icol], ct);
			printf(".if \\n(47-\\n(%2d .nr %2d \\n(47\n",
				icol+50,icol+50);
			}
		}
	/* clean up spanned headings */
	for(ilin=0; ilin&lt;maxchnge; ilin++)
		{
		if( !(span(style[icol],ilin)) &amp;&amp;
		  (icol ==1 || dwide[icol-1] == 0) &amp;&amp;
		  span(style[icol+(dwide[icol]?2:1)],ilin))
			printf(".nr %d \\n(%2d+\\w'%s'+%dn\n",
			  ilin+30, icol+49, tabentry[ilin][icol], ct);
		else if (span(style[icol],ilin) &amp;&amp;
		  (icol+1==ncol || !span(style[icol+1],ilin)))
			printf(".if \\n(%d-\\n(%d .nr %d \\n(%d\n",
				30+ilin, icol+50, icol+50, ilin+30);
		}
	}
/* run out table, put field characters in */
printf (".fc  @\n");
for (ilin=0; ilin&lt;nlin; ilin++)
	{
	if (instead[ilin])
		{
		printf("%s\n",instead[ilin]);
		continue;
		}
	/* is a new set of tab stops needed */
	if (ilin &lt; maxchnge || newtab[ilin])
		settab(ncol, ilin);
	for (icol=0; icol&lt;ncol; icol++)
		{
		switch ( ylet(style[icol],ilin))
			{
			default:
			case 'L': case 'l':
				format = "%s@"; break;
			case 'R': case 'r':
				format= "@%s"; break;
			case 'n': case 'N':
				if (!dwide[icol] || tabentry[ilin][icol+1] != 0)
					{
					format=dwide[icol]? "@%s" : "%s@";
					break;
					}
			case 'c': case 'C':
				format = "@%s@"; break;
			case 's':  case 'S':
				format= "";
				break;
			}
		if (! (dwide [icol-1]&gt;0 &amp;&amp; tabentry[ilin][icol] == 0 ))
		printf(format, tabentry[ilin][icol]);
		if (!span(style[icol+1],ilin))
			for (k=sep[icol]; k&gt;0; k--)
				printf(" ");
		}
	printf("\n");
	}
printf(".fc\n");
printf(".TE\n");
}
match (s1, s2)
	char *s1, *s2;
{
	while (*s1 == *s2)
		if (*s1++ == '\0')
			return(1);
		else
			*s2++;
	return(0);
}
slen(str)
	char *str;
{
	int k;
	for (k=0; *str++ != '\0'; k++);
	return(k);
}
space(ch)
	{
	switch (ch)
		{
		case ' ': case '\t':
			return(1);
	}
	return(0);
	}
letter (ch)
	{
	if (ch &gt;= 'a' &amp;&amp; ch &lt;= 'z')
		return(1);
	if (ch &gt;= 'A' &amp;&amp; ch &lt;= 'Z')
		return(1);
	return(0);
	}
numb(str)
	char *str;
	{
	/* convert to integer */
	int k;
	for (k=0; *str &gt;= '0' &amp;&amp; *str &lt;= '9'; str++)
		k = k*10 + *str - '0';
	return(k);
	}
broken(str, nlin)
	{
	switch(ylet(str,nlin))
		{
		case 'n': case 'N':
			return(1);
		}
	return(0);
	}
ylet (str, k)
	char *str;
{
	k++;
	while (*str &amp;&amp;k--)
		str++;
	return(*--str);
}
span(str, k)
	{
	switch(ylet(str,k))
		{
		case 's': case 'S':
			return(1);
		}
	return(0);
	}
maknew(str)
	char *str;
{
	/* make two numerical fields */
	int point;
	char *p, *q;
	p = str;
	for (point=0; *str; str++)
		if (*str=='.')
			point=1;
	if (!point &amp;&amp; *(str-1)== '$')
		return(0);
	for(; str&gt;p; str--)
		if ( (point &amp;&amp; *str == '.') ||
		    (!point &amp;&amp; digit(*(str-1)) ) )
			break;
	if (!point &amp;&amp; p==str) /* not numerical, don't split */
		return(0);
	p= str;
	q = exstore;
	while (*exstore++ = *str++);
	*p = 0;
	return(q);
	}
digit(x)
	{
	return(x&gt;= '0' &amp;&amp; x&lt;= '9');
	}
settab(ncol, ilin)
{
	int k, icol;
	printf(".ta ");
	for (icol = 0; icol&lt;ncol; icol++)
	   if ((dwide[icol] == 0 || tabentry[ilin][icol+1] != 0)
		&amp;&amp; !span(style[icol+1],ilin))
		printf("\\n(%du ",icol+50);
	printf("\n");
}











