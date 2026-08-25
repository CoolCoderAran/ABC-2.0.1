#ifndef ABC_BHDRS_GETOPT_HEADER
#define ABC_BHDRS_GETOPT_HEADER

extern int opterr;	/* no error messages if zero */
extern int optopt;	/* option letter found */
extern int optind;	/* argv index of next argument */
extern char *optarg;	/* start of option argument */
extern int getopt(int, char * const *, const char *);

#endif  /* HEADER GUARD */
