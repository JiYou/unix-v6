#define	NMOUNT	16
#define	NAMSIZ	32

struct mtab {
	char	file[NAMSIZ];
	char	spec[NAMSIZ];
} mtab[NMOUNT];

main(argc, argv)
char **argv;
{
	register int ro;
	register struct mtab *mp;
	register char *np;
	int n, mf;

	mf = open("/etc/mtab", 0);
	read(mf, mtab, NMOUNT*2*NAMSIZ);
	if (argc==1) {
		for (mp = mtab; mp &lt; &amp;mtab[NMOUNT]; mp++)
			if (mp-&gt;file[0])
				printf("%s on %s\n", mp-&gt;spec, mp-&gt;file);
		return;
	}
	if(argc &lt; 3) {
		printf("arg count\n");
		return;
	}
	ro = 0;
	if(argc &gt; 3)
		ro++;
	if(mount(argv[1], argv[2], ro) &lt; 0) {
		perror("mount");
		return;
	}
	np = argv[1];
	while(*np++)
		;
	np--;
	while(*--np == '/')
		*np = '\0';
	while(np &gt; argv[1] &amp;&amp; *--np != '/')
		;
	if(*np == '/')
		np++;
	argv[1] = np;//smount
	for (mp = mtab; mp &lt; &amp;mtab[NMOUNT]; mp++) {
		if (mp-&gt;file[0] == 0) {
			for (np = mp-&gt;spec; np &lt; &amp;mp-&gt;spec[NAMSIZ-1];)
				if ((*np++ = *argv[1]++) == 0)
					argv[1]--;
			for (np = mp-&gt;file; np &lt; &amp;mp-&gt;file[NAMSIZ-1];)
				if ((*np++ = *argv[2]++) == 0)
					argv[2]--;
			mp = &amp;mtab[NMOUNT];
			while ((--mp)-&gt;file[0] == 0);
			mf = creat("/etc/mtab", 0644);
			write(mf, mtab, (mp-mtab+1)*2*NAMSIZ);
			return;
		}
	}
}











