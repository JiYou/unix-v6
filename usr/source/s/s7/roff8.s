<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s7/roff8.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s7/roff8.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s7/roff8.s>
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
/
/

/ roff5 -- runoff

slow:	1
pto:	9999.
po:	0
ls:	1
ls1:	1
pn:	1
ma1:	2
ma2:	2
ma3:	1
ma4:	3
ll:	65.
llh:	65.
hx:	1
pl:	66.
ad:	1
fi:	1
cc:	'.
ohc:	200
hyf:	1
hypedf:	0
obufp:	obuf
thresh:	240
tabc:	' 
tabtab:
	.byte 8.,16.,24.,32.,40.,48.,56.,64.,72.,0
suffil:
	&lt;/usr/lib/suftab\0&gt;
ones:
	&lt;ixcm&gt;
fives:
	&lt;vld&gt;
ttyx:	&lt;/dev/tty0\0&gt;
bfn:	&lt;/tmp/rtma\0&gt;
	.even
ofile:	-1
nextb:	4
ilistp:	ilist

.bss
old:	.=.+2
stop:	.=.+2
garb:	.=.+2
bname:
nextf: .=.+20.
nx:	.=.+2
ibf:	.=.+2
ibf1:	.=.+2
skp:	.=.+2
ip:	.=.+2
ilist:	.=.+8.
iliste:	.=.+2
column:	.=.+2
ocol:	.=.+2
nspace:	.=.+2
fac:	.=.+2
fmq:	.=.+2
nhl:	.=.+2
nel:	.=.+2
jfomod:	.=.+2
wordp:	.=.+2
nlflg:	.=.+2
/tottext: .=.+2
/totcon: .=.+2
spaceflg: .=.+2
ch:	.=.+2
linep:	.=.+2
undflg:	.=.+2
wordend:	.=.+2
maxdig:	.=.+2
maxloc:	.=.+2
totout:	.=.+2
hstart:	.=.+2
nhstart: .=.+2
nhyph:	.=.+2
argc:	.=.+2
argp:	.=.+2
ibufp:	.=.+2
eibuf:	.=.+2
wne:	.=.+2
nl:	.=.+2
bl:	.=.+2
nc:	.=.+2
ne:	.=.+2
lnumber:	.=.+2
numbmod:	.=.+2
skip:	.=.+2
nwd:	.=.+2
ulstate:	.=.+2
ulc:	.=.+2
bsc:	.=.+2
nsp:	.=.+2
nn:	.=.+2
ro:	.=.+2
pfrom:	.=.+2
ni:	.=.+2
onesp:	.=.+2
fivesp:	.=.+2
ul:	.=.+2
ce:	.=.+2
in:	.=.+2
un:	.=.+2
wch:	.=.+2
suff:	.=.+2
sufb:	.=.+20.
sufbuf:	.=.+512.
suftab:	.=.+[2*26.]
ifile:	.=.+2
char:	.=.+2
nfile:	.=.+2
ehead:	.=.+2
ohead:	.=.+2
efoot:	.=.+2
ofoot:	.=.+2
trtab:	.=.+128.
word:	.=.+200.

stbuf:
linsiz = 500.
line:	.=.+linsiz
end:












