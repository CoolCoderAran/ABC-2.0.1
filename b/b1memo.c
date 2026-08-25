/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* general memory handling */

#include "b.h"
#include "bmem.h"

Visible ptr getmem(size_t syze) {
	ptr p= malloc(syze);
	if (p == Nil) memexh();
#ifdef MEMTRACE
	writetrace(F_ALLOC, p, syze);
#endif
	return p;
}

Visible Procedure regetmem(ptr *v, size_t syze) {
	ptr p= realloc(*v, syze);
	if (p == Nil) memexh();
#ifdef MEMTRACE
	writetrace(F_FREE, *v, 0);
	writetrace(F_ALLOC, p, syze);
#endif
	*v= p;
}

Visible Procedure freemem(ptr p) {
#ifdef MEMTRACE
	writetrace(F_FREE, p, 0);
#endif
	free((char *)p);
}

Visible ptr savestr(char *s) {
	ptr p= (ptr) getmem((unsigned) strlen(s) + 1);
	strcpy(p, s);
	return p;
}

#ifdef MEMTRACE

/*
 * to fix memory that surely won't be free'd
 */
Visible Procedure fixmem(ptr p) {
	writetrace(F_FREE, p, 0);
}

extern FILE *memfp;	/* set in ?/platform.c */

writetrace(flag, p, size) int flag; ptr *p; unsigned size; {
	address *frameptr;

	if (memfp == NULL)
		return;
	fwrite(&flag, sizeof(int), 1, memfp);
	fwrite(&p, sizeof(ptr), 1, memfp);
	fwrite(&size, sizeof(unsigned), 1, memfp);

	frameptr= (unsigned*) &flag - 1;
	frameptr= (unsigned*) *frameptr;	/* skip getmem or freemem */
	do {
		/* dump PC */
		fwrite((char*)(frameptr-2), sizeof(address), 1, memfp);
		/* follow FP */
		frameptr= (unsigned*) *frameptr;
	} while (*frameptr);
	fwrite((char*)frameptr, sizeof(address), 1, memfp);
}

#endif /*MEMTRACE*/

/************************************************************************/

#define BUFINCR 100

Visible Procedure bufinit(bufadm *bp) {
	bp->buf= (char *) getmem((unsigned) BUFINCR);
	bp->pbuf= bp->buf;
	bp->end= bp->buf + BUFINCR;
	*(bp->pbuf)= '\0';
}

Visible Procedure buffree(bufadm *bp) {
	freemem((ptr) bp->buf);
}

Visible Procedure bufreinit(bufadm *bp) {
	buffree(bp);
	bufinit(bp);
}

Visible Procedure bufgrow(bufadm *bp) {
	int n_ptr= bp->pbuf - bp->buf;
	int syze= (bp->end - bp->buf) + BUFINCR;

	regetmem((ptr *) &(bp->buf), (unsigned) syze);
	bp->pbuf= bp->buf + n_ptr;
	bp->end= bp->buf + syze;
}

Visible Procedure bufpush(bufadm *bp, char c) {
	if (bp->pbuf >= bp->end)
		bufgrow(bp);
	*(bp->pbuf)++= c;
}

Visible Procedure bufcpy(bufadm *bp, char *s) {
	int len= strlen(s);

	while (bp->pbuf + len >= bp->end)
		bufgrow(bp);
	strcpy(bp->pbuf, s);
	bp->pbuf+= len;
}

Visible Procedure bufncpy(bufadm *bp, char *s, size_t len) {
	while (bp->pbuf + len >= bp->end)
		bufgrow(bp);
	strncpy(bp->pbuf, s, len);
	bp->pbuf+= len;
	*(bp->pbuf)= '\0';
}
