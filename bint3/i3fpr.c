/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* B formula/predicate invocation */
#include "b.h"
#include "bint.h"
#include "bobj.h"
#include "i0err.h"
#include "b0lan.h"
#include "i1num.h"
#include "i2par.h"
#include "i3cen.h"
#include "i3sou.h"
#include "port.h"

Forward Hidden Procedure defprd(string repr, literal adic, intlet pre);

#define Other 0
#define Nume 1		/* e.g. number1 + number2 */
#define Adjust 5	/* e.g. v >< number2 */
#define Numpair 2	/* e.g. angle(x,y) has numeric pair */
#define Nonzero 3	/* e.g. 0 sin x undefined */
#define Textual 4	/* e.g. stripped t */

#define Xact 0
#define In 1
#define Not_in 2

/*
 * Table defining all predefined functions (but not propositions).
 */

typedef value (*Fun0)(void);
typedef value (*Fun1)(value v);
typedef value (*Fun2)(value v, value u);
typedef value (*Fun3)(value v, value u, value w);

typedef union {
	Fun0 fun0;
	Fun1 fun1;
	Fun2 fun2;
	Fun3 fun3;
} genericfun;

#define CALL0(f) ((f).fun0())
#define CALL1(f, x) ((f).fun1(x))
#define CALL2(f, x, y) ((f).fun2(x,y))
#define CALL3(f, x, y, z) ((f).fun3(x, y, z))

struct funtab {
	string f_name;
	literal f_adic;
	literal f_kind;
	genericfun f_fun;
	bool f_extended;
} funtab[] = {
	{S_ABOUT,	      Mfd, Nume, {.fun1 = approximate}, false},
	{S_PLUS,	      Mfd, Nume, {.fun1 = copy}, false},
	{S_PLUS,	      Dfd, Nume, {.fun2 = sum}, false},
	{S_MINUS,	      Mfd, Nume, {.fun1 = negated}, false},
	{S_MINUS,	      Dfd, Nume, {.fun2 = diff}, false},
	{S_NUMERATOR,	  Mfd, Nume, {.fun1 = numerator}, false},
	{S_DENOMINATOR,	Mfd, Nume, {.fun1 = denominator}, false},

	{S_TIMES,	Dfd, Nume, {.fun2 = prod}, false},
	{S_OVER,	Dfd, Nume, {.fun2 = quot}, false},
	{S_POWER,	Dfd, Nume, {.fun2 = power}, false},

	{S_BEHEAD,	     Dfd, Other,  {.fun2 = behead}, false},
	{S_CURTAIL,	     Dfd, Other,  {.fun2 = curtail}, false},
	{S_JOIN,	       Dfd, Other,  {.fun2 = concat}, false},
	{S_REPEAT,	     Dfd, Other,  {.fun2 = repeat}, false},
	{S_LEFT_ADJUST,	 Dfd, Adjust, {.fun2 = adjleft}, false},
	{S_CENTER,	     Dfd, Adjust, {.fun2 = centre}, false},
	{S_RIGHT_ADJUST, Dfd, Adjust, {.fun2 = adjright}, false},

	{S_NUMBER, Mfd, Other, {.fun1 = size}, false},
	{S_NUMBER, Dfd, Other, {.fun2 = size2}, false},

	{F_pi,      Zfd, Other, {.fun0 = pi}, false},
	{F_e,       Zfd, Other, {.fun0 = e_natural}, false},
	{F_now,	    Zfd, Other, {.fun0 = nowisthetime}, false},
	{F_abs,    	Mfd, Nume,  {.fun1 = absval}, false},
	{F_sign,   	Mfd, Nume,  {.fun1 = signum}, false},
	{F_floor,  	Mfd, Nume,  {.fun1 = floor_f}, false},
	{F_ceiling,	Mfd, Nume,  {.fun1 = ceil_f}, false},
	{F_round,  	Mfd, Nume,  {.fun1 = round1}, false},
	{F_round,  	Dfd, Nume,  {.fun2 = round2}, false},
	{F_mod,    	Dfd, Nume,  {.fun2 = mod}, false},
	{F_root,   	Mfd, Nume,  {.fun1 = root1}, false},
	{F_root,   	Dfd, Nume,  {.fun2 = root2}, false},
	{F_random, 	Zfd, Nume,  {.fun0 = random}, false},
	{F_exactly,	Mfd, Nume,  {.fun1 = exactly}, false},

	{F_sin,    Mfd, Nume, {.fun1 = sin1}, false},
	{F_cos,    Mfd, Nume, {.fun1 = cos1}, false},
	{F_tan,    Mfd, Nume, {.fun1 = tan1}, false},
	{F_arctan, Mfd, Nume, {.fun1 = arctan1}, false},
	{F_angle,  Mfd, Numpair, {.fun2 = angle1}, false},
	{F_radius, Mfd, Numpair, {.fun2 = radius}, false},

	{F_sin,    Dfd, Nonzero, {.fun2 = sin2}, false},
	{F_cos,    Dfd, Nonzero, {.fun2 = cos2}, false},
	{F_tan,    Dfd, Nonzero, {.fun2 = tan2}, false},
	{F_arctan, Dfd, Nume,    {.fun2 = arctan2}, false},
	{F_angle,  Dfd, Numpair, {.fun3 = angle2}, false},

	{F_exp, Mfd, Nume, {.fun1 = exp1}, false},
	{F_log, Mfd, Nume, {.fun1 = log1}, false},
	{F_log, Dfd, Nume, {.fun2 = log2}, false},

	{F_stripped, Mfd, Textual, {.fun1 = stripped}, false},
	{F_split,    Mfd, Textual, {.fun1 = split}, false},
	{F_upper,    Mfd, Textual, {.fun1 = upper}, false},
	{F_lower,    Mfd, Textual, {.fun1 = lower}, false},

	{F_keys, Mfd, Other, {.fun1 = keys}, false},
#ifdef B_COMPAT
	{F_thof,  Dfd, Other, {.fun2 = th_of}, false},
#endif
	{F_item,   Dfd, Other, {.fun2 = item}, false},
	{F_min,    Mfd, Other, {.fun1 = min1}, false},
	{F_min,    Dfd, Other, {.fun2 = min2}, false},
	{F_max,    Mfd, Other, {.fun1 = max1}, false},
	{F_max,    Dfd, Other, {.fun2 = max2}, false},
	{F_choice, Mfd, Other, {.fun1 = choice}, false},
	{"",       Dfd, Other, {nullptr}, false} /*sentinel*/
};

