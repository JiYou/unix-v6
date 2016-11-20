<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/col.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/col.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/col.c>
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
# define PL 102
# define ESC 033
# define SI 017
# define SO 016
# define LINELN 800
char *page[PL];
char lbuff [LINELN], *line;
main ()
{
int c, i, j, ll, cp, mustwr;
	extern int fin, fout;
	fin = dup(0);
	fout = dup(1);

for (ll=0; ll&lt;PL; ll++)
	page[ll] = 0;

c = 1;
cp = ll = 0;
line = lbuff;
mustwr = PL;

while (c&gt;0)
	switch (c = getchar())
		{
		case '\n':
			store (ll%PL);
			if (++ll &gt;= mustwr)
				if (page[ll%PL] != 0)
					{
					printf ("%s\n",page[ll%PL]);
					mustwr++;
					free (page[ll%PL]);
					page[ll%PL]=0;
					}
			fetch (ll%PL);
			cp = 0;
			continue;
		case '\0': continue;
		case ESC:
			c = getchar();
			if (c == '7')
				{
				store(ll%PL);
				ll--;
				fetch (ll%PL);
				}
			else
				{
				outc (ESC, &amp;line);
				outc (c, &amp;line );
				}
			continue;
		case '\r':
			line = lbuff;
			continue;
		case '\t':
			outc (' ', &amp;line);
			cp = line-lbuff;
			while (cp++%8)
				outc(' ', &amp;line);
			continue;
		default:
			outc(c, &amp;line);
		}
for (i=0; i&lt;PL; i++)
   if (page[(mustwr+i)%PL] != 0)
	printf ("%s\n",page[(mustwr+i) % PL]);
flush();
}
outc (c, lp)
	char **lp;
{
int j;
j = 0;
while (j &gt;0 || *(*lp) == '\b' || *(*lp) == ESC || **lp == SI || **lp == SO)
	{
	switch (*(*lp))
		{
		case '\b':
			j++;
			(*lp)++;
			break;
		case '\0':
			*(*lp)++ = ' ';
			j--;
			break;
		case ESC: /* 'escape' */
			(*lp) =+ 2;
			break;
		case SI:
		case SO:
			(*lp)++;
			break;
		default:
			(*lp)++;
			j--;
			break;
		}
	}
if (c != ' ' || *(*lp) == '\0')
	*(*lp) = c;
(*lp)++;
}
store (ll)
{
if (page[ll] != 0)
	free (page[ll]);
page[ll] = alloc ( leng (lbuff) + 2);
copy (page[ll],lbuff);
}
fetch(ll)
{
int i;
for (i=0; i &lt; LINELN; i++)
 lbuff[i] = '\0';
copy (line=lbuff, page[ll]);
}
copy (s,t)
	char *s, *t;
{
if (t == 0)
	return (*s=0);
while (*s++ = *t++);
}
leng (s)
	char *s;
{
int l;
for (l=0; s[l]; l++);
return (l);
}











