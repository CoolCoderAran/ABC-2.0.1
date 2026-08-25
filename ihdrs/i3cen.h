#ifndef ABC_IHDRS_I3CEN_HEADER
#define ABC_IHDRS_I3CEN_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

typedef struct {
	value units;       /* mapping to the how-to's */
	value perm;        /* mapping to the file names */
	value abctypes;    /* mapping to the typecode of how-to's */
	bool permchanges;  /* change in filename mapping ? */
	bool typeschanges; /* change in typecode mapping ? */
	intlet errlino;    /* linenumber in last erroneous how-to */
} wsenv, *wsenvptr;

#define Wnil ((wsenvptr *) 0)

Visible Procedure initcurenv(void);
Visible Procedure initworkspace(void);
Visible Procedure endworkspace(void);
Visible Procedure initcentralworkspace(bool startup);
Visible Procedure endcentralworkspace(bool last);
Visible bool      is_unit(value name, literal type, value **howto, wsenvptr *wse);
Visible Procedure initstdenv(void);
Visible Procedure endstdenv(void);
Visible wsenvptr  setcurenv(wsenvptr wse);
Visible Procedure resetcurenv(wsenvptr wse);

extern wsenvptr cur_env;   /* pointer to current environment  */
extern wsenvptr abc_use_env;   /* pointer to using environment */
extern wsenvptr cen_env;   /* pointer to central environment */
extern wsenvptr std_env;   /* pointer to standard environment */
extern bool incentralws;   /* is the current workspace the central one? */
extern char *cen_dir;      /* absolute path to central workspace */

#define IsUsingEnv(wse)    ((wse) == abc_use_env)
#define IsCentralEnv(wse)  ((wse) == cen_env)
#define IsStandardEnv(wse) ((wse) == std_env)

#define InUsingEnv()       (IsUsingEnv(cur_env))
#define InCentralEnv()     (IsCentralEnv(cur_env))
#define InStandardEnv()    (IsStandardEnv(cur_env))


#endif  /* HEADER GUARD */
