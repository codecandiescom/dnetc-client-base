/*
 * Copyright distributed.net 1997-1999 - All Rights Reserved
 * For use in distributed.net projects only.
 * Any other distribution or use of this source violates copyright.
*/
const char *problem_cpp(void) {
return "@(#)$Id: problem.cpp,v 1.108.2.3 1999/06/23 05:46:05 myshkin Exp $"; }

/* ------------------------------------------------------------- */

#include "cputypes.h"
#include "baseincs.h"
#include "problem.h"
#include "clitime.h" //for CliTimer() which gets a timeval of the current time
#include "logstuff.h" //LogScreen()
#include "cpucheck.h"
#include "console.h"
#include "triggers.h"
#include "client.h" // CONTEST_COUNT
#if (CLIENT_OS == OS_RISCOS)
#include "../platforms/riscos/riscos_x86.h"
extern "C" void riscos_upcall_6(void);
#endif

//#define STRESS_THREADS_AND_BUFFERS /* !be careful with this! */

/* ------------------------------------------------------------- */

#ifdef PIPELINE_COUNT
#error Remove PIPELINE_COUNT from your makefile. It is useless (and 
#error confusing) when different cores have different PIPELINE_COUNTs
#error *Assign it* by core/cputype in LoadState() 
#error .
#error Ideally, the client should not need to know anything about the
#error number of pipelines in use by a core. Create a wrapper function
#error for that core and increment there.
#error .
#error The (projected) ideal prototype for *any* core (RC5/DES/OGR) is 
#error   s32 (*core_unit)( RC5unitWork *, u32 *timeslice, void *membuff );
#error Note that the core wrapper does its own timeslice to nbits 
#error conversion, increments the work space itself, stores the effective
#error timeslice back in the u32 *timeslice, and returns a result code.
#error (result code == RESULT_[FOUND|NOTHING|WORKING] or -1 if error).
#error .
#error Also note that the call is to a function pointer. That too should  
#error be assigned in LoadState() based on contest number and cputype. 
#error .
#error Would some kind user of the ANSI core(s) please rename the cores 
#error from [rc5|des]_unit_func to [rc5|des]_ansi_[x]_unit_func and fix
#error the RC5 cores to use timeslice as a second argument?
#error .
#error ./configure/ users: Please put your nick/cvs id and email address 
#error next to any rules you use in ./configure/.
#endif

#undef PIPELINE_COUNT

#if (CLIENT_CPU == CPU_X86)
  extern "C" u32 rc5_unit_func_486( RC5UnitWork * , u32 timeslice );
  extern "C" u32 rc5_unit_func_p5( RC5UnitWork * , u32 timeslice );
  extern "C" u32 rc5_unit_func_p6( RC5UnitWork * , u32 timeslice );
  extern "C" u32 rc5_unit_func_6x86( RC5UnitWork * , u32 timeslice );
  extern "C" u32 rc5_unit_func_k5( RC5UnitWork * , u32 timeslice );
  extern "C" u32 rc5_unit_func_k6( RC5UnitWork * , u32 timeslice );
  extern "C" u32 rc5_unit_func_p5_mmx( RC5UnitWork * , u32 timeslice );
  extern "C" u32 rc5_unit_func_486_smc( RC5UnitWork * , u32 timeslice );
  extern u32 p1des_unit_func_p5( RC5UnitWork * , u32 nbbits );
  extern u32 p1des_unit_func_pro( RC5UnitWork * , u32 nbbits );
  extern u32 p2des_unit_func_p5( RC5UnitWork * , u32 nbbits );
  extern u32 p2des_unit_func_pro( RC5UnitWork * , u32 nbbits );
  extern u32 des_unit_func_mmx( RC5UnitWork * , u32 nbbits, char *coremem );
  extern u32 des_unit_func_slice( RC5UnitWork * , u32 nbbits );
#elif (CLIENT_OS == OS_AIX)     // this has to stay BEFORE CPU_POWERPC
  #if defined(_AIXALL) || (CLIENT_CPU == CPU_POWER)
  extern "C" s32 rc5_ansi_2_rg_unit_func( RC5UnitWork *rc5unitwork, u32 timeslice );
  #endif
  #if defined(_AIXALL) || (CLIENT_CPU == CPU_POWERPC)
  extern "C" s32 crunch_allitnil( RC5UnitWork *work, u32 iterations);
  extern "C" s32 crunch_lintilla( RC5UnitWork *work, u32 iterations);
  #endif

  extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
#elif (CLIENT_CPU == CPU_POWERPC) && (CLIENT_OS != OS_AIX) 
  #if (CLIENT_OS == OS_WIN32)   // NT PPC doesn't have good assembly
  extern u32 rc5_unit_func( RC5UnitWork *  ); //rc5ansi2-rg.cpp
  #else
  extern "C" s32 rc5_unit_func_g1( RC5UnitWork *work, u32 *timeslice /* , void *scratch_area */);
  extern "C" s32 rc5_unit_func_g2_g3( RC5UnitWork *work, u32 *timeslice /* , void *scratch_area */);
  #endif
  extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
#elif (CLIENT_CPU == CPU_68K)
  extern "C" __asm u32 rc5_unit_func_000_030
      ( register __a0 RC5UnitWork *, register __d0 unsigned long timeslice );
  extern "C" __asm u32 rc5_unit_func_040_060
      ( register __a0 RC5UnitWork *, register __d0 unsigned long timeslice );
  extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
#elif (CLIENT_CPU == CPU_ARM)
  extern "C" u32 rc5_unit_func_arm_1( RC5UnitWork * , unsigned long t);
  extern "C" u32 rc5_unit_func_arm_2( RC5UnitWork * , unsigned long t);
  extern "C" u32 rc5_unit_func_arm_3( RC5UnitWork * , unsigned long t);
  extern "C" u32 des_unit_func_arm( RC5UnitWork * , unsigned long t);
  extern "C" u32 des_unit_func_strongarm( RC5UnitWork * , unsigned long t);
#elif (CLIENT_CPU == CPU_PA_RISC)
  extern u32 rc5_unit_func( RC5UnitWork *  );
  extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
#elif (CLIENT_CPU == CPU_SPARC)
  #if (ULTRA_CRUNCH == 1)
  extern "C++" u32 crunch( register RC5UnitWork * , u32 timeslice);
  extern "C++" u32 des_unit_func( RC5UnitWork * , u32 timeslice );
  #else
  extern "C++" u32 rc5_unit_func( RC5UnitWork *  );
  extern "C++" u32 des_unit_func( RC5UnitWork * , u32 timeslice );
  #endif
  // CRAMER // #error Please verify these core prototypes
