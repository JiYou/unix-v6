<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/quiz.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/quiz.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/quiz.c>
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
#define NF 10
#define NL 200
#define NC 200
#define SL 100
#define NA 10

int tflag;
int xx[NL];
char score[NL];
int rights;
int wrongs;
int guesses;
int buf[259];
int nl 0;
int na NA;
int inc;
int ptr 0;
int nc 0;
char line[150];
char response[100];
char *tmp[NF];
int select[NF];

readline()
{
	char *t;
loop:
	for(t=line;(*t=getc(buf))!=-1;t++) {
		nc++;
		if(*t==' '&amp;&amp;(t==line||t[-1]==' '))
			t--;
		if(*t=='\n') {
			if(t[-1]=='\\')		/*inexact test*/
				continue;
			while(t&gt;line&amp;&amp;t[-1]==' ')
				*--t = '\n';
			*++t = 0;
			return(1);
		}
		if(t-line&gt;=NC) {
			printf("Too hard for me\n");
			do {
				*line = getc(buf);
				if(*line==0377)
					return(0);
			} while(*line!='\n');
			goto loop;
		}
	}
	return(0);
}

char *eu;
char *ev;
cmp(u,v)
{
	int x;
	eu = u;
	ev = v;
	x = disj(1);
	if(x!=1)
		return(x);
	return(eat(1,0));
}

disj(s)
{
	int t, x;
	char *u;
	u = eu;
	t = 0;
	for(;;) {
		x = string(s);
		if(x&gt;1)
			return(x);
		switch(*ev) {
		case 0:
		case ']':
		case '}':
			return(t|x&amp;s);
		case '|':
			ev++;
			t =| s;
			s = 0;
			continue;
		}
		if(s) eu = u;
		if(string(0)&gt;1)
			return(2);
		switch(*ev) {
		case 0:
		case ']':
			return(0);
		case '}':
			return(1);
		case '|':
			ev++;
			continue;
		default:
			return(2);
		}
	}
}

string(s)
{
	int x;
	for(;;) {
		switch(*ev) {
		case 0:
		case '|':
		case ']':
		case '}':
			return(1);
		case '\\':
			ev++;
			if(*ev==0)
				return(2);
			if(*ev=='\n') {
				ev++;
				continue;
			}
		default:
			if(eat(s,*ev)==1)
				continue;
			return(0);
		case '[':
			ev++;
			x = disj(s);
			if(*ev!=']' || x&gt;1)
				return(2);
			ev++;
			if(s==0)
				continue;
			if(x==0)
				return(0);
			continue;
		case '{':
			ev++;
			x = disj(s);
			if(*ev!='}'||x&gt;1)
				return(2);
			ev++;
			continue;
		}
	}
}

eat(s,c)
char c;
{
	if(*ev!=c)
		return(2);
	if(s==0) {
		ev++;
		return(1);
	}
	if(fold(*eu)!=fold(c))
		return(0);
	eu++;
	ev++;
	return(1);
}

fold(c)
char c;
{
	if(c&lt;'A'||c&gt;'Z')
		return(c);
	return(c|040);
}

publish(t)
char *t;
{
	ev = t;
	pub1(1);
}

pub1(s)
{
	for(;;ev++){
		switch(*ev) {
		case '|':
			s = 0;
			ev;
			continue;
		case ']':
		case '}':
		case 0:
			return;
		case '[':
		case '{':
			ev++;
			pub1(s);
			ev;
			continue;
		case '\\':
			if(*++ev=='\n')
				continue;
		default:
			if(s)
				putchar(*ev);
		}
	}
}

segment(u,w)
char *u, *w[];
{
	char *s;
	int i;
	char *t;
	s = u;
	for(i=0;i&lt;NF;i++) {
		u = s;
		t = w[i];
		while(*s!=':'&amp;&amp;*s!='\n'&amp;&amp;s-u&lt;SL) {
			if(*s=='\\')  {
				if(s[1] == '\n') {
					s =+ 2;
					continue;
				}
				*t++ = *s++;
			}
			*t++ = *s++;
		}

		while(*s!=':'&amp;&amp;*s!='\n')
			s++;
		*t = 0;
		if(*s++=='\n') {
			return(i+1);
		}
	}
	printf("Too many facts about one thing\n");
}

perm(u,m,v,n,p)
int p[];
char *u[], *v[];
{
	int i, j;
	int x;
	for(i=0;i&lt;m;i++) {
		for(j=0;j&lt;n;j++) {
			x = cmp(u[i],v[j]);
			if(x&gt;1) badinfo();
			if(x==0)
				continue;
			p[i] = j;
			goto uloop;
		}
		return(0);
uloop:		;
	}
	return(1);
}

