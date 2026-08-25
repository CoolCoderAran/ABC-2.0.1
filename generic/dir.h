#ifndef ABC_GENERIC_DIR_HEADER
#define ABC_GENERIC_DIR_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* Generic version, as basis for porting.
 * See Portability Guide (./PORTING.DOC) for details.
 */

#include "b.h"

#ifdef HAS_READDIR

#include <sys/dir.h>

#else /* !HAS_READDIR */

struct direct {
	char d_name[ /* some value */ ];
	/* other fields */
};

typedef /* whatever */ DIR;

Visible DIR *opendir(char *path);
Visible struct direct *readdir(char *path);
Visible Procedure closedir(DIR *dirp);

#endif /* !HAS_READDIR */

#endif  /* HEADER GUARD */
