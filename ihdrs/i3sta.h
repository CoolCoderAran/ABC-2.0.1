#ifndef ABC_IHDRS_I3STA_HEADER
#define ABC_IHDRS_I3STA_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

#include "b.h"

Visible Procedure formula(value nd1, value name, value nd2, value tor);
Visible Procedure proposition(value nd1, value name, value nd2, value pred);

extern parsetree pc; /* 'Program counter', current parsetree node */
extern parsetree next; /* Next parsetree node (changed by jumps) */
extern bool report; /* 'Condition code register', outcome of last test */

#endif  /* HEADER GUARD */
