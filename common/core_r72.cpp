/*
 * Copyright distributed.net 1998-2003 - All Rights Reserved
 * For use in distributed.net projects only.
 * Any other distribution or use of this source violates copyright.
*/
const char *core_r72_cpp(void) {
return "@(#)$Id: core_r72.cpp,v 1.1.2.4 2003/09/01 22:38:02 mweiser Exp $"; }

//#define TRACE

#include "cputypes.h"
#include "client.h"    // MAXCPUS, Packet, FileHeader, Client class, etc
#include "baseincs.h"  // basic (even if port-specific) #includes
#include "problem.h"   // problem class
#include "cpucheck.h"  // cpu selection, GetTimesliceBaseline()
#include "logstuff.h"  // Log()/LogScreen()
#include "clicdata.h"  // GetContestNameFromID()
#include "bench.h"     // TBenchmark()
#include "selftest.h"  // SelfTest()
#include "selcore.h"   // keep prototypes in sync
#include "probman.h"   // GetManagedProblemCount()
#include "triggers.h"  // CheckExitRequestTriggerNoIO()
#include "util.h"      // TRACE_OUT

#if defined(HAVE_RC5_72_CORES)

/* ======================================================================== */

/* all the core prototypes
   note: we may have more prototypes here than cores in the client
   note2: if you need some 'cdecl' value define it in selcore.h to CDECL */

// These are the standard ANSI cores that are available for all platforms.
extern "C" s32 CDECL rc5_72_unit_func_ansi_4( RC5_72UnitWork *, u32 *, void * );
extern "C" s32 CDECL rc5_72_unit_func_ansi_2( RC5_72UnitWork *, u32 *, void * );
extern "C" s32 CDECL rc5_72_unit_func_ansi_1( RC5_72UnitWork *, u32 *, void * );

