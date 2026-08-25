/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/*
 * B editor -- Read unit from file.
 */


#include "ascii.h"
#include "b.h"
#include "bedi.h"
#include "etex.h"
#include "bmem.h"
#include "erro.h"
#include "bobj.h"
#include "node.h"
#include "tabl.h"
#include "gram.h"
#include "supr.h"
#include "queu.h"

#define TABSIZE 8
#define MAXLEVEL 128

static short *indent2;
static int level;

Forward Hidden bool editindentation(environment *ep, FILE *fp) ;

/*
 * Read (edit) parse tree from file into the focus.
 * Rather ad hoc, we use ins_string for each line
 * and do some magic tricks to get the indentation right
 * (most of the time).
 * If line > 0, position the focus at that line, if possible;
 * otherwise the focus is left at the end of the inserted text.
 */

Visible bool
readfile(environment *ep, string filename, int line, bool creating) {

	int lines = 0;
	register FILE *fp = fopen(filename, "r");
	register int c;
	string buf;
	string cp;
	queue q = Qnil;

	if (!fp) {
		ederrS(MESS(6200, "Sorry, I can't edit file \"%s\""), filename);
		return No;
	}

	buf= (string) getmem(BUFSIZ);
	if (indent2 == NULL) {
		indent2= (short*) getmem((unsigned) (MAXLEVEL * sizeof(short)));
	}

	level= 0;
	indent2[0]= 0;

	do {
		do {
			for (cp = buf; cp < buf + BUFSIZ - 1; ++cp) {
				c = getc(fp);
				if (c == EOF || c == '\n')
					break;
				if (c < ' ' || c >= 0177)
					c = ' ';
				*cp = c;
			}
			if (cp > buf) {
				*cp = 0;
				if (!ins_string(ep, buf, &q, 0) || !emptyqueue(q)) {
					qrelease(q);
					fclose(fp);
					freemem((ptr) buf);
					return No;
				}
				qrelease(q);
			}
		} while (c != EOF && c != '\n');
		++lines;
		if (c != EOF && !editindentation(ep, fp)) {
			fclose(fp);
			freemem((ptr) buf);
			return No;
		}
	} while (c != EOF);
	freemem((ptr) buf);
	fclose(fp);
	if (ep->mode == FHOLE || (ep->mode == VHOLE && (ep->s1&1))) {
		cp = "";
		VOID soften(ep, &cp, 0);
	}
	if (lines > 1 && line > 0) {
		if (line >= lines) line= lines-1;
		VOID gotoyx(ep, line-1, 0);
		oneline(ep);
	}
	if (creating)
		ins_newline(ep, Yes);
	return Yes;
}


/*
 * Do all the footwork required to get the indentation proper.
 */

Hidden bool editindentation(environment *ep, FILE *fp) {
	register int ind= 0;
	register int c;

	for (;;) {
		c= getc(fp);

		if (c == ' ')
			++ind;
		else if (c == '\t')
			ind= (ind/TABSIZE + 1) * TABSIZE;
		else
			break;
	}
	ungetc(c, fp);
	if (c == EOF || c == '\n')
		return Yes;
	if (ind > indent2[level]) {
		if (level == MAXLEVEL-1) {
			ederr(MESS(6201, "excessively nested indentation"));
			return No;
		}
		indent2[++level]= ind;
	}
	else if (ind < indent2[level]) {
		while (level > 0 && ind <= indent2[level-1])
			--level;
		if (ind != indent2[level]) {
			ederr(MESS(6202, "indentation messed up"));
			return No;
		}
	}
	if (!ins_newline(ep, Yes)) {
#ifndef NDEBUG
		debug("[Burp! Can't insert a newline.]");
#endif /* NDEBUG */
		return No;
	}
	if (level > PLevel(ep->focus)) {
		ederr(MESS(6203, "unexpected indentation increase"));
		return No;
	}
	while (level < PLevel(ep->focus)) {
		if (!ins_newline(ep, Yes)) {
#ifndef NDEBUG
			debug("[Burp, burp! Can't decrease indentation.]");
#endif /* NDEBUG */
			return No;
		}
	}
	fixit(ep);
	return Yes;
}

/* ------------------------------------------------------------ */

#ifdef SAVEBUF

/*
 * Read the next non-space character.
 */

Hidden int
skipspace(FILE *fp) {
	register int c;

	do {
		c = getc(fp);
	} while (c == ' ');
	return c;
}


/*
 * Read a text in standard B format when the initial quote has already
 * been read.
 */