#elif (CLIENT_CPU == CPU_MIPS)
  #if (CLIENT_OS != OS_ULTRIX)
    #if (MIPS_CRUNCH == 1)
    extern "C++" u32 crunch( register RC5UnitWork * , u32 timeslice);
    extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
    #else
    extern u32 rc5_unit_func( RC5UnitWork *  );
    extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
    #endif
    //FOXYLOXY// #error Please verify these core prototypes
  #else /* OS_ULTRIX */
    extern u32 rc5_unit_func( RC5UnitWork *  );
    extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
  #endif
#elif (CLIENT_CPU == CPU_ALPHA)
  #if (CLIENT_OS == OS_WIN32)
     extern "C" u32 rc5_unit_func( RC5UnitWork *, unsigned long timeslice );
     extern "C" u32 des_unit_func_alpha_dworz( RC5UnitWork * , u32 nbbits );
  #elif (CLIENT_OS == OS_DEC_UNIX)
     #if defined(DEC_UNIX_CPU_SELECT)
       #include <machine/cpuconf.h>
       extern u32 rc5_alpha_osf_ev4( RC5UnitWork * );
       extern u32 rc5_alpha_osf_ev5( RC5UnitWork * );
       extern u32 des_alpha_osf_ev4( RC5UnitWork * , u32 timeslice );
       extern u32 des_alpha_osf_ev5( RC5UnitWork * , u32 timeslice );
     #else
       extern u32 des_unit_func( RC5UnitWork * , u32 timeslice );
     #endif
  #else
     extern "C" u32 rc5_unit_func_axp_bmeyer
         ( RC5UnitWork * , unsigned long iterations);   // note not u32
     extern "C" u32 des_unit_func_alpha_dworz
         ( RC5UnitWork * , u32 nbbits );
  #endif
#else
  extern u32 rc5_unit_func_ansi( RC5UnitWork * , u32 timeslice );
  extern u32 des_unit_func_ansi( RC5UnitWork * , u32 timeslice );
  #error RC5ANSICORE is disappearing. Please declare/prototype cores by CLIENT_CPU and assert PIPELINE_COUNT
#endif

/* ------------------------------------------------------------------- */

Problem::Problem(long _threadindex /* defaults to -1L */)
{
  threadindex_is_valid = (_threadindex!=-1L);
  threadindex = ((threadindex_is_valid)?((unsigned int)_threadindex):(0));

  /* this next part is essential for alpha, but is probably beneficial to
     all platforms. If it fails for your os/cpu, we may need to redesign 
     how objects are allocated/how rc5unitwork is addressed, so let me know.
                                                       -cyp Jun 14 1999
  */
  RC5UnitWork *w = &rc5unitwork;
  unsigned long ww = ((unsigned long)w);
  if ((ww & 0x7)!=0) 
  {
    Log("rc5unitwork for problem %d is not 64bit aligned!\n");
    RaiseExitRequestTrigger();
    return;
  }  

//LogScreen("Problem created. threadindex=%u\n",threadindex);

  initialized = 0;
  started = 0;
  
#ifdef STRESS_THREADS_AND_BUFFERS 
  static int runlevel = 0;
  if (runlevel != -12345)
    {
    if ((++runlevel) != 1)
      {
      --runlevel;
      return;
      }
    RaisePauseRequestTrigger();
    LogScreen("Warning! STRESS_THREADS_AND_BUFFERS is defined.\n"
              "Are you sure that the client is pointing at\n"
              "a test proxy? If so, type 'yes': ");
    char getyes[10];
    ConInStr(getyes,4,0);
    ClearPauseRequestTrigger();
    if (strcmpi(getyes,"yes") != 0)
      {
      runlevel = +12345;
      RaiseExitRequestTrigger();
      return;
      }
    runlevel = -12345;
    }
#endif    
}

/* ------------------------------------------------------------------- */

Problem::~Problem()
{
  started = 0; // nothing to do. - suppress compiler warning
#if (CLIENT_OS == OS_RISCOS)
  if (GetProblemIndexFromPointer(this) == 1)
    {
    _kernel_swi_regs r;
    r.r[0] = 0;
    _kernel_swi(RC5PC_RetriveBlock,&r,&r);
    _kernel_swi(RC5PC_Off,&r,&r);
    
    }
#endif
}

/* ------------------------------------------------------------------- */

// for some odd reasons, the RC5 algorithm requires keys in reversed order
//         key.hi   key.lo
// ie key 01234567:89ABCDEF is sent to rc5_unit_func like that :
//        EFCDAB89:67452301
// This function switches from one format to the other.
//
// [Even if it looks like a little/big endian problem, it isn't. Whatever
//  endianess the underlying system has, we must swap every byte in the key
//  before sending it to rc5_unit_func()]
//
// Note that DES has a similiar but far more complex system, but everything
// is handled by des_unit_func().

static void  __SwitchRC5Format(u64 *_key)                               
{                                                                       
    register u32 tempkeylo = _key->hi; /* note: we switch the order */  
    register u32 tempkeyhi = _key->lo;                                  
                                                                        
    _key->lo =                                                          
      ((tempkeylo >> 24) & 0x000000FFL) |                               
      ((tempkeylo >>  8) & 0x0000FF00L) |                               
      ((tempkeylo <<  8) & 0x00FF0000L) |                               
      ((tempkeylo << 24) & 0xFF000000L);                                
    _key->hi =                                                          
      ((tempkeyhi >> 24) & 0x000000FFL) |                               
      ((tempkeyhi >>  8) & 0x0000FF00L) |                               
      ((tempkeyhi <<  8) & 0x00FF0000L) |                               
      ((tempkeyhi << 24) & 0xFF000000L);                                
}                                                                       

/* ------------------------------------------------------------------- */

// Input:  - an RC5 key in 'mangled' (reversed) format or a DES key
//         - an incrementation count
//         - a contest identifier (0==RC5 1==DES 2==OGR)
//
// Output: the key incremented

static void __IncrementKey(u64 *key, u32 iters, int contest)        
{                                                                   
  switch (contest)                                                  
  {                                                                 
    case RC5:
      __SwitchRC5Format (key);                                      
      key->lo += iters;                                             
      if (key->lo < iters) key->hi++;                               
      __SwitchRC5Format (key);                                      
      break;                                                        
    case DES:
    case CSC:
      key->lo += iters;                                             
      if (key->lo < iters) key->hi++; /* Account for carry */       
      break;                                                        
    case OGR:
      /* This should never be called for OGR */                     
      break;                                                        
  }                                                                 
}

