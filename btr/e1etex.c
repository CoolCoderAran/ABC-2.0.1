/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

#include "b.h"
#include "bedi.h"
#include "bobj.h"
#include "etex.h"
#include "bmem.h"

Visible int e_length(value v) {
	return Length(v);
}

Visible value mk_etext(string m) {
	value v; intlet len= strlen(m);
	v= grab(Etex, len);
	strcpy(Str(v), m);
	return v;
}

Visible char e_ncharval(int n, value v) {
	return *(Str(v)+n-1);
}

Visible string e_strval(value v) {
	return Str(v);
}


Visible string e_sstrval(value v) {
	return (string) savestr(Str(v));
}

Visible Procedure e_fstrval(string s) {
	freestr(s);
}


Visible value e_icurtail(value v, int k) {
	value w= grab(Etex, k);
	strncpy(Str(w), Str(v), k);
	*(Str(w) + k)= '\0';
	return w;
}


Visible value e_ibehead(value v, int k) {
	value w= grab(Etex, Length(v) - (k - 1));
	strcpy(Str(w), Str(v) + k - 1);
	return w;
}



Visible value e_concat(value s, value t) {
	value v= grab(Etex, Length(s) + Length(t));
	strcpy(Str(v), Str(s));
	strcpy(Str(v) + Length(s), Str(t));
	return v;
}

Visible Procedure e_concto(value *s, value t) {
	value v= *s;
	*s= e_concat(*s, t);
	release(v);
}
