/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1988. */

#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#include "visibility.h"
#include "port.h" /* for makepath() */

/* Date and time includes are in os.h, included from b.h */

/* synonyms for current and parent directory */
#define SYN_CURDIR "."
#define SYN_PARENTDIR ".."

/**************************************************************************/

#ifndef HAS_RENAME

/* rename() - rename a file */

Visible int rename(char *from, char *to) {
	int i;

	if( (i=link(from, to)) < 0 ) return(i);
	if( (i=unlink(from)) < 0 ) {
		unlink(to);
		return(i);
	}
	return(0);
}

#endif /* !HAS_RENAME */

/**************************************************************************/

#ifndef HAS_MKDIR

/* mkdir - make a directory */

Visible int mkdir(char *dir, int mode) {
	char buf[1024];

	sprintf(buf, "(mkdir %s && chmod %o %s) >/dev/null 2>/dev/null",
			dir, mode, dir);
	return(system(buf));
}

/* rmdir - remove directory */

Visible int rmdir(char *dir) {
	char buf[1024];

	sprintf(buf, "rmdir %s >/dev/null 2>/dev/null", dir);
	return(system(buf));
}

#endif /* !HAS_MKDIR */

/**************************************************************************/

/* getseed () - initialise the random generator with a random number */

Visible int getseed()
{
	return (int) getpid();
}

/**************************************************************************/

/* getdatetime() - get the date and time */

Visible Procedure getdatetime(int *year, int *month, int *day, int *hour, int *minute, int *sec, long *fraction, long *units) {
	struct tm *lt;
	long secs1970;

#ifdef HAS_FTIME
	struct timeb tt;

	ftime(&tt);
	secs1970= tt.time;
	*fraction = tt.millitm;
	*units= 1000;
#else
#ifdef HAS_GETTIMEOFDAY
	struct timespec ts;

	if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
		secs1970= ts.tv_sec;
		*fraction= ts.tv_nsec;
		*units= 1'000'000'000;
	} else {
		secs1970= time((long*)0);
		*fraction= 0;
		*units= 1;
	}
#else
	secs1970= time((long*)0);
	*fraction= 0;
	*units= 0;
#endif /* HAS_GETTIMEOFDAY */
#endif /* HAS_FTIME */

	lt= localtime(&secs1970);

	*year = lt->tm_year + 1900;
	*month = lt->tm_mon + 1;
	*day = lt->tm_mday;
	*hour = lt->tm_hour;
	*minute = lt->tm_min;
	*sec = lt->tm_sec;

}

/**************************************************************************/

/* curdir() - return the current directory */

Visible Porting char *curdir()
{
	static char buffer[1024];
	char *res;
#ifdef HAS_GETCWD
	extern int errno;
	errno=0;
	res= getcwd(buffer, 1024);
	if (!res) {
		perror("curdir");
		return ".";
	} else return res;
#else
	char *getwd();
	res= getwd(buffer);
	if (!res) {
		perror(buffer);
		return ".";
	} else return res;
#endif
}

#ifndef HAS_GETWD
#ifndef HAS_GETCWD
/* neither getwd, nor getcwd provided; define our own: */

/* getwd - get working directory */

Hidden char *getwd(char *buf) {
	FILE *fp;

	*buf = 0;
	if ((fp=popen("pwd", "r")) == 0 ) {
		strcpy(buf, "cannot execute pwd");
		return(0);
	}
	if (fgets(buf, 1024, fp) )
		buf[strlen(buf)-1] = 0;
	pclose(fp);
	if (*buf == '/')
		return buf;
	else
		return (char*) NULL;
}

#endif /* !HAS_GETCWD */
#endif /* !HAS_GETWD */

/**************************************************************************/

#define SEPARATOR '/'
#define Issep(c) ((c) == SEPARATOR)

Visible bool is_path(char *path) {
	if (path == (char *) NULL)
		return No;
	if (strcmp(path, SYN_CURDIR) == 0 || strcmp(path, SYN_PARENTDIR) == 0)
		return Yes;
	for (; *path; path++) {
		if (Issep(*path)) return Yes;
	}
	return No;
}

/**************************************************************************/

Visible bool is_abspath(char *path) {
	if (path == NULL) return No;
	else return Issep(*path);
}

/**************************************************************************/

Visible bool is_directory(char *dir, char *name) {
	struct stat statbuf;
	char *path;

	if (dir == NULL || name == NULL) return No;
	path= makepath(dir, name);
	if (stat(path, &statbuf) == 0 &&
	    (S_ISDIR(statbuf.st_mode)) &&
	    (strcmp(name, SYN_CURDIR) != 0 && strcmp(name, SYN_PARENTDIR) != 0)
           ) {
		freepath(path);
		return Yes;
	}
	freepath(path);
	return No;
}

/**************************************************************************/
/**************************************************************************/

Visible Porting long filemodtime(char *filename) {
	struct stat statbuf;

	if (stat(filename, &statbuf) == 0)
	  return (long) statbuf.st_mtime;
	else
	  return 0L;
}

