#ifndef ABC_IHDRS_I3IN2_HEADER
#define ABC_IHDRS_I3IN2_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* Interpreter utilities */

value v_local(value name, value number);
value v_global(value name);
loc local_loc(basidf i);
loc global_loc(basidf i);
loc trim_loc(loc l, value N, char sign);
loc tbsel_loc(loc R, value K);
value pre_fun(value nd1, intlet pre, value nd2);
bool in_ranger(loc l, value *pv);

extern value resval;

value evalthread(parsetree start);

#define Changed_formal(v) (v == Vnil || !Is_indirect(v))

value locvalue(loc l, value **ll, bool err);

#endif  /* HEADER GUARD */
