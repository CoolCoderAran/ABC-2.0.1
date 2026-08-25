.PHONY: static dynamic deps-static
.PHONY: test-system test-cfg coverage
.PHONY: cleanall clean cleantemp cleancoverage cleandist cleandeps

#--------------#
#  PARAMETERS  #
#--------------#

CC= cc
EXTRA_CFLAGS=
EXTRA_LDFLAGS=
BIT=
COVERAGE= 0

#------------------#
#  SMART DEFAULTS  #
#------------------#

SYSTEM=

UNAME:= $(shell uname -s)
ifeq ($(findstring CYGWIN, $(UNAME)), CYGWIN)
	SYSTEM= CYGWIN
else ifeq ($(findstring Linux, $(UNAME)), Linux)
	SYSTEM= linux
else ifeq ($(findstring Darwin, $(UNAME)), Darwin)
	SYSTEM= macos
else
	# Leave the default
endif

SUBMAKE= $(MAKE) CC='$(CC)' SYSTEM='$(SYSTEM)' BIT='$(BIT)' COVERAGE='$(COVERAGE)'
SUBMAKE+= EXTRA_CFLAGS='$(EXTRA_CFLAGS)' EXTRA_LDFLAGS='$(EXTRA_LDFLAGS)'

#-------------------#
#  GENERAL TARGETS  #
#-------------------#

#- default -#
dynamic:
	@cd src/; $(SUBMAKE) all

MUSL_CC= $(shell realpath ./src/deps/bin/musl-gcc)
static: deps-static
	cd src/; $(SUBMAKE) STATIC=1 CC='$(MUSL_CC) -static' all

deps-static:
	@cd src/deps; $(MAKE)

#---------------#
#  TEST THINGS  #
#---------------#

# We don't actually have unit tests.
# Enable and add to PHONY after we start having unit tests.
#test-unit:
#	@cd src/; make clean; make unit_test

test-system:
	@./scripts/run_tests.sh

test-cfg: scripts/abc_32
	@cd src; ../scripts/cfg_fuzzing.py ./abc

coverage:
	@./scripts/report_coverage.sh

#-----------#
#  OLD ABC  #
#-----------#

temp/abc.tar.gz:
	mkdir -p temp
	cd temp; wget 'https://homepages.cwi.nl/~steven/abc/implementations/abc.tar.gz'

#temp/$(SYSTEM)/ABC: temp/abc.tar.gz
temp/linux/ABC/abc: temp/abc.tar.gz
	mkdir -p temp/$(SYSTEM)
	cd temp; tar -xzf abc.tar.gz -C $(SYSTEM)
	patchelf --replace-needed libtinfo.so.5 libtinfo.so.6 $@

scripts/abc_32: temp/$(SYSTEM)/ABC/abc
	cp $^ $@

#----------------#
#  CLEAN THINGS  #
#----------------#

cleanall: clean cleantemp cleancoverage cleandist cleandeps

clean:
	@cd src/; $(SUBMAKE) clean

cleantemp:
	rm -f -r temp
	rm -f scripts/abc_32

cleancoverage:
	find . \( -name '*.gcda' -or -name '*.gcno' -or -name '*.gcov' \) \
		-exec rm {} \;

cleandist:
	@echo "TODO: Clean dist files built"

cleandeps:
	@cd src/deps/; $(MAKE) clean
