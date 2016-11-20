<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/stty.c</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/stty.c</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/stty.c>
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
/*
 * set teletype modes
 */

/*
 * tty flags
 */
#define	HUPCL	01
#define	XTABS	02
#define	LCASE	04
#define	ECHO	010
#define	CRMOD	020
#define	RAW	040
#define	ODDP	0100
#define	EVENP	0200
#define	ANYP	0300

/*
 * Delay algorithms
 */
#define	CR0	0
#define	CR1	010000
#define	CR2	020000
#define	CR3	030000
#define	NL0	0
#define	NL1	000400
#define	NL2	001000
#define	NL3	001400
#define	TAB0	0
#define	TAB1	002000
#define	TAB2	004000
#define	TAB3	006000
#define	FF0	0
#define	FF1	040000
#define	BS0	0
#define	BS1	0100000
#define	ALL	0177400

struct
{
	char	*string;
	int	speed;
} speeds[]
{
	"0",	(0&lt;&lt;8)|0,
	"50",	(1&lt;&lt;8)|1,
	"75",	(2&lt;&lt;8)|2,
	"110",	(3&lt;&lt;8)|3,
	"134",	(4&lt;&lt;8)|4,
	"134.5",(4&lt;&lt;8)|4,
	"150",	(5&lt;&lt;8)|5,
	"200",	(6&lt;&lt;8)|6,
	"300",	(7&lt;&lt;8)|7,
	"600",	(8&lt;&lt;8)|8,
	"1200",	(9&lt;&lt;8)|9,
	"1800",	(10&lt;&lt;8)|10,
	"2400",	(11&lt;&lt;8)|11,
	"4800",	(12&lt;&lt;8)|12,
	"9600",	(13&lt;&lt;8)|13,
	"exta",	(14&lt;&lt;8)|14,
	"extb",	(15&lt;&lt;8)|15,
	0,
};
struct
{
	char	*string;
	int	set;
	int	reset;
} modes[]
{
	"even",
	EVENP, 0,

	"-even",
	0, EVENP,

	"odd",
	ODDP, 0,

	"-odd",
	0, ODDP,

	"raw",
	RAW, 0,

	"-raw",
	0, RAW,

	"cooked",
	0, RAW,

	"-nl",
	CRMOD, 0,

	"nl",
	0, CRMOD,

	"echo",
	ECHO, 0,

	"-echo",
	0, ECHO,

	"LCASE",
	LCASE, 0,

	"lcase",
	LCASE, 0,

	"-LCASE",
	0, LCASE,

	"-lcase",
	0, LCASE,

	"-tabs",
	XTABS, 0,

	"tabs",
	0, XTABS,

	"hup",
	HUPCL, 0,

	"-hup",
	0, HUPCL,

	"cr0",
	CR0, CR3,

	"cr1",
	CR1, CR3,

	"cr2",
	CR2, CR3,

	"cr3",
	CR3, CR3,

	"tab0",
	TAB0, TAB3,

	"tab1",
	TAB1, TAB3,

	"tab2",
	TAB2, TAB3,

	"tab3",
	TAB3, TAB3,

	"nl0",
	NL0, NL3,

	"nl1",
	NL1, NL3,

	"nl2",
	NL2, NL3,

	"nl3",
	NL3, NL3,

	"ff0",
	FF0, FF1,

	"ff1",
	FF1, FF1,

	"bs0",
	BS0, BS1,

	"bs1",
	BS1, BS1,

	"33",
	CR1, ALL,

	"tty33",
	CR1, ALL,

	"37",
	FF1+CR2+TAB1+NL1, ALL,

	"tty37",
	FF1+CR2+TAB1+NL1, ALL,

	"05",
	NL2, ALL,

	"vt05",
	NL2, ALL,

	"tn",
	CR1, ALL,

	"tn300",
	CR1, ALL,

	"ti",
	CR2, ALL,

	"ti700",
	CR2, ALL,

	"tek",
	FF1, ALL,

	0,
	};

char	*arg;
int	mode[3];

struct { char lobyte, hibyte; };

main(argc, argv)
char	*argv[];
{
	int i;

	gtty(1, mode);
	if(argc == 1) {
		prmodes();
		exit(0);
	}
	while(--argc &gt; 0) {

		arg = *++argv;
		if (eq("ek"))
			mode[1] = '#@';
		if (eq("erase")) {
			mode[1].lobyte = **++argv;
			argc--;
		}
		if (eq("kill")) {
			mode[1].hibyte = **++argv;
			argc--;
		}
		for(i=0; speeds[i].string; i++)
			if(eq(speeds[i].string))
				mode[0] = speeds[i].speed;
		for(i=0; modes[i].string; i++)
			if(eq(modes[i].string)) {
				mode[2] =&amp; ~modes[i].reset;
				mode[2] =| modes[i].set;
			}
		if(arg)
			printf("unknown mode: %s\n", arg);
	}
	stty(1,mode);
}

eq(string)
char *string;
{
	int i;

	if(!arg)
		return(0);
	i = 0;
loop:
	if(arg[i] != string[i])
		return(0);
	if(arg[i++] != '\0')
		goto loop;
	arg = 0;
	return(1);
}

prmodes()
{
	register m;

	if(mode[0].lobyte != mode[0].hibyte) {
		prspeed("input speed  ", mode[0].lobyte);
		prspeed("output speed ", mode[0].hibyte);
	} else
		prspeed("speed ", mode[0].lobyte);
	printf("erase = '%c'; kill = '%c'\n", mode[1].lobyte, mode[1].hibyte);
	m = mode[2];
	if(m &amp; 0200) printf("even ");
	if(m &amp; 0100) printf("odd ");
	if(m &amp; 040) printf("raw ");
	if(m &amp; 020) printf("-nl ");
	if(m &amp; 010) printf("echo ");
	if(m &amp; 04) printf("lcase ");
	if(m &amp; 02) printf("-tabs ");
	if(m &amp; 01) printf("hup ");
	delay(m&gt;&gt;8, "nl");
	delay(m&gt;&gt;10, "tab");
	delay(m&gt;&gt;12, "cr");
	delay((m&gt;&gt;14)&amp;1, "ff");
	delay((m&gt;&gt;15)&amp;1, "bs");
	printf("\n");
}

delay(m, s)
char *s;
{

	if(m =&amp; 3)
		printf("%s%d ", s, m);
}

int	speed[]
{
	0,50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,0,0
};

prspeed(c, s)
{

	printf("%s%d baud\n", c, speed[s]);
}

putchar(c)
{

	write(2, &amp;c, 1);
}








//mmread


