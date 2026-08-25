/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/* Generic version, as basis for porting.
 * See Portability Guide (./PORTING.DOC) for details.
 */

#include "b.h"
#include "main.h"  /* for curdir() */
#include "port.h"  /* for MESSFILE, HELPFILE, etc. */

Forward Hidden char *getHOMEdir();
Forward Hidden char *savepath(char *path);
Forward Hidden char *searchfile(char *base);
Forward Hidden Procedure setbuffile();
Forward Hidden Procedure setbwsdefault();
Forward Hidden Procedure sethelpfile();
Forward Hidden Procedure setkeysfile();
Forward Hidden Procedure setmessfile();
Forward Hidden Procedure setstartdir();

#define ABCLIB ""
/* here you can specify the directory where the standard ABC files
 * (messfile, etc.) can be found, if that is a fixed place.
 */

#define DELIM '/'
/* the directory delimiter in filename paths */

Visible char *startdir;   /* the directory that ABC was started up in */
Visible char *bwsdefault; /* the workspaces parent directory */
Visible char *messfile;  /* the error messages file */
Visible char *helpfile;  /* the help file */
Visible char *keysfile;  /* the keys definition file */
Visible char *buffile;   /* the file for storing the copy buffer between sessions */

Visible Procedure initfile()
{
	setstartdir();
	setbwsdefault();
	setmessfile();
	sethelpfile();
	setkeysfile();
	setbuffile();
}

Visible char *makepath(char *path1, char *path2) {
	if (path1 == NULL || *path1 == '\0')
		return savepath(path2);
	else if (path2 == NULL || *path2 == '\0')
		return savepath(path1);
	else {
		char *path, *ppath;
		int len;

		len = strlen(path1) + 1 + strlen(path2);
		path = (char *) malloc((len+1));
		strcpy(path, path1);
		ppath = path + strlen(path);
		if (ppath[-1] == DELIM && *path2 == DELIM)
			--ppath;
		else if (ppath[-1] != DELIM && *path2 != DELIM)
			*ppath++= DELIM;
		strcpy(ppath, path2);
		return path;
	}
}

Hidden char *savepath(char *path) {
	char *path1;

	if (path == NULL) return (char *) NULL;
	path1 = (char *) malloc((strlen(path) + 1));
	strcpy(path1, path);
	return path1;
}

Visible Porting Procedure freepath(char *path) {
	if (path != NULL) free(path);
}

Hidden char *searchfile(char *base) {
	char *file;

	/* search first in startup directory */

	file = makepath(startdir, base);
	if (F_readable(file)) return file;
	freepath(file);

	/* then in bwsdefault directory */

	file = makepath(bwsdefault, base);
	if (F_readable(file)) return file;
	freepath(file);

	/* next in abclib, if there is one */
#ifdef ABCLIB
	if (strlen(ABCLIB) > 0) {
		file = makepath(ABCLIB, base);
		if (F_readable(file)) return file;
		freepath(file);
	}
#endif

	/* next in specific directories, you want to look into */
	/* ...... */


	/* else */

	return (char *) NULL;
}

Hidden char *getHOMEdir()
{
	return getenv("HOME");
}

Hidden Procedure setstartdir()
{
	startdir = savepath(curdir());
}

Hidden Procedure setbwsdefault()
{
	bwsdefault = makepath(getHOMEdir(), BWSNAME);
}

Hidden Procedure setmessfile()
{
	messfile = searchfile(MESSFILE);
}

Hidden Procedure sethelpfile()
{
	helpfile = searchfile(HELPFILE);
}

Hidden Procedure setkeysfile()
{
	char *termname;

	/* search first the file abc$TERM.key */

	if ((termname = getenv("TERM")) != NULL) {
		char termfile[100];
		sprintf(termfile, FORMAT_KEYSFILE, termname);
		keysfile = searchfile(termfile);
	}

	/* next the file abc.key */

	if (keysfile == (string) NULL) {
		keysfile = searchfile(KEYSFILE);
	}
}

Hidden Procedure setbuffile()
{
	buffile = makepath(getHOMEdir(), BUFFILE);
}

Visible int Chdir(char *path) {
	return chdir(path);
}
