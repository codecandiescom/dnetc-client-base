// Copyright distributed.net 1997-1998 - All Rights Reserved
// For use in distributed.net projects only.
// Any other distribution or use of this source violates copyright.
//
// $Log: problem.cpp,v $
// Revision 1.51  1998/12/09 07:38:40  dicamillo
// Fixed missing // in log comment (MacCVS Pro bug!).
//
// Revision 1.50  1998/12/09 07:36:34  dicamillo
// Added support for MacOS client scheduling, and info needed
// by MacOS GUI.
//
// Revision 1.49  1998/12/07 15:21:23  chrisb
// more riscos/x86 changes
//
// Revision 1.48  1998/12/06 03:06:11  cyp
// Define STRESS_THREADS_AND_BUFFERS to configure problem.cpp for 'dummy
// crunching', ie problems are finished before they even start. You will be
// warned (at runtime) if you define it.
//
// Revision 1.47  1998/12/01 11:24:11  chrisb
// more riscos x86 changes
//
// Revision 1.46  1998/11/28 19:43:17  cyp
// Def'd out two LogScreen()s
//
// Revision 1.45  1998/11/25 09:23:36  chrisb
// various changes to support x86 coprocessor under RISC OS
//
// Revision 1.44  1998/11/25 06:05:48  dicamillo
// Use parenthesis when calling a function via a pointer.  Gcc in BeOS R4
// for Intel requires this.
//
// Revision 1.43  1998/11/16 16:44:07  remi
// Allow some targets to use deseval.cpp instead of Meggs' bitslicers.
//
// Revision 1.42  1998/11/14 14:12:05  cyp
// Fixed assignment of -1 to an unsigned variable.
//
// Revision 1.41  1998/11/14 13:56:15  cyp
// Fixed pipeline_count for x86 clients (DES cores were running with 4
// pipelines). Fixed unused parameter warning in LoadState(). Problem manager
// saves its probman_index in the Problem object (needed by chrisb's x86
// copro board code.)
//
// Revision 1.40  1998/11/12 22:58:31  remi
// Reworked a bit AIX ppc & power defines, based on Patrick Hildenbrand
// <patrick@de.ibm.com> advice.
//
// Revision 1.39  1998/11/10 09:18:13  silby
// Added alpha-linux target, should use axp-bmeyer core.
//
// Revision 1.38  1998/11/08 22:25:48  silby
// Fixed RC5_MMX pipeline count selection, was incorrect.
//
// Revision 1.37  1998/10/02 16:59:03  chrisb
// lots of fiddling in a vain attempt to get the NON_PREEMPTIVE_OS_PROFILING to be a bit sane under RISC OS
//
// Revision 1.36  1998/09/29 22:03:00  blast
// Fixed a bug I introduced with generic core usage, and removed
// a few old comments that weren't valid anymore (for 68k)
//
// Revision 1.35  1998/09/25 11:31:18  chrisb
// Added stuff to support 3 cores in the ARM clients.
//
// Revision 1.34  1998/09/23 22:05:20  blast
// Multi-core support added for m68k. Autodetection of cores added for 
// AmigaOS. (Manual selection possible of course). Two new 68k cores are 
// now added. rc5-000_030-jg.s and rc5-040_060-jg.s Both made by John Girvin.
//
// Revision 1.33  1998/08/24 04:43:26  cyruspatel
// timeslice is now rounded up to be multiple of PIPELINE_COUNT and even.
//
// Revision 1.32  1998/08/22 08:00:40  silby
// added in pipeline_count=2 "just in case" for x86
//
// Revision 1.31  1998/08/20 19:34:28  cyruspatel
// Removed that terrible PIPELINE_COUNT hack: Timeslice and pipeline count
// are now computed in Problem::LoadState(). Client::SelectCore() now saves
// core type to Client::cputype.
//
// Revision 1.30  1998/08/14 00:05:07  silby
// Changes for rc5 mmx core integration.
//
// Revision 1.29  1998/08/05 16:43:29  cberry
// ARM clients now define PIPELINE_COUNT=2, and RC5 cores return number of 
// keys checked, rather than number of keys left to check
//
// Revision 1.28  1998/08/02 16:18:27  cyruspatel
// Completed support for logging.
//
// Revision 1.27  1998/07/13 12:40:33  kbracey
// RISC OS update. Added -noquiet option.
//
// Revision 1.26  1998/07/13 03:31:52  cyruspatel
// Added 'const's or 'register's where the compiler was complaining about
// "declaration/type or an expression" ambiguities.
//
// Revision 1.25  1998/07/07 21:55:50  cyruspatel
// client.h has been split into client.h and baseincs.h 
//
// Revision 1.24  1998/07/06 09:21:26  jlawson
// added lint tags around cvs id's to suppress unused variable warnings.
//
// Revision 1.23  1998/06/17 02:14:47  blast
// Added code to test a new 68030 core which I got from an outside
// source ... Commented out of course ...
//
// Revision 1.22  1998/06/16 21:53:28  silby
// Added support for dual x86 DES cores (p5/ppro)
//
// Revision 1.21  1998/06/15 12:04:05  kbracey
// Lots of consts.
//
// Revision 1.20  1998/06/15 06:18:37  dicamillo
// Updates for BeOS
//
// Revision 1.19  1998/06/15 00:12:24  skand
// fix id marker so it won't interfere when another .cpp file is 
// #included here
//
// Revision 1.18  1998/06/14 10:13:43  skand
// use #if 0 (or 1) to turn on some debugging info, rather than // on each line
//
// Revision 1.17  1998/06/14 08:26:54  friedbait
// 'Id' tags added in order to support 'ident' command to display a bill of
// material of the binary executable
//
// Revision 1.16  1998/06/14 08:13:04  friedbait
// 'Log' keywords added to maintain automatic change history
//
// Revision 1.15  1998/06/14 00:06:07  remi
// Added $Log.
//

