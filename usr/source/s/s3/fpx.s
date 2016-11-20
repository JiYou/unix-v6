<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/fpx.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/fpx.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/fpx.s>
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
/ fpx -- floating point simulation

.data
reenter: 1

.bss
asign:	.=.+2
areg:	.=.+8
aexp:	.=.+2
bsign:	.=.+2
breg:	.=.+8
bexp:	.=.+2

fpsr:	.=.+2
trapins: .=.+2

ac0:	.=.+8.
ac1:	.=.+8.
ac2:	.=.+8.
ac3:	.=.+8.
ac4:	.=.+8.
ac5:	.=.+8.

sr0:	.=.+2
sr1:	.=.+2
	.=.+2
	.=.+2
	.=.+2
	.=.+2
ssp:	.=.+2
spc:	.=.+2
sps:	.=.+2
pctmp:	.=.+8












