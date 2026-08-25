/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* Generic version, as basis for porting.
 * See Portability Guide (./PORTING.DOC) for details.
 */

#include "b.h"

#ifndef HAS_READDIR

#include "dir.h"

Visible DIR *opendir(char *name) {
}

Visible struct direct *readdir(DIR *dirp) {
}

Visible Procedure closedir(DIR *dirp) {
}

#endif /* !HAS_READDIR */
