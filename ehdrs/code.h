#ifndef ABC_EHDRS_CODE_HEADER
#define ABC_EHDRS_CODE_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

extern char code_array[];
extern char invcode_array[];
extern int lastcode;

extern Procedure initcodes(void);

#define RANGE 128 /* ASCII characters are in {0 .. RANGE-1} */

#define Code(c) code_array[(unsigned char) c]
#define Invcode(code) invcode_array[code]

#endif  /* HEADER GUARD */
