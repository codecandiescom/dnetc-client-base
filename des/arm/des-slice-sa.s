
; ========================================================
; StrongARM (16+16KB cache) version of the bitslicing
; DES core, incorporating the sbox and deseval functions.
;
; Optimised for ARM by Chris Berry and Steve Lee,
; based on deseval.c from Matthew Kwan's bitslicing DES key search.
;
; $Log: des-slice-sa.s,v $
; Revision 1.4  1998/06/15 10:15:40  kbracey
; New ARM sboxes from Steve Lee based on the latest Kwan sboxes.
;
; Revision 1.3  1998/06/14 10:40:59  friedbait
; 'Log' keywords added
;
;

        AREA    fastdesarea, CODE, READONLY

        DCB     "@(#)$Id: des-slice-sa.s,v 1.4 1998/06/15 10:15:40 kbracey Exp $", 0

        EXPORT  des_unit_func_strongarm
        EXPORT  convert_key_from_des_to_inc__FPUlT1
        EXPORT  convert_key_from_inc_to_des__FPUlT1

        GBLL    patch
patch   SETL    {FALSE}

 [ patch
startofcode     ; &2c000

        DCD     &b7e15163

        B       des_unit_func__FP11RC5UnitWorkUl

__rt_udiv       B       startofcode - &2c000 + &1c440
__rt_sdiv       B       startofcode - &2c000 + &1c3e0
__rt_stkovf_split_big   B       startofcode - &2c000 + &1c794
 |
        IMPORT  __rt_udiv
        IMPORT  __rt_sdiv
        IMPORT  __rt_stkovf_split_big
 ]

; these functiona do not follow APCS.
; a1-a6 (r4-r9) are the inputs.
; x1-x6 are same registers as a1-a6. Considered available for
;       general use after last access of a1-a6.
; t1-t8 are used as temporaries.

;a1 RN 0
x1 RN 0
;a2 RN 1
x2 RN 1
;a3 RN 2
x3 RN 2
;a4 RN 3
x4 RN 3
a5 RN 4
x5 RN 4
a6 RN 5
x6 RN 5

t1 RN 6
t2 RN 7
t3 RN 8
t4 RN 9
t5 RN 10
t6 RN 11
t7 RN 12
t8 RN 14

        MACRO
        s_start $s, $inp
        LCLS    reglist
        LCLS    reg
        LCLS    reg2
        LCLA    i
