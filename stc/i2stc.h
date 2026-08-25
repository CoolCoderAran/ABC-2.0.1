#ifndef ABC_STC_I2STC_HEADER
#define ABC_STC_I2STC_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/*************************************************************************/

/* polytype representation */

#include "b.h"

typedef value typekind;
typedef value polytype;

/* accessing, NOT giving new values */

typekind kind(polytype u);
intlet nsubtypes(polytype u);
polytype subtype(polytype u, intlet i);
polytype asctype(polytype u);
polytype keytype(polytype u);
value ident(polytype u);

/* MaKe Types, where subtypes are "eaten" */

polytype mkt_polytype(typekind k, intlet nsub);
				/* visible only in bunif.c */
/* Procedure putsubtype(); */	/* polytype sub, *pcomp; intlet isub */
				/* to be used after mkt_polytype or
				 * mkt_compound */

polytype mkt_number(void);
polytype mkt_text(void);
polytype mkt_tn(void);
polytype mkt_error(void);
polytype mkt_list(polytype s);
polytype mkt_table(polytype k, polytype a);
polytype mkt_lt(polytype s);
polytype mkt_tlt(polytype s);
polytype mkt_compound(intlet nsub);
polytype mkt_var(value id);
polytype mkt_newvar(void);
polytype mkt_ext(void);

polytype p_copy(polytype u);
/* Procedure p_release(); */		/* polytype u */

/* predicates */

bool are_same_types(polytype u, polytype v);
bool have_same_structure( polytype u, polytype v );

bool t_is_number( typekind k );
bool t_is_text( typekind k );
bool t_is_tn( typekind k );
bool t_is_error( typekind k );
bool t_is_list( typekind k );
bool t_is_table( typekind k );
bool t_is_lt( typekind k );
bool t_is_tlt( typekind k );
bool t_is_compound( typekind k );
bool t_is_var( typekind k );
bool t_is_ext( typekind k );
bool has_number( typekind k );
bool has_text( typekind k );
bool has_lt( typekind k );

/* typetable */

/* Procedure repl_type_of(); */ /* polytype u, tu */
bool table_has_type_of( polytype u );
polytype bottomtype( polytype u );
polytype bottomvar( polytype u );

/* Procedure usetypetable(); */		/* value t */
/* Procedure deltypetable(); */

/* init */

/* Procedure initpol(); */ 	/* */

/*************************************************************************/

/* unification of polytypes */

/* Procedure unify(); */ 	/* polytype a, b, &u; bool &bad */

bool contains( polytype u, polytype a );
bool equal_vars( polytype s, polytype a );

/*************************************************************************/

/* type unification errors */

/* Procedure start_vars(); */ 		/* */
/* Procedure add_var(); */		/* polytype tvar */
/* Procedure end_vars(); */		/* */

/* Procedure setreprtable(); */ 	/* */
/* Procedure delreprtable(); */		/* */

/* Procedure badtyperr(); */		/* polytype a, b */
/* Procedure cyctyperr(); */		/* polytype a */

Visible value conc(value v, value w);

#endif  /* HEADER GUARD */