// These are assembly-optimized versions for each platform.
#if (CLIENT_CPU == CPU_X86) && !defined(HAVE_NO_NASM)
extern "C" s32 CDECL rc5_72_unit_func_ses( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_ses_2( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_dg_2( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_dg_3( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_dg_3a( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_ss_2( RC5_72UnitWork *, u32 *, void *);
#elif (CLIENT_CPU == CPU_ARM)
extern "C" s32 rc5_72_unit_func_arm1( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 rc5_72_unit_func_arm2( RC5_72UnitWork *, u32 *, void *);
#elif (CLIENT_CPU == CPU_68K) && (defined(__GCC__) || defined(__GNUC__))
extern "C" s32 CDECL rc5_72_unit_func_060_mh_2( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_030_mh_1( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_040_mh_1( RC5_72UnitWork *, u32 *, void *);
#elif (CLIENT_CPU == CPU_POWERPC) || (CLIENT_CPU == CPU_POWER)
  #if (CLIENT_CPU == CPU_POWERPC) || defined(_AIXALL)
    #if (CLIENT_OS != OS_WIN32) && (CLIENT_OS != OS_MACOS)
extern "C" s32 CDECL rc5_72_unit_func_ppc_mh_2( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_mh603e_addi( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_mh604e_addi( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_KKS2pipes( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_KKS604e( RC5_72UnitWork *, u32 *, void *);
      #if defined(__VEC__) || defined(__ALTIVEC__) /* OS+compiler support altivec */
extern "C" s32 CDECL rc5_72_unit_func_KKS7400( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_KKS7450( RC5_72UnitWork *, u32 *, void *);
extern "C" s32 CDECL rc5_72_unit_func_KKS970( RC5_72UnitWork *, u32 *, void *);
      #endif
    #endif
  #endif
#elif (CLIENT_CPU == CPU_SPARC)
extern "C" s32 CDECL rc5_72_unit_func_KKS_2 ( RC5_72UnitWork *, u32 *, void * );
extern "C" s32 CDECL rc5_72_unit_func_anbe_1( RC5_72UnitWork *, u32 *, void * );
extern "C" s32 CDECL rc5_72_unit_func_anbe_2( RC5_72UnitWork *, u32 *, void * );
#elif (CLIENT_CPU == CPU_MIPS)
extern "C" s32 CDECL rc5_72_unit_func_MIPS_2 ( RC5_72UnitWork *, u32 *, void * );
#endif


/* ======================================================================== */

int InitializeCoreTable_rc572(int /*first_time*/)
{
  /* rc5-72 does not require any initialization */
  return 0;
}

void DeinitializeCoreTable_rc572()
{
  /* rc5-72 does not require any initialization */
}

/* ======================================================================== */


const char **corenames_for_contest_rc572()
{
  /*
   When selecting corenames, use names that describe how (what optimization)
   they are different from their predecessor(s). If only one core,
   use the obvious "MIPS optimized" or similar.
  */
  static const char *corenames_table[]=
  /* ================================================================== */
  {
  #if (CLIENT_CPU == CPU_X86)
      #if !defined(HAVE_NO_NASM)
      "SES 1-pipe",
      "SES 2-pipe",
      "DG 2-pipe",
      "DG 3-pipe",
      "DG 3-pipe alt",
      "SS 2-pipe",
      #else /* no nasm -> only ansi cores */
      "ANSI 4-pipe",
      "ANSI 2-pipe",
      "ANSI 1-pipe",
      #endif
  #elif (CLIENT_CPU == CPU_X86_64)
      "ANSI 4-pipe",
      "ANSI 2-pipe",
      "ANSI 1-pipe",
  #elif (CLIENT_CPU == CPU_ARM)
      "ARM 1-pipe A",
      "ARM 1-pipe B",
  #elif (CLIENT_CPU == CPU_68K) && (defined(__GCC__) || defined(__GNUC__))
      "MH 1-pipe 68020/030",
      "MH 1-pipe 68000/040",
      "MH 2-pipe 68060",
  #elif (CLIENT_CPU == CPU_POWERPC) || (CLIENT_CPU == CPU_POWER)
      "ANSI 4-pipe",
      "ANSI 2-pipe",
      "ANSI 1-pipe",
      "MH 2-pipe",     /* gas and OSX format */
      "KKS 2-pipe",    /* gas and OSX format */
      "KKS 604e",      /* gas and OSX format */
      "KKS 7400",      /* gas and OSX format, AltiVec only */
      "KKS 7450",      /* gas and OSX format, AltiVec only */
      "MH 1-pipe",     /* gas and OSX format */
      "MH 1-pipe 604e",/* gas and OSX format */
      "KKS 970",       /* gas and OSX format, AltiVec only */
  #elif (CLIENT_CPU == CPU_SPARC)
      "ANSI 4-pipe",
      "ANSI 2-pipe",
      "ANSI 1-pipe",
      "KKS 2-pipe",
      "AnBe 1-pipe",
      "AnBe 2-pipe",
  #elif (CLIENT_CPU == CPU_MIPS)
      "ANSI 4-pipe",
      "ANSI 2-pipe",
      "ANSI 1-pipe",
      "MIPS 2-pipe",
  #else
      "ANSI 4-pipe",
      "ANSI 2-pipe",
      "ANSI 1-pipe",
  #endif
      NULL
  };
  /* ================================================================== */
  return corenames_table;
}

/* -------------------------------------------------------------------- */

/*
** Apply substition according to the same rules enforced by
** selcoreSelectCore() [ie, return the cindex of the core actually used
** after applying appropriate OS/architecture/#define limitations to
** ensure the client doesn't crash]
**
** This is necessary when the list of cores is a superset of the
** cores supported by a particular build. For example, all x86 clients
** display the same core list for RC5, but as not all cores may be
** available in a particular client/build/environment, this function maps
** between the ones that aren't available to the next best ones that are.
**
** Note that we intentionally don't do very intensive validation here. Thats
** selcoreGetSelectedCoreForContest()'s job when the user chooses to let
** the client auto-select. If the user has explicitely specified a core #,
** they have to live with the possibility that the choice will at some point
** no longer be optimal.
*/
int apply_selcore_substitution_rules_rc572(int cindex)
{
  #if (CLIENT_CPU == CPU_POWERPC) || (CLIENT_CPU == CPU_POWER)
  {
    /* AIX note:
    ** A power-only client running on PPC will never get here. So, at this
    ** point its either a power-only client running on power, or a ppc-only
    ** client (no power core) running on PPC or power, or _AIXALL client
    ** running on either power or PPC.
    */
    int have_vec = 0;
    int have_pwr = 0;

    #if defined(_AIXALL) || defined(__VEC__) || defined(__ALTIVEC__) /* only these two need detection*/
    long det = GetProcessorType(1);
    #endif
    #if defined(_AIXALL)                /* is a power/PPC hybrid client */
    have_pwr = (det >= 0 && (det & 1L<<24)!=0);
    #elif (CLIENT_CPU == CPU_POWER)     /* power only */
    have_pwr = 1;                       /* see note above */
    #endif
    #if defined(__VEC__) || defined(__ALTIVEC__) /* OS+compiler support altivec */
    have_vec = (det >= 0 && (det & 1L<<25)!=0); /* have altivec */
    #endif


    #if (CLIENT_OS == OS_AMIGAOS) && defined(__POWERUP__)
        /* PowerUp cannot use the KKS cores, since they modify gpr2 */
        if ((cindex >= 4) && (cindex <= 7))
          cindex = 8;			/* "MH 1-pipe" */
    #elif (CLIENT_OS == OS_WIN32) || (CLIENT_OS == OS_MACOS)
        /* Classic Mac OS doenst have assembly cores yet; Windows never had */
        if (cindex > 2)                 /* asm cores */
          cindex = 0;                   /* "ANSI 4-pipe" */
    #else
        if (have_pwr && cindex > 2)     /* asm cores */
          cindex = 0;                   /* "ANSI 4-pipe" */
        if (!have_pwr && cindex <= 2)   /* ANSI cores */
          cindex = 8;                   /* "MH 1-pipe" */
        if (!have_vec && cindex == 6)   /* "KKS 7400" */
          cindex = 4;                   /* "KKS 2pipes" */
        if (!have_vec && cindex == 7)   /* "KKS 7450" */
          cindex = 5;                   /* "KKS 604e" */
        if (!have_vec && cindex == 10)  /* "KKS 970" */
            cindex = 4;                 /* "KKS 2pipes", see micro-bench in #3310 */
    #endif
  }
  #elif (CLIENT_CPU == CPU_X86)
  {
    long det = GetProcessorType(1);
    //int have_mmx = (det >= 0 && (det & 0x100)!=0);
    int have_3486 = (det >= 0 && (det & 0xff)==1);

    #if !defined(HAVE_NO_NASM)
      if (have_3486 && cindex >= 2)     /* dg-* cores use the bswap instr that's not available on 386 */
        cindex = 0;                     /* "SES 1-pipe" */
    #endif

  }
  #endif
  return cindex;
}

/* ===================================================================== */

int selcoreGetPreselectedCoreForProject_rc572()
{
  static long detected_type = -123;
  static unsigned long detected_flags = 0;
  int cindex = -1;

  if (detected_type == -123) /* haven't autodetected yet? */
  {
    detected_type = GetProcessorType(1 /* quietly */);
    if (detected_type < 0)
      detected_type = -1;
    detected_flags = GetProcessorFeatureFlags();
  }

  // PROJECT_NOT_HANDLED("you may add your pre-selected core depending on arch
  //  and cpu here, but leaving the defaults (runs micro-benchmark) is ok")

  // ===============================================================
  #if (CLIENT_CPU == CPU_ALPHA)
    if (detected_type > 0)
    {
      if (detected_type >= 7 /*EV56 and higher*/)
        cindex = 0;
      else if ((detected_type <= 4 /*EV4 and lower*/) ||
              (detected_type == 6 /*EV45*/))
        cindex = 2;
      else /* EV5 */
        cindex = -1;
    }
  // ===============================================================
  #elif (CLIENT_CPU == CPU_68K)
    if (detected_type > 0)
    {
      #if defined(__GCC__) || defined(__GNUC__)
      switch (detected_type)
      {
        case 68020:
        case 68030:
          cindex = 0; /* 030 optimized (best for 68020 too) */
          break;
        case 68000:
        case 68040:
          cindex = 1; /* 040 optimized (best for 68000 too) */
          break;
        case 68060:
          cindex = 2; /* 060 optimized */
          break;
      }
      #else
      cindex = 2; /* ANSI 1-pipe */
      #endif
    }
  // ===============================================================
  #elif (CLIENT_CPU == CPU_POWERPC) || (CLIENT_CPU == CPU_POWER)
    if (detected_type > 0)
    {
      switch ( detected_type & 0xffff) // only compare the low PVR bits
      {
        case 0x0003: cindex = 8; break; // 603            == MH 1-pipe
        case 0x0004: cindex = 9; break; // 604            == MH 1-pipe 604e
        case 0x0006: cindex = 8; break; // 603e           == MH 1-pipe
        case 0x0007: cindex = 8; break; // 603r/603ev     == MH 1-pipe
        case 0x0008: cindex = 8; break; // 740/750 (G3)   == MH 1-pipe
        case 0x0009: cindex = 9; break; // 604e           == MH 1-pipe 604e
        case 0x000A: cindex = 9; break; // 604ev          == MH 1-pipe 604e
        default:     cindex =-1; break; // no default
      }
      #if defined(_AIXALL)             /* Power/PPC hybrid */
      if (( detected_type & (1L<<24) ) != 0) //ARCH_IS_POWER?
        cindex = -1;                   /* one of the ANSI cores */
      #elif (CLIENT_CPU == CPU_POWER)  /* Power only */
      cindex = -1;                     /* one of the ANSI cores */
      #endif
      #if defined(__VEC__) || defined(__ALTIVEC__) /* OS+compiler support altivec */
      if (( detected_type & (1L<<25) ) != 0) //altivec?
      {
        switch ( detected_type & 0xffff) // only compare the low PVR bits
        {
            case 0x000C: cindex = 6; break; // 7400 (G4)   == KKS 7400
            case 0x8000: cindex = 7; break; // 7450 (G4+)  == KKS 7450
            case 0x8001: cindex = 7; break; // 7455 (G4+)  == KKS 7450
            case 0x800C: cindex = 6; break; // 7410 (G4)   == KKS 7400
            //case 0x0039: cindex = 10; break; // 970 (G5)   == KKS 970
            default:     cindex =-1; break; // no default
        }
      }
      #endif
    }
  // ===============================================================
  #elif (CLIENT_CPU == CPU_X86)
  {
    int have_mmx = ((detected_flags & CPU_F_MMX) == CPU_F_MMX);
      if (detected_type >= 0)
      {
        #if !defined(HAVE_NO_NASM)
        switch (detected_type & 0xff) // FIXME remove &0xff
        {
          case 0x00: cindex = (have_mmx?4   // P5 MMX     == DG 3-pipe alt (#3233)
                                       :2); // P5         == DG 2-pipe
		                 break;
          case 0x01: cindex = 0; break; // 386/486        == SES 1-pipe
          case 0x02: cindex = 1; break; // PII/PIII       == SES 2-pipe
          case 0x03: cindex = 2; break; // Cx6x86         == DG 2-pipe
          case 0x04: cindex = 2; break; // K5             == DG 2-pipe
          case 0x05: cindex = 5; break; // K6             == SS 2-pipe (#3293)
          case 0x06: cindex = 0; break; // Cx486          == SES 1-pipe
          case 0x07: cindex =-1; break; // orig Celeron   == unused?
          case 0x08: cindex =-1; break; // PPro           == ?
          case 0x09: cindex = 5; break; // K7             == SS 2-pipe
          case 0x0A: cindex =-1; break; // Centaur C6     == ?
          case 0x0B: cindex = 3; break; // Pentium 4      == DG 3-pipe
          default:   cindex =-1; break; // no default
        }
        #else
        switch (detected_type & 0xff) // FIXME remove &0xff
        {
          case 0x00: cindex = 2; break; // P5             == ANSI 1-pipe
          case 0x01: cindex = 2; break; // 386/486        == ANSI 1-pipe
          case 0x02: cindex = 1; break; // PII/PIII       == ANSI 2-pipe
          case 0x03: cindex = 2; break; // Cx6x86         == ANSI 1-pipe
          case 0x04: cindex = 2; break; // K5             == ANSI 1-pipe
          case 0x05: cindex = 1; break; // K6             == ANSI 2-pipe
          case 0x06: cindex = 2; break; // Cx486          == ANSI 1-pipe
          case 0x07: cindex =-1; break; // orig Celeron   == unused?
          case 0x08: cindex =-1; break; // PPro           == ?
          case 0x09: cindex = 0; break; // K7             == ANSI 4-pipe
          case 0x0A: cindex =-1; break; // Centaur C6     == ?
          case 0x0B: cindex = 0; break; // Pentium 4      == ANSI 4-pipe
          default:   cindex =-1; break; // no default
        }
        #endif
      }
  }
  // ===============================================================
  #elif (CLIENT_CPU == CPU_ARM)
    if (detected_type > 0)
    {
      if (detected_type == 0x200  || /* ARM 2 */
          detected_type == 0x250  || /* ARM 250 */
          detected_type == 0x300  || /* ARM 3 */
          detected_type == 0x600  || /* ARM 600 */
          detected_type == 0x610  || /* ARM 610 */
          detected_type == 0x700  || /* ARM 700 */
          detected_type == 0x710  || /* ARM 710 */
          detected_type == 0x7500 || /* ARM 7500 */
          detected_type == 0x7500FE) /* ARM 7500FE */
        cindex = 1;
      else if (detected_type == 0x810 || /* ARM 810 */
               detected_type == 0xA10 || /* StrongARM 110 */
               detected_type == 0xA11 || /* StrongARM 1100 */
               detected_type == 0xB11)   /* StrongARM 1110 */
        cindex = 0;
    }
  // ===============================================================
  #elif (CLIENT_CPU == CPU_SPARC)
    #if (CLIENT_OS == OS_SOLARIS)
    if (detected_type > 0)
    {
      switch (detected_type)
      {
        case  1: cindex = 4; break; // SPARCstation SLC == AnBe 1-pipe
        case  2: cindex = 4; break; // SPARCstation ELC == AnBe 1-pipe
        case  3: cindex = 4; break; // SPARCstation IPC == AnBe 1-pipe
        case  4: cindex = 4; break; // SPARCstation IPX == AnBe 1-pipe
        case  5: cindex = 4; break; // SPARCstation 1   == AnBe 1-pipe
        case  6: cindex = 4; break; // SPARCstation 1+  == AnBe 1-pipe
        case  7: cindex = 4; break; // SPARCstation 2   == AnBe 1-pipe
        case  8: cindex = 4; break; // microSPARC       == AnBe 1-pipe
        case  9: cindex = 4; break; // microSPARC II    == AnBe 1-pipe
        case 10: cindex = 4; break; // TurboSPARC       == AnBe 1-pipe
        case 11: cindex = 4; break; // hyperSPARC       == AnBe 1-pipe
        case 12: cindex = 5; break; // SuperSPARC       == AnBe 2-pipe
        case 13: cindex = 5; break; // SuperSPARC SC    == AnBe 2-pipe
        case 14: cindex = 5; break; // SuperSPARC II    == AnBe 2-pipe
        case 15: cindex = 5; break; // SuperSPARC II SC == AnBe 2-pipe
        case 16: cindex = 5; break; // UltraSPARC-I     == AnBe 2-pipe
        case 17: cindex = 5; break; // UltraSPARC-II    == AnBe 2-pipe
        case 18: cindex = 5; break; // UltraSPARC-IIe   == AnBe 2-pipe
        case 19: cindex = 5; break; // UltraSPARC-IIi   == AnBe 2-pipe
        case 20: cindex = 5; break; // UltraSPARC-III   == AnBe 2-pipe
        case 21: cindex = 5; break; // UltraSPARC-IIIi  == AnBe 2-pipe
        default: cindex =-1; break; // no default
      }
    }
    #else /* non-Solaris */
    /* cpu detection and core preselection for all the SPARC OSes needs a
       complete overhaul and generalization
       sparc v8: impl = %psr[31..28], vers = %psr[27..24]
       sparc v9: manufacturer = %ver[63..48], impl = %ver[47..32]
       should be used for the cpu id number (detected_type) and preselection
       derived from this value
    */
    #endif
  // ===============================================================
  #elif (CLIENT_OS == OS_PS2LINUX) // OUCH !!!! SELECT_BY_CPU !!!!! FIXME
  #error Please make this decision by CLIENT_CPU!
  #error CLIENT_OS may only be used for sub-selects (only if neccessary)
    cindex = 1; // now we use ansi-2pipe
  #endif

  return cindex;
}


/* ---------------------------------------------------------------------- */

int selcoreSelectCore_rc572(unsigned int threadindex,
                            int *client_cpuP, struct selcore *selinfo)
{
  int use_generic_proto = 0; /* if rc5/des unit_func proto is generic */
  unit_func_union unit_func; /* declared in problem.h */
  int cruncher_is_asynchronous = 0; /* on a co-processor or similar */
  int pipeline_count = 2; /* most cases */
  int client_cpu = CLIENT_CPU; /* usual case */
  int coresel = selcoreGetSelectedCoreForContest(RC5_72);
  if (coresel < 0)
    return -1;
  memset( &unit_func, 0, sizeof(unit_func));

  /* -------------------------------------------------------------- */

  {
    use_generic_proto = 1;
    switch (coresel)
    {
     /* architectures without ansi cores */
     #if (CLIENT_CPU == CPU_ARM)
      case 0:
      default:
        unit_func.gen_72 = rc5_72_unit_func_arm1;
        pipeline_count = 1;
        coresel = 0;
        break;
      case 1:
        unit_func.gen_72 = rc5_72_unit_func_arm2;
        pipeline_count = 1;
        break;
     #elif (CLIENT_CPU == CPU_68K) && (defined(__GCC__) || defined(__GNUC__))
      case 0:
        unit_func.gen_72 = rc5_72_unit_func_030_mh_1;
        pipeline_count = 1;
        break;
      case 1:
      default:
        unit_func.gen_72 = rc5_72_unit_func_040_mh_1;
        pipeline_count = 1;
        coresel = 1;
        break;
      case 2:
        unit_func.gen_72 = rc5_72_unit_func_060_mh_2;
        pipeline_count = 2;
        break;
     #elif (CLIENT_CPU == CPU_X86) && !defined(HAVE_NO_NASM)
      case 0:
        unit_func.gen_72 = rc5_72_unit_func_ses;
        pipeline_count = 1;
        break;
      case 1:
      default:
        unit_func.gen_72 = rc5_72_unit_func_ses_2;
        pipeline_count = 2;
        coresel = 1;
        break;
      case 2:
        unit_func.gen_72 = rc5_72_unit_func_dg_2;
        pipeline_count = 2;
        break;
      case 3:
        unit_func.gen_72 = rc5_72_unit_func_dg_3;
        pipeline_count = 3;
        break;
      case 4:
        unit_func.gen_72 = rc5_72_unit_func_dg_3a;
        pipeline_count = 3;
        break;
      case 5:
        unit_func.gen_72 = rc5_72_unit_func_ss_2;
        pipeline_count = 2;
        break;
     #elif (CLIENT_CPU == CPU_POWERPC) || (CLIENT_CPU == CPU_POWER)
      #if (CLIENT_CPU == CPU_POWER) || defined(_AIXALL) \
         || (CLIENT_OS == OS_WIN32) || (CLIENT_OS == OS_MACOS)
      case 0:
        unit_func.gen_72 = rc5_72_unit_func_ansi_4;
        pipeline_count = 4;
        #if defined(_AIXALL)
        client_cpu = CPU_POWER;
        #endif
        break;
      case 1:
        unit_func.gen_72 = rc5_72_unit_func_ansi_2;
        pipeline_count = 2;
        #if defined(_AIXALL)
        client_cpu = CPU_POWER;
        #endif
        break;
      case 2:
        unit_func.gen_72 = rc5_72_unit_func_ansi_1;
        pipeline_count = 1;
        #if defined(_AIXALL)
        client_cpu = CPU_POWER;
        #endif
        break;
      #endif
      #if ((CLIENT_CPU == CPU_POWERPC) || defined(_AIXALL)) \
           && (CLIENT_OS != OS_WIN32) && (CLIENT_OS != OS_MACOS)
      case 3:
          unit_func.gen_72 = rc5_72_unit_func_ppc_mh_2;
          pipeline_count = 2;
          break;
      case 4:
          unit_func.gen_72 = rc5_72_unit_func_KKS2pipes;
          pipeline_count = 2;
          break;
      case 5:
          unit_func.gen_72 = rc5_72_unit_func_KKS604e;
          pipeline_count = 2;
          break;
      #if defined(__VEC__) || defined(__ALTIVEC__)
      case 6:
          unit_func.gen_72 = rc5_72_unit_func_KKS7400;
          pipeline_count = 4;
          break;
      case 7:
          unit_func.gen_72 = rc5_72_unit_func_KKS7450;
          pipeline_count = 4;
          break;
      #endif
      case 8:
      default:
        unit_func.gen_72 = rc5_72_unit_func_mh603e_addi;
        pipeline_count = 1;
        coresel = 8;
        break;
      case 9:
        unit_func.gen_72 = rc5_72_unit_func_mh604e_addi;
        pipeline_count = 1;
        break;
      #if defined(__VEC__) || defined(__ALTIVEC__)
      case 10:
          unit_func.gen_72 = rc5_72_unit_func_KKS970;
          pipeline_count = 4;
          break;
      #endif
      #endif

     #else /* the ansi cores */
      case 0:
        unit_func.gen_72 = rc5_72_unit_func_ansi_4;
        pipeline_count = 4;
        break;
      case 1:
        unit_func.gen_72 = rc5_72_unit_func_ansi_2;
        pipeline_count = 2;
        break;
      case 2:
      default:
        unit_func.gen_72 = rc5_72_unit_func_ansi_1;
        pipeline_count = 1;
        coresel = 2; // yes, we explicitly set coresel in the default case !
        break;
     #endif

     /* additional cores */
     #if (CLIENT_CPU == CPU_SPARC)
       case 3:
         unit_func.gen_72 = rc5_72_unit_func_KKS_2;
         pipeline_count = 2;
         break;
       case 4:
         unit_func.gen_72 = rc5_72_unit_func_anbe_1;
         pipeline_count = 1;
         break;
       case 5:
         unit_func.gen_72 = rc5_72_unit_func_anbe_2;
         pipeline_count = 2;
         break;
     #endif
     #if (CLIENT_CPU == CPU_MIPS)
       case 3:
         unit_func.gen_72 = rc5_72_unit_func_MIPS_2;
         pipeline_count = 2;
         break;
     #endif

    }
  }

  /* ================================================================== */

  if (coresel >= 0 && unit_func.gen &&
      coresel < ((int)corecount_for_contest(RC5_72)) )
  {
    if (client_cpuP)
      *client_cpuP = client_cpu;
    if (selinfo)
    {
      selinfo->client_cpu = client_cpu;
      selinfo->pipeline_count = pipeline_count;
      selinfo->use_generic_proto = use_generic_proto;
      selinfo->cruncher_is_asynchronous = cruncher_is_asynchronous;
      memcpy( (void *)&(selinfo->unit_func), &unit_func, sizeof(unit_func));
    }
    return coresel;
  }

  threadindex = threadindex; /* possibly unused. shaddup compiler */
  return -1; /* core selection failed */
}

/* ------------------------------------------------------------- */

#endif  // HAVE_RC5_72_CORES