/* ------------------------------------------------------------- */

u32 Problem::CalcPermille() /* % completed in the current block, to nearest 0.1%. */
{ 
  u32 retpermille = 0;
  if (initialized && last_resultcode >= 0)
  {
    if (!started)
      retpermille = startpermille;
    else if (last_resultcode != RESULT_WORKING)
      retpermille = 1000;
    else
    {
      switch (contest)
      {
        case RC5:
        case DES:
        case CSC:
                {
                retpermille = (u32)( ((double)(1000.0)) *
                (((((double)(contestwork.crypto.keysdone.hi))*((double)(4294967296.0)))+
                             ((double)(contestwork.crypto.keysdone.lo))) /
                ((((double)(contestwork.crypto.iterations.hi))*((double)(4294967296.0)))+
                             ((double)(contestwork.crypto.iterations.lo)))) ); 
                break;
                }
        case OGR:
                WorkStub curstub;
                ogr->getresult(ogrstate, &curstub, sizeof(curstub));
                // This is just a quick&dirty calculation that resembles progress.
                retpermille = curstub.stub.diffs[contestwork.ogr.workstub.stub.length]*10
                            + curstub.stub.diffs[contestwork.ogr.workstub.stub.length+1]/10;
                break;
      }
    }
    if (retpermille > 1000)
      retpermille = 1000;
  }
  return retpermille;
}

/* ------------------------------------------------------------------- */