#if (!defined(lint) && defined(__showids__))
const char *problem_cpp(void) {
return "@(#)$Id: problem.cpp,v 1.51 1998/12/09 07:38:40 dicamillo Exp $"; }
#endif

#include "cputypes.h"
#include "baseincs.h"
#include "problem.h"
#include "network.h" // for timeval and htonl/ntohl
#include "clitime.h" //for CliTimer() which gets a timeval of the current time
#include "logstuff.h" //LogScreen()
#include "console.h"
#include "triggers.h"

//#define STRESS_THREADS_AND_BUFFERS /* !be careful with this! */

#ifndef _CPU_32BIT_
#error "everything assumes a 32bit CPU..."
#endif

#if (CLIENT_CPU == CPU_X86)
  u32 (*rc5_unit_func)( RC5UnitWork * rc5unitwork, u32 timeslice );
  u32 (*des_unit_func)( RC5UnitWork * rc5unitwork, u32 timeslice );
  u32 (*des_unit_func2)( RC5UnitWork * rc5unitwork, u32 timeslice );
#elif (CLIENT_CPU == CPU_POWERPC) && (CLIENT_OS == OS_WIN32)
  // NT PPC doesn't have good assembly
  #include "rc5ansi2-rg.cpp"
  extern u32 des_unit_func( RC5UnitWork * rc5unitwork, u32 timeslice );
#elif (CLIENT_CPU == CPU_ALPHA) && (CLIENT_OS == OS_VMS)
  #include "rc5ansi2-rg.cpp"
  extern u32 des_unit_func( RC5UnitWork * rc5unitwork, u32 timeslice );
#elif (CLIENT_CPU == CPU_ALPHA) && (CLIENT_OS == OS_LINUX)
  extern u32 des_unit_func( RC5UnitWork * rc5unitwork, u32 timeslice );
#elif (CLIENT_CPU == CPU_POWERPC)
  extern "C" int crunch_allitnil( RC5UnitWork *work, unsigned long iterations );
  extern "C" int crunch_lintilla( RC5UnitWork *work, unsigned long iterations );
  extern u32 des_unit_func( RC5UnitWork * rc5unitwork, u32 timeslice );
  int whichcrunch = 0;
#elif (CLIENT_CPU == CPU_68K)
  extern u32 des_unit_func( RC5UnitWork * rc5unitwork, u32 timeslice );
  extern "C" __asm u32 (*rc5_unit_func)( register __a0 RC5UnitWork *work, register __d0 u32 timeslice);
#elif (CLIENT_CPU == CPU_ARM)
  u32 (*rc5_unit_func)( RC5UnitWork * rc5unitwork, unsigned long iterations  );
  u32 (*des_unit_func)( RC5UnitWork * rc5unitwork, u32 timeslice );
#else
  #include "rc5stub.cpp"
  #if defined(KWAN) && !defined(MEGGS)
    #define des_unit_func des_unit_func_slice
  #endif
  extern u32 des_unit_func( RC5UnitWork * rc5unitwork, u32 timeslice );
