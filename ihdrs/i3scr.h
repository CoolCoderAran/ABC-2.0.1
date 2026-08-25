#ifndef ABC_IHDRS_I3SCR_HEADER
#define ABC_IHDRS_I3SCR_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* screen */

extern value iname;
extern bool outeractive;
extern bool at_nwl;
extern bool Eof;
extern FILE *ifile;
extern FILE *sv_ifile;
extern char *get_line(void);
extern char q_answer(int m, char c1, char c2, char c3);
extern char *getfmtbuf(string fmt, int n);


#endif  /* HEADER GUARD */
