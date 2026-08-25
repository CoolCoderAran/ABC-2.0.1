#######################################################################
#                                                                     #
#                 Makefile for ABC system under unix.                 #
#                                                                     #
#######################################################################

# --- Some make's only make love with the Bourne shell ---
#

SHELL=	/bin/sh


.PHONY: all
.PHONY: unit_test examples try_editor lint
.PHONY: dist install installdest
.PHONY: mflags makefiles tags id
.PHONY: clean clobber

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# +++ Start of editable macro definitions; filled in by ./Setup      +++
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# --- C compiler to use ---
#
# In case you might want to change it to e.g. 'gcc' (the GNU C compiler).

CC=	cc


# --- Set whether to compile for 64bit or 32bit --
#
# Change in the `make` invocation:
#     make all BIT=32
BIT=


# --- Set whether to compile with coverage flags --
#
# Change in the `make` invocation:
#     make all COVERAGE=1
COVERAGE=0

# --- Set whether to compile a static executable --
#
# Change in the `make` invocation:
#     make all STATIC=1
STATIC=0

# --- Where to install the stuff ---
#
# These should all be absolute pathnames.

# destination directory for binaries 'abc' and 'abckeys':

DESTABC=/usr/local

# destination directory for auxiliary data files:

DESTLIB=/usr/local/lib/abc

# destination directory for 'abc.1' manual page:

DESTMAN=/usr/man/manl

# local destination if you cross-compile; empty otherwise:

DESTROOT=
# you should first generate unix/mach.h remotely;
# see 'make mach' below.

# directory where 'abc' and 'abckeys' should locate auxiliary files,
# when created in the current directory;
# change this to some proper absolute pathname, like /ufs/$user,
# if `pwd` gives an unsteady pathname, e.g. when using NFS mounted filesystems.

DESTALL="`pwd`"


# --- Software floating point needed? ---

FLOAT=

# --- Flags to the C compiler ---

ifeq ($(BIT), 32)
BIT_FLAGS= -m32
else ifeq ($(BIT), 64)
BIT_FLAGS= -m64
endif

ifeq ($(COVERAGE), 1)
COVERAGE_FLAGS= --coverage
else
COVERAGE_FLAGS=
endif

ifeq ($(STATIC), 1)
STATIC_LINKING_LDFLAGS= -static \
	-L$(PWD)/deps/lib \
	-L$(PWD)/deps/usr/lib
STATIC_LINKING_CFLAGS= -static \
	-I$(PWD)/deps/include \
	-I$(PWD)/deps/usr/include/
else
STATIC_LINKING_LDFLAGS=
STATIC_LINKING_CFLAGS=
endif


# Set this in the make invocation to include extra C flags
EXTRA_CFLAGS=

# DEFS=	-DNDEBUG
DEFS= -Dlint

#Set this to suppress the warnings that we have looked at and deemed unnecessary to fix
UNUSEDWARNFLAGS = \
	-Wno-unused-function \

# https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++.html
HARDENING_FLAGS= \
	-Wall -Wextra -Wpedantic -Wformat -Wformat=1\
	-Werror=format-security \
	-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
	-D_GLIBCXX_ASSERTIONS \
	-fstrict-flex-arrays=3 \
	-fstack-clash-protection -fstack-protector-strong \
	-fPIE -pie

LEGACY_FLAGS= \
	-Werror=implicit -Werror=incompatible-pointer-types -Werror=int-conversion

PRODUCTION_FLAGS = \
	-fno-delete-null-pointer-checks -fno-strict-overflow \
	-fno-strict-aliasing -ftrivial-auto-var-init=zero

# CFLAGS= -O $(FLOAT) $(DEFS)
CFLAGS= $(FLOAT) $(DEFS) $(EXTRA_CFLAGS) \
		$(COVERAGE_FLAGS) \
		-I$(PWD) \
		$(STATIC_LINKING_CFLAGS) \
		-std=c23 \
		$(HARDENING_FLAGS) \
		$(UNUSEDWARNFLAGS) \
		$(BIT_FLAGS)


