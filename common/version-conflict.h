// Hey, Emacs, this a -*-C++-*- file !

// Copyright distributed.net 1997-1998 - All Rights Reserved
// For use in distributed.net projects only.
// Any other distribution or use of this source violates copyright.
// 
// $Log: version-conflict.h,v $
// Revision 1.19  1998/10/10 18:26:19  silby
// Updated version information in preparation of beta2 launch, added comments about versioning.
//
// Revision 1.18  1998/10/05 07:22:15  chrisb
// Added 2 weeks to "BETA_EXPIRATION_TIME" since it expired this weekend.
//
// Revision 1.17  1998/10/04 03:23:39  silby
// Bumped the beta timeout ahead a few days.
//
// Revision 1.16  1998/09/28 12:46:26  cyp
// removed checkifbetaexpired prototype
//
// Revision 1.15  1998/09/23 22:25:43  silby
// There, now it's int. All better. :)
//
// Revision 1.14  1998/09/23 22:20:19  blast
// Blargh Silby.
// Changed my #if 0 to //
//
// Revision 1.13  1998/09/23 22:17:33  blast
// Added #if 0 around the whole BETA client thing
//
// Revision 1.12  1998/09/19 08:50:22  silby
// Added in beta test client timeouts.  Enabled/controlled from version.h by defining BETA, and setting the expiration time.
//
// Revision 1.11  1998/08/20 02:40:41  silby
// Kicked version to 2.7100.418-BETA1, ensured that clients report the string ver (which has beta1 in it) in the startup.
//
// Revision 1.10  1998/07/22 04:28:49  jlawson
// updated version to 417
//
// Revision 1.9  1998/07/12 09:09:24  silby
// updates to 416
//
// Revision 1.8  1998/07/12 08:05:12  silby
// Updated to 416, updated changelog
//
// Revision 1.7  1998/07/07 21:55:55  cyruspatel
// client.h has been split into client.h and baseincs.h 
//
// Revision 1.6  1998/07/07 03:10:22  silby
// Updated to build 414
//
// Revision 1.5  1998/06/29 17:05:43  daa
// bump to 413.
//
// Revision 1.4  1998/06/28 20:52:11  jlawson
// added version string without leading "v" character
//
// Revision 1.3  1998/06/26 15:48:08  daa
// Its Here....V2.7100.412
//
// Revision 1.2  1998/06/25 14:07:18  daa
// add DCTI copyright notice and cvs log header
// bump version to 7026.411
//
//

#ifndef _VERSION_H
#define _VERSION_H

// Note regarding beta vs release versions. Since we can only see
// CLIENT_CONTEST and CLIENT_BUILD in proxy logs for now, let's make
// sure to keep all betas an ODD CLIENT_BUILD and all releases
// an EVEN CLIENT_BUILD. (Naturally, incrementing the CLIENT_BUILD_FRAC
// as well.

#define CLIENT_CONTEST      71
#define CLIENT_BUILD        01
#define CLIENT_BUILD_FRAC   418

#define CLIENT_VERSIONSTRING    "v2.7101.418-BETA2"
#define CLIENT_VERSIONSTRING2   "2.7101.418-BETA2"        // no leading "v"

// When releasing a beta client, please set the expiration time to
// about two weeks into the future; that should be an adequate beta
// time period.

#define BETA
#define BETA_EXPIRATION_TIME    908785600


#endif // _VERSION_H

