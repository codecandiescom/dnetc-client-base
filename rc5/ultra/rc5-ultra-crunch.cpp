/* 
 * Copyright distributed.net 1997-2002 - All Rights Reserved
 * For use in distributed.net projects only.
 * Any other distribution or use of this source violates copyright.
 *
 * dual-key, mixed round 3 and encryption, A1/A2 use for last value,
 * non-arrayed S1/S2 tables, direct use of last value, run-time
 * generation of S0[]
 *
 * $Log: rc5-ultra-crunch.cpp,v $
 * Revision 1.15  2002/09/02 00:35:57  andreasb
 * sync: merged changes from release-2-7111 branch between
 *       2000-07-11 and release-2-7111_20020901 into trunk
 *
 * Revision 1.9.2.5  2002/04/12 23:56:57  andreasb
 * 2002 copyright update - round 2
 *
 * Revision 1.14  2000/07/11 02:18:03  mfeiri
 * sync
 *
 * Revision 1.9.2.4  2000/02/03 22:50:16  ivo
 * removed some warnings.
 *
 * Revision 1.9.2.3  1999/12/02 01:50:07  cyp
 * wrapper fixes
 *
 * Revision 1.9.2.2  1999/11/28 18:01:44  cyp
 * sync
 *
 * Revision 1.12  1999/11/28 15:37:35  cyp
 * made global rc5_unit_func_ultrasparc_cruch(...)
 *
 * Revision 1.11  1999/07/20 04:26:37  cyp
 * sync'd from release branch
 *
 * Revision 1.9.2.1  1999/07/09 15:26:46  cyp
 * sync from dev branch to release. Apparently silby doesn't know that you
 * can't _release_ something that won't compile.
 *
 * Revision 1.10  1999/06/04 21:44:53  silby
 * Removed checks for PIPELINE_COUNT and _CPU_32BIT_
 *
 * Revision 1.9  1998/11/28 18:20:24  remi
 * Fixed the key incrementation bug. Many thanks to Simon Fenney for
 * finding it !
 *
 * Revision 1.7  1998/07/08 22:59:40  remi
 * Lots of $Id stuff.
 *
 * Revision 1.6  1998/06/15 02:44:32  djones
 * First build of UltraSPARC 64-bit/VIS DES client:
 * - many configure file tweaks: split up C++, ASM and C files; make "gcc" the
 *   compiler.
 * - "tr" on SunOS 4.1.4 goes into endless loop when faced with "..-"; change
 *   to "..\-".
 * - Enable generation of whack16()
 *
 * Revision 1.5  1998/06/14 08:27:23  friedbait
 * 'Id' tags added in order to support 'ident' command to display a bill of
 * material of the binary executable
 *
 * Revision 1.4  1998/06/14 08:13:41  friedbait
 * 'Log' keywords added to maintain automatic change history
 *
 *
 */

#if (!defined(lint) && defined(__showids__))
const char *rc5_ultra_crunch_cpp (void) {
return "@(#)$Id: rc5-ultra-crunch.cpp,v 1.15 2002/09/02 00:35:57 andreasb Exp $"; }
#endif

#include <stdio.h>

/* include files for version 1 code */
/* #include "common-2.h" */
/* #include "client-2.h" */

/* include files for version 2 code */
#include "problem.h"
#include "rotate.h"


#define _P_RC5     0xB7E15163
#define _Q     0x9E3779B9


/* Optimized version of the inner code of RC5_KEY_CHECK()

   Lawrence Butcher 16/9/97 lbutcher@eng.sun.com  LB Version 1.3

     - this seems to run well when compiled with gcc 2.6.2 and (?) 2.7.2
     _ on a 296 MHz ultra-II, it claims to do 593,500 keys/sec
     _ on a 167 MHz ultra-I, it claims to do 334,500 keys/sec

     - got inner loop code for a version 1 rc5 program from Remi
       in France.

     - Fiddled around with this code to make an ultra-sparc faster
       This is written as if it were in assembler.

     - There is only one shifter, and shift instructions must be
       the first of the pair.  Bummer.  Fiddled with code some more

     - swiped comments and calling convention from RC5 V2 code made
       available for porting purposes only by Jeff Lawson

     - converted from C calling convention to struct passing the
       way that the C+ V2 code does it.  Will this change the speed?

   R\351mi Guyomarch - 24/06/97 - rguyom@mail.dotcom.fr

   What I've done ?

	- Deleted the unused B variable since B = L_0 or L_1
	
	- Mixed the round 3 with the encryption part, since it save
	  25/26 assignements in Sxx variables, and since it should give
	  better performance on superscalar processors.
	  It save also save tree rotations and many additions if the first
	  test fail at the end.
	  Drawback : two more registers needed (eA and eB)

	- Moved S00 from a static global array to local variables, since many
	  compilers (including gcc) can't assign registers to global (even static)
	  variables

	- Deleted the last "L_1 = ..." line, since L_1 isn't used after.

	- Change the code to check two key at once to speed things up on superscalar
	  architectures. Interleaved the code.
	  Cons : 10 heavy used variables (xA,yA,xeA,yeA,xeB,yeB,L_0,L_1,L_2,L_3)
	         too much for intel

	- Replaced S0[] array by inline values (check client-2.h)

*/

/* LB */
#ifdef KEEP_OLD_CODE
static RC5_WORD remember1, remember2, remember3, remember4;

