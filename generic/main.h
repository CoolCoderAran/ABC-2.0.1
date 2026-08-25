#ifndef ABC_GENERIC_MAIN_HEADER
#define ABC_GENERIC_MAIN_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* Generic version, as basis for porting.
 * See Portability Guide (./PORTING.DOC) for details.
 */

/* In this file you can put your external definitions of functions,
   which are only used inside this directory */

#include "b.h"

Visible Porting long filemodtime(char *filename);
Visible Porting Procedure freepath(char *path);
Visible Porting char *curdir(void);

#endif  /* HEADER GUARD */
