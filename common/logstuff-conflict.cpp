// Copyright distributed.net 1997-1999 - All Rights Reserved
// For use in distributed.net projects only.
// Any other distribution or use of this source violates copyright.

//
// ----------------------------------------------------------------------
// Created by Cyrus Patel (cyp@fb14.uni-mainz.de) 
// ----------------------------------------------------------------------
//
// $Log: logstuff-conflict.cpp,v $
// Revision 1.21.2.9  1999/01/30 16:03:26  remi
// Synced with :
//
//  Revision 1.30  1999/01/29 18:47:28  jlawson
//  fixed formatting.
//
// Revision 1.21.2.8  1999/01/23 14:08:00  remi
// Synced with :
//
//  Revision 1.29  1999/01/21 21:55:56  cyp
//  added reentrancy protection (log->mail->network->log->mail->... ad nauseum)
//
//  Revision 1.28  1999/01/17 14:41:16  cyp
//  Added leading/trailing whitespace stripping and "none" check to logfilename.
//
// Revision 1.21.2.7  1999/01/17 12:40:51  remi
// Synced with :
//
//  Revision 1.27  1999/01/13 10:46:15  cramer
//  Cosmetic update (comments and indenting)
//
// Revision 1.21.2.6  1999/01/09 11:37:16  remi
// Synced with :
//
//  Revision 1.26  1999/01/08 02:56:26  michmarc
//  Fix a trailing ; typo; plus change va_arg for those platforms
//  (like Alpha/NT) where va_arg is a structure, not a pointer
//
// Revision 1.21.2.5  1999/01/04 02:19:18  remi
// Synced with :
//
//  Revision 1.25  1999/01/01 02:45:15  cramer
//  Part 1 of 1999 Copyright updates...
//
//  Revision 1.24  1998/12/29 20:59:41  silby
//  Removed guistuff.h.
//
// Revision 1.21.2.4  1998/12/28 16:44:06  remi
// Fixed the merge.
//
// Revision 1.21.2.3  1998/12/28 14:59:25  remi
// Synced with :
//  Revision 1.23  1998/12/08 05:46:15  dicamillo
//  Add MacOS as a client that doesn't support ftruncate.
//
//  Revision 1.22  1998/11/28 19:44:34  cyp
//  InitializeLogging() and DeinitializeLogging() are no longer Client class
//  methods.
//
// Revision 1.21.2.2  1998/11/08 11:51:26  remi
// Lots of $Log tags.
//
// Synchronized with official 1.21

//-------------------------------------------------------------------------

#if (!defined(lint) && defined(__showids__))
const char *logstuff_cpp(void) {
return "@(#)$Id: logstuff-conflict.cpp,v 1.21.2.9 1999/01/30 16:03:26 remi Exp $"; }
#endif

//-------------------------------------------------------------------------

#include "cputypes.h"
#include "client.h"    // MAXCPUS, Packet, FileHeader, Client class, etc
#include "baseincs.h"  // basic (even if port-specific) #includes
#include "clitime.h"   // CliGetTimeString(NULL,1)
#include "problem.h"   // needed for logscreenpercent
#include "cmpidefs.h"  // strcmpi()
#include "console.h"   // for ConOut() and ConIsScreen()
#include "triggers.h"  // don't print percbar if pause/exit/restart triggered
#include "logstuff.h"  // keep the prototypes in sync

//-------------------------------------------------------------------------

#if (CLIENT_OS == OS_NETWARE || CLIENT_OS == OS_DOS || \
     CLIENT_OS == OS_OS2 || CLIENT_OS == OS_WIN16 || \
     CLIENT_OS == OS_WIN32 || CLIENT_OS == OS_WIN32S )
  #define ftruncate( fd, sz )  chsize( fd, sz )
#elif (CLIENT_OS == OS_VMS || CLIENT_OS == OS_RISCOS || \
     CLIENT_OS == OS_AMIGAOS || CLIENT_OS == OS_MACOS)
  #define ftruncate( fd, sz ) //nada, not supported
  #define FTRUNCATE_NOT_SUPPORTED