# --- Flags to the loader ---

LDFLAGS=$(BIT_FLAGS) $(COVERAGE_FLAGS) $(STATIC_LINKING_LDFLAGS)


# --- Specify termcap or terminfo library ---
#
# Set TERMLIB to the appropriate termcap or terminfo library specification
# (either -lxxx option or absolute pathname) if your system has one.
# Otherwise leave TERMLIB empty and remove the comment symbols before
# the definitions of OWNTLIB, KOWNTLIB and OWNTBASE to install the
# public domain version from ./tc.

TERMLIB= -lncurses

#OWNTLIB= libtermcap.a
#KOWNTLIB= ../libtermcap.a
#OWNTBASE= termcap


# --- Libraries for editor-interpreter 'abc' ---

LIBS=	-lm $(TERMLIB) $(OWNTLIB)


# --- Libraries for utility 'abckeys' ---

KLIBS=	$(TERMLIB) $(KOWNTLIB)


# +++++++++++++++++++++++++++++++++++++++++
# +++ End of editable macro definitions +++
# +++++++++++++++++++++++++++++++++++++++++
#
# The remaining macro definitions should only have to be edited
# if you make very drastic changes.

# --- Specify system directory ---

SYSTEM= linux

ifeq ($(SYSTEM), cygwin)
	# --- remove flags that cygwin doesn't have ---
	HARDENING_FLAGS= \
		-Wall -Wextra -Wpedantic -Wformat -Wformat=2 \
		-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
		-D_GLIBCXX_ASSERTIONS \
		-Wfloat-equal
endif

# --- Include flags to the C compiler for editor and interpreter directories ---

BINCL=	-I../bhdrs -I../$(SYSTEM)
EINCL=	-I../bhdrs -I../ehdrs -I../$(SYSTEM) -I../btr
IINCL=	-I../bhdrs -I../ihdrs -I../$(SYSTEM)
UINCL=	-I../bhdrs -I../ehdrs -I../ihdrs -I../$(SYSTEM)

# --- Editor and interpreter directories ---

CDIRS=	b bed bint1 bint2 bint3 btr $(SYSTEM) stc bio

BDIRS=	b
EDIRS=	bed
IDIRS=	bint1 bint2 bint3 btr stc bio
UDIRS=	$(SYSTEM)

# --- Editor and interpreter files ---

