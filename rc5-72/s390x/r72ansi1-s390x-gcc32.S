/* Copyright distributed.net 1997-2011 - All Rights Reserved
 * For use in distributed.net projects only.
 * Any other distribution or use of this source violates copyright.
 *
 * This assembler code is generated using gcc-3.2 from sles8.
 * This compiler produced the best optimized code for z900 machines.
 *
 * Based on r72ansi1.cpp 1.19 2007/10/22 16:48:35 jlawson
*/

.globl rc5_72_unit_func_ansi_1_s390x_gcc32
rc5_72_unit_func_ansi_1_s390x_gcc32:
	stmg	%r6,%r15,48(%r15)
	larl	%r13,.LT1
	.section	.rodata
	.align	8
.LT1:
.LC1:
	.long	-1089828067
.LC2:
	.long	354637369
.LC3:
	.long	-354637369
.LC4:
	.long	31
.LC5:
	.long	-196066091
.LC6:
	.long	-1836597618
.LC7:
	.long	817838151
.LC8:
	.long	-822693376
.LC9:
	.long	1831742393
.LC10:
	.long	191210866
.LC11:
	.long	-1449320661
.LC12:
	.long	1205115108
.LC13:
	.long	-435416419
.LC14:
	.long	-2075947946
.LC15:
	.long	578487823
.LC16:
	.long	-1062043704
.LC17:
	.long	1592392065
.LC18:
	.long	-48139462
.LC19:
	.long	-1688670989
.LC20:
	.long	965764780
.LC21:
	.long	-674766747
.LC22:
	.long	1979669022
.LC23:
	.long	339137495
.LC24:
	.long	-1301394032
.LC25:
	.long	1353041737
.LC26:
	.long	-287489790
.LC27:
	.long	-1928021317
.LC28:
	.long	726414452
.LC29:
	.long	255
.text
	lgr	%r14,%r15
	aghi	%r15,-472
	l	%r0,0(%r3)
	ahi	%r0,-1
	chi	%r0,-1
	stg	%r14,0(%r15)
	stg	%r3,288(%r15)
	stg	%r2,280(%r15)
	st	%r0,296(%r15)
	je	.L18
	lg	%r4,280(%r15)
	l	%r3,4(%r2)
	lg	%r2,280(%r15)
	st	%r3,460(%r15)
	l	%r1,0(%r4)
	st	%r1,456(%r15)
	l	%r0,12(%r2)
	st	%r0,464(%r15)
