<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/db4.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/db4.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/db4.s>
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
/ db4 -- debugger

maxsym = 24000.
core:
   &lt;core\0&gt;
a.out:
   &lt;a.out\0&gt;
.even
zero:	0
.bss
regbuf:
	.=.+1024.
sigp	= regbuf+310
txtsiz	= regbuf+322
datsiz	= regbuf+324
stksiz	= regbuf+326
rtxtsiz: .=.+2
.data
objmagic: 407
nobjmagic: 410
namsiz:	nambuf
incdot: 2
nlcom: '/

	.bss

starmod:.=.+2
symbol:	.=.+10.
getoff:	.=.+2
namstrt: .=.+2
bytemod: .=.+2
savsp: .=.+2
error: .=.+2
ttyfin: .=.+2
dbfin: .=.+2
symfin:	.=.+2
curfin:	.=.+2
dbfout: .=.+2
ch: .=.+2
lastop: .=.+2
addres: .=.+2
taddr: .=.+2
adrflg: .=.+2
fpsr:	.=.+2
och:	.=.+2
dot: .=.+2
count: .=.+2
syscnt: .=.+2
temp: .=.+2
temp1: .=.+2
obuf: .=.+8.
inbuf: .=.+128.
nambuf:	.=.+20