s$s
 [ $inp = 0
        STMFD   r13!,{t1-t4,r14}
reglist SETS    "t1,t2,t3,t4,t5,t6"
 |
  [ $inp < 3
   [ $inp = 1
        STMFD   r13!,{t2-t4,r14}
reglist SETS    "t2,t3,t4,t5,t6,t7"
   |
        STMFD   r13!,{t1,t3-t4,r14}
reglist SETS    "t1,t3,t4,t5,t6,t7"
   ]
  |
   [ $inp = 3
        STMFD   r13!,{t1-t2,t4,r14}
reglist SETS    "t1,t2,t4,t5,t6,t7"
   |
        STMFD   r13!,{t1-t3,r14}
reglist SETS    "t1,t2,t3,t5,t6,t7"
   ]
  ]
 ]
 [ $s = 1
reg     SETS    reglist:LEFT:2
        LDR     $reg,[t7,#4*31]
reg     SETS    reglist:RIGHT:14
        LDMIA   t7,{$reg}
 |
  [ $s = 8
reg     SETS    reglist:LEFT:14
        LDMIA   t7,{$reg}
reg     SETS    reglist:RIGHT:2
        LDR     $reg,[t7,#4*(-27)]
  |
        LDMIA   t7,{$reglist}
  ]
 ]

reglist SETS    reglist:CC:","
i       SETA    1

        WHILE   i <= 6
reg     SETS    "a":CC:("$i":RIGHT:1)
reg2    SETS    reglist:LEFT:2
reglist SETS    reglist:RIGHT:(:LEN:reglist - 3)
        EOR     $reg,$reg,$reg2
i       SETA    i+1
        WEND

        MEND

        MACRO
        s_end   $s, $got1, $got2, $got3, $got4

        LDR     pc,[r13],#4

        GBLA    got1_$s
        GBLA    got2_$s
        GBLA    got3_$s
        GBLA    got4_$s
got1_$s SETA    $got1
got2_$s SETA    $got2
got3_$s SETA    $got3
got4_$s SETA    $got4

        MEND

        MACRO
        do_s    $s,$i1,$i2,$i3,$i4,$i5,$i6,$o1,$o2,$o3,$o4

 [ $o1 < 32
  [ $s = 1
        ADD     t7,r13,#4*32
  |
   [ $s = 8
        ADD     t7,r13,#4*(32+27)
   |
        ADD     t7,r13,#4*(32+$s*4-5)
   ]
  ]
 |
  [ $s = 1
        ADD     t7,r13,#0
  |
   [ $s = 8
        ADD     t7,r13,#4*27
   |
        ADD     t7,r13,#4*($s*4-5)
   ]
  ]
 ]
        ADD     t1,r13,#4*($o1)
        ADD     t2,r13,#4*($o2)
        ADD     t3,r13,#4*($o3)
        LDR     t8,[r13,#4*(64+1)]
        ADD     t4,r13,#4*($o4)
        LDR     a1,[t8,#4*$i1]
        LDR     a2,[t8,#4*$i2]
        LDR     a3,[t8,#4*$i3]
        LDR     a4,[t8,#4*$i4]
        LDR     a5,[t8,#4*$i5]
        LDR     a6,[t8,#4*$i6]
        BL      s$s

        MEND

        MACRO
        check   $s, $pass, $cyp, $res
        LCLS    reg
 [ $pass = 5
reg     SETS    "4"
 |
reg     SETS    "$pass"
 ]

 [ $pass = 1
        LDR     a2,[r13,#4*64]
 ]
 [ got$reg._$s = 0
        LDR     t$reg,[r13,#4*($res)]
 ]
        LDR     a4,[a2,#4*($cyp)]
 [ $pass = 1
        LDR     a1,[r13,#4*(64+2)]
 |
        BIC     a1,a1,a3
 ]
        EOR     a3,a4,t$reg
 [ $pass > 3
        BICS    a1,a1,a3
  [ $pass = 4
        BEQ     gotresult
        STR     a1,[r13,#4*(64+2)]
  ]
 ]
        MEND

        GET      kwan-sboxes-arm.h

convert_key_from_des_to_inc__FPUlT1
        STMDB    r13!,{r4,lr}
        LDR      r2,[r0,#0]
        AND      r3,r2,#&fe
        MOV      r3,r3,LSR #1
        AND      r12,r2,#&fe00
        ORR      r3,r3,r12,LSR #2
        AND      r12,r2,#&fe0000
        ORR      r3,r3,r12,LSR #3
        AND      r2,r2,#&fe000000
        ORR      r2,r3,r2,LSR #4
        STR      r2,[r0,#0]
        LDR      r2,[r1,#0]
        AND      r3,r2,#&fe
        MOV      r12,r3,LSR #1
        AND      r3,r2,#&fe00
        ORR      r12,r12,r3,LSR #2
        AND      r3,r2,#&fe0000
        ORR      r3,r12,r3,LSR #3
        AND      r2,r2,#&fe000000
        ORR      r2,r3,r2,LSR #4
        STR      r2,[r1,#0]
        AND      r3,r2,#8
        MOV      r3,r3,LSR #3
        AND      r12,r2,#&20
        ORR      r12,r3,r12,LSR #4
        AND      r3,r2,#&100
        ORR      r3,r12,r3,LSR #6
        AND      r12,r2,#&1c00
        ORR      r12,r3,r12,LSR #7
        AND      r3,r2,#&8000
        ORR      r12,r12,r3,LSR #9
        AND      r3,r2,#&40000
        ORR      lr,r12,r3,LSR #11
        LDR      r3,[r0,#0]
        AND      r12,r3,#&f000
        ORR      r12,lr,r12,LSR #4
        AND      lr,r3,#&60000
        ORR      lr,r12,lr,LSR #5
        AND      r12,r3,#&600000
        ORR      lr,lr,r12,LSR #7
        AND      r12,r2,#7
        ORR      lr,lr,r12,LSL #16
        AND      r12,r2,#&10
        ORR      lr,lr,r12,LSL #15
        AND      r12,r2,#&c0
        ORR      r12,lr,r12,LSL #14
        AND      lr,r2,#&200
        ORR      r12,r12,lr,LSL #13
        AND      lr,r2,#&6000
        ORR      r12,r12,lr,LSL #10
        AND      lr,r2,#&30000
        ORR      lr,r12,lr,LSL #9
        AND      r12,r2,#&80000
        ORR      r12,lr,r12,LSL #8
        MOV      lr,r3,LSL #20
        MOV      lr,lr,LSR #20
        MOV      lr,lr,LSL #8
        AND      r4,r3,#&10000
        ORR      lr,lr,r4,LSL #4
        AND      r4,r3,#&180000
        ORR      lr,lr,r4,LSL #2
        AND      r3,r3,#&f800000
        ORR      r3,lr,r3
        AND      r2,r2,#&ff00000
        ORR      r2,r3,r2,LSR #20
        STR      r2,[r0,#0]
        STR      r12,[r1,#0]
        LDR      r2,[r0,#0]
        AND      r2,r2,#&f
        ORR      r2,r12,r2,LSL #28
        STR      r2,[r1,#0]
        LDR      r1,[r0,#0]
        MOV      r1,r1,LSR #4
        STR      r1,[r0,#0]
        LDMIA    r13!,{r4,pc}^

convert_key_from_inc_to_des__FPUlT1
        STMDB    r13!,{r4,lr}
        LDR      r2,[r1,#0]
        MOV      r2,r2,LSR #28
        LDR      r3,[r0,#0]
        ORR      r2,r2,r3,LSL #4
        STR      r2,[r0,#0]
        LDR      r2,[r1,#0]
        BIC      r2,r2,#&f0000000
        STR      r2,[r1,#0]
        AND      r3,r2,#1
        MOV      r3,r3,LSL #3
        AND      r12,r2,#2
        ORR      r12,r3,r12,LSL #4
        AND      r3,r2,#4
        ORR      r12,r12,r3,LSL #6
        AND      r3,r2,#&38
        ORR      r12,r12,r3,LSL #7
        AND      r3,r2,#&40
        ORR      r12,r12,r3,LSL #9
        AND      r3,r2,#&80
        ORR      r12,r12,r3,LSL #11
        AND      r3,r2,#&70000
        ORR      r3,r12,r3,LSR #16
        AND      r12,r2,#&80000
        ORR      r12,r3,r12,LSR #15
        AND      r3,r2,#&300000
        ORR      r12,r12,r3,LSR #14
        AND      r3,r2,#&400000
        ORR      r12,r12,r3,LSR #13
        AND      r3,r2,#&1800000
        ORR      r3,r12,r3,LSR #10
        AND      r12,r2,#&6000000
        ORR      r12,r3,r12,LSR #9
        AND      r3,r2,#&8000000
        ORR      r12,r12,r3,LSR #8
        LDR      r3,[r0,#0]
        AND      lr,r3,#&ff
        ORR      r12,r12,lr,LSL #20
        MOV      lr,r3,LSL #12
        MOV      lr,lr,LSR #12
        BIC      lr,lr,#&ff
        MOV      r4,lr,LSR #8
        AND      lr,r3,#&100000
        ORR      r4,r4,lr,LSR #4
        AND      lr,r3,#&600000
        ORR      lr,r4,lr,LSR #2
        AND      r3,r3,#&f800000
        ORR      r3,lr,r3
        AND      lr,r2,#&f00
        ORR      lr,r3,lr,LSL #4
        AND      r3,r2,#&3000
        ORR      r3,lr,r3,LSL #5
        AND      r2,r2,#&c000
        ORR      r2,r3,r2,LSL #7
        STR      r2,[r0,#0]
        STR      r12,[r1,#0]
        LDR      r3,[r0,#0]
        AND      r12,r3,#&7f
        ADR      r2,odd_parity
        LDRB     lr,[r2,r12,LSL #1]
        AND      r12,r3,#&3f80
        LDRB     r12,[r2,r12,LSR #6]
        ORR      lr,lr,r12,LSL #8
        AND      r12,r3,#&1fc000
        LDRB     r12,[r2,r12,LSR #13]
        ORR      r12,lr,r12,LSL #16
        AND      r3,r3,#&fe00000
        LDRB     r3,[r2,r3,LSR #20]
        ORR      r3,r12,r3,LSL #24
        STR      r3,[r0,#0]
        LDR      r0,[r1,#0]
        AND      r3,r0,#&7f
        LDRB     r12,[r2,r3,LSL #1]
        AND      r3,r0,#&3f80
        LDRB     r3,[r2,r3,LSR #6]
        ORR      r12,r12,r3,LSL #8
        AND      r3,r0,#&1fc000
        LDRB     r3,[r2,r3,LSR #13]
        ORR      r3,r12,r3,LSL #16
        AND      r0,r0,#&fe00000
        LDRB     r0,[r2,r0,LSR #20]
        ORR      r0,r3,r0,LSL #24
        STR      r0,[r1,#0]
        LDMIA    r13!,{r4,pc}^

odd_parity
        DCB      0x01,0x01,0x02,0x02
        DCB      0x04,0x04,0x07,0x07
        DCB      0x08,0x08,0x0b,0x0b
        DCB      0x0d,0x0d,0x0e,0x0e
        DCB      0x10,0x10,0x13,0x13
        DCB      0x15,0x15,0x16,0x16
        DCB      0x19,0x19,0x1a,0x1a
        DCB      0x1c,0x1c,0x1f,0x1f
        DCB      0x20,0x20,0x23,0x23
        DCB      0x25,0x25,0x26,0x26
        DCB      0x29,0x29,0x2a,0x2a
        DCB      0x2c,0x2c,0x2f,0x2f
        DCB      0x31,0x31,0x32,0x32
        DCB      0x34,0x34,0x37,0x37
        DCB      0x38,0x38,0x3b,0x3b
        DCB      0x3d,0x3d,0x3e,0x3e
        DCB      0x40,0x40,0x43,0x43
        DCB      0x45,0x45,0x46,0x46
        DCB      0x49,0x49,0x4a,0x4a
        DCB      0x4c,0x4c,0x4f,0x4f
        DCB      0x51,0x51,0x52,0x52
        DCB      0x54,0x54,0x57,0x57
        DCB      0x58,0x58,0x5b,0x5b
        DCB      0x5d,0x5d,0x5e,0x5e
        DCB      0x61,0x61,0x62,0x62
        DCB      0x64,0x64,0x67,0x67
        DCB      0x68,0x68,0x6b,0x6b
        DCB      0x6d,0x6d,0x6e,0x6e
        DCB      0x70,0x70,0x73,0x73
        DCB      0x75,0x75,0x76,0x76
        DCB      0x79,0x79,0x7a,0x7a
        DCB      0x7c,0x7c,0x7f,0x7f
        DCB      0x80,0x80,0x83,0x83
        DCB      0x85,0x85,0x86,0x86
        DCB      0x89,0x89,0x8a,0x8a
        DCB      0x8c,0x8c,0x8f,0x8f
        DCB      0x91,0x91,0x92,0x92
        DCB      0x94,0x94,0x97,0x97
        DCB      0x98,0x98,0x9b,0x9b
        DCB      0x9d,0x9d,0x9e,0x9e
        DCB      0xa1,0xa1,0xa2,0xa2
        DCB      0xa4,0xa4,0xa7,0xa7
        DCB      0xa8,0xa8,0xab,0xab
        DCB      0xad,0xad,0xae,0xae
        DCB      0xb0,0xb0,0xb3,0xb3
        DCB      0xb5,0xb5,0xb6,0xb6
        DCB      0xb9,0xb9,0xba,0xba
        DCB      0xbc,0xbc,0xbf,0xbf
        DCB      0xc1,0xc1,0xc2,0xc2
        DCB      0xc4,0xc4,0xc7,0xc7
        DCB      0xc8,0xc8,0xcb,0xcb
        DCB      0xcd,0xcd,0xce,0xce
        DCB      0xd0,0xd0,0xd3,0xd3
        DCB      0xd5,0xd5,0xd6,0xd6
        DCB      0xd9,0xd9,0xda,0xda
        DCB      0xdc,0xdc,0xdf,0xdf
        DCB      0xe0,0xe0,0xe3,0xe3
        DCB      0xe5,0xe5,0xe6,0xe6
        DCB      0xe9,0xe9,0xea,0xea
        DCB      0xec,0xec,0xef,0xef
        DCB      0xf1,0xf1,0xf2,0xf2
        DCB      0xf4,0xf4,0xf7,0xf7
        DCB      0xf8,0xf8,0xfb,0xfb
        DCB      0xfd,0xfd,0xfe,0xfe

twiddles
        DCB      0x0c,0x0f,0x12,0x28
        DCB      0x29,0x2a,0x2b,0x2d
        DCB      0x2e,0x31,0x32,0x00
        DCB      0x01,0x02,0x04,0x06
        DCB      0x07,0x09,0x0d,0x00

L000380
lowbits
        DCD      0xaaaaaaaa
L000384
        DCD      0xcccccccc
L000388
        DCD      0xf0f0f0f0

des_unit_func_strongarm
|L000000.J2|
        MOV      r12,r13
        STMDB    r13!,{r0,r1,r4-r9,r11,r12,lr,pc}
        SUB      r11,r12,#4
        SUB      r12,r13,#&2ec
        CMP      r12,r10
        BLMI     __rt_stkovf_split_big
        MOV      r4,r0
        SUB      r13,r13,#&2e8
        LDR      r0,[r0,#&10]
        STR      r0,[r13,#4]
        LDR      r0,[r4,#&14]
        STR      r0,[r13,#0]
        MOV      r1,r13
        ADD      r0,r13,#4
        BL       convert_key_from_inc_to_des__FPUlT1
        MOV      r5,#1
        MOV      r6,#0
        LDMIA    r13,{r7,r8}
|L000048.J5|
        MOV      r1,r6
        MOV      r0,#7
        BL       __rt_udiv
        CMP      r1,#0
        MOVEQ    r5,r5,LSL #1
        ANDS     r1,r7,r5
        MVNNE    r1,#0
        ADD      r0,r13,#&208
        STR      r1,[r0,r6,LSL #2]
        MOVS     r5,r5,LSL #1
        MOVEQ    r7,r8
        MOVEQ    r5,#1
        ADD      r6,r6,#1
        CMP      r6,#&38
        BCC      |L000048.J5|
        MOV      r1,#1
        MOV      r0,#0
        LDR      r2,[r4,#&c]
        LDR      r3,[r4,#4]
|L000094.J14|
        ANDS     lr,r3,r1
        MVNNE    lr,#0
        ADD      r12,r13,#&108
        STR      lr,[r12,r0,LSL #2]
        ANDS     lr,r2,r1
        MVNNE    lr,#0
        ADD      r12,r13,#8
        STR      lr,[r12,r0,LSL #2]
        MOVS     r1,r1,LSL #1
        MOVEQ    r1,#1
        LDREQ    r2,[r4,#8]
        LDREQ    r3,[r4,#0]
        ADD      r0,r0,#1
        CMP      r0,#&40
        BCC      |L000094.J14|
        LDR      r0,[pc, #L000380-.-8]
        STR      r0,[r13,#&214]
        LDR      r0,[pc, #L000384-.-8]
        STR      r0,[r13,#&21c]
        LDR      r0,[pc, #L000388-.-8]
        STR      r0,[r13,#&228]
        MOV      r0,#&ff00
        ADD      r0,r0,r0,ROR #16
        STR      r0,[r13,#&230]
        MOV      r0,#&ff0000
        ADD      r0,r0,#&ff000000
        STR      r0,[r13,#&234]
        LDR      r0,[r11,#-&28]
        SUB      r5,r0,#5

        MOV      r8,#0
        MOV      r7,#0
        MOV      r6,#0
        MOV      r0,#1
        MOV      r9,r0,LSL r5

resettwiddles
        MOV      r0,#0
        MOV      r12,#0
        ADR      r3,twiddles
resettwiddleloop
        LDRB     r1,[r3,r0]
        ADD      r2,r13,#&208
        STR      r12,[r2,r1,LSL #2]
        ADD      r0,r0,#1
        CMP      r0,r5
        BCC      resettwiddleloop
        B        deseval

timingloop
        ADD      r2,r13,#&208
        LDR      r0,[r2,r1,LSL #2]
        MVN      r0,r0
        STR      r0,[r2,r1,LSL #2]
deseval
        ADD      r1,r13,#8
        ADD      r0,r13,#&108

        STMFD   r13!,{r4-r11}
        MVN     r3,#0
        STMFD   r13!,{r1-r3}

        LDR     r11,    [r0,#4*57]
        LDR     r10,    [r0,#4*49]
        LDR     r9,     [r0,#4*41]
        LDR     r8,     [r0,#4*33]
        LDR     r7,     [r0,#4*25]
        LDR     r6,     [r0,#4*17]
        LDR     r5,     [r0,#4*9]
        LDR     r4,     [r0,#4*1]
        LDR     r12,    [r0,#4*59]
        STMFD   r13!,{r4-r11}
        LDR     r10,    [r0,#4*51]
        LDR     r9,     [r0,#4*43]
        LDR     r8,     [r0,#4*35]
        LDR     r7,     [r0,#4*27]
        LDR     r6,     [r0,#4*19]
        LDR     r5,     [r0,#4*11]
        LDR     r4,     [r0,#4*3]
        LDR     r11,    [r0,#4*61]
        STMFD   r13!,{r4-r10,r12}
        LDR     r10,    [r0,#4*53]
        LDR     r9,     [r0,#4*45]
        LDR     r8,     [r0,#4*37]
        LDR     r7,     [r0,#4*29]
        LDR     r6,     [r0,#4*21]
        LDR     r5,     [r0,#4*13]
        LDR     r4,     [r0,#4*5]
        LDR     r12,    [r0,#4*63]
        STMFD   r13!,{r4-r11}
        LDR     r10,    [r0,#4*55]
        LDR     r9,     [r0,#4*47]
        LDR     r8,     [r0,#4*39]
        LDR     r7,     [r0,#4*31]
        LDR     r6,     [r0,#4*23]
        LDR     r5,     [r0,#4*15]
        LDR     r4,     [r0,#4*7]
        LDR     r11,    [r0,#4*56]
        STMFD   r13!,{r4-r10,r12}
        LDR     r10,    [r0,#4*48]
        LDR     r9,     [r0,#4*40]
        LDR     r8,     [r0,#4*32]
        LDR     r7,     [r0,#4*24]
        LDR     r6,     [r0,#4*16]
        LDR     r5,     [r0,#4*8]
        LDR     r4,     [r0,#4*0]
        LDR     r12,    [r0,#4*58]
        STMFD   r13!,{r4-r11}
        LDR     r10,    [r0,#4*50]
        LDR     r9,     [r0,#4*42]
        LDR     r8,     [r0,#4*34]
        LDR     r7,     [r0,#4*26]
        LDR     r6,     [r0,#4*18]
        LDR     r5,     [r0,#4*10]
        LDR     r4,     [r0,#4*2]
        LDR     r11,    [r0,#4*60]
        STMFD   r13!,{r4-r10,r12}
        LDR     r10,    [r0,#4*52]
        LDR     r9,     [r0,#4*44]
        LDR     r8,     [r0,#4*36]
        LDR     r7,     [r0,#4*28]
        LDR     r6,     [r0,#4*20]
        LDR     r5,     [r0,#4*12]
        LDR     r4,     [r0,#4*4]
        LDR     r12,    [r0,#4*62]
        STMFD   r13!,{r4-r11}
        LDR     r10,    [r0,#4*54]
        LDR     r9,     [r0,#4*46]
        LDR     r8,     [r0,#4*38]
        LDR     r7,     [r0,#4*30]
        LDR     r6,     [r0,#4*22]
        LDR     r5,     [r0,#4*14]
        LDR     r4,     [r0,#4*6]
        STMFD   r13!,{r4-r10,r12}

        do_s    1, 47, 11, 26,  3, 13, 41, 8, 16, 22, 30
        do_s    2, 27,  6, 54, 48, 39, 19, 12, 27, 1, 17
        do_s    3, 53, 25, 33, 34, 17,  5, 23, 15, 29, 5
        do_s    4,  4, 55, 24, 32, 40, 20, 25, 19, 9, 0
        do_s    5, 36, 31, 21,  8, 23, 52, 7, 13, 24, 2
        do_s    6, 14, 29, 51,  9, 35, 30, 3, 28, 10, 18
        do_s    7,  2, 37, 22,  0, 42, 38, 31, 11, 21, 6
        do_s    8, 16, 43, 44,  1,  7, 28, 4, 26, 14, 20
        do_s    1, 54, 18, 33, 10, 20, 48, 32+8, 32+16, 32+22, 32+30
        do_s    2, 34, 13,  4, 55, 46, 26, 32+12, 32+27, 32+1, 32+17
        do_s    3,  3, 32, 40, 41, 24, 12, 32+23, 32+15, 32+29, 32+5
        do_s    4, 11,  5,  6, 39, 47, 27, 32+25, 32+19, 32+9, 32+0
        do_s    5, 43, 38, 28, 15, 30,  0, 32+7, 32+13, 32+24, 32+2
        do_s    6, 21, 36, 31, 16, 42, 37, 32+3, 32+28, 32+10, 32+18
        do_s    7,  9, 44, 29,  7, 49, 45, 32+31, 32+11, 32+21, 32+6
        do_s    8, 23, 50, 51,  8, 14, 35, 32+4, 32+26, 32+14, 32+20
        do_s    1, 11, 32, 47, 24, 34,  5, 8, 16, 22, 30
        do_s    2, 48, 27, 18, 12,  3, 40, 12, 27, 1, 17
        do_s    3, 17, 46, 54, 55, 13, 26, 23, 15, 29, 5
        do_s    4, 25, 19, 20, 53,  4, 41, 25, 19, 9, 0
        do_s    5,  2, 52, 42, 29, 44, 14, 7, 13, 24, 2
        do_s    6, 35, 50, 45, 30,  1, 51, 3, 28, 10, 18
        do_s    7, 23, 31, 43, 21,  8,  0, 31, 11, 21, 6
        do_s    8, 37,  9, 38, 22, 28, 49, 4, 26, 14, 20
        do_s    1, 25, 46,  4, 13, 48, 19, 32+8, 32+16, 32+22, 32+30
        do_s    2,  5, 41, 32, 26, 17, 54, 32+12, 32+27, 32+1, 32+17
        do_s    3,  6,  3, 11, 12, 27, 40, 32+23, 32+15, 32+29, 32+5
        do_s    4, 39, 33, 34, 10, 18, 55, 32+25, 32+19, 32+9, 32+0
        do_s    5, 16,  7,  1, 43, 31, 28, 32+7, 32+13, 32+24, 32+2
        do_s    6, 49,  9,  0, 44, 15, 38, 32+3, 32+28, 32+10, 32+18
        do_s    7, 37, 45,  2, 35, 22, 14, 32+31, 32+11, 32+21, 32+6
        do_s    8, 51, 23, 52, 36, 42,  8, 32+4, 32+26, 32+14, 32+20
        do_s    1, 39,  3, 18, 27,  5, 33, 8, 16, 22, 30
        do_s    2, 19, 55, 46, 40,  6, 11, 12, 27, 1, 17
        do_s    3, 20, 17, 25, 26, 41, 54, 23, 15, 29, 5
        do_s    4, 53, 47, 48, 24, 32, 12, 25, 19, 9, 0
        do_s    5, 30, 21, 15,  2, 45, 42, 7, 13, 24, 2
        do_s    6,  8, 23, 14, 31, 29, 52, 3, 28, 10, 18
        do_s    7, 51,  0, 16, 49, 36, 28, 31, 11, 21, 6
        do_s    8, 38, 37,  7, 50,  1, 22, 4, 26, 14, 20
        do_s    1, 53, 17, 32, 41, 19, 47, 32+8, 32+16, 32+22, 32+30
        do_s    2, 33, 12,  3, 54, 20, 25, 32+12, 32+27, 32+1, 32+17
        do_s    3, 34,  6, 39, 40, 55, 11, 32+23, 32+15, 32+29, 32+5
        do_s    4, 10,  4,  5, 13, 46, 26, 32+25, 32+19, 32+9, 32+0
        do_s    5, 44, 35, 29, 16,  0,  1, 32+7, 32+13, 32+24, 32+2
        do_s    6, 22, 37, 28, 45, 43,  7, 32+3, 32+28, 32+10, 32+18
        do_s    7, 38, 14, 30,  8, 50, 42, 32+31, 32+11, 32+21, 32+6
        do_s    8, 52, 51, 21,  9, 15, 36, 32+4, 32+26, 32+14, 32+20
        do_s    1, 10,  6, 46, 55, 33,  4, 8, 16, 22, 30
        do_s    2, 47, 26, 17, 11, 34, 39, 12, 27, 1, 17
        do_s    3, 48, 20, 53, 54, 12, 25, 23, 15, 29, 5
        do_s    4, 24, 18, 19, 27,  3, 40, 25, 19, 9, 0
        do_s    5, 31, 49, 43, 30, 14, 15, 7, 13, 24, 2
        do_s    6, 36, 51, 42,  0,  2, 21, 3, 28, 10, 18
        do_s    7, 52, 28, 44, 22,  9,  1, 31, 11, 21, 6
        do_s    8,  7, 38, 35, 23, 29, 50, 4, 26, 14, 20
        do_s    1, 24, 20,  3, 12, 47, 18, 32+8, 32+16, 32+22, 32+30
        do_s    2,  4, 40,  6, 25, 48, 53, 32+12, 32+27, 32+1, 32+17
        do_s    3,  5, 34, 10, 11, 26, 39, 32+23, 32+15, 32+29, 32+5
        do_s    4, 13, 32, 33, 41, 17, 54, 32+25, 32+19, 32+9, 32+0
        do_s    5, 45,  8,  2, 44, 28, 29, 32+7, 32+13, 32+24, 32+2
        do_s    6, 50, 38,  1, 14, 16, 35, 32+3, 32+28, 32+10, 32+18
        do_s    7,  7, 42, 31, 36, 23, 15, 32+31, 32+11, 32+21, 32+6
        do_s    8, 21, 52, 49, 37, 43,  9, 32+4, 32+26, 32+14, 32+20
        do_s    1,  6, 27, 10, 19, 54, 25, 8, 16, 22, 30
        do_s    2, 11, 47, 13, 32, 55,  3, 12, 27, 1, 17
        do_s    3, 12, 41, 17, 18, 33, 46, 23, 15, 29, 5
        do_s    4, 20, 39, 40, 48, 24,  4, 25, 19, 9, 0
        do_s    5, 52, 15,  9, 51, 35, 36, 7, 13, 24, 2
        do_s    6,  2, 45,  8, 21, 23, 42, 3, 28, 10, 18
        do_s    7, 14, 49, 38, 43, 30, 22, 31, 11, 21, 6
        do_s    8, 28,  0,  1, 44, 50, 16, 4, 26, 14, 20
        do_s    1, 20, 41, 24, 33, 11, 39, 32+8, 32+16, 32+22, 32+30
        do_s    2, 25,  4, 27, 46, 12, 17, 32+12, 32+27, 32+1, 32+17
        do_s    3, 26, 55,  6, 32, 47,  3, 32+23, 32+15, 32+29, 32+5
        do_s    4, 34, 53, 54,  5, 13, 18, 32+25, 32+19, 32+9, 32+0
        do_s    5,  7, 29, 23, 38, 49, 50, 32+7, 32+13, 32+24, 32+2
        do_s    6, 16,  0, 22, 35, 37,  1, 32+3, 32+28, 32+10, 32+18
        do_s    7, 28,  8, 52,  2, 44, 36, 32+31, 32+11, 32+21, 32+6
        do_s    8, 42, 14, 15, 31,  9, 30, 32+4, 32+26, 32+14, 32+20
        do_s    1, 34, 55, 13, 47, 25, 53, 8, 16, 22, 30
        do_s    2, 39, 18, 41,  3, 26,  6, 12, 27, 1, 17
        do_s    3, 40, 12, 20, 46,  4, 17, 23, 15, 29, 5
        do_s    4, 48, 10, 11, 19, 27, 32, 25, 19, 9, 0
        do_s    5, 21, 43, 37, 52,  8,  9, 7, 13, 24, 2
        do_s    6, 30, 14, 36, 49, 51, 15, 3, 28, 10, 18
        do_s    7, 42, 22,  7, 16, 31, 50, 31, 11, 21, 6
        do_s    8,  1, 28, 29, 45, 23, 44, 4, 26, 14, 20
        do_s    1, 48, 12, 27,  4, 39, 10, 32+8, 32+16, 32+22, 32+30
        do_s    2, 53, 32, 55, 17, 40, 20, 32+12, 32+27, 32+1, 32+17
        do_s    3, 54, 26, 34,  3, 18,  6, 32+23, 32+15, 32+29, 32+5
        do_s    4,  5, 24, 25, 33, 41, 46, 32+25, 32+19, 32+9, 32+0
        do_s    5, 35,  2, 51,  7, 22, 23, 32+7, 32+13, 32+24, 32+2
        do_s    6, 44, 28, 50,  8, 38, 29, 32+3, 32+28, 32+10, 32+18
        do_s    7,  1, 36, 21, 30, 45,  9, 32+31, 32+11, 32+21, 32+6
        do_s    8, 15, 42, 43,  0, 37, 31, 32+4, 32+26, 32+14, 32+20
        do_s    1,  5, 26, 41, 18, 53, 24, 8, 16, 22, 30
        do_s    2, 10, 46, 12,  6, 54, 34, 12, 27, 1, 17
        do_s    3, 11, 40, 48, 17, 32, 20, 23, 15, 29, 5
        do_s    4, 19, 13, 39, 47, 55,  3, 25, 19, 9, 0
        do_s    5, 49, 16, 38, 21, 36, 37, 7, 13, 24, 2
        do_s    6, 31, 42,  9, 22, 52, 43, 3, 28, 10, 18
        do_s    7, 15, 50, 35, 44,  0, 23, 31, 11, 21, 6
        do_s    8, 29,  1,  2, 14, 51, 45, 4, 26, 14, 20
        do_s    1, 19, 40, 55, 32, 10, 13, 32+8, 32+16, 32+22, 32+30
        do_s    2, 24,  3, 26, 20, 11, 48, 32+12, 32+27, 32+1, 32+17
        do_s    3, 25, 54,  5,  6, 46, 34, 32+23, 32+15, 32+29, 32+5
        do_s    4, 33, 27, 53,  4, 12, 17, 32+25, 32+19, 32+9, 32+0
        do_s    5,  8, 30, 52, 35, 50, 51, 32+7, 32+13, 32+24, 32+2
        do_s    6, 45,  1, 23, 36,  7,  2, 32+3, 32+28, 32+10, 32+18
        do_s    7, 29,  9, 49, 31, 14, 37, 32+31, 32+11, 32+21, 32+6
        do_s    8, 43, 15, 16, 28, 38,  0, 32+4, 32+26, 32+14, 32+20
        do_s    1, 33, 54, 12, 46, 24, 27, 8, 16, 22, 30
        check   1, 1, 5, 8
        check   1, 2, 3, 16
        check   1, 3, 51, 22
        check   1, 4, 49, 30
        do_s    2, 13, 17, 40, 34, 25,  5, 12, 27, 1, 17
        check   2, 1, 37, 12
        check   2, 2, 25, 27
        check   2, 3, 15, 1
        check   2, 4, 11, 17
        do_s    3, 39, 11, 19, 20,  3, 48, 23, 15, 29, 5
        check   3, 1, 59, 23
        check   3, 2, 61, 15
        check   3, 3, 41, 29
        check   3, 4, 47, 5
        do_s    4, 47, 41, 10, 18, 26,  6, 25, 19, 9, 0
        check   4, 1, 9, 25
        check   4, 2, 27, 19
        check   4, 3, 13, 9
        check   4, 4, 7, 0
        do_s    5, 22, 44,  7, 49,  9, 38, 7, 13, 24, 2
        check   5, 1, 63, 7
        check   5, 2, 45, 13
        check   5, 3, 1, 24
        check   5, 4, 23, 2
        do_s    6,  0, 15, 37, 50, 21, 16, 3, 28, 10, 18
        check   6, 1, 31, 3
        check   6, 2, 33, 28
        check   6, 3, 21, 10
        check   6, 4, 19, 18
        do_s    7, 43, 23,  8, 45, 28, 51, 31, 11, 21, 6
        check   7, 1, 57, 31
        check   7, 2, 29, 11
        check   7, 3, 43, 21
        check   7, 4, 55, 6
        do_s    8,  2, 29, 30, 42, 52, 14, 4, 26, 14, 20
        check   8, 1, 39, 4
        check   8, 2, 17, 26
        check   8, 3, 53, 14
        check   8, 4, 35, 20
        do_s    1, 40,  4, 19, 53,  6, 34, 32+8, 32+16, 32+22, 32+30
        check   1, 1, 4, 32+8
        check   1, 2, 2, 32+16
        check   1, 3, 50, 32+22
        check   1, 4, 48, 32+30
        do_s    2, 20, 24, 47, 41, 32, 12, 32+12, 32+27, 32+1, 32+17
        check   2, 1, 36, 32+12
        check   2, 2, 24, 32+27
        check   2, 3, 14, 32+1
        check   2, 4, 10, 32+17
        do_s    3, 46, 18, 26, 27, 10, 55, 32+23, 32+15, 32+29, 32+5
        check   3, 1, 58, 32+23
        check   3, 2, 60, 32+15
        check   3, 3, 40, 32+29
        check   3, 4, 46, 32+5
        do_s    4, 54, 48, 17, 25, 33, 13, 32+25, 32+19, 32+9, 32+0
        check   4, 1, 8, 32+25
        check   4, 2, 26, 32+19
        check   4, 3, 12, 32+9
        check   4, 4, 6, 32+0
        do_s    5, 29, 51, 14,  1, 16, 45, 32+7, 32+13, 32+24, 32+2
        check   5, 1, 62, 32+7
        check   5, 2, 44, 32+13
        check   5, 3, 0, 32+24
        check   5, 4, 22, 32+2
        do_s    6,  7, 22, 44,  2, 28, 23, 32+3, 32+28, 32+10, 32+18
        check   6, 1, 30, 32+3
        check   6, 2, 32, 32+28
        check   6, 3, 20, 32+10
        check   6, 4, 18, 32+18
        do_s    7, 50, 30, 15, 52, 35, 31, 32+31, 32+11, 32+21, 32+6
        check   7, 1, 56, 32+31
        check   7, 2, 28, 32+11
        check   7, 3, 42, 32+21
        check   7, 4, 54, 32+6
        do_s    8,  9, 36, 37, 49,  0, 21, 32+4, 32+26, 32+14, 32+20
        check   8, 1, 38, 32+4
        check   8, 2, 16, 32+26
        check   8, 3, 52, 32+14
        check   8, 5, 34, 32+20
gotresult
        ADD     r13,r13,#4*(64+3)
        LDMIA   r13!,{r4-r11}
deseval_end

;        CMP      r0,#0   (BICS in deseval bit)
        BNE      foundkey
        EOR     r6,r7,r7,LSR #1
        ADD      r7,r7,#1
        CMP      r7,r9
        BCS      half_keys_done
        EOR      r1,r7,r7,LSR #1
        EOR      r1,r1,r6

        MACRO
        checktwiddle    $bit, $twiddle
        TST     r1,#1<<$bit
        MOVNE   r1,#$twiddle
        BNE     timingloop
        MEND

        checktwiddle    0,12
        checktwiddle    1,15
        checktwiddle    2,18
        checktwiddle    3,40
        checktwiddle    4,41
        checktwiddle    5,42
        checktwiddle    6,43
        checktwiddle    7,45
        checktwiddle    8,46
        checktwiddle    9,49
        checktwiddle    10,50
        checktwiddle    11,0
        checktwiddle    12,1
        checktwiddle    13,2
        checktwiddle    14,4
        checktwiddle    15,6
        checktwiddle    16,7
        checktwiddle    17,9
        checktwiddle    18,13

        ; timeslice is too big... what shall we do???
        ; des-slice.c++ twiddles the wrong bit. That seems unacceptable,
        ; as it will claim to check keys it hasn't but in a manner that
        ; wouldn't show up on -test or on blocks from a test port.

this_shouldnt_happen
        ; While this is perhaps a bad idea, at least it won't give
        ; an incorrect result
        B       this_shouldnt_happen

half_keys_done
        CMP      r8,#0
        BNE      inc_then_exit
        MOV      r8,#1
        MOV      r6,#0
        MOV      r7,r6
        MOV      r0,#&38
        ADD      r1,r13,#&208
invertloop
        LDR      r2,[r1]
        SUBS     r0,r0,#1
        MVN      r2,r2
        STR      r2,[r1],#4
        BNE      invertloop
        B       resettwiddles

inc_then_exit
        LDR      r1,[r4,#&14]
        MOV      r2,#1
        LDR      r0,[r11,#-&28]
        MOV      r0,r2,LSL r0
        ADD      r1,r1,r0
        STR      r1,[r4,#&14]
        LDMDB    r11,{r4-r9,r11,r13,pc}^

foundkey
        MVN      r5,#0
        MOV      r1,#0
        MOV      r2,#1
|L000228.J51|
        TST      r0,r2,LSL r1
        MOVNE    r5,r1
        ADD      r1,r1,#1
        CMP      r1,#&20
        BCC      |L000228.J51|
        MOV      r0,#0
        STR      r0,[r13,#4]
        MOV      r6,#&37
        STR      r0,[r13,#0]
        ADRL      r9,odd_parity
|L000250.J55|
        ADD      r0,r13,#&208
        LDR      r0,[r0,r6,LSL #2]
        MOV      r0,r0,LSR r5
        AND      r0,r0,#1
        MOV      r2,r0,LSL #7
        ADD      r0,r13,#&208
        ADD      r0,r0,r6,LSL #2
        LDR      r1,[r0,#-4]
        MOV      r1,r1,LSR r5
        AND      r1,r1,#1
        ORR      r2,r2,r1,LSL #6
        LDR      r1,[r0,#-8]
        MOV      r1,r1,LSR r5
        AND      r1,r1,#1
        ORR      r2,r2,r1,LSL #5
        LDR      r1,[r0,#-&c]
        MOV      r1,r1,LSR r5
        AND      r1,r1,#1
        ORR      r2,r2,r1,LSL #4
        LDR      r1,[r0,#-&10]
        MOV      r1,r1,LSR r5
        AND      r1,r1,#1
        ORR      r2,r2,r1,LSL #3
        LDR      r1,[r0,#-&14]
        MOV      r1,r1,LSR r5
        AND      r1,r1,#1
        ORR      r1,r2,r1,LSL #2
        LDR      r0,[r0,#-&18]
        MOV      r0,r0,LSR r5
        AND      r0,r0,#1
        ORR      r0,r1,r0,LSL #1
        LDRB     r7,[r9,r0]
        ADD      r1,r6,#1
        MOV      r0,#7
        BL       __rt_sdiv
        SUB      r0,r0,#1
        CMP      r0,#4
        BLT      |L000300.J58|
        MVN      r1,#&1f
        ADD      r0,r1,r0,LSL #3
        LDR      r1,[r13,#4]
        ORR      r0,r1,r7,LSL r0
        STR      r0,[r13,#4]
        B        |L000310.J60|
|L000300.J58|
        MOV      r0,r0,LSL #3
        LDR      r1,[r13,#0]
        ORR      r0,r1,r7,LSL r0
        STR      r0,[r13,#0]
|L000310.J60|
        SUB      r6,r6,#7
        CMP      r6,#0
        BGT      |L000250.J55|
        CMP      r8,#0
        BEQ      |L00033c.J62|
        LDR      r0,[r13,#4]
        MVN      r0,r0
        STR      r0,[r13,#4]
        LDR      r0,[r13,#0]
        MVN      r0,r0
        STR      r0,[r13,#0]
|L00033c.J62|
        MOV      r1,r13
        ADD      r0,r13,#4
        BL       convert_key_from_des_to_inc__FPUlT1
        LDR      r0,[r4,#&14]
        LDR      r1,[r13,#0]
        SUB      r0,r1,r0
        STR      r1,[r4,#&14]
        LDR      r1,[r13,#4]
        STR      r1,[r4,#&10]
        LDMDB    r11,{r4-r9,r11,r13,pc}^

 [ patch
        DCD     &DEAD
 ]
        END
========================================================
