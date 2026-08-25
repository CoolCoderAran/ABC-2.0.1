#ifndef ABC_BHDRS_VISIBILITY_HEADER
#define ABC_BHDRS_VISIBILITY_HEADER

typedef char *string; /* Strings are always terminated with a char '\0'. */

#define Yes ((bool) true)
#define No  ((bool) false)

typedef short intlet;

#define Forward
#define Visible
#define Porting
#define Hidden static
#ifdef NO_VOID
#define Procedure int
#else
#define Procedure void
#endif

#endif  /* HEADER GUARD */