#endif  

#ifdef DONT_USE_PATHWORK
  #define GetFullPathForFilename( x ) ( x )
#endif  

#if ((!defined(MAX_LOGENTRY_LEN)) || (MAX_LOGENTRY_LEN < 1024))
  #ifdef MAX_LOGENTRY_LEN
  #undef MAX_LOGENTRY_LEN
  #endif
  #define MAX_LOGENTRY_LEN 1024
#endif   

// ========================================================================

static struct 
{
  int initlevel;
  char loggingTo;            // LOGTO_xxx bitfields 
  char spoolson;             // mail/file logging and time stamping is on/off.
  char percprint;            // percentprinting is enabled
  
  char stdoutisatty;         //log screen can handle lines not ending in '\n'
  char stableflag;           //last log screen did end in '\n'
  char lastwasperc;          //last log screen was a percentbar
  
} logstatics = { 
  0, 			// initlevel
  LOGTO_NONE,		// loggingTo
  0,			// spoolson
  0,			// percprint
  0,			// stdoutisatty
  0,			// stableflag
  0 };			// lastwasperc

// ========================================================================

static void InternalLogScreen( const char *msgbuffer, unsigned int msglen, int /*flags*/ )
{
  if ((logstatics.loggingTo & LOGTO_SCREEN) != 0)
    {
    if ( msglen && (msgbuffer[msglen-1] == '\n' || ConIsScreen() ) )
      {
      if (strlen( msgbuffer ) == msglen) //we don't do binary data
        ConOut( msgbuffer );             //which shouldn't happen anyway.
      }
    else
      ConOut( "" ); //flush.
    }
  return;
}

// ------------------------------------------------------------------------