int Problem::LoadState( ContestWork * work, unsigned int _contest, 
                              u32 _timeslice, int _cputype )
{
  unsigned int sz = sizeof(int);
  if (sz < sizeof(u32)) /* need to do it this way to suppress compiler warnings. */
  {
    LogScreen("FATAL: sizeof(int) < sizeof(u32)\n");
    //#error "everything assumes a 32bit CPU..."
    RaiseExitRequestTrigger();
    return -1;
  }
  last_resultcode = -1;
  started = initialized = 0;
  timehi = timelo = 0;
  runtime_sec = runtime_usec = 0;
  last_runtime_sec = last_runtime_usec = 0;
  memset((void *)&profiling, 0, sizeof(profiling));
  startpermille = permille = 0;
  loaderflags = 0;
  contest = _contest;
  cputype = _cputype;
  tslice = _timeslice;
    
  if (contest >= CONTEST_COUNT)
    return -1;

  pipeline_count = 2;

#if (CLIENT_CPU == CPU_ARM)
  switch(cputype)
  {
    case 0: rc5_unit_func = rc5_unit_func_arm_1;
            des_unit_func = des_unit_func_arm;
            pipeline_count = 1;
            break;
    default:
    case 1: rc5_unit_func = rc5_unit_func_arm_3;
            des_unit_func = des_unit_func_strongarm;
            pipeline_count = 3;
            break;
    case 2: rc5_unit_func = rc5_unit_func_arm_2;
            des_unit_func = des_unit_func_strongarm;
            pipeline_count = 2;
            break;
    case 3: rc5_unit_func = rc5_unit_func_arm_3;
            des_unit_func = des_unit_func_arm;
            pipeline_count = 3;
            break;
  }
#endif

#if (CLIENT_CPU == CPU_68K)
  if (cputype < 0 || cputype > 5) /* just to be safe */
    cputype = 0;
  if (cputype == 4 || cputype == 5 ) // there is no 68050, so type5=060
    rc5_unit_func = rc5_unit_func_040_060;
  else //if (cputype == 0 || cputype == 1 || cputype == 2 || cputype == 3)
    rc5_unit_func = rc5_unit_func_000_030;
  pipeline_count = 2;
#endif    

#if (CLIENT_CPU == CPU_ALPHA) 
  #if (CLIENT_OS == OS_DEC_UNIX)
  pipeline_count = 2;
  if (cputype ==  EV5_CPU) || (cputype == EV56_CPU) ||
     (cputype ==  PCA56_CPU) || (cputype == EV6_CPU)
  {
    rc5_unit_func = rc5_alpha_osf_ev5;
    des_unit_func = des_alpha_osf_ev5;
  }    
  else // EV3_CPU, EV4_CPU, LCA4_CPU, EV45_CPU and default
  {
    rc5_unit_func = rc5_alpha_osf_ev4;
    des_unit_func = des_alpha_osf_ev4;
  }
  #elif (CLIENT_OS == OS_WIN32)
  pipeline_count = 2;
  rc5_unit_func = ::rc5_unit_func;
  des_unit_func = des_unit_func_alpha_dworz;
  #else
  pipeline_count = 2;
  rc5_unit_func = rc5_unit_func_axp_bmeyer;
  des_unit_func = des_unit_func_alpha_dworz;
  #endif
#endif
#if (CLIENT_OS == OS_AIX)
  static int detectedtype = -1;
  if (detectedtype == -1)
    detectedtype = GetProcessorType(1 /* quietly */);
 
  #if defined(_AIXALL) || (CLIENT_CPU == CPU_POWERPC)
  switch (detectedtype) {
  case 1:                  // PPC 601
    rc5_unit_func = crunch_allitnil;
    pipeline_count = 1;
    break;
  case 2:                  // other PPC
    rc5_unit_func = crunch_lintilla;
    pipeline_count = 1;
    break;
  case 0:                  // that's POWER
  default:
  #ifdef _AIXALL
    rc5_unit_func = rc5_ansi_2_rg_unit_func ;
    pipeline_count = 2;
  #else                 // no POWER support
    rc5_unit_func = crunch_allitnil;
    pipeline_count = 1;
  #endif
    break;
  } /* endswitch */
  #elif (CLIENT_CPU == CPU_POWER)
  rc5_unit_func = rc5_ansi_2_rg_unit_func;
  pipeline_count = 2;
  #else
    #error "Systemtype not supported"
  #endif
#elif (CLIENT_CPU == CPU_POWERPC) && (CLIENT_OS != OS_AIX)
  #if ((CLIENT_OS != OS_BEOS) || (CLIENT_OS != OS_AMIGAOS))
  if (cputype == 0)
    unit_func = rc5_unit_func_g1;
  else
  #endif
    unit_func = rc5_unit_func_g2_g3;
  pipeline_count = 1;
#endif

#if (CLIENT_CPU == CPU_X86)
  static int detectedtype = -1;
  if (detectedtype == -1)
    detectedtype = GetProcessorType(1 /* quietly */);

  if (cputype < 0 || cputype > 5)
    cputype = 0;

  pipeline_count = 2; /* most cases */
  if (contest == DES)
  {
    #if defined(MMX_BITSLICER) 
    if ((detectedtype & 0x100) != 0) 
    {
      unit_func = (u32 (*)(RC5UnitWork *,u32))des_unit_func_mmx;
    }
    else
    #endif
    {
      //
      // p1* and p2* are effectively the same core (as called from
      // des-x86.cpp) if client was not compiled for mt - cyp
      //
      if (cputype == 2 || cputype == 3 || cputype == 5)
        unit_func = p1des_unit_func_pro;
      else
        unit_func = p1des_unit_func_p5;
      #if defined(CLIENT_SUPPORTS_SMP) 
      if (threadindex == 1)
      {
        if (unit_func == p1des_unit_func_p5)
          unit_func = p2des_unit_func_p5;
        else 
          unit_func = p2des_unit_func_pro;
      }
      else if (threadindex == 2)  
      {
        if (unit_func == p1des_unit_func_p5) // use the other unused cores.
          unit_func = p1des_unit_func_pro;   // non-optimal but ...
        else                                 // ... still better than slice
          unit_func = p1des_unit_func_p5;
      }
      else if (threadindex == 3)
      {
        if (unit_func == p1des_unit_func_p5) // use the other unused cores.
          unit_func = p2des_unit_func_pro;   // non-optimal but ...
        else                                 // ... still better than slice
          unit_func = p2des_unit_func_p5;
      }
      else if (threadindex > 4)              // fall back to slice if 
      {                                      // running with > 4 processors
        unit_func = des_unit_func_slice;
      }
      #endif
    }
  }
  else if (contest == RC5) 
  {
    if (cputype == 1)   // Intel 386/486
    {
      #if defined(SMC) 
      if (threadindex == 0)
        unit_func =  rc5_unit_func_486_smc;
      else
      #endif
        unit_func = rc5_unit_func_486;
    }
    else if (cputype == 2) // Ppro/PII
      unit_func = rc5_unit_func_p6;
    else if (cputype == 3) // 6x86(mx)
      unit_func = rc5_unit_func_6x86;
    else if (cputype == 4) // K5
      unit_func = rc5_unit_func_k5;
    else if (cputype == 5) // K6/K6-2
      unit_func = rc5_unit_func_k6;
    else // Pentium (0/6) + others
    {
      unit_func = rc5_unit_func_p5;
      #if defined(MMX_RC5)
      if (detectedtype == 0x106)  /* Pentium MMX only! */
      {
        unit_func = rc5_unit_func_p5_mmx;
        pipeline_count = 4; // RC5 MMX core is 4 pipelines
      }
      #endif
      cputype = 0;
    }
  }
#endif    


  //----------------------------------------------------------------

  switch (contest) 
  {
    case RC5:
    case DES:
    case CSC: // CSC_TEST

      // copy over the state information
      contestwork.crypto.key.hi = ( work->crypto.key.hi );
      contestwork.crypto.key.lo = ( work->crypto.key.lo );
      contestwork.crypto.iv.hi = ( work->crypto.iv.hi );
      contestwork.crypto.iv.lo = ( work->crypto.iv.lo );
      contestwork.crypto.plain.hi = ( work->crypto.plain.hi );
      contestwork.crypto.plain.lo = ( work->crypto.plain.lo );
      contestwork.crypto.cypher.hi = ( work->crypto.cypher.hi );
      contestwork.crypto.cypher.lo = ( work->crypto.cypher.lo );
      contestwork.crypto.keysdone.hi = ( work->crypto.keysdone.hi );
      contestwork.crypto.keysdone.lo = ( work->crypto.keysdone.lo );
      contestwork.crypto.iterations.hi = ( work->crypto.iterations.hi );
      contestwork.crypto.iterations.lo = ( work->crypto.iterations.lo );

      //determine starting key number. accounts for carryover & highend of keysdone
      u64 key;
      key.hi = contestwork.crypto.key.hi + contestwork.crypto.keysdone.hi + 
         ((((contestwork.crypto.key.lo & 0xffff) + (contestwork.crypto.keysdone.lo & 0xffff)) + 
           ((contestwork.crypto.key.lo >> 16) + (contestwork.crypto.keysdone.lo >> 16))) >> 16);
      key.lo = contestwork.crypto.key.lo + contestwork.crypto.keysdone.lo;

      // set up the unitwork structure
      rc5unitwork.plain.hi = contestwork.crypto.plain.hi ^ contestwork.crypto.iv.hi;
      rc5unitwork.plain.lo = contestwork.crypto.plain.lo ^ contestwork.crypto.iv.lo;
      rc5unitwork.cypher.hi = contestwork.crypto.cypher.hi;
      rc5unitwork.cypher.lo = contestwork.crypto.cypher.lo;

      rc5unitwork.L0.lo = key.lo;
      rc5unitwork.L0.hi = key.hi;
      if (contest == RC5)
        __SwitchRC5Format (&(rc5unitwork.L0));

      refL0 = rc5unitwork.L0;

      if (contestwork.crypto.keysdone.lo!=0 || contestwork.crypto.keysdone.hi!=0 )
      {
        startpermille = (u32)( ((double)(1000.0)) *
        (((((double)(contestwork.crypto.keysdone.hi))*((double)(4294967296.0)))+
                           ((double)(contestwork.crypto.keysdone.lo))) /
        ((((double)(contestwork.crypto.iterations.hi))*((double)(4294967296.0)))+
                        ((double)(contestwork.crypto.iterations.lo)))) );
      }     
      break;

    case OGR:

      #ifndef GREGH
      return -1;
      #else
      extern CoreDispatchTable *ogr_get_dispatch_table();
      contestwork.ogr = work->ogr;
      contestwork.ogr.nodes.lo = 0;
      contestwork.ogr.nodes.hi = 0;
      ogr = ogr_get_dispatch_table();
      int r = ogr->init();
      if (r != CORE_S_OK)
        return -1;
      r = ogr->create(&contestwork.ogr.workstub, sizeof(WorkStub), &ogrstate);
      if (r != CORE_S_OK)
        return -1;
      if (contestwork.ogr.workstub.worklength > contestwork.ogr.workstub.stub.length)
      {
        // This is just a quick&dirty calculation that resembles progress.
        startpermille = contestwork.ogr.workstub.stub.diffs[contestwork.ogr.workstub.stub.length]*10
                      + contestwork.ogr.workstub.stub.diffs[contestwork.ogr.workstub.stub.length+1]/10;
      }
      break;
      #endif

  }

  //---------------------------------------------------------------
#if (CLIENT_OS == OS_RISCOS)
  if (threadindex == 1 /*x86 thread*/)
  {
    RC5PCstruct rc5pc;
    _kernel_oserror *err;
    _kernel_swi_regs r;
  
    rc5pc.key.hi = contestwork.key.hi;
    rc5pc.key.lo = contestwork.key.lo;
    rc5pc.iv.hi = contestwork.iv.hi;
    rc5pc.iv.lo = contestwork.iv.lo;
    rc5pc.plain.hi = contestwork.plain.hi;
    rc5pc.plain.lo = contestwork.plain.lo;
    rc5pc.cypher.hi = contestwork.cypher.hi;
    rc5pc.cypher.lo = contestwork.cypher.lo;
    rc5pc.keysdone.hi = contestwork.keysdone.hi;
    rc5pc.keysdone.lo = contestwork.keysdone.lo;
    rc5pc.iterations.hi = contestwork.iterations.hi;
    rc5pc.iterations.lo = contestwork.iterations.lo;
    rc5pc.timeslice = tslice;
  
    err = _kernel_swi(RC5PC_On,&r,&r);
    if (err)
      LogScreen("Failed to start x86 card");
    else
    {
      r.r[1] = (int)&rc5pc;
      err = _kernel_swi(RC5PC_AddBlock,&r,&r);
      if ((err) || (r.r[0] == -1))
      {
        LogScreen("Failed to add block to x86 cruncher\n");
      }
    }
  }
#endif

  last_resultcode = RESULT_WORKING;
  initialized = 1;

  return( 0 );
}

