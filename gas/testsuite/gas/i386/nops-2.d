#source: nops-2.s
#as: -mtune=generic32
#objdump: -drw
#name: i386 nops 2

.*: +file format .*

Disassembly of section .text:

0+ <nop>:
 +[a-f0-9]+:	0f be f0             	movsbl %al,%esi
 +[a-f0-9]+:	2e 8d 74 26 00       	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+10 <nop15>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d b4 26 00 00 00 00 	lea    (0x)?0\(%esi,%eiz,1\),%esi
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+20 <nop14>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d b6 00 00 00 00    	lea    (0x)?0\(%esi\),%esi
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+30 <nop13>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	2e 8d 74 26 00       	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+40 <nop12>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d 74 26 00          	lea    (0x)?0\(%esi,%eiz,1\),%esi
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+50 <nop11>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d 76 00             	lea    (0x)?0\(%esi\),%esi
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+60 <nop10>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	66 90                	xchg   %ax,%ax
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+70 <nop9>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	90                   	nop
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+80 <nop8>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	2e 8d b4 26 00 00 00 00 	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+90 <nop7>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d b4 26 00 00 00 00 	lea    0x0\(%esi,%eiz,1\),%esi

0+a0 <nop6>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d b6 00 00 00 00    	lea    0x0\(%esi\),%esi

0+b0 <nop5>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	2e 8d 74 26 00       	lea    %cs:(0x)?0\(%esi,%eiz,1\),%esi

0+c0 <nop4>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d 74 26 00          	lea    0x0\(%esi,%eiz,1\),%esi

0+d0 <nop3>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	8d 76 00             	lea    0x0\(%esi\),%esi

0+e0 <nop2>:
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	f8                   	clc
 +[a-f0-9]+:	66 90                	xchg   %ax,%ax
#pass
