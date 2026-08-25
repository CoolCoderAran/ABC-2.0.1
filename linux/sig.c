/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/*Handle interrupts and signals*/

#define _POSIX_C_SOURCE 199309L

#include "visibility.h"
#include "comp.h"

// FIXME: Non-unix header included under unix. We shouldn't do this.
#include "bvis.h"

#ifdef SIGNAL

#include <signal.h>

/*The operating system provides a function signal(s,f)
  that associates function f with the signal s, and returns
  a pointer to the previous function associated with s.
  Then, when signal s occurs, f is called and the function associated with s
  may or may not be reset. Thus f may need to call signal(s,f) again to.
  The code here doesn't depend on either interpretation, always being explicit
  about which handler to use.

  There are two signals that can come from the user: quit and interrupt.
  Interrupt should just stop the interpreter and return to B command level;
  quit should stop the B system completely.
  All other signals are caused by errors (eg memory exhausted)
  or come from outside the program, and are therefore fatal.

  SIG_IGN is the system supplied routine to ignore a signal.
  SIG_DFL is the system supplied default for a signal.
  kill(getpid(), signal) kills the program according to 'signal'

  On BSD systems, SIGTSTP and other signals causing the process to be
  suspended, and SIGCONT and others that are ignored by default,
  must not be caught.  It is assumed that all these are defined
  when SIGTSTP is defined.
*/

#ifdef SIGTSTP

/*
 * For Linux, here is an inverted list: All the signals that are not listed as
 * ignored above -- Boldi.
 *
 * int should_handle[] = {
 *     SIGINT,  SIGILL,  SIGABRT,   SIGFPE,  SIGSEGV,   SIGTERM, SIGHUP, SIGQUIT,
 *     SIGTRAP, SIGKILL, SIGPIPE,   SIGALRM, SIGSTKFLT, SIGPWR,  SIGBUS, SIGSYS,
 *     SIGXFSZ, SIGXCPU, SIGVTALRM, SIGPROF, SIGUSR1,   SIGUSR2,
 * };
*/

Hidden bool must_handle(int sig) {
	/* Shouldn't we enumerate the list of signals we *do* want to catch? */
	/* It seems that new signals are all of the type that should be
	   ignored by most processes... */
	switch (sig) {
	case SIGURG:
	case SIGSTOP:
	case SIGTSTP:
	case SIGCONT:
	case SIGCHLD:
	case SIGTTIN:
	case SIGTTOU:
	case SIGIO:
#ifdef SIGWINCH
	case SIGWINCH: /* Window size changed */
#endif
		return No;
	default:
		return Yes;
	}
}

#else /* !SIGTSTP */

#ifdef SIGCLD /* System V */

#define must_handle(sig) ((sig) != SIGCLD)

#else /* !SIGCLD */

#define must_handle(sig) Yes

#endif /* SIGCLD */
#endif /* SIGTSTP */

extern bool vtrmactive;

Hidden Procedure oops(int sig, int m) {
	signal(sig, SIG_DFL); /* Don't call handler recursive -- just die... */
#ifdef sigmask /* 4.2 BSD */
	sigset_t zerosig;
	sigemptyset(&zerosig);
	sigprocmask(SIG_SETMASK, &zerosig, NULL);
	/* Don't block signals in handler -- just die... */
#endif
	putmess(m); /* implies fflush */
	crashend();
	putmess(MESS(3900, "*** abc: killed by signal\n"));
#ifndef NDEBUG
	if (vtrmactive)
		endterm(); /* resets terminal modes; doesn't belong here !!! */
	kill(getpid(), sig);
#else
	immexit(-1);
#endif
}

Hidden SIGTYPE burp(int sig) {
	oops(sig, MESS(3901, "*** Oops, I feel suddenly (BURP!) indisposed. I'll call it a day. Sorry.\n"));
}

Hidden SIGTYPE aog(int sig) {
	oops(sig, MESS(3902, "*** Oops, an act of God has occurred compelling me to discontinue service.\n"));
}

Visible bool intrptd= No;

Hidden SIGTYPE intsig(int sig) {   /* sig == SIGINT */
	(void)sig;
	intrptd= Yes;
	signal(SIGINT, intsig);
}

Hidden SIGTYPE fpesig(int sig) { /* sig == SIGFPE */
	(void)sig;
	signal(SIGFPE, SIG_IGN);
	fpe_signal();
	signal(SIGFPE, fpesig);
}


Hidden SIGTYPE (*setsig(int sig, SIGTYPE (*func)(int)))(int) {
	/*Set a signal, unless it's being ignored*/
	SIGTYPE (*f)(int)= signal(sig, SIG_IGN);
	if (f != SIG_IGN) signal(sig, func);
	return f;
}

Visible Procedure initsig() {
	int i;
	for (i = 1; i <= _NSIG; ++i)
		if (must_handle(i)) VOID setsig(i, burp);
	VOID setsig(SIGINT,  intsig);
	VOID setsig(SIGTRAP, burp);
	VOID setsig(SIGQUIT, aog);
	VOID setsig(SIGTERM, aog);
	VOID setsig(SIGFPE,  fpesig);
}

#endif /* SIGNAL */