/* ------------------------------------------------------------------- */

int Problem::RetrieveState( ContestWork * work, unsigned int *contestid, int dopurge )
{
  if (!initialized)
    return -1;
  if (work) // store back the state information
  {
    switch (contest) {
      case RC5:
      case DES:
      case CSC:
        // nothing special needs to be done here
        break;
      case OGR:
        if (ogrstate != NULL)
        {
          ogr->getresult(ogrstate, &contestwork.ogr.workstub, sizeof(WorkStub));
        }
        break;
    }
    memcpy( (void *)work, (void *)&contestwork, sizeof(ContestWork));
  }
  if (contestid)
    *contestid = contest;
  if (dopurge)
    initialized = 0;
  if (last_resultcode < 0)
    return -1;
  return ( last_resultcode );
}

/* ------------------------------------------------------------- */

u32 rc5_singlestep_core_wrapper( RC5UnitWork * rc5unitwork, u32 timeslice,
                int pipeline_count, auto u32 (*unit_func)( RC5UnitWork *) )
{                                
  u32 kiter = 0;
  int keycount = timeslice;
  //LogScreenf ("rc5unitwork = %08X:%08X (%X)\n", rc5unitwork.L0.hi, rc5unitwork.L0.lo, keycount);
  while ( keycount-- ) // timeslice ignores the number of pipelines
  {
    u32 result = (*unit_func)( rc5unitwork );
    if ( result )
    {
      kiter += result-1;
      break;
    }
    else
    {
      // "mangle-increment" the key number by the number of pipelines
      __IncrementKey (&(rc5unitwork->L0), pipeline_count, 0 );
      kiter += pipeline_count;
    }
  }
  return kiter;
}  

/* ------------------------------------------------------------- */

int Problem::Run_RC5(u32 *timesliceP, int *resultcode)
{
  u32 kiter = 0;
  u32 timeslice = *timesliceP;

  // align the timeslice to an even-multiple of pipeline_count and 2 
  u32 alignfact = pipeline_count + (pipeline_count & 1);
  timeslice = ((timeslice + (alignfact - 1)) & ~(alignfact - 1));

  // don't allow a too large of a timeslice be used ie (>(iter-keysdone)) 
  // (technically not necessary, but may save some wasted time)
  if (contestwork.crypto.keysdone.hi == contestwork.crypto.iterations.hi)
  {
    u32 todo = contestwork.crypto.iterations.lo-contestwork.crypto.keysdone.lo;
    if (todo < timeslice)
    {
      timeslice = todo;
      timeslice = ((timeslice + (alignfact - 1)) & ~(alignfact - 1));
    }
  }

#if 0
LogScreen("alignTimeslice: effective timeslice: %lu (0x%lx),\n"
          "suggested timeslice: %lu (0x%lx)\n"
          "pipeline_count = %lu, timeslice%%pipeline_count = %lu\n", 
          (unsigned long)timeslice, (unsigned long)timeslice,
          (unsigned long)tslice, (unsigned long)tslice,
          pipeline_count, timeslice%pipeline_count );
#endif

  timeslice /= pipeline_count;

  #if (CLIENT_CPU == CPU_X86)
    kiter = (*unit_func)( &rc5unitwork, timeslice );
  #elif ((CLIENT_CPU == CPU_SPARC) && (ULTRA_CRUNCH == 1)) || \
        ((CLIENT_CPU == CPU_MIPS) && (MIPS_CRUNCH == 1)) 
    kiter = crunch( &rc5unitwork, timeslice );
  #elif (CLIENT_CPU == CPU_68K) || (CLIENT_OS == OS_AIX) || \
        (CLIENT_CPU == CPU_POWER)
    kiter = (*rc5_unit_func)( &rc5unitwork, timeslice );
  #elif (CLIENT_CPU == CPU_POWERPC)
    kiter = timeslice;
    *resultcode = (*unit_func)( &rc5unitwork, &kiter );
  #elif (CLIENT_CPU == CPU_ARM)
    #if (CLIENT_OS == OS_RISCOS)
    if (threadindex == 1) /* RISC OS specific x86 2nd thread magic. */
    {
      if (runtime_sec == 0 && runtime_usec == 0) /* first time */
      {                          /* load the work onto the coprocessor */
        return RESULT_WORKING; /* ... or -1 if load failed */
        /* runtime_* will remain 0 as long as -1 is returned */
      }
      /* otherwise copy the state of the copro back to contestwork */
      /* fall through if copy-back succeeded or return -1 if it failed */
    }
    else
    #endif
    /* do other ARM goodstuff */
  #elif (CLIENT_CPU == CPU_ALPHA) && (CLIENT_OS == OS_WIN32)
    kiter = (timeslice * pipeline_count) - 
      rc5_unit_func(&rc5unitwork,timeslice);
  #elif (CLIENT_CPU == CPU_ALPHA)
    kiter = rc5_unit_func(&rc5unitwork, timeslice);
  #else
    kiter = rc5_singlestep_core_wrapper( &rc5unitwork, timeslice,
                pipeline_count, rc5_unit_func );
  #endif

  // Mac OS needs to yield here, since yielding works differently
  // depending on the core
  #if (CLIENT_OS == OS_MACOS)
    if (MP_active == 0) YieldToMain(1);
  #endif

  timeslice *= pipeline_count;
  *timesliceP = timeslice;

  __IncrementKey (&refL0, timeslice, contest);
    // Increment reference key count

  if (((refL0.hi != rc5unitwork.L0.hi) ||  // Compare ref to core
      (refL0.lo != rc5unitwork.L0.lo)) &&  // key incrementation
      (kiter == timeslice))
  {
    #if 0 /* can you spell "thread safe"? */
    Log("Internal Client Error #23: Please contact help@distributed.net\n"
        "Debug Information: %08x:%08x - %08x:%08x\n",
        rc5unitwork.L0.hi, rc5unitwork.L0.lo, refL0.hi, refL0.lo);
    #endif
    *resultcode = -1;
    return -1;
  };

  contestwork.crypto.keysdone.lo += kiter;
  if (contestwork.crypto.keysdone.lo < kiter)
    contestwork.crypto.keysdone.hi++;
    // Checks passed, increment keys done count.

  if (kiter < timeslice)
  {
    // found it!
    u32 keylo = contestwork.crypto.key.lo;
    contestwork.crypto.key.lo += contestwork.crypto.keysdone.lo;
    contestwork.crypto.key.hi += contestwork.crypto.keysdone.hi;
    if (contestwork.crypto.key.lo < keylo) 
      contestwork.crypto.key.hi++; // wrap occured ?
    *resultcode = RESULT_FOUND;
    return RESULT_FOUND;
  }
  else if (kiter != timeslice)
  {
    #if 0 /* can you spell "thread safe"? */
    Log("Internal Client Error #24: Please contact help@distributed.net\n"
        "Debug Information: k: %x t: %x\n"
        "Debug Information: %08x:%08x - %08x:%08x\n", kiter, timeslice,
        rc5unitwork.L0.lo, rc5unitwork.L0.hi, refL0.lo, refL0.hi);
    #endif
    *resultcode = -1;
    return -1;
  };

  if ( ( contestwork.crypto.keysdone.hi > contestwork.crypto.iterations.hi ) ||
       ( ( contestwork.crypto.keysdone.hi == contestwork.crypto.iterations.hi ) &&
       ( contestwork.crypto.keysdone.lo >= contestwork.crypto.iterations.lo ) ) )
  {
    // done with this block and nothing found
    *resultcode = RESULT_NOTHING;
    return RESULT_NOTHING;
  }

  // more to do, come back later.
  *resultcode = RESULT_WORKING;
  return RESULT_WORKING;    // Done with this round
}  