#endif

#if (CLIENT_OS == OS_RISCOS)
#include "../platforms/riscos/riscos_x86.h"
extern "C" void riscos_upcall_6(void);
extern void CliSignalHandler(int);
#endif

//-----------------------------------------------------------------------

Problem::Problem()
{
  probman_index = (unsigned int)(-1); /* assume not managed by probman */
  initialized = 0;
  finished = 0;
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

Problem::~Problem()
{
  started = 0; // nothing to do. - suppress compiler warning
#if (CLIENT_OS == OS_RISCOS)
  if (GetProblemIndexFromPointer(this) == 1)
  {
      _kernel_swi_regs r;

      r.r[0] = 0;
      _kernel_swi(RC5PC_RetriveBlock,&r,&r);
  }
#endif

}

s32 Problem::IsInitialized()
{
  return (initialized!=0);
}

s32 Problem::LoadState( ContestWork * work, u32 _contest, 
                                            u32 _timeslice, int _cputype )
{
  contest = (_cputype != 0); //use up variable
  contest = _contest;

#if (CLIENT_OS == OS_RISCOS)
  int x86thread = 0;
  if (GetProblemPointerFromIndex(1) == this)
    {
    x86thread = 1;
    } 
#endif


#ifdef _CPU_32BIT_
  // copy over the state information
  contestwork.key.hi = ntohl( work->key.hi );
  contestwork.key.lo = ntohl( work->key.lo );
  contestwork.iv.hi = ntohl( work->iv.hi );
  contestwork.iv.lo = ntohl( work->iv.lo );
  contestwork.plain.hi = ntohl( work->plain.hi );
  contestwork.plain.lo = ntohl( work->plain.lo );
  contestwork.cypher.hi = ntohl( work->cypher.hi );
  contestwork.cypher.lo = ntohl( work->cypher.lo );
  contestwork.keysdone.hi = ntohl( work->keysdone.hi );
  contestwork.keysdone.lo = ntohl( work->keysdone.lo );
  contestwork.iterations.hi = ntohl( work->iterations.hi );
  contestwork.iterations.lo = ntohl( work->iterations.lo );
#if 0
  LogScreen("key    hi/lo:  %08x:%08x\n", contestwork.key.hi, contestwork.key.lo);
  LogScreen("iv     hi/lo:  %08x:%08x\n", contestwork.iv.hi, contestwork.iv.lo);
  LogScreen("plain  hi/lo:  %08x:%08x\n",
   contestwork.plain.hi, contestwork.plain.lo);
  LogScreen("cipher hi/lo:  %08x:%08x\n",
   contestwork.cypher.hi, contestwork.cypher.lo);
  LogScreen("iter   hi/lo:  %08x:%08x\n",
   contestwork.iterations.hi, contestwork.iterations.lo);
#endif

  // determine the starting key number
  // (note: doesn't account for carryover to hi or high end of keysdone)
  u64 key;
  key.hi = contestwork.key.hi;
  key.lo = contestwork.key.lo + contestwork.keysdone.lo;

  // set up the unitwork structure
  rc5unitwork.plain.hi = contestwork.plain.hi ^ contestwork.iv.hi;
  rc5unitwork.plain.lo = contestwork.plain.lo ^ contestwork.iv.lo;
  rc5unitwork.cypher.hi = contestwork.cypher.hi;
  rc5unitwork.cypher.lo = contestwork.cypher.lo;

  if (contest == 0)
    {
    rc5unitwork.L0.lo = ((key.hi >> 24) & 0x000000FFL) |
        ((key.hi >>  8) & 0x0000FF00L) |
        ((key.hi <<  8) & 0x00FF0000L) |
        ((key.hi << 24) & 0xFF000000L);
    rc5unitwork.L0.hi = ((key.lo >> 24) & 0x000000FFL) |
        ((key.lo >>  8) & 0x0000FF00L) |
        ((key.lo <<  8) & 0x00FF0000L) |
        ((key.lo << 24) & 0xFF000000L);
    } 
  else 
    {
    rc5unitwork.L0.lo = key.lo;
    rc5unitwork.L0.hi = key.hi;
    }

  // set up the current result state
  rc5result.key.hi = contestwork.key.hi;
  rc5result.key.lo = contestwork.key.lo;
  rc5result.keysdone.hi = contestwork.keysdone.hi;
  rc5result.keysdone.lo = contestwork.keysdone.lo;
  rc5result.iterations.hi = contestwork.iterations.hi;
  rc5result.iterations.lo = contestwork.iterations.lo;
  rc5result.result = RESULT_WORKING;

#endif

  //---------------------------------------------------------------

  pipeline_count = PIPELINE_COUNT;
  
#if (CLIENT_CPU == CPU_X86)
  if (contest == 0 && rc5_unit_func == rc5_unit_func_p5_mmx)
    pipeline_count = 4; // RC5 MMX core is 4 pipelines
#elif (CLIENT_CPU == CPU_ARM)
  if (_cputype == 0)
    pipeline_count = 1;
  else if (_cputype == 2)
    pipeline_count = 2;
  else
    pipeline_count = 3;
#endif
  
  tslice = (( pipeline_count + 1 ) & ( ~1L ));
  if ( _timeslice > tslice )
    tslice = ((_timeslice + (tslice - 1)) & ~(tslice - 1));

  //--------------------------------------------------------------- 

  startpercent = (u32) ( (double) 100000.0 *
     ( (double) (contestwork.keysdone.lo) /
       (double) (contestwork.iterations.lo) ) );
  percent=0;
  restart = ( work->keysdone.lo > 0 );

  initialized = 1;
  finished = 0;
  started = 0;


#if (CLIENT_OS == OS_RISCOS)
  if (x86thread == 1)
    {
    RC5PCstruct rc5pc;
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

    r.r[1] = (int)&rc5pc;
    _kernel_swi(RC5PC_AddBlock,&r,&r);
    if (r.r[0] == -1)
      {
      LogScreen("Failed to add block to x86 cruncher\n");
      }
    }
#endif

  return( 0 );
}

s32 Problem::RetrieveState( ContestWork * work , s32 setflags )
{
#ifdef _CPU_32BIT_
  // store back the state information
  work->key.hi = htonl( contestwork.key.hi );
  work->key.lo = htonl( contestwork.key.lo );
  work->iv.hi = htonl( contestwork.iv.hi );
  work->iv.lo = htonl( contestwork.iv.lo );
  work->plain.hi = htonl( contestwork.plain.hi );
  work->plain.lo = htonl( contestwork.plain.lo );
  work->cypher.hi = htonl( contestwork.cypher.hi );
  work->cypher.lo = htonl( contestwork.cypher.lo );
  work->keysdone.hi = htonl( contestwork.keysdone.hi );
  work->keysdone.lo = htonl( contestwork.keysdone.lo );
  work->iterations.hi = htonl( contestwork.iterations.hi );
  work->iterations.lo = htonl( contestwork.iterations.lo );
#endif
  if (setflags) 
    {
    initialized = 0;
    finished = 0;
    }
  return( contest );
}


s32 Problem::Run( u32 threadnum )
{
  u32 timeslice;
  if ( !initialized )
    return ( -1 );

  if ( finished )
    return ( 1 );

  if (!started)
    {
    struct timeval stop;
    CliTimer(&stop);
    timehi = stop.tv_sec;
    timelo = stop.tv_usec;
    #if (CLIENT_CPU != CPU_X86)
       started = (threadnum == 0); //squelch 'unused variable' warning
    #endif
    started=1;

#ifdef STRESS_THREADS_AND_BUFFERS 
    contestwork.keysdone.hi = contestwork.iterations.hi;
    contestwork.keysdone.lo = contestwork.iterations.lo;
    rc5result.result = RESULT_NOTHING;
    rc5result.key.hi = contestwork.key.hi;
    rc5result.key.lo = contestwork.key.lo;
    rc5result.keysdone.hi = contestwork.keysdone.hi;
    rc5result.keysdone.lo = contestwork.keysdone.lo;
    rc5result.iterations.hi = contestwork.iterations.hi;
    rc5result.iterations.lo = contestwork.iterations.lo;

    finished = 1;
    return 1;
#endif    
    }

  // don't allow a too large of a timeslice be used
  // (technically not necessary, but may save some wasted time)
  // note: doesn't account for high end or carry over
  if ( ( contestwork.keysdone.lo + tslice ) > contestwork.iterations.lo )
    timeslice = ( contestwork.iterations.lo - contestwork.keysdone.lo +
                pipeline_count - 1 ) / pipeline_count + 1;
  else
    timeslice = tslice / pipeline_count; //from the problem object
  
  if (timeslice <= tslice)
    timeslice = tslice;
  else
    timeslice = (( timeslice + (tslice - 1)) & ~(tslice - 1));

#if (CLIENT_CPU == CPU_POWERPC)
  {
  unsigned long kiter = 0;
  if (contest == 0) 
    {
#if ((CLIENT_OS != OS_BEOS) || (CLIENT_OS != OS_AMIGAOS))
    if (whichcrunch == 0)
      kiter = crunch_allitnil( &rc5unitwork, timeslice );
    else
#endif
      {
      do{
        kiter += crunch_lintilla( &rc5unitwork, timeslice - kiter );
        if (kiter < timeslice) 
          {
          if (crunch_allitnil( &rc5unitwork, 1 ) == 0) 
            {
            break;
            }
          kiter++;
          }
        } while (kiter < timeslice);
      }
    }
  else
    {
    // protect the innocent
    timeslice *= pipeline_count;

    u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

    if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
    else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
    timeslice = (1ul << nbits) / pipeline_count;
    kiter = des_unit_func ( &rc5unitwork, nbits );
    }

  // Mac code yields here because it needs to know the contest
  // DES code yields in the core itself because it can run for a
  // long time (with respect to good user response time) before returning
  #if (CLIENT_OS == OS_MACOS)
    if (contest == 0) {
      #if defined(MULTITHREAD)
	  if (MP_active == 0) {
		YieldToMain(1);		// cannot do this in real thread!
		}
      #else
  	  YieldToMain(1);
      #endif
  	}
  #endif

  contestwork.keysdone.lo += kiter;

  if (kiter < timeslice)
    {
    // found it?
    rc5result.key.hi = contestwork.key.hi;
    rc5result.key.lo = contestwork.key.lo;
    rc5result.keysdone.hi = contestwork.keysdone.hi;
    rc5result.keysdone.lo = contestwork.keysdone.lo;
    rc5result.iterations.hi = contestwork.iterations.hi;
    rc5result.iterations.lo = contestwork.iterations.lo;
    rc5result.result = RESULT_FOUND;
    finished = 1;
    return( 1 );
    }
  }
#elif (CLIENT_CPU == CPU_X86)
  {
  unsigned long kiter;
  if (contest == 0)
    {
    kiter = (rc5_unit_func) ( &rc5unitwork, timeslice );
    }
  else
    {
    // protect the innocent
    timeslice *= pipeline_count;
    u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

    if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
    else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
    timeslice = (1ul << nbits) / pipeline_count;

    if (threadnum == 0) //both are the same on non_mt
      kiter = (des_unit_func) ( &rc5unitwork, nbits );
    else
      kiter = (des_unit_func2) ( &rc5unitwork, nbits );
    }

  contestwork.keysdone.lo += kiter;
  if ( kiter < timeslice * pipeline_count )
    {
    // found it?
    rc5result.key.hi = contestwork.key.hi;
    rc5result.key.lo = contestwork.key.lo;
    rc5result.keysdone.hi = contestwork.keysdone.hi;
    rc5result.keysdone.lo = contestwork.keysdone.lo;
    rc5result.iterations.hi = contestwork.iterations.hi;
    rc5result.iterations.lo = contestwork.iterations.lo;
    rc5result.result = RESULT_FOUND;
    finished = 1;
    return( 1 );
    }
  else if ( kiter != timeslice * pipeline_count )
    {
    LogScreen("kiter wrong %ld %d\n", kiter, (int)(timeslice*pipeline_count));
    }
  }
#elif (CLIENT_CPU == CPU_SPARC) && (ULTRA_CRUNCH == 1)
  {
  unsigned long kiter;
  if (contest == 0) 
    {
    kiter = crunch( &rc5unitwork, timeslice );
    } 
  else 
    {
    // protect the innocent
    timeslice *= pipeline_count;
    u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

    if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
    else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
    timeslice = (1ul << nbits) / pipeline_count;
    kiter = des_unit_func ( &rc5unitwork, nbits );
    }
  contestwork.keysdone.lo += kiter;
  if (kiter < ( timeslice * pipeline_count ) )
    {
    // found it?
    rc5result.key.hi = contestwork.key.hi;
    rc5result.key.lo = contestwork.key.lo;
    rc5result.keysdone.hi = contestwork.keysdone.hi;
    rc5result.keysdone.lo = contestwork.keysdone.lo;
    rc5result.iterations.hi = contestwork.iterations.hi;
    rc5result.iterations.lo = contestwork.iterations.lo;
    rc5result.result = RESULT_FOUND;
    finished = 1;
    return( 1 );
    }
  else if (kiter != ( timeslice * pipeline_count ) )
    {
    LogScreen("kiter wrong %ld %d\n", (long) kiter, (int) (timeslice*pipeline_count));
    }
  }
#elif ((CLIENT_CPU == CPU_MIPS) && (MIPS_CRUNCH == 1))
  {
  unsigned long kiter;
  if (contest == 0) 
    {
    kiter = crunch( &rc5unitwork, timeslice );
    } 
  else 
    {
    // protect the innocent
    timeslice *= pipeline_count;
    u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

    if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
    else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
    timeslice = (1ul << nbits) / pipeline_count;
    kiter = des_unit_func ( &rc5unitwork, nbits );
    }
  contestwork.keysdone.lo += kiter;
  if (kiter < ( timeslice * pipeline_count ) )
    {
    // found it?
    rc5result.key.hi = contestwork.key.hi;
    rc5result.key.lo = contestwork.key.lo;
    rc5result.keysdone.hi = contestwork.keysdone.hi;
    rc5result.keysdone.lo = contestwork.keysdone.lo;
    rc5result.iterations.hi = contestwork.iterations.hi;
    rc5result.iterations.lo = contestwork.iterations.lo;
    rc5result.result = RESULT_FOUND;
    finished = 1;
    return( 1 );
    }
  else if (kiter != (timeslice * pipeline_count))
    {
    LogScreen("kiter wrong %ld %d\n", kiter, timeslice*pipeline_count);
    }
  }
#elif (CLIENT_CPU == CPU_ARM)
  {
  unsigned long kiter;
#if (CLIENT_OS == OS_RISCOS)
  if (_kernel_escape_seen())
    {
    CliSignalHandler(SIGINT);
    }
#endif
//  timeslice *= pipeline_count;
//  done in the cores.

  if (contest == 0)
    {
#if (CLIENT_OS == OS_RISCOS)
    if (threadnum == 0)
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
        rc5result.key.hi = contestwork.key.hi;
        rc5result.key.lo = contestwork.key.lo;
        rc5result.keysdone.hi = contestwork.keysdone.hi;
        rc5result.keysdone.lo = contestwork.keysdone.lo;
        rc5result.iterations.hi = contestwork.iterations.hi;
        rc5result.iterations.lo = contestwork.iterations.lo;
        rc5result.result = RESULT_FOUND;
        finished = 1;
        return( 1 );
        }
#if (CLIENT_OS == OS_RISCOS)
      }
    else // threadnum == 1
      {
	  /*
	    This is the RISC OS specific x86 2nd thread magic.
	  */
	  _kernel_swi_regs r;
	  volatile RC5PCstruct *rc5pcr;
	  _kernel_swi(RC5PC_BufferStatus,&r,&r);

	  /*
	    contestwork.keysdone.lo is 0 for a completed block,
	    so take care when setting it.
	   */
	  rc5pcr = (volatile RC5PCstruct *)r.r[1];

//LogScreen("x86: Keysdone %08lx",contestwork.keysdone.lo);
	  
	  if (r.r[2]==1)
	  {
	      /*
		block finished
	      */
	      r.r[0] = 0;
	      _kernel_swi(RC5PC_RetriveBlock,&r,&r);
	      rc5pcr = (volatile RC5PCstruct *)r.r[1];
	      
	      if (rc5pcr->result == RESULT_FOUND)
	      {
		  contestwork.keysdone.lo = rc5pcr->keysdone.lo;

		  rc5result.key.hi = contestwork.key.hi;
		  rc5result.key.lo = contestwork.key.lo;
		  rc5result.keysdone.hi = contestwork.keysdone.hi;
		  rc5result.keysdone.lo = contestwork.keysdone.lo;
		  rc5result.iterations.hi = contestwork.iterations.hi;
		  rc5result.iterations.lo = contestwork.iterations.lo;
		  rc5result.result = RESULT_FOUND;
		  finished = 1;
		  return( 1 );
	      }
	      else
	      {
		  contestwork.keysdone.lo = contestwork.iterations.lo;
	      }

	  }
	  else
	  {
	      contestwork.keysdone.lo = rc5pcr->keysdone.lo;
	  }
      }
#endif
    }
  else
    {
    // protect the innocent
    u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

    if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
    else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
    timeslice = (1ul << nbits) / pipeline_count;
    kiter = des_unit_func ( &rc5unitwork, nbits );
    contestwork.keysdone.lo += kiter;
    if (kiter < timeslice)
      {
      // found it?
      rc5result.key.hi = contestwork.key.hi;
      rc5result.key.lo = contestwork.key.lo;
      rc5result.keysdone.hi = contestwork.keysdone.hi;
      rc5result.keysdone.lo = contestwork.keysdone.lo;
      rc5result.iterations.hi = contestwork.iterations.hi;
      rc5result.iterations.lo = contestwork.iterations.lo;
      rc5result.result = RESULT_FOUND;
      finished = 1;
      return( 1 );
      }
    }
  }
