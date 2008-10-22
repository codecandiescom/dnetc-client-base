/* do_s1_s3.c v4.0 */

/*
 * $Log: do_s1_s3.c,v $
 * Revision 1.2  1998/06/14 15:18:37  remi
 * Avoid tons of warnings due to a brain-dead CVS.
 *
 * Revision 1.1.1.1  1998/06/14 14:23:49  remi
 * Initial integration.
 *
 */

#include "des.h"

static char *id="@(#)$Id: do_s1_s3.c,v 1.2 1998/06/14 15:18:37 remi Exp $";

unsigned long
do_s1_s3 ( register struct INNER_OFFSET_DISTANCES *offsets
) {
    register INNER_LOOP_SLICE *in = offsets->Source;
    register INNER_LOOP_SLICE *merge = offsets->Merge;
    register INNER_LOOP_SLICE *out = offsets->Dest;

    register INNER_LOOP_SLICE a1, a2, a3, a4, a5, a6;

/* set sentinel value into high half of frame pointer */
    SET_64_BIT_SENTINEL;

    PIPELINE_S1 (in, offsets);

    do {
	wrap_s1 (in, merge, out, offsets, a3, a5, a4, a6);

	PIPELINE_S3 (in, offsets);

	wrap_s3 (in, merge, out, offsets, a2, a3, a6, a5);

/* link forward to next pass, dovetailed into previous function */
	in = offsets->Next_Source;
	offsets = offsets->Next_Offsets;

	PIPELINE_S1 (in, offsets);

	merge = offsets->Merge;
	out = offsets->Dest;

    } while (merge != (INNER_LOOP_SLICE *)0);

/* return sentinel value in high half of frame pointer */
    RETURN_64_BIT_SENTINEL;
}