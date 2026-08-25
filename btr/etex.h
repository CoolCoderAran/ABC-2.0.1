#ifndef ABC_BTR_ETEX_HEADER
#define ABC_BTR_ETEX_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

extern int e_length(value v);
extern value mk_etext(string m);
extern char e_ncharval(int n, value v);
extern string e_strval(value v);
extern string e_sstrval(value v);
/* extern Procedure e_fstrval(); */
extern value e_icurtail( value v, int k);
extern value e_ibehead(value v, int k);
extern value e_concat(value s, value t);
/* extern Procedure e_concto(); */

#endif  /* HEADER GUARD */
