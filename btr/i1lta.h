#ifndef ABC_BTR_I1LTA_HEADER
#define ABC_BTR_I1LTA_HEADER

#include "i1btr.h"

typedef struct {
	btreeptr s_ptr;
	int s_lim;
} finger[Maxheight], *fingertip;

Visible fingertip unzip(btreeptr p, int at, fingertip s);
Visible int movnptrs(btreeptr *to, btreeptr *from, int n);
Visible value l2max(value e, value v);
Visible value l2min(value e, value v);
Visible int l2size(value e, value v);
#endif /* HEADER GUARD */
