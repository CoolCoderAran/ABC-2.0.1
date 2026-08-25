/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

#include "b.h"
#include "b0lan.h"
#include "bmem.h"
#include "bobj.h"
#include "i3cen.h"
#include "i3sou.h"
#include "i4bio.h"

Forward Hidden char *getlocname(value pname);
Forward Hidden char *getlocval(value fname);

/* list how-to's and locations on the file 'ofp' */

Visible Procedure abcpack(FILE *ofp) {
	intlet k, len;
	char *locname;
	char *locval;

	abclist(ofp); /* list how-to's */

	len= length(cur_env->perm);
	for (k= 0; k<len && !Interrupted(); ++k) {
		if ((locname= getlocname(*key(cur_env->perm, k))) == (char *)0) {
			continue;
		}
		if ((locval= getlocval(*assoc(cur_env->perm, k))) == (char *)0) {
			freestr(locname);
			continue;
		}
		fprintf(ofp, "%s %s %s %s\n", K_PUT, locval, K_IN_put, locname);
		fflush(ofp);
		freestr(locval);
		freestr(locname);
	}
}

Hidden char *getlocname(value pname) {
	value name;
	char *locname;

	if (!Is_text(pname) || Permtype(pname) != Tar) return (char *)NULL;
	name= Permname(pname);
	locname= sstrval(name);
	release(name);
	return locname;
}

Hidden char *getlocval(value fname) {
	FILE *fp;
	char *buffer, *pbuf;
	int c;

	if (!Is_text(fname) || !targetfile(fname))
		return (char *)NULL;
	fp= fopen(strval(fname), "r");
	if (fp == (FILE *)NULL)
		return (char *)NULL;
	buffer= pbuf= (char *) getmem((unsigned)(f_size(fp)+2)*sizeof(char));
	while ((c= getc(fp)) != EOF) {
		if (c != '\n') *pbuf++= c;
	}
	*pbuf= '\0';
	fclose(fp);
	if (pbuf == buffer) { /* no value */
		freestr(buffer);
		return (char *)NULL;
	}
	return buffer;
}
