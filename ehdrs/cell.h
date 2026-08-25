#ifndef ABC_EHDRS_CEL_HEADER
#define ABC_EHDRS_CEL_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/*
 * B editor -- Definitions for linked lists of screen lines, baptized `cells'.
 * (This is NOT an abstract data type!)
 */

#include "bedi.h"

struct cell {
	struct cell *c_link;
	node c_data;
	short c_onscreen;
	short c_oldindent;
	short c_newindent;
	short c_length;
	char c_oldvhole;
	char c_newvhole; /* Yes if this line contains a `vhole' */
	char c_oldfocus;
	char c_newfocus; /* Yes if this line contains underlining */
};

typedef struct cell cell;

#define Cnil ((cell*) NULL)

#define Nowhere (-9999)

#define SpaceRound(x) ((indent + (x) + llength - 1) / llength)
#define Newspace(p) \
	SpaceRound((p)->c_length + (p)->c_newindent + (p)->c_newvhole)
#define Oldspace(p) \
	SpaceRound((p)->c_length + (p)->c_oldindent + (p)->c_oldvhole)

extern int llength;
extern int winheight;
extern int indent;

#endif  /* HEADER GUARD */