int RC5_KEY_CHECK_INNER_REMI (RC5_WORD L_0, RC5_WORD L_1,
			 RC5_WORD L_2, RC5_WORD L_3)
{
    RC5_WORD xS00,xS01,xS02,xS03,xS04,xS05,xS06,xS07,xS08,xS09,xS10,xS11,xS12,
	     xS13,xS14,xS15,xS16,xS17,xS18,xS19,xS20,xS21,xS22,xS23,xS24,xS25;
    RC5_WORD xA, xeA, xeB;

    RC5_WORD yS00,yS01,yS02,yS03,yS04,yS05,yS06,yS07,yS08,yS09,yS10,yS11,yS12,
	     yS13,yS14,yS15,yS16,yS17,yS18,yS19,yS20,yS21,yS22,yS23,yS24,yS25;
    RC5_WORD yA, yeA, yeB;

    int ret;



      /* Begin round 1 of key expansion */

    #define RC5_ROUND1_PARTIAL(_x0,_y0, _x1,_y1, _s0,_s1) \
	xA = _x0 = ROTL3(_s0 + xA + L_1);	\
	yA = _y0 = ROTL3(_s0 + yA + L_3);	\
	L_0 = ROTL(L_0 + xA + L_1, xA + L_1);	\
	L_2 = ROTL(L_2 + yA + L_3, yA + L_3);	\
	xA = _x1 = ROTL3(_s1 + xA + L_0);	\
	yA = _y1 = ROTL3(_s1 + yA + L_2);	\
	L_1 = ROTL(L_0 + xA + L_1, xA + L_0);	\
	L_3 = ROTL(L_2 + yA + L_3, yA + L_2)
						
  /*printf("round1 - L0L1 : 0x%.08X%.08X\n",L_0,L_1);*/
  /*printf("round1 - L2L3 : 0x%.08X%.08X\n",L_2,L_3);*/

      /* avoid inline assembly, since these values can be generated by the compiler */
      /* warning : SHR(x,3) != x>>3 */

  /*xA = xS00 = ROTL3(S0_00);
    yA = yS00 = ROTL3(S0_00);
    L_0 = ROTL(L_0 + xA, xA);
    L_2 = ROTL(L_2 + yA, yA);
    xA = xS01 = ROTL3(S0_01 + xA + L_0);
    yA = yS01 = ROTL3(S0_01 + yA + L_2);
    L_1 = ROTL(L_0 + xA + L_1, xA + L_0);
    L_3 = ROTL(L_2 + yA + L_3, yA + L_2);*/
	
    #define _init ((RC5_WORD) (SHL(S0_00,3) | SHR(S0_00,3)))
    /* no need to store in xS00,yS00 since it isn't used in round 2 */
    xA = yA = /*xS00 = yS00 =*/ _init;
    L_0 = ROTL(L_0 + _init, _init);
    L_2 = ROTL(L_2 + _init, _init);
    xA = xS01 = ROTL3(S0_01 + _init + L_0);
    yA = yS01 = ROTL3(S0_01 + _init + L_2);
    L_1 = ROTL(L_0 + xA + L_1, xA + L_0);
    L_3 = ROTL(L_2 + yA + L_3, yA + L_2);


  /*RC5_ROUND1_PARTIAL (xS00,yS00, xS01,yS01, S0_00,S0_01);*/
    RC5_ROUND1_PARTIAL (xS02,yS02, xS03,yS03, S0_02,S0_03);
    RC5_ROUND1_PARTIAL (xS04,yS04, xS05,yS05, S0_04,S0_05);
    RC5_ROUND1_PARTIAL (xS06,yS06, xS07,yS07, S0_06,S0_07);
    RC5_ROUND1_PARTIAL (xS08,yS08, xS09,yS09, S0_08,S0_09);
    RC5_ROUND1_PARTIAL (xS10,yS10, xS11,yS11, S0_10,S0_11);
    RC5_ROUND1_PARTIAL (xS12,yS12, xS13,yS13, S0_12,S0_13);
    RC5_ROUND1_PARTIAL (xS14,yS14, xS15,yS15, S0_14,S0_15);
    RC5_ROUND1_PARTIAL (xS16,yS16, xS17,yS17, S0_16,S0_17);
    RC5_ROUND1_PARTIAL (xS18,yS18, xS19,yS19, S0_18,S0_19);
    RC5_ROUND1_PARTIAL (xS20,yS20, xS21,yS21, S0_20,S0_21);
    RC5_ROUND1_PARTIAL (xS22,yS22, xS23,yS23, S0_22,S0_23);
    RC5_ROUND1_PARTIAL (xS24,yS24, xS25,yS25, S0_24,S0_25);



      /* Begin round 2 of key expansion */

    #define RC5_ROUND2_PARTIAL(_x0,_y0, _x1,_y1) \
	xA = _x0 = ROTL3(_x0 + xA + L_1);	\
	yA = _y0 = ROTL3(_y0 + yA + L_3);	\
	L_0 = ROTL(L_0 + xA + L_1, xA + L_1);	\
	L_2 = ROTL(L_2 + yA + L_3, yA + L_3);	\
	xA = _x1 = ROTL3(_x1 + xA + L_0);	\
	yA = _y1 = ROTL3(_y1 + yA + L_2);	\
	L_1 = ROTL(L_0 + xA + L_1, xA + L_0);	\
	L_3 = ROTL(L_2 + yA + L_3, yA + L_2)
						
  /*printf("round2 - AL0L1 : 0x%.08X 0x%.08X%.08X\n",xA,L_0,L_1);*/
  /*printf("round2 - AL2L3 : 0x%.08X 0x%.08X%.08X\n",yA,L_2,L_3);*/

      /* xS00 & yS00 has been precomputed (see start of round 1) */
  /*xA = xS00 = ROTL3(xS00 + xA + L_1);
    yA = yS00 = ROTL3(yS00 + yA + L_3);
    L_0 = ROTL(L_0 + xA + L_1, xA + L_1);
    L_2 = ROTL(L_2 + yA + L_3, yA + L_3);
    xA = xS01 = ROTL3(xS01 + xA + L_0);
    yA = yS01 = ROTL3(yS01 + yA + L_2);
    L_1 = ROTL(L_0 + xA + L_1, xA + L_0);
    L_3 = ROTL(L_2 + yA + L_3, yA + L_2); */

    xA = xS00 = ROTL3(_init + xA + L_1);
    yA = yS00 = ROTL3(_init + yA + L_3);
    L_0 = ROTL(L_0 + xA + L_1, xA + L_1);
    L_2 = ROTL(L_2 + yA + L_3, yA + L_3);
    xA = xS01 = ROTL3(xS01 + xA + L_0);
    yA = yS01 = ROTL3(yS01 + yA + L_2);
    L_1 = ROTL(L_0 + xA + L_1, xA + L_0);
    L_3 = ROTL(L_2 + yA + L_3, yA + L_2);

  /*RC5_ROUND2_PARTIAL (xS00,yS00, xS00,yS00);*/
    RC5_ROUND2_PARTIAL (xS02,yS02, xS03,yS03);
    RC5_ROUND2_PARTIAL (xS04,yS04, xS05,yS05);
    RC5_ROUND2_PARTIAL (xS06,yS06, xS07,yS07);
    RC5_ROUND2_PARTIAL (xS08,yS08, xS09,yS09);
    RC5_ROUND2_PARTIAL (xS10,yS10, xS11,yS11);
    RC5_ROUND2_PARTIAL (xS12,yS12, xS13,yS13);
    RC5_ROUND2_PARTIAL (xS14,yS14, xS15,yS15);
    RC5_ROUND2_PARTIAL (xS16,yS16, xS17,yS17);
    RC5_ROUND2_PARTIAL (xS18,yS18, xS19,yS19);
    RC5_ROUND2_PARTIAL (xS20,yS20, xS21,yS21);
    RC5_ROUND2_PARTIAL (xS22,yS22, xS23,yS23);
    RC5_ROUND2_PARTIAL (xS24,yS24, xS25,yS25);



      /* Begin round 3 of key expansion */
      /* mixed with encryption */

   #define RC5_ROUND3_PARTIAL(_x0,_y0,_x1,_y1)			\
	xeA = ROTL(xeA^xeB,xeB) + (xA = ROTL3(_x0+xA+L_1));	\
	yeA = ROTL(yeA^yeB,yeB) + (yA = ROTL3(_y0+yA+L_3));	\
	L_0 = ROTL(L_0+xA+L_1,xA+L_1);				\
	L_2 = ROTL(L_2+yA+L_3,yA+L_3);				\
	xeB = ROTL(xeB^xeA,xeA) + (xA = ROTL3(_x1+xA+L_0));	\
	yeB = ROTL(yeB^yeA,yeA) + (yA = ROTL3(_y1+yA+L_2));	\
	L_1 = ROTL(L_0+xA+L_1,xA+L_0);				\
	L_3 = ROTL(L_2+yA+L_3,yA+L_2)

  /*printf("round3 - AL0L1 : 0x%.08X 0x%.08X%.08X\n",xA,L_0,L_1);*/
  /*printf("round3 - AL2L3 : 0x%.08X 0x%.08X%.08X\n",yA,L_2,L_3);*/

    xeA = P_0 + (xA = ROTL3(xS00+xA+L_1));
    yeA = P_0 + (yA = ROTL3(yS00+yA+L_3));
    L_0 = ROTL(L_0+xA+L_1,xA+L_1);
    L_2 = ROTL(L_2+yA+L_3,yA+L_3);
    xeB = P_1 + (xA = ROTL3(xS01+xA+L_0));
    yeB = P_1 + (yA = ROTL3(yS01+yA+L_2));
    L_1 = ROTL(L_0+xA+L_1,xA+L_0);
    L_3 = ROTL(L_2+yA+L_3,yA+L_2);

  /*RC5_ROUND3_PARTIAL (xS00,yS00, xS00,yS00);*/
    RC5_ROUND3_PARTIAL (xS02,yS02, xS03,yS03);
    RC5_ROUND3_PARTIAL (xS04,yS04, xS05,yS05);
    RC5_ROUND3_PARTIAL (xS06,yS06, xS07,yS07);
    RC5_ROUND3_PARTIAL (xS08,yS08, xS09,yS09);
    RC5_ROUND3_PARTIAL (xS10,yS10, xS11,yS11);
    RC5_ROUND3_PARTIAL (xS12,yS12, xS13,yS13);
    RC5_ROUND3_PARTIAL (xS14,yS14, xS15,yS15);
    RC5_ROUND3_PARTIAL (xS16,yS16, xS17,yS17);
    RC5_ROUND3_PARTIAL (xS18,yS18, xS19,yS19);
    RC5_ROUND3_PARTIAL (xS20,yS20, xS21,yS21);
    RC5_ROUND3_PARTIAL (xS22,yS22, xS23,yS23);
  /*RC5_ROUND3_PARTIAL (xS24,yS24, xS25,yS25);*/

      /* early exit */

    xeA = ROTL(xeA^xeB,xeB) + (xA = ROTL3(xS24+xA+L_1));/*L_0 = ROTL(L_0+xA+L_1,xA+L_1);*/
    yeA = ROTL(yeA^yeB,yeB) + (yA = ROTL3(yS24+yA+L_3));/*L_2 = ROTL(L_2+yA+L_3,yA+L_3);*/
  /*xeB = ROTL(xeB^xeA,xeA) + (     ROTL3(xS25+xA+L_0));  L_1 = ROTL(L_0+xA+L_1,xA+L_0);*/
  /*yeB = ROTL(yeB^yeA,yeA) + (     ROTL3(yS25+yA+L_2));  L_3 = ROTL(L_2+yA+L_3,yA+L_2);*/

    ret = 0;

    if (C_0 == xeA &&
	C_1 == ROTL(xeB^xeA,xeA) + ROTL3(xS25+xA+ROTL(L_0+xA+L_1,xA+L_1)))
	ret |= 1;

    if (C_0 == yeA &&
	C_1 == ROTL(yeB^yeA,yeA) + ROTL3(yS25+yA+ROTL(L_2+yA+L_3,yA+L_3)))
	ret |= 2;

/* #define CHECK_LLB */
#ifdef CHECK_LLB
    remember1 = xeA;
    remember2 = ROTL(xeB^xeA,xeA) + ROTL3(xS25+xA+ROTL(L_0+xA+L_1,xA+L_1));
    remember3 = yeA;
    remember4 = ROTL(yeB^yeA,yeA) + ROTL3(yS25+yA+ROTL(L_2+yA+L_3,yA+L_3));
/* LB stick result into globals here to check below */
#endif

    return ret;
}
#endif /* KEEP_OLD_CODE */

/* musings and macros from Butcher start here
 *
 * the big picture:
 * Magic constants P and Q are defined.  An array S of 2*(12+1) words is
 * defined, with S0 == P, S1 == P+Q, S2 == P+2*Q, and so on, all constants!
 * These are defined in macros as S0_00, S0_01, and up to S0_25
 *
 * The key is stuck into another array L byte at a time, with extra bytes 0.
 * In our case, this will be 2 words.
 *
 * The key is mixed into the constants by pasing over the bigger array 3 times,
 *
 * i=j=0; A=B=0;
 * Do 3 times through the big array:
 *   A = S[i] = (S[i] + A + B) <<< 3;
 *   B = L[j] = (L[j] + A + B) <<< (A+B);
 * inc and wrap i and j;
 *
 * Finally, once the array S is calculated, encryption can follow.
 * Take the plaintext and make it into 2 32-bit words A and B
 *
 * A = A + S[0]
 * B = B + S[1]
 * for i = 1 to 12 do
 *   A = ((A xor B) <<< B) + S[2*i]
 *   B = ((B xor A) <<< A) + S[2*i + 1]
 *
 *
 * The primitives are, as above, these:
 *  A = S[i] = (S[i] + A + B) <<< 3;
 *  B = L[i] = (L[i] + A + B) <<< (A + B);
 *I expect the assembler to be roughly:
 *  add A + B;
 *  load S[i];
 *  add (A + B) to S[i];
 *  mask (A + B) to 5 bits to get shift distance; (don't need this on Sparc)
 *  shift result left by specified distance.
 *  subtract shift distance from 32
 *  shift result right by specified distance
 *  OR shift results to get new result
 *  store S[i] back;
 * this seems to be 10 instructions, some of them not dependent
 * in fact, the dependency seems to be:
 *  add A + B
 *  add (A + B) to S[i], calculate 32 - (A + B)
 *  shift left and right by different distances
 *  OR together to create new result
 */

/* calculate first pass key constants into S0 array
 *   A = B = 0;
 *
 *   A = S[0] = (S[0]) <<< 3;
 *   B = L[0] = (L[0] + A) <<< (A);
 *   A = S[1] = (S[1] + (A + B)) <<< 3;
 *   B = L[1] = (L[1] + (A + B)) <<< (A + B);
 *   repeat
 *	A = S[i] = (S[i] + (A + B)) <<< 3;
 *	B = L[i] = (L[i] + (A + B)) <<< (A + B);
 *
 *   unwind this a little more to make dependencies obvious:
 *   A = B = C = D = 0;
 *   A = S[0] = (S[0]) <<< 3;
 *   B = L[0] = (L[0] + A) <<< (A);
 *   C = S[1] = (S[1] + (A + B)) <<< 3;
 *   D = L[1] = (L[1] + (B + C)) <<< (B + C);
 *   repeat
 *	A = S[i+0] = (S[i+0] + (C + D)) <<< 3;
 *	B = L[0] = (L[0] + (D + A)) <<< (D + A);
 *	C = S[i+1] = (S[i+1] + (A + B)) <<< 3;
 *	D = L[1] = (L[1] + (B + C)) <<< (B + C);
 *
 * The second loop has primitives
 *   A = A + S0
 *   B = B + S1
 *   repeat
 *	A = ((A xor B) <<< B) + S[2*i]
 *	B = ((B xor A) <<< A) + S[2*i + 1]
 *  I expect this to take 4 primitive ops, too
 *  xor A and B, calculate 32 - B
 *  shift left, shift right
 *  OR together partial results to get variable part
 *  add key derived constant
 *
 * How fast SHOULD this thing run?
 * It looks to me like there are 26 instructions in the first part of the
 * loop and 36 instructions in the last part.
 * 26 * 13 + 26 * 13 + 36 * 12 == 1100 instructions
 * assuming a maximum of 2 instructions per clock, that is 550 clocks.
 * at 166 MHz, that takes 3.3 uS, or 300,000 keys per second.
 *
 * actual measurememts using Sun's compiler on 2-key-at-once is 250,000 k/s
 * gcc on one key per pass is 220,000 k/sec, or 4 uSec.
 *
 * An optimist might not count load/stores, in which case there are
 * 22 * 13 + 22 * 13 + 34 * 12 = 980 clocks, or 3 uSec/ or 333,000 k/sec
 *
 * This might be hindered by the fact that there are 14+ live variables
 * The 2-keys-at-once routine seems to have 25 live variables
 *
 * This runs faster with GCC than Sun's compiler!!!!
 * However GCC is still sucking.  An assembler version of this code,
 * using exactly the instructions as specified here, should be able to
 * run faster than 300,000 keys per second.  I think the estimate of
 * 333,000 keys per second is the correct one.  Since GCC makes this
 * run at a lowly 290,000, that is a 15% loss due to compiler.
 */

