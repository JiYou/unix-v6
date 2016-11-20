<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/tp4.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/tp4.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/tp4.s>
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
/ tap4 -- dec-tape lod/dmp

.data
tc:	&lt;/dev/tap&gt;
tcx:	&lt;x\0&gt;
mt:	&lt;/dev/mt&gt;
mtx:	&lt;0\0&gt;
tboot:	&lt;/usr/mdec/tboot\0&gt;
mboot:	&lt;/usr/mdec/mboot\0&gt;
.even
.bss
dir:	. = .+[mdirent*dirsiz]
tapeb:
map:	.=.+4096.
emap:
ch:	.=.+1
flc:	.=.+1
flf:	.=.+1
fli:	.=.+1
flm:	.=.+1
flu:	.=.+1
flv:	.=.+1
flw:	.=.+1
.even

command:.=.+2
sum:	.=.+2
size:	.=.+2
nentr:	.=.+2
nused:	.=.+2
nfree:	.=.+2
lused:	.=.+2
catlb:	.=.+20.
narg:	.=.+2
rnarg:	.=.+2
parg:	.=.+2
fio:	.=.+2
mss:	.=.+2
ndirent:.=.+2
ndentd8:.=.+2
edir:	.=.+2
rseeka:	.=.+2
wseeka:	.=.+2
tapsiz:	.=.+2
name:	.=.+32.
name1:	.=.+32.
statb:	.=.+40.

smdate = 30.











