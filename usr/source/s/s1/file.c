int in;
int i 0;
char buf[512];
int *wd {
	&amp;buf[0]};
char *fort[]{
	"function","subroutine","common","dimension","block","integer",
	"real","data","double",0};
char *asc[]{
	"sys","mov","tst","clr","jmp",0};
char *c[]{
	"int","char","float","double","struct","extern",0};
char *as[]{
	"globl","byte","even","text","data","bss","comm",0};
int ibuf[260];
main(argc, argv)
char **argv;
{

	while(argc &gt; 1) {
		printf("%s:	", argv[1]);
		type(argv[1]);
		argc--;
		argv++;
	}
}

type(file)
char *file;
{
	int j,nl;
	char ch;
	int mbuf[20];

	if(stat(file, mbuf) &lt; 0) {
		printf("cannot stat\n");
		return;
	}
	switch(mbuf[2]&amp;060000) {

	case 020000:
		printf("character");
		goto spcl;

	case 040000:
		printf("directory\n");
		return;

	case 060000:
		printf("block");

spcl:
		printf(" special (%d/%d)\n",
		(mbuf[6]&gt;&gt;8)&amp;0377,
		mbuf[6]&amp;0377);
		return;
	}

	ibuf[0] = open(file, 0);
	if(ibuf[0] &lt; 0) {
		printf("cannot open\n");
		return;
	}
	in = read(ibuf[0], buf, 512);
	switch(*wd) {

	case 0407:
		printf("executable\n");
		goto out;

	case 0410:
		printf("pure executable\n");
		goto out;

	case 0411:
		printf("separate executable\n");
		goto out;

	case 0177555:
		printf("archive\n");
		goto out;
	}

	i = 0;
	if(ccom() == 0)goto notc;
	while(buf[i] == '#'){
		j = i;
		while(buf[i++] != '\n'){
			if(i - j &gt; 255){
				printf("data\n"); 
				goto out;
			}
			if(i &gt;= in)goto notc;
		}
		if(ccom() == 0)goto notc;
	}
check:
	if(lookup(c) == 1){
		while((ch = buf[i++]) != ';' &amp;&amp; ch != '{')if(i &gt;= in)goto notc;
		printf("c program");
		goto outa;
	}
	nl = 0;
	while(buf[i] != '('){
		if(buf[i] &lt;= 0){
			printf("data\n"); 
			goto out; 
		}
		if(buf[i] == ';'){
			i++; 
			goto check; 
		}
		if(buf[i++] == '\n')
			if(nl++ &gt; 6)goto notc;
		if(i &gt;= in)goto notc;
	}
	while(buf[i] != ')'){
		if(buf[i++] == '\n')
			if(nl++ &gt; 6)goto notc;
		if(i &gt;= in)goto notc;
	}
	while(buf[i] != '{'){
		if(buf[i++] == '\n')
			if(nl++ &gt; 6)goto notc;
		if(i &gt;= in)goto notc;
	}
	printf("c program");
	goto outa;
notc:
	i = 0;
	while(buf[i] == 'c' || buf[i] == '#'){
		while(buf[i++] != '\n')if(i &gt;= in)goto notfort;
	}
	if(lookup(fort) == 1){
		printf("fortran");
		goto outa;
	}
notfort:
	i=0;
	if(ascom() == 0)goto notas;
	j = i-1;
	if(buf[i] == '.'){
		i++;
		if(lookup(as) == 1){
			printf("assembler program"); 
			goto outa;
		}
		else if(buf[j] == '\n'){
			printf("roff, nroff, or eqn input");
			goto outa;
		}
	}
	while(lookup(asc) == 0){
		if(ascom() == 0)goto notas;
		while(buf[i] != '\n' &amp;&amp; buf[i++] != ':')
			if(i &gt;= in)goto notas;
		while(buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\t')if(i++ &gt;= in)goto notas;
		j = i-1;
		if(buf[i] == '.'){
			i++;
			if(lookup(as) == 1){
				printf("assembler program"); 
				goto outa; 
			}
			else if(buf[j] == '\n'){
				printf("roff, nroff, or eqn input");
				goto outa;
			}
		}
	}
	printf("assembler program");
	goto outa;
notas:
	for(i=0; i &lt; in; i++)if(buf[i] &lt;= 0){
		printf("data\n"); 
		goto out; 
	}
	if((mbuf[2] &amp; 00111) != 0)
		printf("commands");
	else printf("probably text");
outa:
	while(i &lt; in)
		if(buf[i++] &lt;= 0){
			printf(" with garbage\n");
			goto out;
		}
	while((in = read(ibuf[0],buf,512)) &gt; 0)
		for(i = 0; i &lt; in; i++)
			if(buf[i] &lt;= 0){
				printf(" with garbage\n");
				goto out;
			}
	printf("\n");
out:
	close(ibuf[0]);
}
lookup(tab)
char *tab[];
{
	char r;
	int k,j,l;
	while(buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')i++;
	for(j=0; tab[j] != 0; j++){
		l=0;
		for(k=i; ((r=tab[j][l++]) == buf[k] &amp;&amp; r != '\0');k++);
		if(r == '\0')
			if(buf[k] == ' ' || buf[k] == '\n' || buf[k] == '\t'
			    || buf[k] == '{' || buf[k] == '/'){
				i=k;
				return(1);
			}
	}
	return(0);
}
ccom(){
	char cc;
	while((cc = buf[i]) == ' ' || cc == '\t' || cc == '\n')if(i++ &gt;= in)return(0);
	if(buf[i] == '/' &amp;&amp; buf[i+1] == '*'){
		i =+ 2;
		while(buf[i] != '*' || buf[i+1] != '/'){
			if(buf[i] == '\\')i =+ 2;
			else i++;
			if(i &gt;= in)return(0);
		}
		if((i =+ 2) &gt;= in)return(0);
	}
	if(buf[i] == '\n')if(ccom() == 0)return(0);
	return(1);
}
ascom(){
	while(buf[i] == '/'){
		i++;
		while(buf[i++] != '\n')if(i &gt;= in)return(0);
		while(buf[i] == '\n')if(i++ &gt;= in)return(0);
	}
	return(1);
}