#elif (CLIENT_CPU == CPU_68K)
  unsigned long kiter = 0;
  if (contest == 0) 
    {
    kiter = rc5_unit_func( &rc5unitwork, timeslice );
    if ( kiter < timeslice*pipeline_count )
      {
      // found it?
      rc5result.key.hi = contestwork.key.hi;
      rc5result.key.lo = contestwork.key.lo;
      rc5result.keysdone.hi = contestwork.keysdone.hi;
      rc5result.keysdone.lo = contestwork.keysdone.lo + kiter;
      rc5result.iterations.hi = contestwork.iterations.hi;
      rc5result.iterations.lo = contestwork.iterations.lo;
      rc5result.result = RESULT_FOUND;
      finished = 1;
      return( 1 );
      }
    // increment the count of keys done
    // note: doesn't account for carry
    contestwork.keysdone.lo += ((pipeline_count*timeslice) + pipeline_count);
    }
  else
    {
    timeslice *= pipeline_count;
    u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

    if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
    else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
    timeslice = (1ul << nbits) / pipeline_count;
    kiter = des_unit_func ( &rc5unitwork, nbits );
    contestwork.keysdone.lo += kiter;
    if (kiter < ( timeslice * pipeline_count ) )
      {
      // found it?
      rc5result.key.hi = contestwork.key.hi;
      rc5result.key.lo = contestwork.key.lo;
      rc5result.keysdone.hi = contestwork.keysdone.hi;
      rc5result.keysdone.lo = contestwork.keysdone.lo;
      rc5result.iterations.hi = contestwork.iterations.hi;
      rc5result.iterations.lo = contestwork.iterations.lo;
      rc5result.result = RESULT_FOUND;
      finished = 1;
      return( 1 );
      }
    else if (kiter != (timeslice * pipeline_count))
      {
      LogScreen("kiter wrong %ld %ld\n",
               (long) kiter, (long)(timeslice*pipeline_count));
      }
    }