/* ------------------------------------------------------------- */

int Problem::Run_CSC(u32 *timesliceP, int *resultcode)
{
#ifndef CSC_TEST
  timesliceP = timesliceP;
  *resultcode = -1;
  return -1;
#else  
  s32 rescode = scs_ansi_unit_func(&rc5unitwork, timesliceP, (void *)0 );
  if (rescode < 0) /* "kiter" error */
  {
    *resultcode = -1;
    return -1;
  }
  *resultcode = (int)rescode;

  // Increment reference key count
  __IncrementKey (&refL0, *timesliceP, contest);

  // Compare ref to core key incrementation
  if ((refL0.hi != rc5unitwork.L0.hi) || (refL0.lo != rc5unitwork.L0.lo))
  { 
    #ifdef DEBUG_CSC_CORE /* can you spell "thread safe"? */
    Log("CSC incrementation mismatch:\n"
        "expected %08x:%08x, got %08x:%08x\n"
  refL0.lo, refL0.hi, rc5unitwork.L0.lo, rc5unitwork.L0.hi );
    #endif
    *resultcode = -1;
    return -1;
  }

  // Checks passed, increment keys done count.
  contestwork.crypto.keysdone.lo += *timesliceP;
  if (contestwork.crypto.keysdone.lo < *timesliceP)
    contestwork.crypto.keysdone.hi++;

  // Update data returned to caller
  if (*resultcode == RESULT_FOUND)
  {
    u32 keylo = contestwork.crypto.key.lo;
    contestwork.crypto.key.lo += contestwork.crypto.keysdone.lo;
    contestwork.crypto.key.hi += contestwork.crypto.keysdone.hi;
    if (contestwork.crypto.key.lo < keylo) 
      contestwork.crypto.key.hi++; // wrap occured ?
    return RESULT_FOUND;
  }
  if (*resultcode == RESULT_NOTHING) // done with this block and nothing found
  {
    return RESULT_NOTHING;
  }
  // more to do, come back later.
  *resultcode = RESULT_WORKING;
  return RESULT_WORKING; // Done with this round
#endif  
}

/* ------------------------------------------------------------- */

