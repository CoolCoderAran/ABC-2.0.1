#ifndef ABC_BTR_I1TLT_HEADER
#define ABC_BTR_I1TLT_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* Private definitions for B texts, lists and tables */

#include "btr/i1btr.h"

typedef struct telita {
    HEADER; btreeptr root;
} a_telita, *telita;

#define Itemtype(v) (((telita) (v))->len) /* Itemtype */
#define Root(v) (((telita) (v))->root)
#define Tltsize(v) (Root(v) EQ Bnil ? 0 : Size(Root(v)))

#define Character(v)	((bool) (Type(v) EQ Tex && Tltsize(v) EQ 1))

#endif  /* HEADER GUARD */