// On NT/Alpha (and maybe some other platforms) the va_list type is a struct,
// not an int or a pointer-type.  Hence, NULL is not a valid va_list.  Pass
// a (va_list *) instead to avoid this problem
void LogWithPointer( int loggingTo, const char *format, va_list *arglist ) 
{
  static int recursion_check = 0;
  if ((++recursion_check) > 2) /* log->mail->network */
    loggingTo &= ~LOGTO_MAIL;

  char msgbuffer[MAX_LOGENTRY_LEN];
  unsigned int msglen = 0;
  char *buffptr, *obuffptr;
  const char *timestamp;
  int sel, old_loggingTo = loggingTo;
  
  msgbuffer[0]=0;
  loggingTo &= (logstatics.loggingTo|LOGTO_RAWMODE);


  if ( !format || !*format )
    loggingTo = LOGTO_NONE;
  
  if ( loggingTo != LOGTO_NONE && *format == '\r' )   //can only be screen
    {                                                 //(or nothing)
    if (( loggingTo & LOGTO_SCREEN ) != 0 )
      loggingTo = (LOGTO_SCREEN|LOGTO_RAWMODE);  //force into raw mode
    else
      loggingTo = LOGTO_NONE;
    }

  if ( loggingTo != LOGTO_NONE )
    {
    if ( arglist == NULL )
      strcat( msgbuffer, format );
    else 
      vsprintf( msgbuffer, format, *arglist );
    msglen = strlen( msgbuffer );

    if ( msglen == 0 )
      loggingTo = LOGTO_NONE;
    else if (msgbuffer[msglen-1] != '\n')
      loggingTo &= LOGTO_SCREEN|LOGTO_RAWMODE;  //screen only obviously
    }

  if (loggingTo != LOGTO_NONE && logstatics.spoolson /* timestamps */ && 
      (old_loggingTo & LOGTO_RAWMODE) == 0 )
    {
    buffptr = &msgbuffer[0];
    sel = 1;
    do{
      while (*buffptr == '\r' || *buffptr=='\n' )
          buffptr++;
      if (*buffptr == ' ' || *buffptr == '\t')
        {
        obuffptr = buffptr;
        while (*obuffptr == ' ' || *obuffptr == '\t')
          obuffptr++;
        memmove( buffptr, obuffptr, strlen( obuffptr )+1 );
        }
      if (*buffptr && *buffptr!='[' && *buffptr!='\r' && *buffptr!='\n' )
        {
        timestamp = CliGetTimeString( NULL, sel );
        memmove( buffptr+(strlen(timestamp)+3), buffptr, strlen( buffptr )+1 );
        *buffptr++=((sel)?('['):(' '));
        while (*timestamp) 
          *buffptr++ = *timestamp++;
        *buffptr++=((sel)?(']'):(' '));
        *buffptr=' ';
        }
      sel = 0;
      while (*buffptr && *buffptr != '\n' && *buffptr != '\r')
        buffptr++;
      } while (*buffptr);
    msglen = strlen( msgbuffer );
    }

  #ifdef ASSERT_WIDTH_80  //"show" where badly formatted lines are cropping up
  if (loggingTo != LOGTO_NONE)
    {
    buffptr = &msgbuffer[0];
    do{
      while (*buffptr == '\r' || *buffptr == '\n' )
         buffptr++;
      obuffptr = buffptr;
      while (*buffptr && *buffptr != '\r' && *buffptr != '\n' )
        buffptr++;
      if ((buffptr-obuffptr) > 79)
        {
        obuffptr[75] = ' '; obuffptr[76] = obuffptr[77] = obuffptr[78] = '.';
        memmove( obuffptr+79, buffptr, strlen(buffptr)+1 );
        buffptr = obuffptr+79;
        }    
      } while (*buffptr);
    msglen = strlen( msgbuffer );
    }      
  #endif

  if (( loggingTo & LOGTO_SCREEN ) != 0)
    {
    buffptr = &msgbuffer[0];
    if ((loggingTo & LOGTO_RAWMODE)==0)
      {
      if (*buffptr=='\n' && logstatics.stableflag) 
        {
        buffptr++;
        msglen--;
        }
      else if (*buffptr!='\r' && *buffptr!='\n' && !logstatics.stableflag) 
        {
        msglen++;
        memmove( msgbuffer+1, msgbuffer, msglen );
        msgbuffer[0] = '\n';
  // CRAMER - should this even be here? (I think not)
        logstatics.stableflag = 1;
        }
      }  
    if (msglen)
      {
      logstatics.lastwasperc = 0; //perc bar looks for this
      logstatics.stableflag = ( buffptr[(msglen-1)] == '\n' );
      InternalLogScreen( buffptr, msglen, 0 );
      }
    }
  
  return;
}

// ------------------------------------------------------------------------

void LogFlush( int forceflush )
{
  if (( logstatics.loggingTo & LOGTO_SCREEN ) != 0)
    {
    if ( logstatics.stableflag == 0 )
      LogWithPointer( LOGTO_SCREEN, "\n", NULL ); //LF if needed then fflush()
    }
  return;
}

// ------------------------------------------------------------------------

void LogScreen( const char *format, ... )
{
  va_list argptr;
  va_start(argptr, format);
  LogWithPointer( LOGTO_SCREEN, format, &argptr );
  va_end(argptr);
  return;
}    

void LogScreenRaw( const char *format, ... )
{
  va_list argptr;
  va_start(argptr, format);
  LogWithPointer( LOGTO_RAWMODE|LOGTO_SCREEN, format, &argptr );
  va_end(argptr);
  return;
}  

void Log( const char *format, ... )
{
  va_list argptr;
  va_start(argptr, format);
  LogWithPointer( LOGTO_SCREEN, format, &argptr );
  va_end(argptr);
  return;
}  

void LogRaw( const char *format, ... )
{
  va_list argptr;
  va_start(argptr, format);
  LogWithPointer( LOGTO_RAWMODE|LOGTO_SCREEN, format, &argptr );
  va_end(argptr);
  return;
}  

// ---------------------------------------------------------------------------

#include "probman.h"

