/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

#include "b.h"
#include "bmem.h"
#include "i1btr.h"
#include "i1tlt.h"

/*********************************************************************/
/* grab, copy, release of btree(node)s                               */
/*********************************************************************/


/*
*
* This method takes as arguments
* pnode - a generic pointer to a btree node
* l - the index of the position of returned item
* it - item type (Ct, Lt, Tt, Kt)
* flag - char flag: Bottom or Inner node indicator
* This method returns a pointer to the item at the specified position in a tree node of the it
*
* This method ensures that there is no general cast from a more specific struct like btexptr to a bottomptr,
* as with how memory is alligned to 8 bytes on 64-bit architectures, that cast could treat the array as 4 byte
* padding and only then the actual itemarray. Obtaining the address of the itemarray from that cast gives a 4 byte
* shift compared to where it actually starts within memory, so the retrieved item was incorrect.
*
* This method is called in Piitm and Pbitm macros to get item at lth position
*
* With this switch casting we ensure that the cast is always to a correct type.
*/
Visible itemptr castandgetxitm(btreeptr pnode, int l,literal it, int flag) {
	intlet iw = Itemwidth(it);

#define GET_FIELD(node, type, field) \
	((itemptr) (((char*) &(((type) (node))->field)) + ((l)*(iw))));

	switch (flag)
	{
		case Inner:
			switch (it)
			{
				case Ct:
					return GET_FIELD(pnode, itexptr, icitm);
				case Lt:
					return GET_FIELD(pnode, ilisptr, ilitm);
				case Tt:
					return GET_FIELD(pnode, ilisptr, ilitm);
				case Kt:
					return GET_FIELD(pnode, itabptr, ititm);
				default:
					/* This should never be reached */
					return NULL;
			}

		case CBottom:
			switch (it)
			{
				case Ct:
					return GET_FIELD(pnode, btexptr, bcitm);
				case Lt:
					return GET_FIELD(pnode, blisptr, blitm);
				case Tt:
					return GET_FIELD(pnode, btabptr, btitm);
				case Kt:
					return GET_FIELD(pnode, btabptr, btitm);
				default:
					/* This should never be reached */
					return NULL;
			}

		default:
			/* This should never be reached */
			return NULL;
	}

#undef GET_FIELD

}

Visible btreeptr
grabbtreenode(literal flag, literal it) {
	btreeptr pnode; unsigned syz;
	static intlet isize[]= {
		sizeof(itexnode), sizeof(ilisnode),
		sizeof(itabnode), sizeof(itabnode)};
	static intlet bsize[]= {
		sizeof(btexnode), sizeof(blisnode),
		sizeof(btabnode), sizeof(btabnode)};
	switch (flag) {
	case Inner:
		syz= isize[(unsigned char) it];
		break;
	case CBottom:
		syz= bsize[(unsigned char) it];
		break;
	case Irange:
	case Crange:
		syz = sizeof(rangenode);
		break;
	}
	pnode = (btreeptr) getmem((unsigned) syz);
	Refcnt(pnode) = 1;
	Flag(pnode) = flag;
	return(pnode);
}

/* ----------------------------------------------------------------- */

Visible btreeptr copybtree(btreeptr pnode) {
	if (pnode != Bnil && Refcnt(pnode) < Maxrefcnt) ++Refcnt(pnode);
	return(pnode);
}

Visible Procedure uniqlbtreenode(btreeptr *pptr, literal it) {
	if (*pptr NE Bnil && Refcnt(*pptr) > 1) {
		btreeptr qnode = *pptr;
		*pptr = ccopybtreenode(*pptr, it);
		relbtree(qnode, it);
	}
}

Visible btreeptr ccopybtreenode(btreeptr pnode, literal it) {
	intlet limp;
	btreeptr qnode;

	qnode = grabbtreenode(Flag(pnode), it);
	Lim(qnode) = limp = Lim(pnode);
	Size(qnode) = Size(pnode);
	switch (Flag(qnode)) {
	case Inner:
		cpynitms(Piitm(qnode, 0, it), Piitm(pnode, 0, it), limp, it);
		cpynptrs(&Ptr(qnode, 0), &Ptr(pnode, 0), limp+1);
		break;
	 case CBottom:
		cpynitms(Pbitm(qnode, 0, it), Pbitm(pnode, 0, it), limp, it);
		break;
	case Irange:
	case Crange:
		Lwbval(qnode) = copy(Lwbval(pnode));
		Upbval(qnode) = copy(Upbval(pnode));
		break;
	default:
		syserr(MESS(400, "unknown flag in ccopybtreenode"));
	}
	return(qnode);
}

/* make a new root (after the old ptr0 split) */

Visible btreeptr mknewroot(btreeptr ptr0, itemptr pitm0, btreeptr ptr1, literal it) {
	int r;
	intlet iw = Itemwidth(it);
	btreeptr qnode = grabbtreenode(Inner, it);
	Ptr(qnode, 0) = ptr0;
	movnitms(Piitm(qnode, 0, it), pitm0, 1, iw);
	Ptr(qnode, 1) = ptr1;
	Lim(qnode) = 1;
	r= Sincr(Size(ptr0));
	Size(qnode) = Ssum(r, Size(ptr1));
	return(qnode);
}

/* ----------------------------------------------------------------- */

/* release btree */

Visible Procedure relbtree(btreeptr pnode, literal it) {
	if (pnode EQ Bnil)
		return;
	if (Refcnt(pnode) EQ 0) {
		syserr(MESS(401, "releasing unreferenced btreenode"));
		return;
	}
	if (Refcnt(pnode) < Maxrefcnt && --Refcnt(pnode) EQ 0) {
		intlet l;
		switch (Flag(pnode)) {
		case Inner:
			for (l = 0; l < Lim(pnode); l++) {
				relbtree(Ptr(pnode, l), it);
				switch (it) {
				case Tt:
				case Kt:
					release(Ascval(Piitm(pnode, l, it)));
					[[fallthrough]];
				case Lt:
					release(Keyval(Piitm(pnode, l, it)));
					break;
				}
			}
			relbtree(Ptr(pnode, l), it);
			break;
		case CBottom:
			for (l = 0; l < Lim(pnode); l++) {
				switch (it) {
				case Tt:
				case Kt:
					release(Ascval(Pbitm(pnode, l, it)));
					[[fallthrough]];
				case Lt:
					release(Keyval(Pbitm(pnode, l, it)));
				}
			}
			break;
		case Irange:
		case Crange:
			release(Lwbval(pnode));
			release(Upbval(pnode));
			break;
		default:
			syserr(MESS(402, "wrong flag in relbtree()"));
		}
		freemem((ptr) pnode);
	}
}