int Problem::Run_DES(u32 *timesliceP, int *resultcode)
{
  u32 kiter = 0;
  u32 timeslice = *timesliceP;
  
#if (CLIENT_CPU == CPU_X86)
  u32 min_bits = 8;  /* bryd and kwan cores only need a min of 256 */
  u32 max_bits = 24; /* these are the defaults if !MEGGS && !DES_ULTRA */

  #if defined(MMX_BITSLICER)
  if (((u32 (*)(RC5UnitWork *,u32, char *))(unit_func) == des_unit_func_mmx))
  {
    #if defined(BITSLICER_WITH_LESS_BITS)
    min_bits = 16;
    #else
    min_bits = 20;
    #endif
    max_bits = min_bits; /* meggs driver has equal MIN and MAX */
  }
  #endif
  u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

  if (nbits < min_bits) nbits = min_bits;
  else if (nbits > max_bits) nbits = max_bits;
  timeslice = (1ul << nbits);

  #if defined(MMX_BITSLICER)
  if (((u32 (*)(RC5UnitWork *,u32, char *))(unit_func) == des_unit_func_mmx))
    kiter = des_unit_func_mmx( &rc5unitwork, nbits, core_membuffer );
  else
  #endif
  kiter = (*unit_func)( &rc5unitwork, nbits );
#elif (CLIENT_CPU == CPU_ALPHA) && (CLIENT_OS == OS_WIN32)
  u32 nbits = 20;  // FROM des-slice-dworz.cpp
  timeslice = (1ul << nbits);
  kiter = des_unit_func ( &rc5unitwork, nbits );
#else
  u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

  if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
  else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
  timeslice = (1ul << nbits);

  kiter = des_unit_func ( &rc5unitwork, nbits );
#endif

  *timesliceP = timeslice;

  __IncrementKey (&refL0, timeslice, contest);
  // Increment reference key count

  if (((refL0.hi != rc5unitwork.L0.hi) ||  // Compare ref to core
      (refL0.lo != rc5unitwork.L0.lo)) &&  // key incrementation
      (kiter == timeslice))
  {
    #if 0 /* can you spell "thread safe"? */
    Log("Internal Client Error #23: Please contact help@distributed.net\n"
        "Debug Information: %08x:%08x - %08x:%08x\n",
        rc5unitwork.L0.lo, rc5unitwork.L0.hi, refL0.lo, refL0.hi);
    #endif
    *resultcode = -1;
    return -1;
  };

  contestwork.crypto.keysdone.lo += kiter;
  if (contestwork.crypto.keysdone.lo < kiter)
    contestwork.crypto.keysdone.hi++;
    // Checks passed, increment keys done count.

  // Update data returned to caller
  if (kiter < timeslice)
  {
    // found it!
    u32 keylo = contestwork.crypto.key.lo;
    contestwork.crypto.key.lo += contestwork.crypto.keysdone.lo;
    contestwork.crypto.key.hi += contestwork.crypto.keysdone.hi;
    if (contestwork.crypto.key.lo < keylo) 
      contestwork.crypto.key.hi++; // wrap occured ?
    *resultcode = RESULT_FOUND;
    return RESULT_FOUND;
  }
  else if (kiter != timeslice)
  {
    #if 0 /* can you spell "thread safe"? */
    Log("Internal Client Error #24: Please contact help@distributed.net\n"
        "Debug Information: k: %x t: %x\n"
        "Debug Information: %08x:%08x - %08x:%08x\n", kiter, timeslice,
        rc5unitwork.L0.lo, rc5unitwork.L0.hi, refL0.lo, refL0.hi);
    #endif
    *resultcode = -1; /* core error */
    return -1;
  };

  if ( ( contestwork.crypto.keysdone.hi > contestwork.crypto.iterations.hi ) ||
     ( ( contestwork.crypto.keysdone.hi == contestwork.crypto.iterations.hi ) &&
     ( contestwork.crypto.keysdone.lo >= contestwork.crypto.iterations.lo ) ) )
  {
    // done with this block and nothing found
    *resultcode = RESULT_NOTHING;
    return RESULT_NOTHING;
  }

  // more to do, come back later.
  *resultcode = RESULT_WORKING;
  return RESULT_WORKING; // Done with this round
}

/* ------------------------------------------------------------- */

int Problem::Run_OGR(u32 *timesliceP, int *resultcode)
{
#ifndef GREGH  
  timesliceP = timesliceP;
#else
  int r, nodes;

  if (*timesliceP > 0x100000UL)
    *timesliceP = 0x100000UL;

  nodes = (int)(*timesliceP);
  r = ogr->cycle(ogrstate, &nodes);
  *timesliceP = (u32)nodes;

  u32 newnodeslo = contestwork.ogr.nodes.lo + nodes;
  if (newnodeslo < contestwork.ogr.nodes.lo) {
    contestwork.ogr.nodes.hi++;
  }
  contestwork.ogr.nodes.lo = newnodeslo;

  switch (r) 
  {
    case CORE_S_OK:
    {
      r = ogr->destroy(ogrstate);
      if (r == CORE_S_OK) 
      {
        ogrstate = NULL;
        *resultcode = RESULT_NOTHING;
        return RESULT_NOTHING;
      }
      break;
    }
    case CORE_S_CONTINUE:
    {
      *resultcode = RESULT_WORKING;
      return RESULT_WORKING;
    }
    case CORE_S_SUCCESS:
    {
      if (ogr->getresult(ogrstate, &contestwork.ogr.workstub, sizeof(WorkStub)) == CORE_S_OK)
      {
        //Log("OGR Success!\n");
        contestwork.ogr.workstub.stub.length = contestwork.ogr.workstub.worklength;
        *resultcode = RESULT_FOUND;
        return RESULT_FOUND;
      }
      break;
    }
  }
  /* Something bad happened */
#endif
 *resultcode = -1; /* this will cause the problem to be discarded */
 return -1;
}

/* ------------------------------------------------------------- */

int Problem::Run(void) /* returns RESULT_*  or -1 */
{
  struct timeval stop, start;
  int retcode, core_resultcode;
  u32 timeslice;

  if ( !initialized )
    return ( -1 );

  if ( last_resultcode != RESULT_WORKING ) /* _FOUND, _NOTHING or -1 */
    return ( last_resultcode );
    
  CliTimer(&start);
  if (!started)
  {
    timehi = start.tv_sec; timelo = start.tv_usec;
    runtime_sec = runtime_usec = 0;
    memset((void *)&profiling, 0, sizeof(profiling));
    started=1;

#ifdef STRESS_THREADS_AND_BUFFERS 
    contest = RC5;
    contestwork.crypto.key.hi = contestwork.crypto.key.lo = 0;
    contestwork.crypto.keysdone.hi = contestwork.crypto.iterations.hi;
    contestwork.crypto.keysdone.lo = contestwork.crypto.iterations.lo;
    runtime_usec = 1; /* ~1Tkeys for a 2^20 packet */
    last_resultcode = RESULT_NOTHING;
    return RESULT_NOTHING;
#endif    
  }

  /* 
    On return from the Run_XXX contestwork must be in a state that we
    can put away to disk - that is, do not expect the loader (probfill 
    et al) to fiddle with iterations or key or whatever.
    
    The Run_XXX functions do *not* update problem.last_resultcode, they use
    core_resultcode instead. This is so that members of the problem object
    that are updated after the resultcode has been set will not be out of
    sync when the main thread gets it with RetrieveState(). 
    
    note: although the value returned by Run_XXX is usually the same as 
    the core_resultcode it is not always so. For instance, if 
    post-LoadState() initialization  failed, but can be deferred, Run_XXX 
    may choose to return -1, but keep core_resultcode at RESULT_WORKING.
  */

  timeslice = tslice;
  last_runtime_usec = last_runtime_sec = 0;
  core_resultcode = last_resultcode;
  retcode = -1;

  switch (contest)
  {
    case RC5: retcode = Run_RC5( &timeslice, &core_resultcode );
              break;
    case DES: retcode = Run_DES( &timeslice, &core_resultcode );
              break;
    case OGR: retcode = Run_OGR( &timeslice, &core_resultcode );
              break;
    case CSC: retcode = Run_CSC( &timeslice, &core_resultcode );
              break;
    default: retcode = core_resultcode = last_resultcode = -1;
       break;
  }

  
  if (retcode < 0) /* don't touch tslice or runtime as long as < 0!!! */
  {
    return -1;
  }
  
  core_run_count++;
  CliTimer(&stop);
  if ( core_resultcode != RESULT_WORKING ) /* _FOUND, _NOTHING */
  {
    if (((u32)(stop.tv_sec)) > ((u32)(timehi)))
    {
      u32 tmpdif = timehi - stop.tv_sec;
      tmpdif = (((tmpdif >= runtime_sec) ?
        (tmpdif - runtime_sec) : (runtime_sec - tmpdif)));
      if ( tmpdif < core_run_count )
      {
        runtime_sec = runtime_usec = 0;
        start.tv_sec = timehi;
        start.tv_usec = timelo;
      }
    }
  }
  if (stop.tv_sec < start.tv_sec || 
     (stop.tv_sec == start.tv_sec && stop.tv_usec <= start.tv_usec))
  {
    //AIEEE! clock is whacky (or unusably inaccurate if ==)
  }
  else
  {
    if (stop.tv_usec < start.tv_usec)
    {
      stop.tv_sec--;
      stop.tv_usec+=1000000L;
    }
    runtime_usec += (last_runtime_usec = (stop.tv_usec - start.tv_usec));
    runtime_sec  += (last_runtime_sec = (stop.tv_sec - start.tv_sec));
    if (runtime_usec > 1000000L)
    {
      runtime_sec++;
      runtime_usec-=1000000L;
    }
  }

  tslice = timeslice;

  last_resultcode = core_resultcode;
  return last_resultcode;
}