void LogScreenPercent( unsigned int load_problem_count )
{
  static unsigned int lastperc = 0, displevel = 0;
  unsigned int percent, restartperc, endperc, equals, prob_i;
  int isatty, multiperc;
  char ch; char buffer[88];
  char *bufptr = &buffer[0];
  unsigned char pbuf[30]; /* 'a'-'z' */

  if (CheckExitRequestTrigger() || CheckPauseRequestTrigger() || 
    !logstatics.percprint || ( logstatics.loggingTo & LOGTO_SCREEN ) == 0 )
    return;

  isatty  = ConIsScreen();
  endperc = restartperc = 0;

  for (prob_i = 0; prob_i < load_problem_count; prob_i++)
    {
    Problem *selprob = GetProblemPointerFromIndex(prob_i);
    percent = 0; 

    if (selprob && selprob->IsInitialized())
      {
      if (selprob->finished)
        percent = 100;
      else if (selprob->started)
        percent = selprob->CalcPercent();
      if (percent == 0)
        percent = selprob->startpercent / 1000;
      if (load_problem_count == 1 && percent != 100) 
        {   /* don't do 'R' if multiple-problems */
        restartperc = selprob->startpercent / 1000;
        restartperc = (!restartperc || percent == 100) ? 0 : 
            ( restartperc - ((restartperc > 90) ? (restartperc & 1) : 
            (1 - (restartperc & 1))) );
        }
      if (percent > endperc)
          endperc = percent;
      if (percent && ((percent>90)?((percent&1)!=0):((percent&1)==0)))
        percent--;  /* make sure that it is visible */
      }
    if (load_problem_count <= 26) /* a-z */
      pbuf[prob_i] = (unsigned char)(percent);
    }
    
  if (!logstatics.lastwasperc || isatty)
    lastperc = 0;
  multiperc = (load_problem_count > 1 && load_problem_count <= 26 /*a-z*/ 
                 && lastperc == 0 && isatty && endperc < 100);
  if (lastperc == 0 && endperc > 0 && isatty )
    *bufptr++ = '\r';

  for (percent = lastperc+1; percent <= endperc; percent++)
    {
    if ( percent >= 100 )
      { strcpy( bufptr, "100\n" ); bufptr+=sizeof("100\n"); endperc=0; break;}
    else if ( ( percent % 10 ) == 0 )
      { sprintf( bufptr, "%d%%", (int)(percent) ); bufptr+=3; }
    else if ( restartperc == percent) 
      { *bufptr++='R'; }
    else if (((percent&1)?(percent<90):(percent>90)))
      {
      ch = '.';
      if (multiperc)
        {
        equals = 0;
        for ( prob_i=0; prob_i<load_problem_count; prob_i++ )
          {
          if ( pbuf[prob_i] == (unsigned char)(percent) )
            {
            ch = (char)('a'+prob_i);
            if ( (++equals)>displevel )
              break;
            }
          }
        }
      *bufptr++ = ch; 
      }
    }
  displevel++;
  if (displevel >= load_problem_count)
    displevel=0;
  lastperc = endperc;

  if ( (buffer[0]==0) || (buffer[0]=='\r' && buffer[1]==0) )
    ;
  else
    {
    *bufptr = 0;
    LogWithPointer( LOGTO_SCREEN|LOGTO_RAWMODE, buffer, NULL );
    logstatics.stableflag = (endperc == 0);  //cursor is not at column 0 
    logstatics.lastwasperc = (endperc != 0); //percbar requires reset
    }
  return;
}

// ------------------------------------------------------------------------

void DeinitializeLogging(void)
{
  return;
}

// ---------------------------------------------------------------------------

void InitializeLogging( int noscreen, int nopercent, const char *logfilename, 
                        unsigned int logfiletype, int logfilelimit, 
                        long mailmsglen, const char *smtpsrvr, 
                        unsigned int smtpport, const char *smtpfrom, 
                        const char *smtpdest, const char *id )
{
  DeinitializeLogging();
    
  logstatics.loggingTo = LOGTO_SCREEN;
  logstatics.lastwasperc = 0;
  logstatics.spoolson = 1;
  logstatics.percprint = (nopercent == 0);

  logstatics.stableflag = 0;   //assume next log screen needs a '\n' first

  return;
}  

// ---------------------------------------------------------------------------