#else
  unsigned long kiter = 0;
  if (contest == 0) 
    {
    while ( timeslice-- ) // timeslice ignores the number of pipelines
      {
      u32 result = rc5_unit_func( &rc5unitwork );
      if ( result )
        {
        // found it?
        rc5result.key.hi = contestwork.key.hi;
        rc5result.key.lo = contestwork.key.lo;
        rc5result.keysdone.hi = contestwork.keysdone.hi;
        rc5result.keysdone.lo = contestwork.keysdone.lo + result - 1;
        rc5result.iterations.hi = contestwork.iterations.hi;
        rc5result.iterations.lo = contestwork.iterations.lo;
        rc5result.result = RESULT_FOUND;
        finished = 1;
        return( 1 );
        }
      else
        {
        // "mangle-increment" the key number by the number of pipelines
        rc5unitwork.L0.hi = (rc5unitwork.L0.hi + (pipeline_count << 24)) & 0xFFFFFFFF;
        if (!(rc5unitwork.L0.hi & 0xFF000000)) 
          {
          rc5unitwork.L0.hi = (rc5unitwork.L0.hi + 0x00010000) & 0x00FFFFFF;
          if (!(rc5unitwork.L0.hi & 0x00FF0000)) 
            {
            rc5unitwork.L0.hi = (rc5unitwork.L0.hi + 0x00000100) & 0x0000FFFF;
            if (!(rc5unitwork.L0.hi & 0x0000FF00)) 
              {
              rc5unitwork.L0.hi = (rc5unitwork.L0.hi + 0x00000001) & 0x000000FF;
              if (!(rc5unitwork.L0.hi & 0x000000FF)) 
                {
                rc5unitwork.L0.hi = 0x00000000;
                rc5unitwork.L0.lo = rc5unitwork.L0.lo + 0x01000000;
                if (!(rc5unitwork.L0.lo & 0xFF000000)) 
                  {
                  rc5unitwork.L0.lo = (rc5unitwork.L0.lo + 0x00010000) & 0x00FFFFFF;
                  if (!(rc5unitwork.L0.lo & 0x00FF0000)) 
                    {
                    rc5unitwork.L0.lo = (rc5unitwork.L0.lo + 0x00000100) & 0x0000FFFF;
                    if (!(rc5unitwork.L0.lo & 0x0000FF00)) 
                      {
                      rc5unitwork.L0.lo = (rc5unitwork.L0.lo + 0x00000001) & 0x000000FF;
                      }
                    }
                  }
                }
              }
            }
          }
        // increment the count of keys done
        // note: doesn't account for carry
        contestwork.keysdone.lo += pipeline_count;
        }
      }
	  #ifdef MAC_GUI
	  #if defined(MULTITHREAD)
		  if (MP_Active == 0) {			
			  YieldToMain(1);
			  }
	  #else
	  	   YieldToMain(1);
	  #endif
	  #endif
    }
  else
    {
    timeslice *= pipeline_count;
    u32 nbits=1; while (timeslice > (1ul << nbits)) nbits++;

    if (nbits < MIN_DES_BITS) nbits = MIN_DES_BITS;
    else if (nbits > MAX_DES_BITS) nbits = MAX_DES_BITS;
    timeslice = (1ul << nbits) / pipeline_count;
    kiter = des_unit_func ( &rc5unitwork, nbits );
    contestwork.keysdone.lo += kiter;
    if (kiter < ( timeslice * pipeline_count ) )
      {
      // found it?
      rc5result.key.hi = contestwork.key.hi;
      rc5result.key.lo = contestwork.key.lo;
      rc5result.keysdone.hi = contestwork.keysdone.hi;
      rc5result.keysdone.lo = contestwork.keysdone.lo;
      rc5result.iterations.hi = contestwork.iterations.hi;
      rc5result.iterations.lo = contestwork.iterations.lo;
      rc5result.result = RESULT_FOUND;
      finished = 1;
      return( 1 );
      }
    else if (kiter != (timeslice * pipeline_count))
      {
      LogScreen("kiter wrong %ld %ld\n",
               (long) kiter, (long)(timeslice*pipeline_count));
      }
    }