Visible Procedure initfpr()
{
	struct funtab *fp; value r, f, pname;

	initstdenv();
	for (fp= funtab; *(fp->f_name) != '\0'; ++fp) {
		/* Define function */
		r= mk_text(fp->f_name);
		f= mk_fun(fp->f_adic, (intlet) (fp-funtab), NilTree, Yes);
		pname= permkey(r, fp->f_adic);
		def_std_howto(pname, f);
		release(f); release(r); release(pname);
	}

	defprd(P_exact, Mpd, Xact);
	defprd(P_in, Dpd, In);
	defprd(P_notin, Dpd, Not_in);
}

Hidden Procedure defprd(string repr, literal adic, intlet pre) {
	value r= mk_text(repr), p= mk_prd(adic, pre, NilTree, Yes), pname;
	pname= permkey(r, adic);
	def_std_howto(pname, p);
	release(p); release(r); release(pname);
}

Visible Procedure endfpr()
{
	endstdenv();
}

Hidden bool is_funprd(value t, value *f, literal adicity, bool func) {
	value *aa;
	bool rethow = (f != Pnil); /* get internal repr. howto */

	if (rethow)
		*f = Vnil;
	if (!Valid(t) || !Is_text(t))
		return No;
	if (!is_unit(t, adicity, rethow ? &aa : PPnil, Wnil))
		return No;
	if (still_ok) {
		if (!rethow)
			return Yes;
		if (func) {
			if (!Is_function(*aa))
				return No;
		}
		else {
			if (!Is_predicate(*aa))
				return No;
		}
		*f = *aa;
		return Yes;
	}
	else return No;
}

Visible bool is_zerfun(value t, value *f) {
	return is_funprd(t, f, Zfd, Yes);
}

Visible bool is_monfun(value t, value *f) {
	return is_funprd(t, f, Mfd, Yes);
}

Visible bool is_dyafun(value t, value *f) {
	return is_funprd(t, f, Dfd, Yes);
}

Visible bool is_zerprd(value t, value *p) {
	return is_funprd(t, p, Zpd, No);
}

Visible bool is_monprd(value t, value *p) {
	return is_funprd(t, p, Mpd, No);
}

Visible bool is_dyaprd(value t, value *p) {
	return is_funprd(t, p, Dpd, No);
}

#define Is_numpair(v) (Is_compound(v) && Nfields(v) == 2 && \
			Is_number(*Field(v, 0)) && Is_number(*Field(v, 1)))

