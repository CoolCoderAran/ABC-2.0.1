#!/usr/bin/env bash

set -euo pipefail

MAKE="${MAKE:-make}"

prefix="$PWD"

# Build musl
cd musl-*/
./configure --prefix="$prefix"
$MAKE -j"$(nproc)"
$MAKE install
cd ..

# The list of CCs to try
try_cc_list=()
if [ -v CC ]; then
	try_cc_list+=("$CC")
fi
try_cc_list+=(gcc clang)

for cc in "${try_cc_list[@]}"; do
	if [ -f "$prefix/bin/musl-$cc" ]; then
		musl_cc="$prefix/bin/musl-$cc"

		>&2 echo "Found musl-$cc!"
		break
	fi
done

if ! [ -v musl_cc ]; then
	>&2 echo "error: Could not find musl C compiler."
	exit 1
fi

unset TERMINFO TERMINFO_DIRS
export TERMINFO_DIRS="/etc/terminfo"
TERMINFO_DIRS+=":/usr/share/terminfo"
TERMINFO_DIRS+=":/lib/terminfo"
TERMINFO_DIRS+=":/usr/lib/terminfo"
TERMINFO_DIRS+=":/nix/var/nix/profiles/default/share/terminfo"
TERMINFO_DIRS+=":/run/current-system/sw/share/terminfo"
TERMINFO_DIRS+=":/nix/profile/share/terminfo"
TERMINFO_DIRS+=":/var/lib/flatpak/exports/share/terminfo"
export TERMINFO="$TERMINFO_DIRS"

# Build ncurses
cd ncurses-*/
./configure --with-install-prefix="$prefix" --disable-lib-suffixes CC="$musl_cc -static"
$MAKE -j"$(nproc)"
$MAKE install
cd ..