#endif

  if ( ( contestwork.keysdone.hi > contestwork.iterations.hi ) ||
       ( ( contestwork.keysdone.hi == contestwork.iterations.hi ) &&
       ( contestwork.keysdone.lo >= contestwork.iterations.lo ) ) )
    {
    // done with this block and nothing found
    rc5result.result = RESULT_NOTHING;
    finished = 1;
    }
  else
    {
    // more to do, come back later.
    rc5result.result = RESULT_WORKING;
    finished = 0;
    }

  rc5result.key.hi = contestwork.key.hi;
  rc5result.key.lo = contestwork.key.lo;
  rc5result.keysdone.hi = contestwork.keysdone.hi;
  rc5result.keysdone.lo = contestwork.keysdone.lo;
  rc5result.iterations.hi = contestwork.iterations.hi;
  rc5result.iterations.lo = contestwork.iterations.lo;

#if 0
#if (CLIENT_OS == OS_RISCOS)    
if (!finished)
  LogScreen("Thread %d: Didn't find the key",threadnum);
else if (threadnum == 1)
  LogScreen("working... %08lx",contestwork.keysdone.lo);
#endif
#endif
  return( finished );
}


s32 Problem::GetResult( RC5Result * result )
{
  if ( !initialized )
    return ( -1 );

  // note that all but result go back to network byte order at this point.
  result->key.hi = htonl( rc5result.key.hi );
  result->key.lo = htonl( rc5result.key.lo );
  result->keysdone.hi = htonl( rc5result.keysdone.hi );
  result->keysdone.lo = htonl( rc5result.keysdone.lo );
  result->iterations.hi = htonl( rc5result.iterations.hi );
  result->iterations.lo = htonl( rc5result.iterations.lo );
  result->result = rc5result.result;

  return ( contest );
}

u32 Problem::CalcPercent()
{
  return (u32) ( (double) 100.0 *
                  ( ((double) rc5result.keysdone.lo) /
                    ((double) rc5result.iterations.lo) ) );
}

#if (CLIENT_OS == OS_MACOS) && defined(MAC_GUI)
u32 Problem::GetKeysDone()
{
  return(rc5result.keysdone.lo);
}
#endif