/* ======================================================================= */
/*                                 FINIS                                   */
/* ======================================================================= */

#if (CLIENT_CPU == CPU_ARM)
// ARM looks like a mess, I'll wait for ARM porters to integrate it
#error Chris, (the comment above is someone elses remark) I have migrated 
#error ARM/DES into Run_DES() and added comments in Run_RC5() suggesting how 
#error the rest could be done. The _kernel_escape_seen() poll is now handled
#error from triggers.cpp                                               - cyp

#if 0
  {
  unsigned long kiter;
//  timeslice *= pipeline_count;
//  done in the cores.

  if (contest == RC5)
    {
#if (CLIENT_OS == OS_RISCOS)
    if (threadindex == 0)
      {
#endif
#ifdef DEBUG
      static u32 ts;
      if ((ts < timeslice-500) || (ts > timeslice+500))
        {
        ts = timeslice;
        printf("timeslice = %d\n",timeslice);
        }
#endif
//    printf("ARM thread running, ts=%08lx\n",timeslice);
      if ((rc5_unit_func == rc5_unit_func_arm_2)&&( rc5unitwork.L0.hi&(1<<24)))
        {
        rc5unitwork.L0.hi -= 1<<24;
        if (contestwork.keysdone.lo & 1)
          {
          contestwork.keysdone.lo--;
          }
        else
          {
          LogScreen("Something really bad has happened - the number of keys looks wrong.\n");
          for(;;); // probably a bit bogus, but hey.
          }
        }
      /*
      Now returns number of keys processed!
      (Since 5/8/1998, SA core 1.5, ARM core 1.6).
      */
      kiter = rc5_unit_func(&rc5unitwork, timeslice);
      contestwork.keysdone.lo += kiter;
         
//    printf("kiter is %d\n",kiter);
      if (kiter != (timeslice*pipeline_count))
        {
        // found it?
        u32 keylo = contestwork.crypto.key.lo;
        contestwork.crypto.key.lo += contestwork.crypto.keysdone.lo;
        contestwork.crypto.key.hi += contestwork.crypto.keysdone.hi;
        if (contestwork.crypto.key.lo < keylo) 
          contestwork.crypto.key.hi++; // wrap occured ?
        resultcode = RESULT_FOUND;
        return( 1 );
        }
#if (CLIENT_OS == OS_RISCOS)
      }
    else // threadindex == 1
    {
      /* This is the RISC OS specific x86 2nd thread magic. */
      _kernel_swi_regs r;
      _kernel_oserror *err;
      volatile RC5PCstruct *rc5pcr;
      err = _kernel_swi(RC5PC_BufferStatus,&r,&r);
      if (err)
        {
        LogScreen("Error retrieving buffer status from x86 card");
        }
      else
      {
        /*
        contestwork.keysdone.lo is 0 for a completed block,
        so take care when setting it.
        */
        rc5pcr = (volatile RC5PCstruct *)r.r[1];
        
        if (r.r[2]==1)
        {
          /* block finished */
          r.r[0] = 0;
          err = _kernel_swi(RC5PC_RetriveBlock,&r,&r);
          if (err)
          {
            LogScreen("Error retrieving block from x86 card");
          }
          else
          {
            rc5pcr = (volatile RC5PCstruct *)r.r[1];
      
            if (rc5pcr->result == RESULT_FOUND)
            {
              contestwork.keysdone.lo = rc5pcr->keysdone.lo;
              contestwork.keysdone.hi = rc5pcr->keysdone.hi;
//printf("x86:FF Keysdone %08lx\n",contestwork.keysdone.lo);
              u32 keylo = contestwork.crypto.key.lo;
              contestwork.crypto.key.lo += contestwork.crypto.keysdone.lo;
              contestwork.crypto.key.hi += contestwork.crypto.keysdone.hi;
              if (contestwork.crypto.key.lo < keylo) 
                contestwork.crypto.key.hi++; // wrap occured ?
              resultcode = RESULT_FOUND;
              return( 1 );
            }
            else
            {
              contestwork.keysdone.lo = contestwork.iterations.lo;
              contestwork.keysdone.hi = contestwork.iterations.hi;
  //printf("x86:FN Keysdone %08lx\n",contestwork.keysdone.lo);
             }
          }
        }
        else
        {
          contestwork.keysdone.lo = rc5pcr->keysdone.lo;
          contestwork.keysdone.hi = rc5pcr->keysdone.hi;
//printf("x86:NF Keysdone %08lx\n",contestwork.keysdone.lo);
        }
      }
    }
#endif
    }
  }
#endif
#endif