Visible value pre_fun(value nd1, intlet pre, value nd2) {
	struct funtab *fp= &funtab[pre];
	literal adic= fp->f_adic, kind= fp->f_kind;
	value name= mk_text(fp->f_name);
	switch (adic) {
	case Dfd:
		if ((kind==Nume||kind==Numpair||kind==Nonzero) && !Is_number(nd1)) {
	interrV(MESS(3200, "in x %s y, x is not a number"), name);
			release(name);
			return Vnil;
		}
		else if ((kind==Nume||kind==Nonzero||kind==Adjust)
			 && !Is_number(nd2)) {
	interrV(MESS(3201, "in x %s y, y is not a number"), name);
			release(name);
			return Vnil;
		}
		else if (kind==Numpair && !Is_numpair(nd2)) {
	interrV(MESS(3202, "in x %s y, y is not a compound of two numbers"), name);
			release(name);
			return Vnil;
		} else if (kind==Nonzero && numcomp(nd1, zero)==0) {
	interrV(MESS(3203,"in c %s x, c is zero"), name);
			release(name);
			return Vnil;
		}
		break;
	case Mfd:
		switch (kind) {
		case Nume:
			if (!Is_number(nd2)) {
	interrV(MESS(3204, "in %s x, x is not a number"), name);
				release(name);
				return Vnil;
			}
			break;
		case Numpair:
			if (!Is_numpair(nd2)) {
	interrV(MESS(3205, "in %s y, y is not a compound of two numbers"), name);
				release(name);
				return Vnil;
			}
			break;
		case Textual:
			if (!Is_text(nd2)) {
	interrV(MESS(3206, "in %s t, t is not a text"), name);
				release(name);
				return Vnil;
			}
			break;
		}
		break;
	}
	release(name);

	switch (adic) {
	case Zfd:
		return CALL0(fp->f_fun);
	case Mfd:
		if (fp->f_kind == Numpair)
			return CALL2(fp->f_fun, *Field(nd2,0), *Field(nd2,1));
		else
			return CALL1(fp->f_fun, nd2);
	case Dfd:
		if (fp->f_kind == Numpair)
			return CALL3(fp->f_fun, nd1, *Field(nd2,0), *Field(nd2,1));
		else
			return CALL2(fp->f_fun, nd1, nd2);
	default:
		syserr(MESS(3207, "pre-defined fpr wrong"));
		/*NOTREACHED*/
	}
}

Visible bool pre_prop(value nd1, intlet pre, value nd2) {
	switch (pre) {
	case Xact:
		if (!Is_number(nd2)) {
		interr(MESS(3208, "in the test exact x, x is not a number"));
			return No;
		}
		return exact(nd2);
	case In:
		if (!Is_tlt(nd2)) {
interr(MESS(3209, "in the test e in t, t is not a text list or table"));
			return No;
		}
		if (Is_text(nd2) && (!character(nd1))) {
			interr(
MESS(3210, "in the test e in t, t is a text, but e is not a character")
			);
			return No;
		}
		return in(nd1, nd2);
	case Not_in:
		if (!Is_tlt(nd2)) {
			interr(
MESS(3211, "in the test e not.in t, t is not a text list or table"));
			return No;
		}
		if (Is_text(nd2) && (!character(nd1))) {
			interr(
MESS(3212, "in the test e not.in t, t is a text, but e isn't a character")
			);
			return No;
		}
		return !in(nd1, nd2);
	default:
		syserr(MESS(3213, "predicate not covered by proposition"));
		/*NOTREACHED*/
	}
}

Visible value nowisthetime()
{
	value now;
	int year, month, day;
	int hour, minute, sec;
	long fraction, units;

	getdatetime(&year, &month, &day, &hour, &minute,
		    &sec, &fraction, &units);

	now = mk_compound(6);
	*Field(now, 0) = mk_integer(year);
	*Field(now, 1) = mk_integer(month);
	*Field(now, 2) = mk_integer(day);
	*Field(now, 3) = mk_integer(hour);
	*Field(now, 4) = mk_integer(minute);

	if (units == 0) {
		*Field(now, 5) = mk_integer(sec);
	}
	else {
		value v_sec =        mk_integer(sec);
		integer v_fraction = mk_int((double) fraction);
		integer v_units =    mk_int((double) units);
		value v =            mk_exact(v_fraction, v_units, 0);

		*Field(now, 5) =     sum(v_sec, v);
		release(v_sec);
		release((value) v_fraction);
		release((value) v_units);
		release(v);
	}
	return now;
}
