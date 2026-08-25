#ifndef ABC_EHDRS_GETC_HEADER
#define ABC_EHDRS_GETC_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

#include "b.h"

extern Procedure initkeys(void);

typedef struct tabent {
	int code;
	int deflen;
	string def;
	string rep;
	string name;
} tabent;

extern struct tabent deftab[];
extern int ndefs; 		/* number of entries in deftab */
extern Procedure addkeydef(int code,  int deflen, string def, string rep,  string name);

#ifdef KEYS

#define MAXDEFS 200

#else

#define MAXDEFS 100
extern Procedure initgetc(void);
extern Procedure endgetc(void);

#endif

#endif  /* HEADER GUARD */
