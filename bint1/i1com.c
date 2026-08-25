/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/************************************************************************/
/* Hows Funs and other odd types that don't fit anywhere else           */
/* and are modelled as compounds                                        */
/*                                                                      */
/* Compounds are handled in bobj.h                                     */
/*                                                                      */
/************************************************************************/

#include "b.h"
#include "bint.h"
#include "bobj.h"
#include "i2nod.h"
#include "i2gen.h" /* Must be after i2nod.h */
#include "i3env.h"

/* Values */

/* Rangebounds is a special compound of the 2 lwb..upb fields */
/* used for the evaluation of mixed list_displays like {a;b..z} */
/* More #define's for its interface are in bint.h */

#define RANGE_ILLEGAL	MESS(1400, "in p..q, p is neither a text nor a number")

#define NRANGE_L_INT	MESS(1401, "in p..q, p is a number but not an integer")
#define NRANGE_U_NUM	MESS(1402, "in p..q, p is a number, but q is not")
#define NRANGE_U_INT	MESS(1403, "in p..q, q is a number but not an integer")

#define CRANGE_L_CHAR	MESS(1404, "in p..q, p is a text but not a character")
#define CRANGE_U_TEX	MESS(1405, "in p..q, p is a text, but q is not")
#define CRANGE_U_CHAR	MESS(1406, "in p..q, q is a text, but not a character")

Hidden bool bounds_ok(value lo, value hi) {
	bool r= No;
	if (Is_text(lo)) {
		if (!character(lo)) interr(CRANGE_L_CHAR);
		else if (!Is_text(hi)) interr(CRANGE_U_TEX);
		else if (!character(hi)) interr(CRANGE_U_CHAR);
		else r= Yes;
	}
	else if (Is_number(lo)) {
		if (!integral(lo)) interr(NRANGE_L_INT);
		else if (!Is_number(hi)) interr(NRANGE_U_NUM);
		else if (!integral(hi)) interr(NRANGE_U_INT);
		else r= Yes;
	}
	else {
		interr(RANGE_ILLEGAL);
	}
	return r;
}

Visible value mk_rbounds(value l, value u) {
	value r, *p;
	if (bounds_ok(l, u)) {
		r= grab(Rangebounds, 2); p= Ats(r);
		*p++= copy(l); *p++= copy(u);
	}
	else
		r= Vnil;
	return r;
}

/* NODES */

Visible typenode nodetype(parsetree v) {
	return Is_node(v) ? Nodetype(v) : Nonode;
}

/* make parsetree node */

Hidden value mk_ptn(typenode type, intlet len) {
	parsetree v= (parsetree) grab(Ptn, (len<<8) | type);
	*Branch(v, len)= *Branch(v, len+1)= NilTree;
	return v;
}

Visible unsigned ptnsyze(intlet len, int *nptrs) {
	len= _Nbranches(len);
	*nptrs= len;
	return (unsigned) ((len+2)*sizeof(value));
}

Visible parsetree node1(typenode type) {
	return mk_ptn(type, 0);
}

Visible parsetree node2(typenode type, value a1) {
	parsetree v= mk_ptn(type, 1); value *p= Ats(v);
	*p++= a1;
	return v;
}

Visible parsetree node3(typenode type, value a1, value a2) {
	parsetree v= mk_ptn(type, 2); value *p= Ats(v);
	*p++= a1; *p++= a2;
	return v;
}

Visible parsetree node4(typenode type, value a1, value a2, value a3) {
	parsetree v= mk_ptn(type, 3); value *p= Ats(v);
	*p++= a1; *p++= a2; *p++= a3;
	return v;
}

Visible parsetree node5(typenode type, value a1, value a2, value a3, value a4) {
	parsetree v= mk_ptn(type, 4); value *p= Ats(v);
	*p++= a1; *p++= a2; *p++= a3; *p++= a4;
	return v;
}

Visible parsetree node6(typenode type, value a1, value a2, value a3, value a4, value a5) {
	parsetree v= mk_ptn(type, 5); value *p= Ats(v);
	*p++= a1; *p++= a2; *p++= a3; *p++= a4; *p++= a5;
	return v;
}

Visible parsetree node8(typenode type, value a1, value a2, value a3, value a4, value a5, value a6, value a7) {
	parsetree v= mk_ptn(type, 7); value *p= Ats(v);
	*p++= a1; *p++= a2; *p++= a3; *p++= a4; *p++= a5; *p++= a6; *p++= a7;
	return v;
}

Visible parsetree node9(typenode type, value a1, value a2, value a3, value a4, value a5, value a6, value a7, value a8) {
	parsetree v= mk_ptn(type, 8); value *p= Ats(v);
	*p++= a1; *p++= a2; *p++= a3; *p++= a4; *p++= a5; *p++= a6;
	*p++= a7; *p++= a8;
	return v;
}

/* OTHER TYPES */

Visible loc mk_simploc(basidf id, env en) {
	loc l= (loc) grab(Sim, 0);
	(*Ats(l))= copy(id); (*(Ats(l)+1))= (value) en;
	return l;
}

Visible loc mk_trimloc(loc R, value B, value C) {
	loc l= (loc) grab(Tri, 0); trimloc *ll= (trimloc *)Ats(l);
	ll->R= copy(R); ll->B= copy(B); ll->C= copy(C);
	return l;
}

Visible loc mk_tbseloc(loc R, value K) {
	loc l= (loc) grab(Tse, 0); tbseloc *ll= (tbseloc *)Ats(l);
	ll->R= copy(R); ll->K= copy(K);
	return l;
}

Visible fun mk_fun(literal adic, intlet pre, parsetree unit, bool filed) {
	fun f= (fun) grab(Fun, 0); funprd *ff= (funprd *)Ats(f);
	ff->adic= adic; ff->pre= pre; ff->unit= unit;
	ff->unparsed= Yes; ff->filed= filed;
	ff->code= NilTree;
	return f;
}

Visible prd mk_prd(literal adic, intlet pre, parsetree unit, bool filed) {
	prd p= (prd) grab(Prd, 0); funprd *pp= (funprd *)Ats(p);
	pp->adic= adic; pp->pre= pre; pp->unit= unit;
	pp->unparsed= Yes; pp->filed= filed;
	pp->code= NilTree;
	return p;
}

Visible value mk_how(parsetree unit, bool filed) {
	value h= grab(How, 0); how *hh= (how *)Ats(h);
	hh->unit= unit; hh->unparsed= Yes; hh->filed= filed;
	hh->code= NilTree;
	return h;
}

Visible value mk_ref(parsetree rp) {
	value r= grab(Ref, 0);
	*Ats(r)= copy(rp);
	return r;
}

Visible value mk_indirect(value v) {
	value p= grab(Ind, 0);
	*Ats(p)= copy(v);
	return p;
}
