/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/*
 * B editor -- Editor command processor.
 */

#include "b.h"
#include "bedi.h"
#include "bcom.h"
#include "node.h"
#include "supr.h"       /* for environment */
#include "tabl.h"
#include "port.h"
#ifdef GFX
#include "bgfx.h"
#endif

Forward value editqueue(string buffile);

Visible int doctype;
extern bool canceled;

Visible environment *tobesaved;
Visible string savewhere;

environment top_env, *top_ep;

Visible Procedure initbed() {
	top_ep= &top_env;

	savewhere = (string)NULL;
	tobesaved = (environment*)NULL;
	clrenv(top_ep);
#ifdef SAVEBUF
	top_ep->copybuffer = editqueue((string) buffile);
	if (top_ep->copybuffer)
		top_ep->copyflag = Yes;
#endif /* SAVEBUF */
}

Visible Procedure endbed() {
	register environment *ep = tobesaved;

	tobesaved = (environment*)NULL;
		/* To avoid loops if saving is cancelled. */
	if (savewhere && ep) {
		if (ep->generation > 0) {
			VOID save(ep->focus, savewhere);
#ifdef USERSUGG
			writesugg(ep->focus);
#endif /* USERSUGG */
		}
#ifdef SAVEBUF
		if (ep->copyflag)
			VOID savequeue(ep->copybuffer, (string) buffile);
		else
			VOID savequeue(Vnil, (string) buffile);
#endif /* SAVEBUF */
#ifdef SAVEPOS
		savpos(savewhere, ep);
#endif /* SAVEPOS */
	}
#ifdef SAVEBUF
	if (top_ep->copyflag)
		VOID savequeue(top_ep->copybuffer, (string) buffile);
	else
		VOID savequeue(Vnil, (string) buffile);
#endif /* SAVEBUF */
	Erelease(*top_ep);
}

Visible Procedure abced_file(string filename, intlet errline, literal kind, bool creating, bool *changed) {
	environment *ep= top_ep;

#ifdef GFX
	if (gfx_mode != TEXT_MODE)
		exit_gfx();
#endif
	setindent(0);
	doctype= D_perm;
	VOID dofile(ep, filename, errline, kind, creating, changed);
	endshow();
	top(&ep->focus);
	ep->mode = WHOLE;
	VOID deltext(ep);
	if (!ep->copyflag) {
		release(ep->copybuffer);
		ep->copybuffer = Vnil;
	}
	if (canceled) {
		int_signal();
		canceled= No;
	}
}

Visible char *ed_line(literal kind, int indent) {
	char *buf= (char *) NULL;
	environment *ep= top_ep;

	if (kind == R_cmd)
		setroot(Imm_cmd);
	else if (kind == R_expr)
		setroot(Expression);
	else
		setroot(Raw_input);
	delfocus(&ep->focus);
	if (kind == R_cmd) {
		cmdprompt(CMDPROMPT);
		doctype= D_immcmd;
	}
	else if (kind == R_expr || kind == R_raw || kind == R_ioraw)
		setindent(indent);
	else
		setindent(0);
	if (kind != R_cmd) {
		doctype= D_input;
	}
	VOID editdocument(ep, No);
	endshow();
	top(&ep->focus);
	ep->mode = WHOLE;
	if (!canceled)
		buf= senddoc(ep->focus);
	VOID deltext(ep);

	if (canceled) {
		int_signal();
		canceled= No;
	}

	return buf;
}


