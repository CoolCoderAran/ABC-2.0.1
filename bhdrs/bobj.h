#ifndef ABC_BHDRS_BOBJ_HEADER
#define ABC_BHDRS_BOBJ_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* B values, locations, environments: the B abstract machine */

#include <stddef.h>

#include "b.h"

/* Avoid name conflicts with standard header files: */
#define power b_power
#define exp1 b_exp1
#define log1 b_log1
#define log2 b_log2
#define pi b_pi
#define random b_random
#define remove b_remove
#define mod b_mod
#include <stdbool.h>

/****************************** general ******************************/

typedef int relation; /* < 0, == 0, > 0 */
relation compare(value v, value w);

/*************************************************************************/

value grab(literal type, short len);
unsigned tltsyze(literal type, intlet len, int *nptrs);
unsigned numsyze(short len, int *nptrs);
unsigned ptnsyze(short len, int *nptrs);

double hash(value v);

Visible bool is_abcname(value name);

/****************************** Texts ******************************/

bool character(value v);

Visible value mkchar(char c);
Visible value stripped(value t);
Visible value upper(value t);
Visible value lower(value t);
Visible value split(value t);

value mk_text(string s);
char charval(value v);
char ncharval(int n, value v);
string strval(value v);
string sstrval(value v);

value concat(value tleft, value tright);
value behead(value t, value before);
value curtail(value t, value after);
value repeat(value t, value n);

value adjleft(value v, value w);
value centre(value v, value w);
value adjright(value v, value w);

value convert(value v, bool coll, bool outer);

/****************************** Numbers ******************************/

/* Predicates */
bool integral(value v); /* is the value an integer? */
bool large(value v);    /* can a number be represented by a C int? */
#ifdef RANGEPRINT
bool is_increment(value a, value b); /* a = b+1 ? */
#endif

/* Constants */
#define zero MkSmallInt(0)
#define one MkSmallInt(1)

/* Conversion of abstract values to concrete objects */
double numval(value v);             /* numeric value of any number */
int intval(value v);                /* numeric value of integral number */
int propintlet(int i);              /* checks int for fitting in intlet */
string convnum(register value v);   /* character string approximation of any number */
relation numcomp(value v, value u); /* comparison of two numbers: yields -1, 0 or 1 */
double numhash(value v);            /* hashes any abstract number to a 'double' */

/* Conversion of concrete objects to abstract numbers */
value numconst(register value v); /* string argument */
value mk_integer(int x);          /* int argument */

/* Functions on numbers */
value sum(value u, value v);
value diff(value u, value v);
value negated(value u);
value prod(value u, value v);
value quot(value u, value v);
value floor_f(value u);
value ceil_f(value u);
value round1(value u);
value round2(value n, value v);
value mod(value u, value v);
value power(value u, value v);
value absval(value u);
value signum(value u);
value numerator(value v);
value denominator(value u);
value approximate(value u);
value random(void);
value root1(value u);
value sin1(value u);
value cos1(value u);
value tan1(value u);
value arctan1(value u);
value angle1(value u, value v);
value sin2(value u, value v);
value cos2(value u, value v);
value tan2(value u, value v);
value arctan2(value u, value v);
value angle2(value c, value u, value v);
value radius(value u, value v);
value exp1(value u);
value log1(value u);
value root2(value u, value v);
value log2(value u, value v);
value pi(void);
value e_natural(void);
value nowisthetime(void);
value exactly(value v);
bool exact(value v);

/****************************** Compounds ******************************/
#define Nfields(c) Length(c)
#define Field(c, i) ((Ats(c) + (i)))
#define k_Overfields for (k = 0; k < len; k++)
#define Lastfield(k) ((k) == len - 1)

#define mk_compound(len) grab(Com, len)

/****************************** Lists ******************************/
value mk_range(value v1, value v2);
bool is_rangelist(value v);

Visible Procedure insert(value v, value *pl);
Visible Procedure remove(value v, value *pl);

/****************************** Tables ******************************/

value keys(value t);
bool in_keys(value k, value t);
value associate(value t, value k);

Visible Procedure replace(value a, value *pt, value k);
Visible bool delete (value *pt, value k);

value *adrassoc(value t, value k);
value *key(value v, intlet k);
value *assoc(value v, intlet k);

Visible bool uniq_assoc(value t, value k);

/****************************** Texts, Lists, and Tables *******************/
value mk_elt(void);

bool in(value e, value t);

value size(value t);
value size2(value e, value t);
value min1(value t);
value min2(value e, value t_is_ext);
value max1(value t);
value max2(value e, value t);
#ifdef B_COMPAT
value th_of(value num, value v);
#endif
value thof(int k, value v);
value item(value v, value num);
Visible value choice(value train);

int length(value v); /* The same as size, temporary until part2 is written in B */
bool empty(value v); /* whether #v=0: also temporary */

#endif /* HEADER GUARD */