BOBJS=	b/*.o
EOBJS=	bed/*.o
IOBJS=	bint1/*.o bint2/*.o bint3/*.o btr/*.o stc/*.o bio/*.o
UOBJS=	$(SYSTEM)/*.o

BSRCS=	b/*.c
ESRCS=	bed/*.c
ISRCS=	bint1/*.c bint2/*.c bint3/*.c btr/*.c stc/*.c bio/*.c
USRCS=	$(SYSTEM)/*.c

BHDRS=	bhdrs/*.h
EHDRS=	ehdrs/*.h
IHDRS=	ihdrs/*.h btr/*.h bio/*.h stc/*.h
UHDRS=	$(SYSTEM)/*.h

# --- Preliminary dependencies (do not change for Unix) ---

MACH=	$(SYSTEM)/mach.h
OSHDIR= $(SYSTEM)

DEST=	$(SYSTEM)/file.h

# --- Stuff for programmers ---

LINT=		lint
# change the next one to -p for ATT System V
LINTFLAGS=	-abhxp
LINCL=		-Ibhdrs -Iehdrs -Iihdrs -I$(SYSTEM) -Ibtr

TAGDIRS=b bed bint1 bint2 bint3 btr stc bio $(SYSTEM) keys bhdrs ehdrs ihdrs


# -------------------------------------------
# ---  make all: make everything locally  ---
# -------------------------------------------
#
# This makes all programs and utilities in the current directory.
# (Except for the ready-for-use default key definitions files).

all:	alldest $(MACH) abc.ld abckeys.ld
	@./ch_all "$(DESTROOT)"

# The target 'alldest' is used to communicate the place of auxiliary files.
# This target is also used for a separate 'make abc' or 'make abckeys'.
#
# Dependency on the (non-existent) file "ALWAYS" causes this entry to
# be (re)made unconditionally. Make won't complain about ALWAYS not being
# found because there is also a rule referencing it as target at the
# very end (which actually doesn't make it, but make doesn't care).

alldest: ALWAYS
	echo "#define ABCLIB \"$(DESTALL)\"" >dest
	if cmp -s dest $(DEST); then rm dest; else mv dest $(DEST); fi

# mach: generate include file with info about the hardware configuration.
#
# Special care is taken to remove an incomplete $(MACH) if mkmach
# fails halfway. Otherwise a subsequent 'make depend' will happily go on.

mach: $(MACH)

$(MACH): mkmach.c $(OSHDIR)/comp.h $(OSHDIR)/feat.h
	@./ch_mach "$(DESTROOT)" "$(MACH)"
	$(CC) $(CFLAGS) -I$(OSHDIR) mkmach.c -o mkmach
	./mkmach >$(MACH) || (rm -f $(MACH) && exit 1)

# abc: make the executable that is the kernel of the system.
#
# The load must be unconditional, since we cannot know whether
# any of the submakes had to update some subtarget.

abc: alldest abc.ld

abc.objects: alldest $(MACH) $(BDIRS) $(EDIRS) $(IDIRS) $(UDIRS) \
		$(OWNTLIB) $(OWNTBASE) ALWAYS

abc.ld: $(MACH) abc.objects $(SYSTEM)/main.o ALWAYS
	$(CC) $(LDFLAGS) $(BOBJS) $(EOBJS) $(IOBJS) $(UOBJS) \
		-o abc \
		$(LIBS)

# Unit testing

# Make sure to pass `EXTRA_CFLAGS=-DNO_ENTRY_POINT`
unit_tests/main.o: $(MACH) unit_tests/main.c
	$(CC) $(CFLAGS) -Iunix/ -Ibhdrs/ -Iihdrs/ -Iehdrs/ -Ibtr/ \
		unit_tests/main.c -c -o $@

# FIXME: Removing the unix/main.o is a naughty workaround...
# But there is no nice way with excluding it as things currently stand.
unit_tests/main: unit_tests/main.o abc.objects
	@rm -f $(UDIRS)/main.o
	$(CC) $(CFLAGS) $(LDFLAGS) unit_tests/main.o $(BOBJS) $(EOBJS) \
		$(IOBJS) $(UOBJS) -o $@ \
		$(LIBS)

unit_test: $(MACH) ALWAYS
	$(MAKE) CC='$(CC)' CFLAGS='$(CFLAGS) -DNO_ENTRY_POINT' unit_tests/main
	./unit_tests/main

# Call make for each editor and interpreter subdirectory with proper flags.
#
# If a dependency line has more than one item left of the colon, the
# commands are executed for each of the items, with $@ substituted
# by the item's name.

$(BDIRS): $(MACH) ALWAYS
	cd $@; $(MAKE) -f MF -f DEP CC='$(CC)' SYSTEM='$(SYSTEM)' CFLAGS='$(CFLAGS) $(BINCL)' all

$(EDIRS): $(MACH) ALWAYS
	cd $@; $(MAKE) -f MF -f DEP CC='$(CC)' SYSTEM='$(SYSTEM)' CFLAGS='$(CFLAGS) $(EINCL)' all

$(IDIRS): $(MACH) ALWAYS
	cd $@; $(MAKE) -f MF -f DEP CC='$(CC)' SYSTEM='$(SYSTEM)' CFLAGS='$(CFLAGS) $(IINCL)' all

$(UDIRS): $(MACH) ALWAYS
	cd $@; $(MAKE) -f MF -f DEP CC='$(CC)' SYSTEM='$(SYSTEM)' CFLAGS='$(CFLAGS) $(UINCL)' all

$(SYSTEM)/main.o: $(MACH) $(SYSTEM)/main.c
	$(CC) $(CFLAGS) -Ibhdrs/ -I$(OSHDIR) -c $(SYSTEM)/main.c -o $@

# Make utility 'abckeys' for redefinition of keybindings.
#
# The submake will find out whether recompilation is necessary.

abckeys: alldest abckeys.ld

abckeys.ld: $(MACH) $(OWNTLIB) $(OWNTBASE) ALWAYS
	cd keys; \
	 $(MAKE) -f Makefile -f DEP SYSTEM="$(SYSTEM)" \
	 	CC='$(CC)' CFLAGS="$(CFLAGS)" SYSTEM='$(SYSTEM)' LDFLAGS="$(LDFLAGS)" LIBS="$(KLIBS)" all


# ----------------------------------------------
# --- make examples: try the ABC interpreter ---
# ----------------------------------------------

examples:
	./ex/DoExamples local
#	ch_examples is embedded in DoExamples to cope with cross compilation.


# ---------------------------------------------------------
# --- make try_editor: try the ABC editor interactively ---
# ---------------------------------------------------------

try_editor:
	@cd ex; ./TryEditor local
#	ch_tryeditor embedded in TryEditor.

# ---------------------------------------------------------
# --- make install: install everything in public places ---
# ---------------------------------------------------------
#
# The dependency of 'install' on installdest communicates the place
# and place of auxiliary files to the binaries 'abc' and 'abckeys'.
# The unconditional submakes of the targets 'abc.ld' and 'abckeys.ld'
# causes the proper files to be remade.
#
# The directory ukeys contains default keydefinitions files for
# several terminals.

install: installdest abc.ld abckeys.ld
	cp abc abckeys $(DESTROOT)$(DESTABC)
	cp abc.msg abc.hlp $(DESTROOT)$(DESTLIB)
	cd ukeys; cp abc*.key $(DESTROOT)$(DESTLIB)
	cp abc.1 $(DESTROOT)$(DESTMAN)
	@./ch_install "$(DESTABC)" "$(DESTLIB)" "$(DESTMAN)" "$(DESTROOT)"

installdest: ALWAYS
	echo "#define ABCLIB \"$(DESTLIB)\"" >dest
	if cmp -s dest $(DEST); then rm dest; else mv dest $(DEST); fi


# -------------------------------------------------
# --- Make our own termcap library and database ---
# -------------------------------------------------
#
# For systems that really don't have any termlib-like library
# this makes our own from public domain sources in ./tc.
# See ./tc/README for details.
# This happens automatically if you remove the comment symbols before
# the definitions of OWNTLIB and OWNTBASE above.

libtermcap.a:
	cd tc; make library

termcap:
	cd tc; make database


# -----------------------------------
# ---  make clean: local cleanup  ---
# -----------------------------------

clean:
	rm -f \
		*/*.o \
		mkmach $(MACH) $(DEST) \
		ex/out \
		unit_tests/main \
		ex/*/*.cts \
		abc abckeys


# ---------------------------------------------
# ---  Utilities for the programmer/porter  ---
# ---------------------------------------------

mflags:
	echo MFLAGS="$(MFLAGS)", MAKEFLAGS="$(MAKEFLAGS)"

# --- make makefiles: construct trivial makefiles in subdirectories ---
#
# This reconstructs trivial makefiles called 'MF' in relevant subdirectories.
# See ./Port for details.

# TODO: `makefiles` needs updating. We do not want to have `main.c`
# nor `main.o` mentioned in `*/MF` files.

makefiles:
	for i in $(CDIRS); do \
		( cd $$i; echo all: *.c | sed 's/\.c/.o/g' >MF ) done

# No automatic makefile in ./keys. Edit that one yourself if need be.

# --- How to generate dependency information for make ---
#
# Set MKDEP to $(CC) -M $(DEFS) or to ../scripts/mkdep $(DEFS).
# 'cc -M' is a 4.2BSD-only feature which causes the C preprocessor
# to output a list of dependencies that is directly usable by make.
# This can be simulated exactly by piping the output of your preprocessor
# through the shell script ./scripts/mkdep.
# Check the comments there to see if it needs polishing for your system.

MKDEP=	$(CC) -M $(DEFS)

# --- make depend: construct makefiles with dependencies in subdirectories ---
#
# This reconstructs additional makefiles called 'DEP' in subdirectories
# containing the dependency information.
# Dependencies on system include files are grep'ed out, to make the
# resulting DEP files distributable.
# See ./Port.

# TODO: This is fully broken. The CC invocations need complete rewriting.
# CC always fails due to faulty include paths provided.

depend: $(MACH) $(DEST) bdep edep idep udep kdep

bdep:
	for i in $(BDIRS); do \
	( echo $$i; cd $$i; $(MKDEP) $(BINCL) *.c | grep -v ': /' >DEP ) done

edep:
	for i in $(EDIRS); do \
	( echo $$i; cd $$i; $(MKDEP) $(EINCL) *.c | grep -v ': /' >DEP ) done

idep:
	for i in $(IDIRS); do \
	( echo $$i; cd $$i; $(MKDEP) $(IINCL) *.c | grep -v ': /' >DEP ) done

udep:
	for i in $(UDIRS); do \
	( echo $$i; cd $$i; $(MKDEP) $(UINCL) *.c | grep -v ': /' >DEP ) done

kdep:
	cd keys; \
	$(MAKE) MKDEP="$(MKDEP)" DEFS="$(DEFS)" SYSTEM="$(SYSTEM)" depend \
	 | grep -v ': /' >DEP

$(DEST): alldest

# --- make messages ---
#
# Make new messages file when you have changed or added any MESS/GMESS
# definitions in the source.
# Note: the Collect and Change scripts can be found in ./scripts.
# See ./Problems for details.

messages: abc.msg

abc.msg: $(BSRCS) $(ESRCS) $(ISRCS) $(USRCS) \
		ihdrs/i0err.h ehdrs/erro.h bio/i4bio.h
	./scripts/Collect $(BSRCS) $(ESRCS) $(ISRCS) $(USRCS) \
		ihdrs/i0err.h ehdrs/erro.h bio/i4bio.h >abc.msg

# --- make help ---
#
# Help file from manual entry.
# Sorry, the file unix/abc.mac was created from copyrighted material;
# therefore, it is not in the distribution.
#

help: abc.hlp

abc.hlp: unix/abc.mac abc.1
	nroff unix/abc.mac abc.1 >abc.help
	(echo "SUMMARY OF SPECIAL ACTIONS"; \
	 sed -e '1,/^SUMMARY/d' abc.help; \
	 echo " "; \
	 sed -e '/^SUMMARY/,$$d' abc.help) >abc.hlp
	rm abc.help

# ---  make clobber: additional local cleanup   ---

clobber:
	rm -f abc abckeys */tags tags ID

# ---  make dist: turn an ABC version into a distribution  ---
# cleans auxiliary files and CVS administration

dist:	clean clobber
	find . -name CVS.adm -exec rm -rf {} \;
	find . \( -name \*~ -o -name .\*~ \) -exec rm -f {} \;
	rm -f Makefile answers unix/abc.mac
	rm -f userdirs repositories repos-userdirs
	rm -f Dbxabc Lint Out

# --- make lint, tags and id ---

lint:	$(DEST) abclint klint

abclint:
	$(LINT) $(LINTFLAGS) $(DEFS) $(LINCL) \
		$(BSRCS) $(ESRCS) $(ISRCS) $(USRCS)

klint:
	cd keys; \
	 $(MAKE) LINT="$(LINT)" LINTFLAGS="$(LINTFLAGS)" DEFS="$(DEFS)" \
		SYSTEM="$(SYSTEM)" lint

tags:	ALWAYS
	etags */*.h */*.c
	rm -f tags   # Remove it so it will be remade when an interrupt hits
	for i in $(TAGDIRS); \
	do \
		( echo $$i; cd $$i; ctags -w *.[ch]; \
		  sed "s,	,	$$i/," tags \
		) \
	done | sort -o tags


id:	ALWAYS
	mkid */*.[hc]


ALWAYS: # Must not exist, but must be mentioned in the makefile
