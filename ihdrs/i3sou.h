#ifndef ABC_IHDRS_I3SOU_HEADER
#define ABC_IHDRS_I3SOU_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

#include "b.h"

Visible bool is_zerfun(value t, value *f);
Visible bool is_monfun(value t, value *f);
Visible bool is_dyafun(value t, value *f);
Visible bool is_zerprd(value t, value *p);
Visible bool is_monprd(value t, value *p);
Visible bool is_dyaprd(value t, value *p);
Visible bool is_loaded(char *fname, value pname, value **aa);
Visible bool u_exists(value pname, value **aa);
Visible bool p_exists(value pname, value **aa);
Visible Procedure def_unit(value pname, value u);
Visible Procedure def_std_howto(value pname, value h);
Visible Procedure def_perm(value pname, value fname);
Visible Procedure free_perm(value pname);
Visible unsigned f_size(FILE *file);
Visible value get_unit(value *pname, bool filed, bool editing);

extern value last_unit;
extern value last_target;
extern bool need_rec_suggestions;

#define Permname(pname) (behead(pname, MkSmallInt(2)))
#define Permtype(pname) (strval(pname)[0])
	/* possible types:
	 *	1-8 [Cmd ... Tar]	(order used in mac/m1print.c!)
	 *	: [last_unit]
	 *	= [last_target]
	 */

#define Cmd '1'
#define Zfd '2'
#define Mfd '3'
#define Dfd '4'
#define Zpd '5'
#define Mpd '6'
#define Dpd '7'
#define Tar '8'
#define OldHow '+'	/* old how-to type (used in bio) */
#define OldTar '-'	/* old target type (used in bio) */
#define Wsp '.'		/* workspace type */

#define	Cmd_ext ".cmd"
#define Zfd_ext ".zfd"
#define Mfd_ext ".mfd"
#define Dfd_ext ".dfd"
#define Zpd_ext ".zpd"
#define Mpd_ext ".mpd"
#define Dpd_ext ".dpd"
#define Cts_ext ".cts"
#define Wsp_ext ""

Visible value permkey(value name, literal type);
Visible value get_pname(parsetree v);
Visible value getval(value fname, literal ct);
Visible value gettarval(value fname, value name);

Visible value new_fname(value name, literal type);	/* devise a filename for a unit or target */
Visible value mkabcname(char *name);	/* vice versa for recovering target name */
#define CONVP_SIGN '_'		/* to map point */
#define CONVDQ_SIGN '@'		/* to map double quote */

Visible bool ckws_writable(int m);

#endif  /* HEADER GUARD */