find(u,m)
char *u[];
{
	int n;
	while(readline()){
		n = segment(line,tmp);
		if(perm(u,m,tmp+1,n-1,select))
			return(1);
	}
	return(0);
}

readindex()
{
	xx[0] = nc = 0;
	while(readline()) {
		xx[++nl] = nc;
		if(nl&gt;=NL) {
			printf("I've forgotten some of it\n");
			break;
		}
	}
}

talloc()
{
	int i;
	for(i=0;i&lt;NF;i++)
		tmp[i] = alloc(SL);
}

main(argc,argv)
char *argv[];
{
	int i;
	int x;
	char *info;
	int tvec[2];
	char *t;
	extern done();
	int count;
	info = "/usr/lib/quiz/index";
	time(tvec);
	inc = tvec[1]&amp;077774|01;
loop:
	if(argc&gt;1&amp;&amp;*argv[1]=='-') {
		switch(argv[1][1]) {
		case 'i':
			if(argc&gt;2) 
				info = argv[2];
			argc =- 2;
			argv =+ 2;
			goto loop;
		case 't':
			tflag = 1;
			argc--;
			argv++;
			goto loop;
		}
	}
	if(fopen(info,buf)== -1) {
		printf("No info\n");
		exit();
	}
	talloc();
	if(argc&lt;=2)
		instruct(info);
	signal(2,done);
	argv[argc] = 0;
	if(find(&amp;argv[1],argc-1)==0)
		dunno();
	close(buf[0]);
	if(fopen(tmp[0],buf)== -1) 
		dunno();
	readindex();
	if(!tflag || na&gt;nl)
		na = nl;
	for(;;) {
		i = next();
		seek(buf[0],xx[i],0);
		read(buf[0],line,xx[i+1]-xx[i]);
		segment(line,tmp);
		if(*tmp[select[0]] == '\0' || *tmp[select[1]] == '\0') {
			score[i] = 1;
			continue;
		}
		publish(tmp[select[0]]);
		printf("\n");
		for(count=0;;count++) {
			if(query(response)==0) {
				publish(tmp[select[1]]);
				printf("\n");
				if(count==0) wrongs++;
				score[i] = tflag?-1:1;
				break;
			}
			x = cmp(response,tmp[select[1]]);
			if(x&gt;1) badinfo();
			if(x==1) {
				printf("Right!\n");
				if(count==0) rights++;
				if(++score[i]&gt;=1 &amp;&amp; na&lt;nl)
					na++;
				break;
			}
			printf("What?\n");
			if(count==0) wrongs++;
			score[i] = tflag?-1:1;
		}
		guesses =+ count;
	}
}

query(r)
char *r;
{
	char *t;
	for(t=r;;t++) {
		if(read(0,t,1)==0)
			done();
		if(*t==' '&amp;&amp;(t==r||t[-1]==' '))
			t--;
		if(*t=='\n') {
			while(t&gt;r&amp;&amp;t[-1]==' ')
				*--t = '\n';
			break;
		}
	}
	*t = 0;
	return(t-r);
}

next()
{
	int flag;
	inc = inc*3125&amp;077777;
	ptr = (inc&gt;&gt;2)%na;
	flag = 0;
	while(score[ptr]&gt;0)
		if(++ptr&gt;=na) {
			ptr = 0;
			if(flag) done();
			flag = 1;
		}
	return(ptr);
}

done()
{
	printf("\nRights %d, wrongs %d, ", rights, wrongs);
	if(guesses)
		printf("extra guesses %d, ", guesses);
	printf("score %d%%\n",100*rights/(rights+wrongs));
	exit();
}
instruct(info)
{
	char *t;
	int i, n;
	printf("Subjects:\n\n");
	while(readline()) {
		printf("-");
		n = segment(line,tmp);
		for(i=1;i&lt;n;i++) {
			printf(" ");
			publish(tmp[i]);
		}
		printf("\n");
	}
	printf("\n");
	if(fopen(info,buf)== -1)
		abort();
	readline();
	segment(line,tmp);
	printf("For example,\n");
	printf("    quiz ");
	publish(tmp[1]);
	printf(" ");
	publish(tmp[2]);
	printf("\nasks you a ");
	publish(tmp[1]);
	printf(" and you answer the ");
	publish(tmp[2]);
	printf("\n    quiz ");
	publish(tmp[2]);
	printf(" ");
	publish(tmp[1]);
	printf("\nworks the other way around\n");
	printf("\nType empty line to get correct answer.\n");
	exit();
}

badinfo(){
	printf("Bad info %s\n",line);
}

dunno()
{
	printf("I don't know about that\n");
	exit();
}











