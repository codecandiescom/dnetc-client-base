/*
 * Copyright distributed.net 1997-2002 - All Rights Reserved
 * For use in distributed.net projects only.
 * Any other distribution or use of this source violates copyright.
 *
 * $Id: main.h,v 1.1.2.1 2002/04/11 11:44:56 oliver Exp $
 *
 * Created by Oliver Roberts <oliver@futaura.co.uk>
 *
 * ----------------------------------------------------------------------
 * ReAction GUI module for AmigaOS clients
 * ----------------------------------------------------------------------
*/

extern struct MsgPort *IDCMPPort;
extern struct Gadget *GlbGadgetsP[];
extern struct Window *GlbIWindowP;
extern Object *GlbWindowP;

struct ConsoleLines {
   struct List list;
   ULONG numlines;
};

extern struct ConsoleLines ConsoleLines68K, ConsoleLinesPPC;

enum { GAD_MAINLAYOUT=0, GAD_CON68K, GAD_CONPPC, NUM_GADS };

#undef NewObject

VOID UpdateGadget(struct Window *win, struct Gadget *gad, ...);
