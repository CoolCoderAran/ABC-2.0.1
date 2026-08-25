#ifndef ABC_BHDRS_BMEM_HEADER
#define ABC_BHDRS_BMEM_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* B memory management */

#include <stddef.h>

#include "visibility.h"

typedef char *ptr;
#define Nil ((ptr) 0)

ptr getmem(size_t size);
ptr savestr(string s);
#define freestr(s) (freemem((ptr)(s)))

#ifdef MEMTRACE
typedef unsigned long address;	/* for PC and symbol table (on a tahoe) */
#define F_ALLOC 'A'
#define F_FREE  'F'
#endif

struct bufadm {char *buf, *pbuf, *end; };
typedef struct bufadm bufadm;

#endif  /* HEADER GUARD */
