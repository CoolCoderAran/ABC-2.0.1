/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* Generic version, as basis for porting.
 * See Portability Guide (./PORTING.DOC) for details.
 */

/* Handle signals */

#include "b.h"

#ifdef SIGNAL

#include <signal.h>

Visible bool intrptd = No;

/**
 * sig == SIGINT
 */
Hidden SIGTYPE intsig(int sig)
{
	intrptd = Yes;
	signal(sig, intsig);
}

Visible Procedure initsig()
{
	signal(SIGINT, intsig);  /* interrupt */
}

#endif /* SIGNAL */

