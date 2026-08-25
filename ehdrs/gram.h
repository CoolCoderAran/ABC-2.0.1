#ifndef ABC_EHDRS_GRAM_HEADER
#define ABC_EHDRS_GRAM_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/*
 * Routines defined in "gram.c".
 */

#include "b.h"

string *noderepr(register node n);
node gram(register int sym);
Visible string symname(int sym);
bool allows_colon(int sym);

/*
 * Macros for oft-used functions.
 */

#define Fwidth(str) ((str) ? fwidth(str) : 0)

#define Fw_zero(str) (!(str) || strchr("\b\t", (str)[0]))
#define Fw_positive(str) ((str) && (str)[0] >= ' ')
#define Fw_negative(str) ((str) && (str)[0] == '\n')

#define MAXNBUILTIN 50	/* should be calculated by boot/mktable */

#endif  /* HEADER GUARD */