.L16:
	lg	%r12,280(%r15)
	lg	%r2,280(%r15)
	mvc	264(4,%r15),24(%r12)
	l	%r0,264(%r15)
	st	%r0,300(%r15)
	a	%r0,.LC1-.LT1(%r13)
	rll	%r9,%r0,29
	mvc	268(4,%r15),20(%r12)
	a	%r9,.LC2-.LT1(%r13)
	l	%r10,268(%r15)
	rll	%r5,%r9,3
	a	%r9,.LC3-.LT1(%r13)
	st	%r5,304(%r15)
	lr	%r7,%r10
	ar	%r5,%r9
	ar	%r7,%r5
	n	%r5,.LC4-.LT1(%r13)
	st	%r10,308(%r15)
	l	%r6,304(%r15)
	rll	%r10,%r7,0(%r5)
	ar	%r6,%r10
	a	%r6,.LC5-.LT1(%r13)
	rll	%r8,%r6,3
	st	%r8,312(%r15)
	mvc	272(4,%r15),16(%r2)
	l	%r1,272(%r15)
	l	%r4,312(%r15)
	ar	%r4,%r10
	lr	%r11,%r1
	ar	%r11,%r4
	n	%r4,.LC4-.LT1(%r13)
	rll	%r2,%r11,0(%r4)
	l	%r0,312(%r15)
	ar	%r0,%r2
	a	%r0,.LC6-.LT1(%r13)
	rll	%r11,%r0,3
	lr	%r12,%r11
	ar	%r12,%r2
	ar	%r9,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r0,%r9,0(%r12)
	lr	%r7,%r11
	ar	%r7,%r0
	a	%r7,.LC7-.LT1(%r13)
	rll	%r6,%r7,3
	st	%r6,320(%r15)
	ar	%r6,%r0
	ar	%r10,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r7,%r10,0(%r6)
	l	%r8,320(%r15)
	ar	%r8,%r7
	a	%r8,.LC8-.LT1(%r13)
	rll	%r12,%r8,3
	st	%r12,324(%r15)
	ar	%r12,%r7
	ar	%r2,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r8,%r2,0(%r12)
	l	%r9,324(%r15)
	ar	%r9,%r8
	a	%r9,.LC9-.LT1(%r13)
	rll	%r6,%r9,3
	st	%r1,316(%r15)
	st	%r6,328(%r15)
	ar	%r6,%r8
	ar	%r0,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r9,%r0,0(%r6)
	l	%r10,328(%r15)
	ar	%r10,%r9
	a	%r10,.LC10-.LT1(%r13)
	rll	%r12,%r10,3
	st	%r12,332(%r15)
	ar	%r12,%r9
	ar	%r7,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r10,%r7,0(%r12)
	l	%r2,332(%r15)
	ar	%r2,%r10
	a	%r2,.LC11-.LT1(%r13)
	rll	%r6,%r2,3
	st	%r6,336(%r15)
	ar	%r6,%r10
	ar	%r8,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r2,%r8,0(%r6)
	l	%r0,336(%r15)
	ar	%r0,%r2
	a	%r0,.LC12-.LT1(%r13)
	rll	%r12,%r0,3
	st	%r12,340(%r15)
	ar	%r12,%r2
	ar	%r9,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r0,%r9,0(%r12)
	l	%r7,340(%r15)
	ar	%r7,%r0
	a	%r7,.LC13-.LT1(%r13)
	rll	%r6,%r7,3
	st	%r6,344(%r15)
	ar	%r6,%r0
	ar	%r10,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r7,%r10,0(%r6)
	l	%r8,344(%r15)
	ar	%r8,%r7
	a	%r8,.LC14-.LT1(%r13)
	rll	%r12,%r8,3
	st	%r12,348(%r15)
	ar	%r12,%r7
	ar	%r2,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r8,%r2,0(%r12)
	l	%r9,348(%r15)
	ar	%r9,%r8
	a	%r9,.LC15-.LT1(%r13)
	rll	%r6,%r9,3
	st	%r6,352(%r15)
	ar	%r6,%r8
	ar	%r0,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r9,%r0,0(%r6)
	l	%r10,352(%r15)
	ar	%r10,%r9
	a	%r10,.LC16-.LT1(%r13)
	rll	%r12,%r10,3
	st	%r12,356(%r15)
	ar	%r12,%r9
	ar	%r7,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r10,%r7,0(%r12)
	l	%r2,356(%r15)
	ar	%r2,%r10
	a	%r2,.LC17-.LT1(%r13)
	rll	%r6,%r2,3
	st	%r6,360(%r15)
	ar	%r6,%r10
	ar	%r8,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r2,%r8,0(%r6)
	l	%r0,360(%r15)
	ar	%r0,%r2
	a	%r0,.LC18-.LT1(%r13)
	rll	%r12,%r0,3
	st	%r12,364(%r15)
	ar	%r12,%r2
	ar	%r9,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r0,%r9,0(%r12)
	l	%r7,364(%r15)
	ar	%r7,%r0
	a	%r7,.LC19-.LT1(%r13)
	rll	%r6,%r7,3
	st	%r6,368(%r15)
	ar	%r6,%r0
	ar	%r10,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r7,%r10,0(%r6)
	l	%r8,368(%r15)
	ar	%r8,%r7
	a	%r8,.LC20-.LT1(%r13)
	rll	%r12,%r8,3
	st	%r12,372(%r15)
	ar	%r12,%r7
	ar	%r2,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r8,%r2,0(%r12)
	l	%r9,372(%r15)
	ar	%r9,%r8
	a	%r9,.LC21-.LT1(%r13)
	rll	%r6,%r9,3
	st	%r6,376(%r15)
	ar	%r6,%r8
	ar	%r0,%r6
	n	%r6,.LC4-.LT1(%r13)
	rll	%r9,%r0,0(%r6)
	l	%r10,376(%r15)
	ar	%r10,%r9
	a	%r10,.LC22-.LT1(%r13)
	rll	%r12,%r10,3
	st	%r12,380(%r15)
	ar	%r12,%r9
	ar	%r7,%r12
	n	%r12,.LC4-.LT1(%r13)
	rll	%r10,%r7,0(%r12)
	l	%r1,380(%r15)
	ar	%r1,%r10
	a	%r1,.LC23-.LT1(%r13)
	rll	%r7,%r1,3
	lr	%r2,%r7
	ar	%r2,%r10
	ar	%r8,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r12,%r8,0(%r2)
	lr	%r6,%r7
	ar	%r6,%r12
	a	%r6,.LC24-.LT1(%r13)
	rll	%r8,%r6,3
	lr	%r2,%r8
	ar	%r2,%r12
	ar	%r9,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r6,%r9,0(%r2)
	lr	%r0,%r8
	ar	%r0,%r6
	a	%r0,.LC25-.LT1(%r13)
	rll	%r9,%r0,3
	lr	%r2,%r9
	ar	%r2,%r6
	ar	%r10,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r0,%r10,0(%r2)
	lr	%r2,%r9
	ar	%r2,%r0
	a	%r2,.LC26-.LT1(%r13)
	rll	%r10,%r2,3
	lr	%r1,%r10
	ar	%r1,%r0
	ar	%r12,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r2,%r12,0(%r1)
	lr	%r1,%r10
	ar	%r1,%r2
	a	%r1,.LC27-.LT1(%r13)
	rll	%r12,%r1,3
	lr	%r1,%r12
	ar	%r1,%r2
	ar	%r6,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r6,0(%r1)
	lr	%r1,%r12
	ar	%r1,%r3
	a	%r1,.LC28-.LT1(%r13)
	rll	%r6,%r1,3
	lr	%r1,%r6
	ar	%r1,%r3
	ar	%r0,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r0,0(%r1)
	lr	%r1,%r6
	ar	%r1,%r5
	a	%r1,.LC1-.LT1(%r13)
	rll	%r0,%r1,3
	lr	%r1,%r0
	ar	%r1,%r5
	ar	%r2,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r2,0(%r1)
	lr	%r2,%r0
	ar	%r2,%r4
	a	%r2,304(%r15)
	rll	%r1,%r2,3
	st	%r1,384(%r15)
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	l	%r1,384(%r15)
	ar	%r1,%r3
	a	%r1,312(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	st	%r2,388(%r15)
	ar	%r2,%r5
	ar	%r11,%r2
	rll	%r11,%r11,3
	lr	%r2,%r11
	ar	%r2,%r5
	ar	%r4,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r2)
	lr	%r2,%r11
	ar	%r2,%r4
	a	%r2,320(%r15)
	rll	%r1,%r2,3
	st	%r1,392(%r15)
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	l	%r1,392(%r15)
	ar	%r1,%r3
	a	%r1,324(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	st	%r2,396(%r15)
	ar	%r2,%r5
	a	%r2,328(%r15)
	rll	%r1,%r2,3
	st	%r1,400(%r15)
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	l	%r1,400(%r15)
	ar	%r1,%r4
	a	%r1,332(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	st	%r2,404(%r15)
	ar	%r2,%r3
	a	%r2,336(%r15)
	rll	%r1,%r2,3
	st	%r1,408(%r15)
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	l	%r1,408(%r15)
	ar	%r1,%r5
	a	%r1,340(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	st	%r2,412(%r15)
	ar	%r2,%r4
	a	%r2,344(%r15)
	rll	%r1,%r2,3
	st	%r1,416(%r15)
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	l	%r1,416(%r15)
	ar	%r1,%r3
	a	%r1,348(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	st	%r2,420(%r15)
	ar	%r2,%r5
	a	%r2,352(%r15)
	rll	%r1,%r2,3
	st	%r1,424(%r15)
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	l	%r1,424(%r15)
	ar	%r1,%r4
	a	%r1,356(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	st	%r2,428(%r15)
	ar	%r2,%r3
	a	%r2,360(%r15)
	rll	%r1,%r2,3
	st	%r1,432(%r15)
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	l	%r1,432(%r15)
	ar	%r1,%r5
	a	%r1,364(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	st	%r2,436(%r15)
	ar	%r2,%r4
	a	%r2,368(%r15)
	rll	%r1,%r2,3
	st	%r1,440(%r15)
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	l	%r1,440(%r15)
	ar	%r1,%r3
	a	%r1,372(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	st	%r2,444(%r15)
	ar	%r2,%r5
	a	%r2,376(%r15)
	rll	%r1,%r2,3
	st	%r1,448(%r15)
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	l	%r1,448(%r15)
	ar	%r1,%r4
	a	%r1,380(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	st	%r2,452(%r15)
	ar	%r2,%r3
	ar	%r7,%r2
	rll	%r7,%r7,3
	lr	%r2,%r7
	ar	%r2,%r3
	ar	%r5,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r2)
	lr	%r1,%r7
	ar	%r1,%r5
	ar	%r8,%r1
	rll	%r8,%r8,3
	lr	%r2,%r8
	ar	%r2,%r5
	ar	%r4,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r2)
	lr	%r1,%r8
	ar	%r1,%r4
	ar	%r9,%r1
	rll	%r9,%r9,3
	lr	%r2,%r9
	ar	%r2,%r4
	ar	%r3,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r2)
	lr	%r1,%r9
	ar	%r1,%r3
	ar	%r10,%r1
	rll	%r10,%r10,3
	lr	%r2,%r10
	ar	%r2,%r3
	ar	%r5,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r2)
	lr	%r1,%r10
	ar	%r1,%r5
	ar	%r12,%r1
	rll	%r12,%r12,3
	lr	%r2,%r12
	ar	%r2,%r5
	ar	%r4,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r2)
	lr	%r1,%r12
	ar	%r1,%r4
	ar	%r6,%r1
	rll	%r6,%r6,3
	lr	%r2,%r6
	ar	%r2,%r4
	ar	%r3,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r2)
	lr	%r1,%r6
	ar	%r1,%r3
	ar	%r0,%r1
	rll	%r0,%r0,3
	lr	%r2,%r0
	ar	%r2,%r3
	ar	%r5,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r2)
	lr	%r2,%r0
	ar	%r2,%r5
	a	%r2,384(%r15)
	rll	%r1,%r2,3
	st	%r1,164(%r15)
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	l	%r1,164(%r15)
	ar	%r1,%r4
	a	%r1,388(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	st	%r2,168(%r15)
	ar	%r2,%r3
	ar	%r11,%r2
	rll	%r2,%r11,3
	lr	%r1,%r2
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	lr	%r1,%r2
	ar	%r1,%r5
	a	%r1,392(%r15)
	rll	%r11,%r1,3
	st	%r11,176(%r15)
	ar	%r11,%r5
	ar	%r4,%r11
	n	%r11,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r11)
	l	%r1,176(%r15)
	ar	%r1,%r4
	a	%r1,396(%r15)
	st	%r2,172(%r15)
	rll	%r2,%r1,3
	lr	%r11,%r2
	ar	%r11,%r4
	ar	%r3,%r11
	n	%r11,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r11)
	st	%r2,180(%r15)
	ar	%r2,%r3
	a	%r2,400(%r15)
	rll	%r11,%r2,3
	st	%r11,184(%r15)
	ar	%r11,%r3
	ar	%r5,%r11
	n	%r11,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r11)
	l	%r11,184(%r15)
	ar	%r11,%r5
	a	%r11,404(%r15)
	rll	%r2,%r11,3
	lr	%r1,%r2
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r11,%r4,0(%r1)
	st	%r2,188(%r15)
	ar	%r2,%r11
	a	%r2,408(%r15)
	rll	%r1,%r2,3
	st	%r1,192(%r15)
	ar	%r1,%r11
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	l	%r1,192(%r15)
	ar	%r1,%r3
	a	%r1,412(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	st	%r2,196(%r15)
	ar	%r2,%r5
	a	%r2,416(%r15)
	rll	%r1,%r2,3
	st	%r1,200(%r15)
	ar	%r1,%r5
	ar	%r11,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r11,0(%r1)
	l	%r11,200(%r15)
	ar	%r11,%r4
	a	%r11,420(%r15)
	rll	%r2,%r11,3
	lr	%r1,%r2
	ar	%r1,%r4
	st	%r2,204(%r15)
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r11,%r3,0(%r1)
	ar	%r2,%r11
	a	%r2,424(%r15)
	rll	%r1,%r2,3
	st	%r1,208(%r15)
	ar	%r1,%r11
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r1)
	l	%r1,208(%r15)
	ar	%r1,%r5
	a	%r1,428(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	st	%r2,212(%r15)
	ar	%r2,%r4
	a	%r2,432(%r15)
	rll	%r1,%r2,3
	st	%r1,216(%r15)
	ar	%r1,%r4
	ar	%r11,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r11,0(%r1)
	l	%r11,216(%r15)
	ar	%r11,%r3
	a	%r11,436(%r15)
	rll	%r2,%r11,3
	lr	%r1,%r2
	ar	%r1,%r3
	ar	%r5,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r11,%r5,0(%r1)
	st	%r2,220(%r15)
	ar	%r2,%r11
	a	%r2,440(%r15)
	rll	%r1,%r2,3
	st	%r1,224(%r15)
	ar	%r1,%r11
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	l	%r1,224(%r15)
	ar	%r1,%r4
	a	%r1,444(%r15)
	rll	%r2,%r1,3
	lr	%r1,%r2
	ar	%r1,%r4
	ar	%r3,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r3,%r3,0(%r1)
	st	%r2,228(%r15)
	ar	%r2,%r3
	a	%r2,448(%r15)
	rll	%r1,%r2,3
	st	%r1,232(%r15)
	ar	%r1,%r3
	ar	%r11,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r5,%r11,0(%r1)
	l	%r1,232(%r15)
	ar	%r1,%r5
	a	%r1,452(%r15)
	rll	%r11,%r1,3
	lr	%r1,%r11
	ar	%r1,%r5
	ar	%r4,%r1
	n	%r1,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r1)
	st	%r11,236(%r15)
	ar	%r11,%r4
	ar	%r7,%r11
	rll	%r7,%r7,3
	lr	%r2,%r7
	ar	%r2,%r4
	ar	%r3,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r11,%r3,0(%r2)
	lr	%r1,%r7
	ar	%r1,%r11
	ar	%r8,%r1
	rll	%r8,%r8,3
	lr	%r2,%r8
	ar	%r2,%r11
	ar	%r5,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r5,%r5,0(%r2)
	lr	%r1,%r8
	ar	%r1,%r5
	ar	%r9,%r1
	rll	%r9,%r9,3
	lr	%r2,%r9
	ar	%r2,%r5
	ar	%r4,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r4,%r4,0(%r2)
	lr	%r1,%r9
	ar	%r1,%r4
	ar	%r10,%r1
	rll	%r10,%r10,3
	lr	%r2,%r10
	ar	%r2,%r4
	ar	%r11,%r2
	n	%r2,.LC4-.LT1(%r13)
	rll	%r3,%r11,0(%r2)
	lr	%r1,%r10
	ar	%r1,%r3
	ar	%r12,%r1
	rll	%r12,%r12,3
	l	%r11,456(%r15)
	a	%r11,164(%r15)
	lhi	%r2,31
	st	%r0,160(%r15)
	lr	%r1,%r12
	a	%r0,460(%r15)
	nr	%r2,%r11
	xr	%r0,%r11
	ar	%r1,%r3
	ar	%r5,%r1
	rll	%r0,%r0,0(%r2)
	n	%r1,.LC4-.LT1(%r13)
	a	%r0,168(%r15)
	rll	%r5,%r5,0(%r1)
	lr	%r2,%r12
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	ar	%r2,%r5
	ar	%r6,%r2
	rll	%r11,%r11,0(%r1)
	a	%r11,172(%r15)
	rll	%r6,%r6,3
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	lr	%r2,%r6
	ar	%r2,%r5
	rll	%r0,%r0,0(%r1)
	ar	%r4,%r2
	lhi	%r1,31
	n	%r2,.LC4-.LT1(%r13)
	a	%r0,176(%r15)
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,180(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	a	%r0,184(%r15)
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,188(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	a	%r0,192(%r15)
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,196(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	a	%r0,200(%r15)
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,204(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	a	%r0,208(%r15)
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,212(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	a	%r0,216(%r15)
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,220(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	a	%r0,224(%r15)
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,228(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	a	%r0,232(%r15)
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	a	%r11,236(%r15)
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	ar	%r0,%r7
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	ar	%r11,%r8
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	ar	%r0,%r9
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	ar	%r11,%r10
	lhi	%r1,31
	nr	%r1,%r11
	xr	%r0,%r11
	rll	%r0,%r0,0(%r1)
	ar	%r0,%r12
	lhi	%r1,31
	nr	%r1,%r0
	xr	%r11,%r0
	rll	%r11,%r11,0(%r1)
	ar	%r11,%r6
	rll	%r1,%r4,0(%r2)
	c	%r0,464(%r15)
	st	%r7,240(%r15)
	st	%r8,244(%r15)
	st	%r9,248(%r15)
	st	%r10,252(%r15)
	st	%r3,264(%r15)
	st	%r1,272(%r15)
	st	%r12,256(%r15)
	st	%r5,268(%r15)
	st	%r6,260(%r15)
	je	.L19
.L6:
	lg	%r7,280(%r15)
	l	%r5,16(%r7)
	ahi	%r5,1
	n	%r5,.LC29-.LT1(%r13)
	ltr	%r5,%r5
	st	%r5,16(%r7)
	jne	.L3
	lg	%r3,280(%r15)
	llilh	%r10,256
	l	%r2,20(%r3)
	ar	%r10,%r2
	tmh	%r10,65280
	st	%r10,20(%r3)
	jne	.L3
	llilh	%r12,257
	ar	%r12,%r2
	lr	%r2,%r12
	nilh	%r2,255
	tmh	%r12,255
	st	%r2,20(%r3)
	jne	.L3
	lr	%r0,%r2
	ahi	%r0,256
	lr	%r2,%r0
	nilh	%r2,0
	tml	%r0,65280
	st	%r2,20(%r3)
	jne	.L3
	lr	%r9,%r2
	ahi	%r9,1
	n	%r9,.LC29-.LT1(%r13)
	ltr	%r9,%r9
	st	%r9,20(%r3)
	jne	.L3
	l	%r2,24(%r3)
	llilh	%r11,256
	ar	%r11,%r2
	tmh	%r11,65280
	st	%r11,24(%r3)
	jne	.L3
	llilh	%r4,257
	ar	%r4,%r2
	lr	%r2,%r4
	nilh	%r2,255
	tmh	%r4,255
	st	%r2,24(%r3)
	jne	.L3
	lr	%r8,%r2
	ahi	%r8,256
	lr	%r2,%r8
	nilh	%r2,0
	tml	%r8,65280
	st	%r2,24(%r3)
	jne	.L3
	ahi	%r2,1
	n	%r2,.LC29-.LT1(%r13)
	st	%r2,24(%r3)
.L3:
	l	%r3,296(%r15)
	ahi	%r3,-1
	chi	%r3,-1
	st	%r3,296(%r15)
	jne	.L16
.L18:
	lghi	%r2,1
.L2:
	lg	%r1,584(%r15)
	lmg	%r6,%r15,520(%r15)
	br	%r1
.L19:
	lg	%r9,280(%r15)
	l	%r2,28(%r9)
	ahi	%r2,1
	st	%r2,28(%r9)
	l	%r4,316(%r15)
	st	%r4,32(%r9)
	c	%r11,8(%r9)
	l	%r11,308(%r15)
	st	%r11,36(%r9)
	l	%r3,300(%r15)
	st	%r3,40(%r9)
	jne	.L6
	lg	%r1,288(%r15)
	lg	%r8,288(%r15)
	lghi	%r2,2
	l	%r6,0(%r1)
	s	%r6,296(%r15)
	ahi	%r6,-1
	st	%r6,0(%r8)
	j	.L2

