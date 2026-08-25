#ifndef ABC_UNIX_COMP_HEADER
#define ABC_UNIX_COMP_HEADER

/* This file was generated and edited by the Setup command. */

/* To make lasting changes edit the files unix/comp.h.gen and Setup, */
/* and run Setup again. */

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

typedef char literal;	/* to emphasize meaning only */

typedef int expint;	/*The 2nd argument of frexp points to this */
			/*(see manual page frexp(3)).              */
			/*On some 68K systems must be short (foo!) */

/* can we #include <signal.h>: (check man 3 signal) */
#define SIGNAL
/* type returned by signal handler function: (used to be int) */
#define SIGTYPE void

/* can #include <setjmp.h> */
#define SETJMP

/* #define NO_VOID if your compiler doesn't have void */
#undef NO_VOID
/* the NO_VOID define is also used in b.h for Procedure */
/* VOID is used in casts only, for quieter lint */
#ifdef NO_VOID
#define VOID
#else
#define VOID (void)
#endif

/* C compiler knows structure assignment */
#define STRUCTASS

/* use index for strchr */

/* and rindex for strrchr */

#endif  /* HEADER GUARD */
