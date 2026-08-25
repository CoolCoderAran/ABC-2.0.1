#ifndef ABC_IHDRS_I2EXP_HEADER
#define ABC_IHDRS_I2EXP_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* General definitions for parsing expressions */

/* Avoid conflict with extra reserved word: */
#define comp b_comp

typedef struct {
	parsetree *stack;
	parsetree *sp;
	parsetree *top;
	int nextend;
	char level;		/* PARSER or FIXER */
	char /* bool */ prop;	/* Yes while fixing left expr dya pred */
	intlet nfield;		/* fieldnr unparsed node during fixing */
} expadm;

#define Stack(adm)	(adm->stack)
#define Sp(adm)		(adm->sp)
/*naming conflict note: Top(s, p, l) is defined in i1lta.c. since the
signatures are different and both should be called Top, we leave this.*/
#define Top(adm)	(adm->top)
#define Nextend(adm)	(adm->nextend)
#define Level(adm)	(adm->level)
#define Prop(adm)	(adm->prop)
#define Nfld(adm)	(adm->nfield)

#define N_EXP_STACK	15
#define N_LTA_STACK	100

/*naming conflict note: Pop(s, p, l) is defined in i1tex.c and i1lta.c.
since the signatures are different and both should be called Pop, we leave this.*/
#define Pop(adm)	*--Sp(adm)

#define PARSER 'p'
#define FIXER  'f'

#define Bottom "$"

#define Dya_opr(v) (Valid(v) && Is_compound(v))

/************************************************************************/

struct prio {
	string fun;
	char adic;
	int L, H;
};

#define P_mon '1'
#define P_dya '2'

#define dprio(i) pprio(i, P_dya)
#define mprio(i) pprio(i, P_mon)

struct prio * pprio(value f, char adic);



#endif  /* HEADER GUARD */