#define S0_00 _P_RC5
#define S0_01 ((u32)(S0_00 + _Q))
#define S0_02 ((u32)(S0_01 + _Q))
#define S0_03 ((u32)(S0_02 + _Q))
#define S0_04 ((u32)(S0_03 + _Q))
#define S0_05 ((u32)(S0_04 + _Q))
#define S0_06 ((u32)(S0_05 + _Q))
#define S0_07 ((u32)(S0_06 + _Q))
#define S0_08 ((u32)(S0_07 + _Q))
#define S0_09 ((u32)(S0_08 + _Q))
#define S0_10 ((u32)(S0_09 + _Q))
#define S0_11 ((u32)(S0_10 + _Q))
#define S0_12 ((u32)(S0_11 + _Q))
#define S0_13 ((u32)(S0_12 + _Q))
#define S0_14 ((u32)(S0_13 + _Q))
#define S0_15 ((u32)(S0_14 + _Q))
#define S0_16 ((u32)(S0_15 + _Q))
#define S0_17 ((u32)(S0_16 + _Q))
#define S0_18 ((u32)(S0_17 + _Q))
#define S0_19 ((u32)(S0_18 + _Q))
#define S0_20 ((u32)(S0_19 + _Q))
#define S0_21 ((u32)(S0_20 + _Q))
#define S0_22 ((u32)(S0_21 + _Q))
#define S0_23 ((u32)(S0_22 + _Q))
#define S0_24 ((u32)(S0_23 + _Q))
#define S0_25 ((u32)(S0_24 + _Q))

#define _init ((u32) (SHL(S0_00,3) | SHR(S0_00,3)))

/* NOTE cycle counts are fake, because I don't have an Ultra book */

#define LB_ROTATE_LEFT_3(val,result)					\
    macro_tmp = (val) << (3);				/* clk 1 */ 	\
    macro_tmp2 = (val) >> (29);				/* clk 2 */ 	\
    (result) = macro_tmp | macro_tmp2;			/* clk 3 */


/* NOTE cycle counts are fake, because I don't have an Ultra book */

#define LB_ROTATE_LEFT(val,left_dist,result)				\
    right_dist = (word_size) - (left_dist);		/* clk 1 */ 	\
    macro_tmp = (val) << (left_dist);			/* clk 1 */ 	\
    macro_tmp2 = (val) >> (right_dist);			/* clk 2 */ 	\
    (result) = macro_tmp | macro_tmp2;			/* clk 3 */


/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* before reorder */
#define LB_RC5_ROUND12_PARTIAL2_SIMPLE(index, index2, Input_Array, Output_Array) \
{									\
    part_sum = pipelined_read + C;			/* 1 */ 	\
  pipelined_read = Input_Array[((index) + 1)];		/* 1+ */ 	\
    whole_sum = D + part_sum;				/* 2 */ 	\
    A = whole_sum << (3);				/* 3 */ 	\
    macro_tmp = whole_sum >> (29);			/* 4 */ 	\
    A = A | macro_tmp;			/* left 3 */ 	/* 5 */ 	\
									\
    part_sum2 = pipelined_read2 + C2;			/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
    whole_sum2 = D2 + part_sum2;			/* -2 */ 	\
    A2 = whole_sum2 << (3);				/* -3 */ 	\
    macro_tmp2 = whole_sum2 >> (29);			/* -4 */ 	\
    A2 = A2 | macro_tmp2;		/* left 3 */ 	/* -5 */ 	\
									\
  Output_Array[(index)] = A;				/* 5+ */ 	\
									\
    part_sum = D + A;					/* 6 */ 	\
    whole_sum = B + part_sum;				/* 7 */ 	\
    right_dist = word_size - part_sum;			/* 8 */ 	\
    B = whole_sum << part_sum; 				/* 9 */ 	\
    macro_tmp = whole_sum >> right_dist;		/* 10 */ 	\
    B = B | macro_tmp;			/* left var */ 	/* 11 */ 	\
									\
  Output_Array[(index2)] = A2;				/* -5+ */ 	\
									\
    part_sum2 = D2 + A2;				/* -6 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -7 */ 	\
    right_dist2 = word_size - part_sum2;		/* -8 */ 	\
    B2 = whole_sum2 << part_sum2; 			/* -9 */ 	\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -10 */ 	\
    B2 = B2 | macro_tmp2;		/* left var */ 	/* -11 */ 	\
									\
    part_sum = pipelined_read + A;			/* 12 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 12+ */ 	\
    whole_sum = B + part_sum;				/* 13 */ 	\
    C = whole_sum << (3);				/* 14 */ 	\
    macro_tmp = whole_sum >> (29);			/* 15 */ 	\
    C = C | macro_tmp;			/* left 3 */	/* 16 */ 	\
									\
    part_sum2 = pipelined_read2 + A2;			/* -12 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -12+ */ 	\
    whole_sum2 = B2 + part_sum2;			/* -13 */ 	\
    C2 = whole_sum2 << (3);				/* -14 */ 	\
    macro_tmp2 = whole_sum2 >> (29);			/* -15 */ 	\
    C2 = C2 | macro_tmp2;		/* left 3 */	/* -16 */ 	\
									\
  Output_Array[((index) + 1)] = C;			/* 16+ */ 	\
									\
    part_sum = B + C;					/* 17 */ 	\
    whole_sum = D + part_sum;				/* 18 */ 	\
    right_dist = word_size - part_sum;			/* 19 */ 	\
    D = whole_sum << part_sum;		 		/* 20 */ 	\
    macro_tmp = whole_sum >> right_dist;		/* 21 */ 	\
    D = D | macro_tmp;			/* left var */	/* 22 */ 	\
									\
  Output_Array[((index2) + 1)] = C2;			/* -16+ */ 	\
									\
    part_sum2 = B2 + C2;				/* -17 */ 	\
    whole_sum2 = D2 + part_sum2;			/* -18 */ 	\
    right_dist2 = word_size - part_sum2;		/* -19 */ 	\
    D2 = whole_sum2 << part_sum2;	 		/* -20 */ 	\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -21 */ 	\
    D2 = D2 | macro_tmp2;		/* left var */	/* -22 */ 	\
}

#define LB_RC5_ROUND3_PARTIAL2_1_SIMPLE(index,index2,Input_Array)	\
{									\
    part_sum = pipelined_read + C;			/* 1 */ 	\
  pipelined_read = Input_Array[((index) + 1)];		/* 1+ */ 	\
    whole_sum = D + part_sum;				/* 2 */ 	\
									\
    A = whole_sum << (3);				/* 3 */ 	\
    macro_tmp = whole_sum >> (29);			/* 4 */ 	\
    A = A | macro_tmp;			/* left 3 */ 	/* 5 */ 	\
									\
    part_sum2 = pipelined_read2 + C2;			/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
    whole_sum2 = D2 + part_sum2;			/* -2 + */ 	\
									\
    A2 = whole_sum2 << (3);				/* -3 */ 	\
    macro_tmp2 = whole_sum2 >> (29);			/* -4 */ 	\
    A2 = A2 | macro_tmp2;		/* left 3 */ 	/* -5 */ 	\
									\
      enc_word11 = enc_word11 ^ enc_word12;		/* 6 */		\
      right_dist = word_size - enc_word12;		/* 7 */ 	\
      macro_tmp = enc_word11 << enc_word12; 		/* 8 */ 	\
									\
      enc_word11 = enc_word11 >> right_dist;		/* 9 */ 	\
      enc_word11 = macro_tmp | enc_word11;/* left var */ /* 10 */ 	\
      enc_word11 = enc_word11 + A;			/* 11 */ 	\
									\
      enc_word21 = enc_word21 ^ enc_word22;		/* -6 */ 	\
      right_dist2 = word_size - enc_word22;		/* -7 */ 	\
      macro_tmp2 = enc_word21 << enc_word22; 		/* -8 */ 	\
									\
      enc_word21 = enc_word21 >> right_dist2;		/* -9 */ 	\
      enc_word21 = macro_tmp2 | enc_word21;/* left var */ /* -10 */ 	\
      enc_word21 = enc_word21 + A2;			/* -11 */ 	\
}


