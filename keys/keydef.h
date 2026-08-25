#ifndef ABC_KEYS_KEYDEF_HEADER
#define ABC_KEYS_KEYDEF_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

#include "b.h"

#define ABC_RELEASE "ABC Release %s."
#define HEADING "This program allows you to redefine the key bindings for the ABC editor operations, producing a key definitions file."

/****************************************************************************/

#define E_ILLEGAL	"*** You are not allowed to start a definition with '%c' since that would make that character unavailable."
#define E_TOO_MANY 	"*** Sorry, can't remember more key definitions"
#define E_UNLAWFUL	"*** It may not contain an unprintable character"
#define E_IN_USE	"*** That representation is in use for %s."
#define E_UNKNOWN	"*** unknown operation name"
#define E_KEYFILE	"*** Can't open key definitions file %s for writing; writing to standard output instead."

#ifndef CANLOOKAHEAD
#define E_INTERRUPT	"*** You cannot include your interrupt character."
#define E_NOTALLOWED	"*** you are not allowed to change \"%s\""
#endif

Visible Procedure setup_bindings(int width, int *nlines);
Visible Procedure bind_all_changed(void);
Visible Procedure putbindings(int yfirst);
Visible Procedure bind_changed(int code);
Visible Procedure confirm_operation(int code, string name);
Visible Procedure saveharddefs(void);
Visible Procedure savefiledefs(void);


#endif  /* HEADER GUARD */