Hidden value
readtext(FILE *fp, char quote) {
	value v = Vnil;
	char buf[BUFSIZ];
	register string cp = buf;
	register int c;
	int i;
	value w;

	for (; ; ++cp) {
		c = getc(fp);
		if (!chr_isascii(c) || (c != ' ' && !isprint(c))) {
#ifndef NDEBUG
			if (c == EOF)
				debug("readtext: EOF");
			else
				debug1("readtext: bad char (0%02o)", c);
#endif /* NDEBUG */
			release(v);
			return Vnil; /* Bad character or EOF */
		}
		if (c == quote) {
			c = getc(fp);
			if (c != quote) {
				ungetc(c, fp);
				break;
			}
		}
		else if (c == '`') {
			c = skipspace(fp);
			if (c == '$') {
				i = 0;
				if (fscanf(fp, "%d", &i) != 1
					|| i == 0 || !chr_isascii(i)) {
#ifndef NDEBUG
					debug("readtext: error in conversion");
#endif /* NDEBUG */
					release(v);
					return Vnil;
				}
				c = skipspace(fp);
			}
			else
				i = '`';
			if (c != '`') {
#ifndef NDEBUG
				if (c == EOF)
					debug("readtext: EOF in conversion");
				else
					debug1("readtext: bad char in conversion (0%o)", c);
#endif /* NDEBUG */
				release(v);
				return Vnil;
			}
			c = i;
		}
		if (cp >= &buf[sizeof buf - 1]) {
			*cp = 0;
			w= mk_etext(buf);
			if (v) {
				e_concto(&v, w);
				release(w);
			}
			else
				v = w;
			cp = buf;
		}
		*cp = c;
	}
	*cp = 0;
	w= mk_etext(buf);
	if (!v)
		return w;
	e_concto(&v, w);
	release(w);
	return v;
}


Hidden int
readsym(FILE *fp) {
	register int c;
	char buf[100];
	register string bufp;

	for (bufp = buf; ; ++bufp) {
		c = getc(fp);
		if (c == EOF)
			return -1;
		if (!chr_isascii(c) || (!isalnum(c) && c != '_')) {
			if (ungetc(c, fp) == EOF)
				syserr(MESS(6204, "readsym: ungetc failed"));
			break;
		}
		*bufp = c;
	}
	*bufp = 0;
	if (isdigit(buf[0]))
		return atoi(buf);
	if (strcmp(buf, "Required") == 0) /***** Compatibility hack *****/
		return Hole;
	return nametosym(buf);
}


/*
 * Read a node in internal format (recursively).
 * Return nil pointer if EOF or error.
 */

Hidden node
readnode(FILE *fp) {
	int c;
	int nch;
	node ch[MAXCHILD];
	node n;
	int sym;

	c = skipspace(fp);
	switch (c) {
	case EOF:
		return Nnil; /* EOF hit */

	case '(':
		sym = readsym(fp);
		if (sym < 0) {
#ifndef NDEBUG
			debug("readnode: missing symbol");
#endif /* NDEBUG */
			return Nnil; /* No number as first item */
		}
		if (sym < 0 || sym > Hole) {
#ifndef NDEBUG
			debug1i("readnode: bad symbol (%d)", sym);
#endif /* NDEBUG */
			return Nnil;
		}
		nch = 0;
		while ((c = skipspace(fp)) == ',' && nch < MAXCHILD) {
			n = readnode(fp);
			if (!n) {
				for (; nch > 0; --nch)
					noderelease(ch[nch-1]);
				return Nnil; /* Error encountered in child */
			}
			ch[nch] = n;
			++nch;
		}
		if (c != ')') {
#ifndef NDEBUG
			if (c == ',')
				debug1i("readnode: node too long (sym=%d)", sym);
			else
				debug1i("readnode: no ')' where expected (sym=%d)", sym);
#endif /* NDEBUG */
			for (; nch > 0; --nch)
				noderelease(ch[nch-1]);
			return Nnil; /* Not terminated with ')' or too many children */
		}
		if (nch == 0)
			return gram(sym); /* Saves space for Optional/Hole nodes */
		return newnode(nch, sym, ch);

	case '\'':
	case '"':
		return (node) readtext(fp, c);

	default:
#ifndef NDEBUG
		debug("readnode: bad initial character");
#endif /* NDEBUG */
		return Nnil; /* Bad initial character */
	}
}


/*
 * Read a node written in a more or less internal format.
 */

Visible value
editqueue(string filename) {
	register FILE *fp = fopen(filename, "r");
	queue q = Qnil;
	register node n;

	if (!fp)
		return Vnil;
	do {
		n = readnode(fp);
		if (!n)
			break; /* EOF or error */
		addtoqueue(&q, n);
		noderelease(n);
	} while (skipspace(fp) == '\n');
	fclose(fp);
	return (value)q;
}

#endif /* SAVEBUF */