/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_2_SIMPLE(index,index2,Input_Array)	\
{									\
    part_sum = D + A;					/* 1 */ 	\
    whole_sum = B + part_sum;				/* 2 */ 	\
    right_dist = word_size - part_sum;			/* 3 */ 	\
									\
    B = whole_sum << part_sum;		 		/* 4 */ 	\
    macro_tmp = whole_sum >> right_dist;		/* 5 */ 	\
    B = B | macro_tmp;			/* left var */ 	/* 6 */ 	\
									\
    part_sum2 = D2 + A2;				/* -1 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -2 */ 	\
    right_dist2 = word_size - part_sum2;		/* -3 */ 	\
									\
    B2 = whole_sum2 << part_sum2;	 		/* -4 */ 	\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -5 */ 	\
    B2 = B2 | macro_tmp2;		/* left var */ 	/* -6 */ 	\
									\
    part_sum = pipelined_read + A;			/* 7 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 7+ */ 	\
    whole_sum = B + part_sum;				/* 8 */ 	\
									\
    C = whole_sum << (3);				/* 9 */ 	\
    macro_tmp = whole_sum >> (29);			/* 10 */ 	\
    C = C | macro_tmp;			/* left 3 */	/* 11 */ 	\
									\
    part_sum2 = pipelined_read2 + A2;			/* -7 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -7+ */ 	\
    whole_sum2 = B2 + part_sum2;			/* -8 */ 	\
									\
    C2 = whole_sum2 << (3);				/* -9 */ 	\
    macro_tmp2 = whole_sum2 >> (29);			/* -10 */ 	\
    C2 = C2 | macro_tmp2;		/* left 3 */	/* -11 */ 	\
									\
      enc_word12 = enc_word12 ^ enc_word11;		/* 12 */ 	\
      right_dist = word_size - enc_word11;		/* 13 */ 	\
      macro_tmp = enc_word12 << enc_word11; 		/* 14 */ 	\
									\
      enc_word12 = enc_word12 >> right_dist;		/* 15 */ 	\
      enc_word12 = macro_tmp | enc_word12;/* left var */ /* 16 */ 	\
      enc_word12 = enc_word12 + C;			/* 17 */ 	\
									\
      enc_word22 = enc_word22 ^ enc_word21;		/* -12 */ 	\
      right_dist2 = word_size - enc_word21;		/* -13 */ 	\
      macro_tmp2 = enc_word22 << enc_word21; 		/* -14 */ 	\
									\
      enc_word22 = enc_word22 >> right_dist2;		/* -15 */ 	\
      enc_word22 = macro_tmp2 | enc_word22;/* left var */ /* -16 */ 	\
      enc_word22 = enc_word22 + C2;			/* -17 */ 	\
									\
    part_sum = B + C;					/* 18 */ 	\
    whole_sum = D + part_sum;				/* 19 */ 	\
    right_dist = word_size - part_sum;			/* 20 */ 	\
									\
    D = whole_sum << part_sum;		 		/* 21 */ 	\
    macro_tmp = whole_sum >> right_dist;		/* 22 */ 	\
    D = D | macro_tmp;			/* left var */	/* 23 */ 	\
									\
    part_sum2 = B2 + C2;				/* -18 */ 	\
    whole_sum2 = D2 + part_sum2;			/* -19 */ 	\
    right_dist2 = word_size - part_sum2;		/* -20 */ 	\
									\
    D2 = whole_sum2 << part_sum2;	 		/* -21 */ 	\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -22 */ 	\
    D2 = D2 | macro_tmp2;		/* left var */	/* -23 */ 	\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */

#define LB_RC5_ROUND0_PARTIAL2_C					\
{									\
    A = _init;		       	/* 1 */ /* S[0] rotated left 3 by macro */ \
    A2 = A;			/* -1 */				\
									\
    pipelined_read2 = pipelined_read = S0_Source[1];			\
/*    S0_Dest[0] = A; */		/* no need to store not used */	\
/*    S0_Dest[30] = A2;	*/		/* no need to store not used */	\
									\
    whole_sum = L_0 + A;		/* 2 */ /* key first encryption */ \
    whole_sum2 = L_2 + A2;		/* -2 */ /* key second encryption */ \
									\
    macro_tmp = whole_sum << A;		/* 3 */				\
    right_dist = word_size - A;		/* 4 */				\
									\
    whole_sum = whole_sum >> right_dist; /* 5 */			\
    right_dist2 = word_size - A2;	/* -3 */			\
									\
    macro_tmp2 = whole_sum2 << A2;	/* -4 */			\
    B = macro_tmp | whole_sum;		/* 6 */				\
									\
    whole_sum2 = whole_sum2 >> right_dist2; /* -5 */			\
    part_sum = pipelined_read + A;	/* 7 */				\
  pipelined_read = S0_Source[2]; /* 7 + */		\
									\
    whole_sum = B + part_sum;		/* 8 */				\
    B2 = macro_tmp2 | whole_sum2;	/* -6 */			\
									\
    macro_tmp = whole_sum << (3);	/* 9 */				\
    part_sum2 = pipelined_read2 + A2;	/* -7 */			\
  pipelined_read2 = S0_Source[2]; /* -7 + */		\
									\
    whole_sum = whole_sum >> (29);	/* 10 */			\
    whole_sum2 = B2 + part_sum2;	/* -8 */			\
									\
    macro_tmp2 = whole_sum2 << (3);	/* -9 */			\
    C = macro_tmp | whole_sum;		/* 11 */			\
									\
    whole_sum2 = whole_sum2 >> (29);	/* -10 */			\
    part_sum = B + C;			/* 12 */			\
    S0_Dest[1] = C;			/* 11 + */			\
									\
    whole_sum = L_1 + part_sum;		/* 13 */ /* key first encryption */ \
									\
/* TOADS one extra clock loading constant, then adding to L_1 to get L_3 */ \
    INC_L_1_making_L_2;							\
									\
    C2 = macro_tmp2 | whole_sum2;	/* -11 */			\
									\
    right_dist = word_size - part_sum;	/* 14 */			\
    part_sum2 = B2 + C2;		/* -12 */			\
    S0_Dest[31] = C2;			/* -11 + */			\
									\
    macro_tmp = whole_sum << part_sum;	/* 15 */			\
    whole_sum2 = L_3 + part_sum2;	/* -13 */ /* key second encryption*/ \
									\
    whole_sum = whole_sum >> right_dist; /* 16 */			\
    right_dist2 = word_size - part_sum2; /* -14 */			\
									\
    macro_tmp2 = whole_sum2 << part_sum2; /* -15 */			\
    D = macro_tmp | whole_sum;		/* 17 */			\
									\
    whole_sum2 = whole_sum2 >> right_dist2; /* -16 */			\
    part_sum = pipelined_read + C;			/* 1 */		\
  pipelined_read = S0_Source[3];			/* 1+ */	\
									\
    D2 = macro_tmp2 | whole_sum2;	/* -17 */			\
    whole_sum = D + part_sum;				/* 2 */		\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */

#define LB_RC5_ROUND12_PARTIAL2_C(index, index2, Input_Array, Output_Array) \
{									\
/* these 3 instructions are moved to the end of the previous iteration*/ \
/*    part_sum = pipelined_read + C; */			/* 1 */ 	\
/*    pipelined_read = Input_Array[((index) + 1)]; */	/* 1+ */ 	\
									\
/*    whole_sum = D + part_sum;	*/			/* 2 */ 	\
									\
    A = whole_sum << (3);				/* 3 */ 	\
    part_sum2 = pipelined_read2 + C2;			/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
									\
    macro_tmp = whole_sum >> (29);			/* 4 */ 	\
    whole_sum2 = D2 + part_sum2;			/* -2 */ 	\
									\
    A2 = whole_sum2 << (3);				/* -3 */ 	\
    A = A | macro_tmp;			/* left 3 */ 	/* 5 */ 	\
									\
    macro_tmp2 = whole_sum2 >> (29);			/* -4 */ 	\
    part_sum = D + A;					/* 6 */ 	\
  Output_Array[(index)] = A;				/* 5+ */ 	\
									\
    A2 = A2 | macro_tmp2;		/* left 3 */ 	/* -5 */ 	\
    whole_sum = B + part_sum;				/* 7 */ 	\
									\
    part_sum2 = D2 + A2;				/* -6 */ 	\
    right_dist = word_size - part_sum;			/* 8 */ 	\
  Output_Array[(index2)] = A2;				/* -5+ */ 	\
									\
    B = whole_sum << part_sum; 				/* 9 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -7 */ 	\
									\
    macro_tmp = whole_sum >> right_dist;		/* 10 */ 	\
    right_dist2 = word_size - part_sum2;		/* -8 */ 	\
									\
    B2 = whole_sum2 << part_sum2; 			/* -9 */ 	\
    B = B | macro_tmp;			/* left var */ 	/* 11 */ 	\
									\
/* 1..11 is half, then 12..22 should be a duplicate */			\
									\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -10 */ 	\
    part_sum = pipelined_read + A;			/* 12 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 12+ */ 	\
									\
    B2 = B2 | macro_tmp2;		/* left var */ 	/* -11 */ 	\
    whole_sum = B + part_sum;				/* 13 */ 	\
									\
    C = whole_sum << (3);				/* 14 */ 	\
    part_sum2 = pipelined_read2 + A2;			/* -12 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -12+ */ 	\
									\
    macro_tmp = whole_sum >> (29);			/* 15 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -13 */ 	\
									\
    C2 = whole_sum2 << (3);				/* -14 */ 	\
    C = C | macro_tmp;			/* left 3 */	/* 16 */ 	\
									\
    macro_tmp2 = whole_sum2 >> (29);			/* -15 */ 	\
    part_sum = B + C;					/* 17 */ 	\
  Output_Array[((index) + 1)] = C;			/* 16+ */ 	\
									\
    whole_sum = D + part_sum;				/* 18 */ 	\
    C2 = C2 | macro_tmp2;		/* left 3 */	/* -16 */ 	\
									\
    right_dist = word_size - part_sum;			/* 19 */ 	\
    part_sum2 = B2 + C2;				/* -17 */ 	\
  Output_Array[((index2) + 1)] = C2;			/* -16+ */ 	\
									\
    D = whole_sum << part_sum;		 		/* 20 */ 	\
    whole_sum2 = D2 + part_sum2;			/* -18 */ 	\
									\
    macro_tmp = whole_sum >> right_dist;		/* 21 */ 	\
    right_dist2 = word_size - part_sum2;		/* -19 */ 	\
									\
    D2 = whole_sum2 << part_sum2;	 		/* -20 */ 	\
    D = D | macro_tmp;			/* left var */	/* 22 */ 	\
									\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -21 */ 	\
    part_sum = pipelined_read + C;			/* 1 */ 	\
  pipelined_read = Input_Array[((index) + 3)];		/* 1+ */ 	\
									\
    whole_sum = D + part_sum;				/* 2 */ 	\
    D2 = D2 | macro_tmp2;		/* left var */	/* -22 */ 	\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_END1_C(index,index2, Input_Array)	\
{									\
/* these 3 instructions are moved to the end of the previous iteration */ \
/*    part_sum = pipelined_read + C; */			/* 1 */ 	\
/*    pipelined_read = Input_Array[((index) + 1)]; */	/* 1+ */ 	\
									\
/*    whole_sum = D + part_sum;	 */			/* 2 */ 	\
									\
    A = whole_sum << (3);				/* 3 */ 	\
    part_sum2 = pipelined_read2 + C2;			/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
									\
    macro_tmp = whole_sum >> (29);			/* 4 */ 	\
    whole_sum2 = D2 + part_sum2;			/* -2 */ 	\
									\
    A2 = whole_sum2 << (3);				/* -3 */ 	\
    A = A | macro_tmp;			/* left 3 */ 	/* 5 */ 	\
									\
    macro_tmp2 = whole_sum2 >> (29);			/* -4 */ 	\
      enc_word11 = enc_word11 ^ enc_word12;		/* 6 */		\
									\
    A2 = A2 | macro_tmp2;		/* left 3 */ 	/* -5 */ 	\
      right_dist = word_size - enc_word12;		/* 7 */ 	\
									\
      macro_tmp = enc_word11 << enc_word12; 		/* 8 */ 	\
      enc_word21 = enc_word21 ^ enc_word22;		/* -6 */ 	\
									\
      enc_word11 = enc_word11 >> right_dist;		/* 9 */ 	\
      right_dist2 = word_size - enc_word22;		/* -7 */ 	\
									\
      macro_tmp2 = enc_word21 << enc_word22; 		/* -8 */ 	\
      enc_word11 = macro_tmp | enc_word11;/* left var */ /* 10 */ 	\
									\
      enc_word21 = enc_word21 >> right_dist2;		/* -9 */ 	\
      enc_word11 = enc_word11 + A;			/* 11 */ 	\
									\
      enc_word21 = macro_tmp2 | enc_word21;/* left var */ /* -10 */ 	\
									\
      enc_word21 = enc_word21 + A2;			/* -11 */ 	\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_END2_C(index,index2,Input_Array)		\
{									\
    part_sum = D + A;					/* 1 */ 	\
									\
    whole_sum = B + part_sum;				/* 2 */ 	\
									\
    part_sum2 = D2 + A2;				/* -1 */ 	\
    right_dist = word_size - part_sum;			/* 3 */ 	\
									\
    B = whole_sum << part_sum;		 		/* 4 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -2 */ 	\
									\
    macro_tmp = whole_sum >> right_dist;		/* 5 */ 	\
    right_dist2 = word_size - part_sum2;		/* -3 */ 	\
									\
    B2 = whole_sum2 << part_sum2;	 		/* -4 */ 	\
    B = B | macro_tmp;			/* left var */ 	/* 6 */ 	\
									\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -5 */ 	\
    part_sum = pipelined_read + A;			/* 7 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 7+ */ 	\
									\
    B2 = B2 | macro_tmp2;		/* left var */ 	/* -6 */ 	\
    whole_sum = B + part_sum;				/* 8 */ 	\
									\
    C = whole_sum << (3);				/* 9 */ 	\
    part_sum2 = pipelined_read2 + A2;			/* -7 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -7+ */ 	\
									\
    macro_tmp = whole_sum >> (29);			/* 10 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -8 */ 	\
									\
    C2 = whole_sum2 << (3);				/* -9 */ 	\
    C = C | macro_tmp;			/* left 3 */	/* 11 */ 	\
									\
    macro_tmp2 = whole_sum2 >> (29);			/* -10 */ 	\
      enc_word12 = enc_word12 ^ enc_word11;		/* 12 */ 	\
									\
    C2 = C2 | macro_tmp2;		/* left 3 */	/* -11 */ 	\
      right_dist = word_size - enc_word11;		/* 13 */ 	\
									\
      macro_tmp = enc_word12 << enc_word11; 		/* 14 */ 	\
      enc_word22 = enc_word22 ^ enc_word21;		/* -12 */ 	\
									\
      enc_word12 = enc_word12 >> right_dist;		/* 15 */ 	\
      right_dist2 = word_size - enc_word21;		/* -13 */ 	\
									\
      macro_tmp2 = enc_word22 << enc_word21; 		/* -14 */ 	\
      enc_word12 = macro_tmp | enc_word12;/* left var */ /* 16 */ 	\
									\
      enc_word22 = enc_word22 >> right_dist2;		/* -15 */ 	\
      enc_word12 = enc_word12 + C;			/* 17 */ 	\
									\
      enc_word22 = macro_tmp2 | enc_word22;/* left var */ /* -16 */ 	\
    part_sum = B + C;					/* 18 */ 	\
									\
      enc_word22 = enc_word22 + C2;			/* -17 */ 	\
    whole_sum = D + part_sum;				/* 19 */ 	\
									\
    right_dist = word_size - part_sum;			/* 20 */ 	\
    part_sum2 = B2 + C2;				/* -18 */ 	\
									\
    D = whole_sum << part_sum;		 		/* 21 */ 	\
    whole_sum2 = D2 + part_sum2;			/* -19 */ 	\
									\
    macro_tmp = whole_sum >> right_dist;		/* 22 */ 	\
    right_dist2 = word_size - part_sum2;		/* -20 */ 	\
									\
    D2 = whole_sum2 << part_sum2;	 		/* -21 */ 	\
    D = D | macro_tmp;			/* left var */	/* 23 */ 	\
									\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -21 */ 	\
    part_sum = pipelined_read + C;			/* 1 */ 	\
  pipelined_read = Input_Array[((index) + 3)];		/* 1+ */ 	\
									\
    whole_sum = D + part_sum;				/* 2 */ 	\
    D2 = D2 | macro_tmp2;		/* left var */	/* -22 */ 	\
}

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_C(index,index2,Input_Array)	\
{   LB_RC5_ROUND3_PARTIAL2_END1_C(index,index2,Input_Array);		\
    LB_RC5_ROUND3_PARTIAL2_END2_C(index,index2,Input_Array);		\
}

#define LB_RC5_ROUND3_PARTIAL2_NO_XOR_C(index,index2,Input_Array)	\
    LB_RC5_ROUND3_PARTIAL2_C(index,index2,Input_Array)


#define ASM_ADD(res, op1, op2)		\
  __asm__ volatile ("add %1, %2, %0" : "=r" (res) : "r" (op1), "r" (op2));
#define ASM_SUB(res, opp1, opm2)	\
  __asm__ volatile ("sub %1, %2, %0" : "=r" (res) : "rc" (opp1), "rc" (opm2));
#define ASM_OR(res, op1, op2)		\
  __asm__ volatile ("or %1, %2, %0" : "=r" (res) : "r" (op1), "r" (op2));
#define ASM_XOR(res, op1, op2)	\
  __asm__ volatile ("xor %1, %2, %0" : "=r" (res) : "r" (op1), "r" (op2));
#define ASM_SL(res, op, dist)		\
  __asm__ volatile ("sll %1, %2, %0" : "=r" (res) : "r" (op), "rc" (dist));
#define ASM_SR(res, op, dist)		\
  __asm__ volatile ("srl %1, %2, %0" : "=r" (res) : "r" (op), "rc" (dist));
#define ASM_SL3(res, op)		\
  __asm__ volatile ("sll %1, 3, %0" : "=r" (res) : "r" (op));
#define ASM_SR29(res, op)		\
  __asm__ volatile ("srl %1, 29, %0" : "=r" (res) : "r" (op));
#define ASM_LOAD(res, addr)		\
  __asm__ volatile ("ld [%1], %0" : "=r" (res) : "m" (addr));
#define ASM_STORE(val, addr)		\
  __asm__ volatile ("st %0, [%1]" : : "=r" (res), "o" (addr));


/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */

#define LB_RC5_ROUND0_PARTIAL2_A					\
{									\
    A = _init;		       	/* 1 */ /* S[0] rotated left 3 by macro */ \
    A2 = A;			/* -1 */				\
									\
    pipelined_read2 = pipelined_read = S0_Source[1];			\
/*    S0_Dest[0] = A; */		/* no need to store not used */	\
/*    S0_Dest[30] = A2;	*/		/* no need to store not used */	\
									\
    ASM_ADD (whole_sum, L_0, A);	/* 2 */ /* key first encryption */ \
    ASM_ADD (whole_sum2, L_2, A2);	/* -2 */ /* key second encryption */ \
									\
    ASM_SL (macro_tmp, whole_sum, A);	/* 3 */				\
    ASM_SUB (right_dist, word_size, A);	/* 4 */				\
									\
    ASM_SR (whole_sum, whole_sum, right_dist); /* 5 */			\
    ASM_SUB (right_dist2, word_size, A2);/* -3 */			\
									\
    ASM_SL (macro_tmp2, whole_sum2, A2);/* -4 */			\
    ASM_OR (B, macro_tmp, whole_sum);	/* 6 */				\
									\
    ASM_SR (whole_sum2, whole_sum2, right_dist2); /* -5 */		\
    ASM_ADD (part_sum, pipelined_read, A);/* 7 */			\
  pipelined_read = S0_Source[2];	/* 7 + */		\
									\
    ASM_ADD (whole_sum, B, part_sum);	/* 8 */				\
    ASM_OR (B2, macro_tmp2, whole_sum2);/* -6 */			\
									\
    ASM_SL3 (macro_tmp, whole_sum);	/* 9 */				\
    ASM_ADD (part_sum2, pipelined_read2, A2);/* -7 */			\
  pipelined_read2 = S0_Source[2]; /* -7 + */		\
									\
    ASM_SR29 (whole_sum, whole_sum);	/* 10 */			\
    ASM_ADD (whole_sum2, B2, part_sum2);/* -8 */			\
									\
    ASM_SL3 (macro_tmp2, whole_sum2);	/* -9 */			\
    ASM_OR (C, macro_tmp, whole_sum);	/* 11 */			\
									\
    ASM_SR29 (whole_sum2, whole_sum2);	/* -10 */			\
    ASM_ADD (part_sum, B, C);		/* 12 */			\
    S0_Dest[1] = C;			/* 11 + */			\
									\
    ASM_ADD (whole_sum, L_1, part_sum);	/* 13 */ /* key first encryption */ \
									\
/* TOADS one extra clock loading constant, then adding to L_1 to get L_3 */ \
/* This specialization means that this routine can only test key and key + 1 */ \
/* This also means that opportunities are being missed to cache the results */ \
/* of the above calculations between tests, as B, B2, C, C2, part_sum, */ \
/* part_sum2, S0_Dest[1], S0_Dest[31], rignt_dist, right_dist2 are constant */ \
    INC_L_1_making_L_2;							\
									\
    ASM_OR (C2, macro_tmp2, whole_sum2);/* -11 */			\
									\
    ASM_SUB (right_dist, word_size, part_sum);/* 14 */			\
    ASM_ADD (part_sum2, B2, C2);	/* -12 */			\
    S0_Dest[31] = C2;			/* -11 + */			\
									\
    ASM_SL (macro_tmp, whole_sum, part_sum);/* 15 */			\
    ASM_ADD (whole_sum2, L_3, part_sum2);/* -13 */ /* key second encryption*/ \
									\
    ASM_SR (whole_sum, whole_sum, right_dist); /* 16 */			\
    ASM_SUB (right_dist2, word_size, part_sum2);/* -14 */		\
									\
    ASM_SL (macro_tmp2, whole_sum2, part_sum2);/* -15 */		\
    ASM_OR (D, macro_tmp, whole_sum);	/* 17 */			\
									\
/* In order to force dove-tailing of routines, take out the first 2 un-paired \
 * instructions of the routine.  Duplicate here.  THEN add them in at the end \
 * of the routine, paired.						\
 *									\
 * This will ALSO need to fix things up on the transition between	\
 * first pass and second, second and third, and going into last round	\
 */									\
    ASM_SR (whole_sum2, whole_sum2, right_dist2);	/* -16 */	\
    ASM_ADD (part_sum, pipelined_read, C);		/* 1 */		\
  pipelined_read = S0_Source[3];			/* 1+ */	\
									\
    ASM_OR (D2, macro_tmp2, whole_sum2);		/* -17 */	\
    ASM_ADD (whole_sum, D, part_sum);			/* 2 */		\
									\
/* end of tricky pipelined code fixup */				\
}

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */

#define LB_RC5_ROUND12_PARTIAL2_A(index, index2, Input_Array, Output_Array) \
{									\
/* these 3 instructions are moved to the end of the previous iteration*/ \
/*    part_sum = pipelined_read + C; */			/* 1 */ 	\
/*    pipelined_read = Input_Array[((index) + 1)]; */	/* 1+ */ 	\
									\
/*    whole_sum = D + part_sum;	*/			/* 2 */ 	\
									\
    ASM_SL3 (A, whole_sum);				/* 3 */ 	\
    ASM_ADD (part_sum2, pipelined_read2, C2);		/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
									\
/* before this shift, would have to mask whole_sum to 32 bits */	\
    ASM_SR29 (macro_tmp, whole_sum);			/* 4 */ 	\
    ASM_ADD (whole_sum2, D2, part_sum2);		/* -2 */ 	\
									\
    ASM_SL3 (A2, whole_sum2);				/* -3 */ 	\
    ASM_OR (A, A, macro_tmp);		/* left 3 */ 	/* 5 */ 	\
									\
/* before this shift, would have to mask whole_sum to 32 bits */	\
    ASM_SR29 (macro_tmp2, whole_sum2);			/* -4 */ 	\
    ASM_ADD (part_sum, D, A);				/* 6 */ 	\
  Output_Array[(index)] = A;				/* 5+ */ 	\
									\
    ASM_ADD (whole_sum, B, part_sum);			/* 7 */ 	\
    ASM_OR (A2, A2, macro_tmp2);	/* left 3 */ 	/* -5 */ 	\
									\
/* before this sub, would have to mask part_sum to 5 bits for shift */	\
    ASM_SUB (right_dist, word_size, part_sum);		/* 8 */ 	\
    ASM_ADD (part_sum2, D2, A2);			/* -6 */ 	\
  Output_Array[(index2)] = A2;				/* -5+ */ 	\
									\
    ASM_SL (B, whole_sum, part_sum);			/* 9 */ 	\
    ASM_ADD (whole_sum2, B2, part_sum2);		/* -7 */ 	\
									\
/* before this sub, would have to mask part_sum2 to 5 bits for shift */	\
/* before this shift, would have to mask whole_sum to 32 bits */	\
    ASM_SR (macro_tmp, whole_sum, right_dist);		/* 10 */ 	\
    ASM_SUB (right_dist2, word_size, part_sum2);	/* -8 */ 	\
									\
    ASM_SL (B2, whole_sum2, part_sum2);			/* -9 */ 	\
    ASM_OR (B, B, macro_tmp);		/* left var */ 	/* 11 */ 	\
									\
/* 1..11 is half, then 12..22 should be a duplicate */			\
									\
/* before this shift, would have to mask whole_sum2 to 32 bits */	\
    ASM_SR (macro_tmp2, whole_sum2, right_dist2);	/* -10 */ 	\
    ASM_ADD (part_sum, pipelined_read, A);		/* 12 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 12+ */ 	\
									\
    ASM_OR (B2, B2, macro_tmp2);	/* left var */ 	/* -11 */ 	\
    ASM_ADD (whole_sum, B, part_sum);			/* 13 */ 	\
									\
    ASM_SL3 (C, whole_sum);				/* 14 */ 	\
    ASM_ADD (part_sum2, pipelined_read2, A2);		/* -12 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -12+ */ 	\
									\
/* before this shift, would have to mask whole_sum2 to 32 bits */	\
    ASM_SR29 (macro_tmp, whole_sum);			/* 15 */ 	\
    ASM_ADD (whole_sum2, B2, part_sum2);		/* -13 */ 	\
									\
    ASM_SL3 (C2, whole_sum2);				/* -14 */ 	\
    ASM_OR (C, C, macro_tmp);		/* left 3 */	/* 16 */ 	\
									\
/* before this shift, would have to mask whole_sum2 to 32 bits */	\
    ASM_SR29 (macro_tmp2, whole_sum2);			/* -15 */ 	\
    ASM_ADD (part_sum, B, C);				/* 17 */ 	\
  Output_Array[((index) + 1)] = C;			/* 16+ */ 	\
									\
    ASM_OR (C2, C2, macro_tmp2);	/* left 3 */	/* -16 */ 	\
    ASM_ADD (whole_sum, D, part_sum);			/* 18 */ 	\
									\
/* before this sub, would have to mask part_sum to 5 bits for shift */	\
    ASM_ADD (part_sum2, B2, C2);			/* -17 */ 	\
    ASM_SUB (right_dist, word_size, part_sum);		/* 19 */ 	\
  Output_Array[((index2) + 1)] = C2;			/* -16+ */ 	\
									\
    ASM_SL (D, whole_sum, part_sum);		 	/* 20 */ 	\
    ASM_ADD (whole_sum2, D2, part_sum2);		/* -18 */ 	\
									\
/* before this sub, would have to mask part_sum2 to 5 bits for shift */	\
/* before this shift, would have to mask whole_sum to 32 bits */	\
    ASM_SR (macro_tmp, whole_sum, right_dist);		/* 21 */ 	\
    ASM_SUB (right_dist2, word_size, part_sum2);	/* -19 */ 	\
									\
    ASM_SL (D2, whole_sum2, part_sum2);	 		/* -20 */ 	\
    ASM_OR (D, D, macro_tmp);		/* left var */	/* 22 */ 	\
									\
/* note 3 instructions brought back from end of next iteration */	\
									\
/* before this shift, would have to mask whole_sum2 to 32 bits */	\
    ASM_SR (macro_tmp2, whole_sum2, right_dist2);	/* -21 */ 	\
    ASM_ADD (part_sum, pipelined_read, C);		/* 1 */ 	\
  pipelined_read = Input_Array[((index) + 3)];		/* 1+ */ 	\
									\
    ASM_ADD (whole_sum, D, part_sum);			/* 2 */ 	\
    ASM_OR (D2, D2, macro_tmp2);	/* left var */	/* -22 */ 	\
									\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_A(index,index2,Input_Array)		\
{									\
/* these 3 instructions are moved to the end of the previous iteration*/ \
/*    ASM_ADD (part_sum, pipelined_read, C); */		/* 1 */ 	\
/*  pipelined_read = Input_Array[((index) + 1)]; */	/* 1+ */ 	\
									\
/*    ASM_ADD (whole_sum, D, part_sum);	*/		/* 2 */ 	\
									\
    ASM_SL3 (A, whole_sum);				/* 3 */ 	\
    ASM_ADD (part_sum2, pipelined_read2, C2);		/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
									\
    ASM_SR29 (macro_tmp, whole_sum);			/* 4 */ 	\
    ASM_ADD (whole_sum2, D2, part_sum2);		/* -2 */ 	\
									\
    ASM_SL3 (A2, whole_sum2);				/* -3 */ 	\
    ASM_OR (A, A, macro_tmp);		/* left 3 */ 	/* 5 */ 	\
									\
    ASM_SR29 (macro_tmp2, whole_sum2);			/* -4 */ 	\
      ASM_XOR (enc_word11, enc_word11, enc_word12);	/* 6 */		\
									\
    ASM_OR (A2, A2, macro_tmp2);	/* left 3 */ 	/* -5 */ 	\
      ASM_SUB (right_dist, word_size, enc_word12);	/* 7 */ 	\
									\
      ASM_SL (macro_tmp, enc_word11, enc_word12);	/* 8 */ 	\
      ASM_XOR (enc_word21, enc_word21, enc_word22);	/* -6 */ 	\
									\
      ASM_SR (enc_word11, enc_word11, right_dist);	/* 9 */ 	\
      ASM_SUB (right_dist2, word_size, enc_word22);	/* -7 */ 	\
									\
      ASM_SL (macro_tmp2, enc_word21, enc_word22);	/* -8 */ 	\
      ASM_OR (enc_word11, macro_tmp, enc_word11); /* left var */ /* 10 */ \
									\
      ASM_SR (enc_word21, enc_word21, right_dist2);	/* -9 */ 	\
      ASM_ADD (enc_word11, enc_word11, A);		/* 11 */ 	\
									\
    ASM_ADD (part_sum, D, A);				/* 1 */ 	\
      ASM_OR (enc_word21, macro_tmp2, enc_word21); /* left var */ /* -10 */ \
									\
    ASM_ADD (whole_sum, B, part_sum);			/* 2 */ 	\
      ASM_ADD (enc_word21, enc_word21, A2);		/* -11 */ 	\
									\
    ASM_ADD (part_sum2, D2, A2);			/* -1 */ 	\
    ASM_SUB (right_dist, word_size, part_sum);		/* 3 */ 	\
									\
    ASM_SL (B, whole_sum, part_sum);	 		/* 4 */ 	\
    ASM_ADD (whole_sum2, B2, part_sum2);		/* -2 */ 	\
									\
    ASM_SR (macro_tmp, whole_sum, right_dist);		/* 5 */ 	\
    ASM_SUB (right_dist2, word_size, part_sum2);	/* -3 */ 	\
									\
    ASM_SL (B2, whole_sum2, part_sum2); 		/* -4 */ 	\
    ASM_OR (B, B, macro_tmp);		/* left var */ 	/* 6 */ 	\
									\
    ASM_SR (macro_tmp2, whole_sum2, right_dist2);	/* -5 */ 	\
    ASM_ADD (part_sum, pipelined_read, A);		/* 7 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 7+ */ 	\
									\
    ASM_OR (B2, B2, macro_tmp2);	/* left var */ 	/* -6 */ 	\
    ASM_ADD (whole_sum, B, part_sum);			/* 8 */ 	\
									\
    ASM_SL3 (C, whole_sum);				/* 9 */ 	\
    ASM_ADD (part_sum2, pipelined_read2, A2);		/* -7 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -7+ */ 	\
									\
    ASM_SR29 (macro_tmp, whole_sum);			/* 10 */ 	\
    ASM_ADD (whole_sum2, B2, part_sum2);		/* -8 */ 	\
									\
    ASM_SL3 (C2, whole_sum2);				/* -9 */ 	\
    ASM_OR (C, C, macro_tmp);		/* left 3 */	/* 11 */ 	\
									\
    ASM_SR29 (macro_tmp2, whole_sum2);			/* -10 */ 	\
      ASM_XOR (enc_word12, enc_word12, enc_word11);	/* 12 */ 	\
									\
    ASM_OR (C2, C2, macro_tmp2);	/* left 3 */	/* -11 */ 	\
      ASM_SUB (right_dist, word_size, enc_word11);	/* 13 */ 	\
									\
      ASM_SL (macro_tmp, enc_word12, enc_word11);	/* 14 */ 	\
      ASM_XOR (enc_word22, enc_word22, enc_word21);	/* -12 */ 	\
									\
      ASM_SR (enc_word12, enc_word12, right_dist);	/* 15 */ 	\
      ASM_SUB (right_dist2, word_size, enc_word21);	/* -13 */ 	\
									\
      ASM_SL (macro_tmp2, enc_word22, enc_word21);	/* -14 */ 	\
      ASM_OR (enc_word12, macro_tmp, enc_word12); /* left var */ /* 16 */ \
									\
      ASM_SR (enc_word22, enc_word22, right_dist2);	/* -15 */ 	\
      ASM_ADD (enc_word12, enc_word12, C);		/* 17 */ 	\
									\
      ASM_OR (enc_word22, macro_tmp2, enc_word22); /* left var */ /* -16 */ \
    ASM_ADD (part_sum, B, C);				/* 18 */ 	\
									\
      ASM_ADD (enc_word22, enc_word22, C2);		/* -17 */ 	\
    ASM_ADD (whole_sum, D, part_sum);			/* 19 */ 	\
									\
    ASM_SUB (right_dist, word_size, part_sum);		/* 20 */ 	\
    ASM_ADD (part_sum2, B2, C2);			/* -18 */ 	\
									\
    ASM_SL (D, whole_sum, part_sum);	 		/* 21 */ 	\
    ASM_ADD (whole_sum2, D2, part_sum2);		/* -19 */ 	\
									\
    ASM_SR (macro_tmp, whole_sum, right_dist);		/* 22 */ 	\
    ASM_SUB (right_dist2, word_size, part_sum2);	/* -20 */ 	\
									\
    ASM_SL (D2, whole_sum2, part_sum2); 		/* -21 */ 	\
    ASM_OR (D, D, macro_tmp);		/* left var */	/* 23 */ 	\
									\
    ASM_SR (macro_tmp2, whole_sum2, right_dist2);	/* -22 */ 	\
    ASM_ADD (part_sum, pipelined_read, C);		/* 1 */ 	\
  pipelined_read = Input_Array[((index) + 3)];		/* 1+ */ 	\
									\
    ASM_ADD (whole_sum, D, part_sum);			/* 2 */ 	\
    ASM_OR (D2, D2, macro_tmp2);	/* left var */	/* -23 */ 	\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_NO_XOR_A(index,index2,Input_Array)		\
{									\
/* these 3 instructions are moved to the end of the previous iteration */ \
/*    ASM_ADD (part_sum, pipelined_read, C); */		/* 1 */ 	\
/*  pipelined_read = Input_Array[((index) + 1)]; */	/* 1+ */ 	\
									\
/*    ASM_ADD (whole_sum, D, part_sum);	*/		/* 2 */ 	\
									\
    ASM_SL3 (A, whole_sum);				/* 3 */ 	\
    ASM_ADD (part_sum2, pipelined_read2, C2);		/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
									\
    ASM_SR29 (macro_tmp, whole_sum);			/* 4 */ 	\
    ASM_ADD (whole_sum2, D2, part_sum2);		/* -2 */ 	\
									\
    ASM_SL3 (A2, whole_sum2);				/* -3 */ 	\
    ASM_OR (A, A, macro_tmp);		/* left 3 */ 	/* 5 */ 	\
									\
    ASM_SR29 (macro_tmp2, whole_sum2);			/* -4 */ 	\
    ASM_ADD (part_sum, D, A);				/* 1 */ 	\
									\
    ASM_OR (A2, A2, macro_tmp2);	/* left 3 */ 	/* -5 */ 	\
    ASM_ADD (whole_sum, B, part_sum);			/* 2 */ 	\
									\
    ASM_ADD (part_sum2, D2, A2);			/* -1 */ 	\
    ASM_SUB (right_dist, word_size, part_sum);		/* 3 */ 	\
									\
    ASM_SL (B, whole_sum, part_sum);	 		/* 4 */ 	\
    ASM_ADD (whole_sum2, B2, part_sum2);		/* -2 */ 	\
									\
    ASM_SR (macro_tmp, whole_sum, right_dist);		/* 5 */ 	\
    ASM_SUB (right_dist2, word_size, part_sum2);	/* -3 */ 	\
									\
    ASM_SL (B2, whole_sum2, part_sum2); 		/* -4 */ 	\
    ASM_OR (B, B, macro_tmp);		/* left var */ 	/* 6 */ 	\
									\
    ASM_SR (macro_tmp2, whole_sum2, right_dist2);	/* -5 */ 	\
    ASM_ADD (part_sum, pipelined_read, A);		/* 7 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 7+ */ 	\
									\
    ASM_OR (B2, B2, macro_tmp2);	/* left var */ 	/* -6 */ 	\
    ASM_ADD (whole_sum, B, part_sum);			/* 8 */ 	\
									\
    ASM_SL3 (C, whole_sum);				/* 9 */ 	\
    ASM_ADD (part_sum2, pipelined_read2, A2);		/* -7 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -7+ */ 	\
									\
    ASM_SR29 (macro_tmp, whole_sum);			/* 10 */ 	\
    ASM_ADD (whole_sum2, B2, part_sum2);		/* -8 */ 	\
									\
    ASM_SL3 (C2, whole_sum2);				/* -9 */ 	\
    ASM_OR (C, C, macro_tmp);		/* left 3 */	/* 11 */ 	\
									\
    ASM_SR29 (macro_tmp2, whole_sum2);			/* -10 */ 	\
    ASM_ADD (part_sum, B, C);				/* 18 */ 	\
									\
    ASM_OR (C2, C2, macro_tmp2);	/* left 3 */	/* -11 */ 	\
    ASM_ADD (whole_sum, D, part_sum);			/* 19 */ 	\
									\
    ASM_SUB (right_dist, word_size, part_sum);		/* 20 */ 	\
    ASM_ADD (part_sum2, B2, C2);			/* -18 */ 	\
									\
    ASM_SL (D, whole_sum, part_sum);	 		/* 21 */ 	\
    ASM_ADD (whole_sum2, D2, part_sum2);		/* -19 */ 	\
									\
    ASM_SR (macro_tmp, whole_sum, right_dist);		/* 22 */ 	\
    ASM_SUB (right_dist2, word_size, part_sum2);	/* -20 */ 	\
									\
    ASM_SL (D2, whole_sum2, part_sum2); 		/* -21 */ 	\
    ASM_OR (D, D, macro_tmp);		/* left var */	/* 23 */ 	\
									\
    ASM_SR (macro_tmp2, whole_sum2, right_dist2);	/* -22 */ 	\
    ASM_ADD (part_sum, pipelined_read, C);		/* 1 */ 	\
  pipelined_read = Input_Array[((index) + 3)];		/* 1+ */ 	\
									\
    ASM_ADD (whole_sum, D, part_sum);			/* 2 */ 	\
    ASM_OR (D2, D2, macro_tmp2);	/* left var */	/* -23 */ 	\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_END1_A(index,index2,Input_Array)	\
{									\
/* these 3 instructions are moved to the end of the previous iteration */ \
/*    part_sum = pipelined_read + C; */			/* 1 */ 	\
/*    pipelined_read = Input_Array[((index) + 1)]; */	/* 1+ */ 	\
									\
/*    whole_sum = D + part_sum;	*/			/* 2 */ 	\
									\
    ASM_SL3 (A, whole_sum);				/* 3 */ 	\
    ASM_ADD (part_sum2, pipelined_read2, C2);		/* -1 */ 	\
  pipelined_read2 = Input_Array[((index2) + 1)];	/* -1+ */ 	\
									\
    ASM_SR29 (macro_tmp, whole_sum);			/* 4 */ 	\
    ASM_ADD (whole_sum2, D2, part_sum2);		/* -2 */ 	\
									\
    ASM_SL3 (A2, whole_sum2);				/* -3 */ 	\
    ASM_OR (A, A, macro_tmp);		/* left 3 */ 	/* 5 */ 	\
									\
    ASM_SR29 (macro_tmp2, whole_sum2);			/* -4 */ 	\
      ASM_XOR (enc_word11, enc_word11, enc_word12);	/* 6 */		\
									\
    ASM_OR (A2, A2, macro_tmp2);	/* left 3 */ 	/* -5 */ 	\
      ASM_SUB (right_dist, word_size, enc_word12);	/* 7 */ 	\
									\
      ASM_SL (macro_tmp, enc_word11, enc_word12);	/* 8 */ 	\
      ASM_XOR (enc_word21, enc_word21, enc_word22);	/* -6 */ 	\
									\
      ASM_SR (enc_word11, enc_word11, right_dist);	/* 9 */ 	\
      ASM_SUB (right_dist2, word_size, enc_word22);	/* -7 */ 	\
									\
      ASM_SL (macro_tmp2, enc_word21, enc_word22);	/* -8 */ 	\
      ASM_OR (enc_word11, macro_tmp, enc_word11); /* left var */ /* 10 */ \
									\
      ASM_SR (enc_word21, enc_word21, right_dist2);	/* -9 */ 	\
      ASM_ADD (enc_word11, enc_word11, A);		/* 11 */ 	\
									\
      enc_word21 = macro_tmp2 | enc_word21; /* left var */ /* -10 */	\
									\
      enc_word21 = enc_word21 + A2;			/* -11 */ 	\
}

/* NOTE cycle counts are fake, because I don't have an Ultra book */

/* precondition: C, D valid upon entry */
/* precondition: pipelined_read valid upon entry */
/* precondition: enc_word11, enc_word12 valid upon entry */

#define LB_RC5_ROUND3_PARTIAL2_END2_A(index,index2,Input_Array)	\
{									\
    part_sum = D + A;					/* 1 */ 	\
									\
    whole_sum = B + part_sum;				/* 2 */ 	\
									\
    part_sum2 = D2 + A2;				/* -1 */ 	\
    right_dist = word_size - part_sum;			/* 3 */ 	\
									\
    B = whole_sum << part_sum;		 		/* 4 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -2 */ 	\
									\
    macro_tmp = whole_sum >> right_dist;		/* 5 */ 	\
    right_dist2 = word_size - part_sum2;		/* -3 */ 	\
									\
    B2 = whole_sum2 << part_sum2;	 		/* -4 */ 	\
    B = B | macro_tmp;			/* left var */ 	/* 6 */ 	\
									\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -5 */ 	\
    part_sum = pipelined_read + A;			/* 7 */ 	\
  pipelined_read = Input_Array[(index) + 2];		/* 7+ */ 	\
									\
    B2 = B2 | macro_tmp2;		/* left var */ 	/* -6 */ 	\
    whole_sum = B + part_sum;				/* 8 */ 	\
									\
    C = whole_sum << (3);				/* 9 */ 	\
    part_sum2 = pipelined_read2 + A2;			/* -7 */ 	\
  pipelined_read2 = Input_Array[(index2) + 2];		/* -7+ */ 	\
									\
    macro_tmp = whole_sum >> (29);			/* 10 */ 	\
    whole_sum2 = B2 + part_sum2;			/* -8 */ 	\
									\
    C2 = whole_sum2 << (3);				/* -9 */ 	\
    C = C | macro_tmp;			/* left 3 */	/* 11 */ 	\
									\
    macro_tmp2 = whole_sum2 >> (29);			/* -10 */ 	\
      enc_word12 = enc_word12 ^ enc_word11;		/* 12 */ 	\
									\
    C2 = C2 | macro_tmp2;		/* left 3 */	/* -11 */ 	\
      right_dist = word_size - enc_word11;		/* 13 */ 	\
									\
      macro_tmp = enc_word12 << enc_word11; 		/* 14 */ 	\
      enc_word22 = enc_word22 ^ enc_word21;		/* -12 */ 	\
									\
      enc_word12 = enc_word12 >> right_dist;		/* 15 */ 	\
      right_dist2 = word_size - enc_word21;		/* -13 */ 	\
									\
      macro_tmp2 = enc_word22 << enc_word21; 		/* -14 */ 	\
      enc_word12 = macro_tmp | enc_word12;/* left var */ /* 16 */ 	\
									\
      enc_word22 = enc_word22 >> right_dist2;		/* -15 */ 	\
      enc_word12 = enc_word12 + C;			/* 17 */ 	\
									\
      enc_word22 = macro_tmp2 | enc_word22;/* left var */ /* -16 */ 	\
    part_sum = B + C;					/* 18 */ 	\
									\
      enc_word22 = enc_word22 + C2;			/* -17 */ 	\
    whole_sum = D + part_sum;				/* 19 */ 	\
									\
    right_dist = word_size - part_sum;			/* 20 */ 	\
    part_sum2 = B2 + C2;				/* -18 */ 	\
									\
    D = whole_sum << part_sum;		 		/* 21 */ 	\
    whole_sum2 = D2 + part_sum2;			/* -19 */ 	\
									\
    macro_tmp = whole_sum >> right_dist;		/* 22 */ 	\
    right_dist2 = word_size - part_sum2;		/* -20 */ 	\
									\
    D2 = whole_sum2 << part_sum2;	 		/* -21 */ 	\
    D = D | macro_tmp;			/* left var */	/* 23 */ 	\
									\
    macro_tmp2 = whole_sum2 >> right_dist2;		/* -22 */ 	\
									\
    D2 = D2 | macro_tmp2;		/* left var */	/* -23 */ 	\
}

/* code from Butcher starts here */

const char *LB_VERSION(void) {
return "Lawrence Butcher 16/9/97 lbutcher@eng.sun.com  LB Version 1.3"; }

/* an array of constants that can be LOADed instead of SETHI/ORI'd */
static u32 S0_INIT[60] =  { (SHL(S0_00,3) | SHR(S0_00,3)), S0_01,
	S0_02, S0_03, S0_04, S0_05, S0_06, S0_07, S0_08, S0_09,
	S0_10, S0_11, S0_12, S0_13, S0_14, S0_15, S0_16, S0_17,
	S0_18, S0_19, S0_20, S0_21, S0_22, S0_23, S0_24, S0_25,
	(SHL(S0_00,3) | SHR(S0_00,3)), 0, 0, 0,
				 (SHL(S0_00,3) | SHR(S0_00,3)), S0_01,
	S0_02, S0_03, S0_04, S0_05, S0_06, S0_07, S0_08, S0_09,
	S0_10, S0_11, S0_12, S0_13, S0_14, S0_15, S0_16, S0_17,
	S0_18, S0_19, S0_20, S0_21, S0_22, S0_23, S0_24, S0_25,
	(SHL(S0_00,3) | SHR(S0_00,3)), 0, 0, 0}; /* duplicate first near end */


/* these are how to access variables in C++ version
 *    register u32 Llo1, Lhi1;
 *    register u32 Llo2, Lhi2;
 *
 * pick up one key, derive second key from that, was called L_0, L_1, L_2, L_3
 *  Llo2 = Llo1 = rc5unitwork->L0.lo;
 *  Lhi2 = (Lhi1 = rc5unitwork->L0.hi) + 0x01000000;
 *
 * pick up plaintext for encryption, old code calls this P_0 and P_1
 *  eA1 = rc5unitwork->plain.lo + (A1 = ROTL3(S1_00 + Lhi1 + S1_25));
 *  eB1 = rc5unitwork->plain.hi + (A1 = ROTL3(S1_01 + Llo1 + A1));
 *
 * pick up desired encryption to check for match, old code calls this C_0, C_1
 *  if (rc5unitwork->cypher.lo == eA1 &&
 *	    rc5unitwork->cypher.hi == ROTL(eB1 ^ eA1, eA1) +
 */

/* use these defines to replace global and argument references in the V1 code,
 * allowing me to reuse source code without adding bugs
 */
#define L_0 enc_word21
#define L_1 enc_word22
//#define INC_L_1_making_L_2 {enc_word22 = enc_word22 + 0x00010000;}
#define INC_L_1_making_L_2 {enc_word22 = enc_word22 + 0x01000000;}
#define L_2 enc_word21
#define L_3 enc_word22	/* post-inc */
#define P_0 rc5unitwork->plain.lo
#define P_1 rc5unitwork->plain.hi
#define C_0 rc5unitwork->cypher.lo
#define C_1 rc5unitwork->cypher.hi


/* args if this is run in old client
 *int RC5_KEY_CHECK_INNER (u32 L_0, u32 L_1,
 *			 u32 L_2, u32 L_3)
 */

/* but NOOO, much fancier to use C++ */
static /* __inline__ */
u32 crunch(register RC5UnitWork * rc5unitwork, u32 iterations )
{
    u32 S0_Storage[30 + 30];	/* intermediate key values is RAM */
    u32 S0_Storage1[30 + 30];	/* intermediate key values is RAM */
    register u32 *S0_Dest;
    register u32 *S0_Source;

    register u32 A, B, C, D, part_sum, whole_sum;
    register u32 A2, B2, C2, D2, part_sum2, whole_sum2;
    register u32 macro_tmp, macro_tmp2;
    register u32 pipelined_read;
    register int right_dist;
    register u32 pipelined_read2;
    register int right_dist2;
    register int word_size;
    register u32 enc_word11, enc_word12;
    register u32 enc_word21, enc_word22;

    u32 running_key_lo;
    u32 running_key_hi;
    register int iterations_tried;

    word_size = 32;		/* RC5_WORDSIZE */
    iterations_tried = iterations;

/* load ahead of time, for future use */
    enc_word22 = rc5unitwork->L0.hi & 0xFEFFFFFF;
    enc_word21 = rc5unitwork->L0.lo;
    running_key_hi = enc_word22;
    running_key_lo = enc_word21;

/* crunch interate in this routine, instead of outside, for speed */
    while ( iterations_tried )
    { /* beginning of crunch loop */

/***************************************************************************/
/* Begin round 1 of key expansion */
/* mix in initial values from trial key, in specialized first block */

    iterations_tried--;
    S0_Source = &S0_INIT[0];	/* pick up values from constant array */
    enc_word11 = P_0;		/* Pick up 2 words that will be encrypted */
    S0_Dest = &S0_Storage[0];	/* pick up values from constant array */
    enc_word12 = P_1;		/* Pick up 2 words that will be encrypted */

    LB_RC5_ROUND0_PARTIAL2_A;

    LB_RC5_ROUND12_PARTIAL2_A ( 2, 32, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A ( 4, 34, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A ( 6, 36, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A ( 8, 38, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (10, 40, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (12, 42, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (14, 44, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (16, 46, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (18, 48, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (20, 50, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (22, 52, S0_Source, S0_Dest);
    LB_RC5_ROUND12_PARTIAL2_A (24, 54, S0_Source, S0_Dest);

/* fixup needed because of fancy pipelining and block rotation */
    pipelined_read = S0_Dest[1];	/* pipelined_read2 already right */
/* end of fixup for fancy pipelining */

/* Begin round 2 of key expansion */
    S0_Source = &S0_Storage1[0]; /* put where loads don't hit stores */

    LB_RC5_ROUND12_PARTIAL2_A (0, 30, S0_Dest, S0_Source);/* array 1 */

    LB_RC5_ROUND12_PARTIAL2_A ( 2, 32, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A ( 4, 34, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A ( 6, 36, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A ( 8, 38, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A (10, 40, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A (12, 42, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A (14, 44, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A (16, 46, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A (18, 48, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A (20, 50, S0_Dest, S0_Source);
    LB_RC5_ROUND12_PARTIAL2_A (22, 52, S0_Dest, S0_Source);

    LB_RC5_ROUND12_PARTIAL2_A (24, 54, S0_Dest, S0_Source);

/* DARN this can be sped up a tiny bit if the last call of
 * LB_RC5_ROUND12_PARTIAL2_A is specialized to include the
 * following fixup code, saving 2 clocks out of 500.  This
 * is unfortunately worth about 2000 keys/sec, which is visible
 */

/* fixup for fancy pipelining and block rotation */
    part_sum = S0_Source[0];					/* 1 */
    pipelined_read2 = S0_Source[30]; /* first val from prev iteration */
    whole_sum = (D + C) + part_sum;				/* 2 */
    pipelined_read = S0_Source[1]; /* second val from prev iteration */

/* end of fixup for fancy pipelining */

/* Begin round 3 of key expansion, mixed with encryption */

    LB_RC5_ROUND3_PARTIAL2_NO_XOR_A ( 0, 30, S0_Source); /* use running working array */

/* start encrypting, using initial values previously picked up */

    enc_word21 = enc_word11 + A2;	/* enc_word21 = P_0 + A2; */
    enc_word11 = enc_word11 + A;	/* enc_word11 = P_0 + A; */

    enc_word22 = enc_word12 + C2;	/* enc_word22 = P_1 + C2; */
    enc_word12 = enc_word12 + C;	/* enc_word12 = P_1 + C; */

    LB_RC5_ROUND3_PARTIAL2_A ( 2, 32, S0_Source); /* uses working array 2 */
    LB_RC5_ROUND3_PARTIAL2_A ( 4, 34, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A ( 6, 36, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A ( 8, 38, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A (10, 40, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A (12, 42, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A (14, 44, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A (16, 46, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A (18, 48, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A (20, 50, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_A (22, 52, S0_Source);
    LB_RC5_ROUND3_PARTIAL2_END1_A (24, 54, S0_Source);

    if ((C_0 == enc_word11) || (C_0 == enc_word21))
    {
	rc5unitwork->L0.lo = running_key_lo;
	LB_RC5_ROUND3_PARTIAL2_END2_A (24, 54, S0_Source);
	if ((C_0 == enc_word11) && (C_1 == enc_word12))
	{
	    rc5unitwork->L0.hi = running_key_hi;
	    return (2*(iterations - iterations_tried - 1) + 0);
	}

	if ((C_0 == enc_word21) && (C_1 == enc_word22))
	{
	    rc5unitwork->L0.hi = running_key_hi + 0x01000000;
	    return (2*(iterations - iterations_tried - 1) + 1);
	}
    }

/* "mangle-increment" the key number by the number of pipelines */
    enc_word22 = running_key_hi;	/* preload to lightly used var */
    enc_word21 = running_key_lo;	/* preload to lightly used var */

      enc_word22 = (enc_word22 + (2 /*PIPELINE_COUNT*/ << 24));
      if (!(enc_word22 & 0xFF000000)) {

        enc_word22 = (enc_word22 + 0x00010000) & 0x00FFFFFF;
        if (!(enc_word22 & 0x00FF0000)) {

           enc_word22 = (enc_word22 + 0x00000100) & 0x0000FFFF;
           if (!(enc_word22 & 0x0000FF00)) {

              enc_word22 = (enc_word22 + 0x00000001) & 0x000000FF;
              if (!(enc_word22 & 0x000000FF)) {

                enc_word22 = 0x00000000;
                enc_word21 = enc_word21 + 0x01000000;
                if (!(enc_word21 & 0xFF000000)) {

                  enc_word21 = (enc_word21 + 0x00010000) & 0x00FFFFFF;
                  if (!(enc_word21 & 0x00FF0000)) {

                    enc_word21 = (enc_word21 + 0x00000100) & 0x0000FFFF;
                    if (!(enc_word21 & 0x0000FF00)) {

                      enc_word21 = (enc_word21 + 0x00000001) & 0x000000FF;
                    }
                 }
              }
            }
	    running_key_lo = enc_word21;
          }
        }
      }
    running_key_hi = enc_word22;	/* preload to lightly used var */
    }    /* end of crunch loop */

	rc5unitwork->L0.lo = running_key_lo;
	rc5unitwork->L0.hi = running_key_hi;
	return (2 * iterations);
}

#ifdef __cplusplus
extern "C" u32 rc5_unit_func_ultrasparc_crunch( register RC5UnitWork * rc5unitwork, u32 iterations );
#endif

u32 rc5_unit_func_ultrasparc_crunch( RC5UnitWork * rc5unitwork, u32 iterations )
{
  return crunch( rc5unitwork, iterations );
}