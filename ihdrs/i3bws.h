#ifndef ABC_IHDRS_I3BWS_HEADER
#define ABC_IHDRS_I3BWS_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

extern char *bwsdir;
extern value ws_group;
extern bool groupchanges;
extern value curwskey;
extern value lastwskey;
extern value cur_ws;
#ifdef WSP_DIRNAME
value abc_wsname(char *wsp);
#endif
extern char *cur_dir; /* absolute path to current workspace */

#endif  /* HEADER GUARD */
