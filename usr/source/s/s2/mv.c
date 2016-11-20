<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/mv.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/mv.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/mv.c>
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
/*
mv [-d] file1 file2

unlink file2
link file1 file2
unlink file1
*/
int stbuf[42];
struct sbuf {
int dev;
int inum;
int imode;
char nlink;
char uid;
char gid;
char siz0;
char siz1;
int addr[8];
int adate[2];
int mdate[2];
};
char strbuf[70];

main(argc,argv)
int argc;
char *argv[];
{
char **argp;
char *argp1, *argp2, *argp3, *argp4;
char *p, *p1, *p2;
char place[100];
int i;
int status;
int b;
argp = argv;
/*
	check for correct number
	of arguments
*/
if(argc != 3){
	write(1,"Usage: mv name1 name2\n",22);
	exit();
}
/*
	is there anything to do?
*/
argp3 = argp[1];
argp4 = argp[2];
if(stat(argp[1], stbuf) &lt; 0){
	write(1,"Source file non-existent\n",25);
	exit();
}
/*
	yes, there is a source.
	check whether file or directory
*/
if((stbuf[0].imode &amp; 060000) == 040000){
/*
	The source is a directory, so
	we do lots of checking and
	messing around so as not to get
	into trouble.  This patch of
	code contains administrative
	policies rather than system
	restrictions.
*/
	if(stat(argp[2], stbuf) &gt;= 0){
		write(1,"Directory target exists.\n",25);
		exit();
	}
	argp1 = argp[1];
	argp2 = argp[2];
	while(*argp1 == *argp2){
		argp1++;
		if(*argp2++ == 0){
			write(1,"???\n",4);
			exit();
		}
	}
	while(*argp1)if(*argp1++ == '/'){
		write(1,"Directory rename only\n",22);
		exit();
	}
	while(*argp2)if(*argp2++ == '/'){
		write(1,"Directory rename only\n",22);
		exit();
	}
	if(*--argp1 == '.'){
		write(1,"values of B will give rise to dom!\n",37);
		exit();
	}
}else{
/*
	the source is a file.
*/
	setuid(getuid());
	if(stat(argp4, &amp;stbuf[2]) &gt;= 0){
		if((stbuf[2].imode &amp; 060000) == 040000){
			argp2 = strbuf;
			while(*argp2++ = *argp4++);
			argp2[-1] = '/';
			argp4 = argp[1];
			argp1 = argp[1];
			while(*argp4)
				if(*argp4++ == '/')
					argp1 = argp4;
			while(*argp2++ = *argp1++);
			argp4 = strbuf;
		}
		if(stat(argp4, &amp;stbuf[2]) &gt;= 0){
			if((stbuf[0]==stbuf[2]) &amp;&amp; (stbuf[1]==stbuf[3])){
				write(1,"Files are identical.\n",21);
				exit();
			}
			if((getuid()&amp;0377) == stbuf[2].uid)
				b = 0200; else
			if((getgid()&amp;0377) == stbuf[2].gid)
				b = 020; else
				b = 02;
			if((stbuf[2].imode &amp; b) == 0) {
				printf("%s: %o mode ", argp4,
					stbuf[2].imode &amp; 07777);
				i = b = getchar();
				while(b != '\n' &amp;&amp; b != '\0')
					b = getchar();
				if(i != 'y')
					exit();
			}
			if(unlink(argp4) &lt; 0){
				write(1,"Cannot remove target file.\n",27);
				exit();
			}
		}
	}
}
if(link(argp3, argp4) &lt; 0){
	i = fork();
	if(i == -1){
		write(1,"Try again.\n",11);
		exit();
	}
	if(i){
		while(wait(&amp;status) != i);
	}else{
		p = place;
		p1 = p;
		while(*p++ = *argp3++);
		p2 = p;
		while(*p++ = *argp4++);
		execl("/bin/cp","cp", p1, p2, 0);
		write(1, "no cp\n", 6);
		exit(1);
	}
	if((status &amp; 0377) != 0){
		write(1,"?\n", 2);
		exit();
	}
	if(status != 0) exit();
}
if(unlink(argp3) &lt; 0){
	write(1,"Cannot unlink source file.\n",27);
	exit();
}
}

putchar(c)
{
	write(1, &amp;c, 1);
}

getchar()
{
	char c;

	if(read(0, &amp;c, 1) != 1) return(0);
	return(c);
}











