#!/usr/bin/env bash

set -euo pipefail

SCRIPT_PATH="${BASH_SOURCE:-$0}"
ROOT="$(realpath "$(dirname "$SCRIPT_PATH")/..")"

polyfill_glibc=~/src/polyfill-glibc/polyfill-glibc

arch="$(uname -m)"

function main() {
	if which git >/dev/null && [ -e "$ROOT"/.git ]; then
		if ! [ -v 1 ]; then
			>&2 echo "error: must provide git rev/tag to archive"
			exit 1
		fi
		local tag="$1"

		# It only archives the CWD, not the entire repository
		archive-source "$tag"

		git checkout "$tag"
	else
		>&2 echo "warn: No 'git' found. Skipping 'archive' and 'checkout'..."
	fi

	cd "$ROOT/"

	build-dynamic
	package-as "abc-linux-${arch}-dynamic"

	# We concluded that glibc-polyfill is sus.
	#polyfill-binary
	#package-as "abc-linux-${arch}-dynamic-compat"

	if ! (
		build-dynamic "BIT=32"
		# 32-bit
		package-as "abc-linux-i386-dynamic"
	); then
		>&2 echo "warn: Failed to build 32-bit binary. Skipping..."
	fi

	if ! (
		build-static
		package-as "abc-linux-${arch}-static"
	); then
		>&2 echo "warn: Failed to build static binary. Skipping..."
	fi
}

function build-dynamic() {
	make clean
	make -j"$(nproc)" EXTRA_CFLAGS="-s" "$@"
	strip abc abckeys
}

function polyfill-binary() {
	$polyfill_glibc --target-glibc=2.31 abc
}

function build-static() {
	make clean
	make static -j"$(nproc)"
	strip abc abckeys
}

FILES=(abc abckeys abc.1 abc.hlp abc.msg)

function package-as() {
	local name="$1"
	tar -czf "$ROOT/dist/$name.tar.gz" "${FILES[@]}" -C dist/linux README.txt --transform 's,^,ABC/,'
	tar -cJf "$ROOT/dist/$name.tar.xz" "${FILES[@]}" -C dist/linux README.txt --transform 's,^,ABC/,'
}

function archive-source() {
	local tag="$1"

	git archive "$tag" --prefix=abc/ -o "$ROOT/dist/abc-source.zip"
	git archive "$tag" --prefix=abc/ -o "$ROOT/dist/abc-source.tar.gz"

	if ! which xz >/dev/null; then
		>&2 echo "warn: Could not find 'xz'. Skipping tar.xz source..."
		return 0
	fi

	git archive "$tag" --prefix=abc/ -o "$ROOT/dist/abc-source.tar"
	xz "$ROOT/dist/abc-source.tar"
}

main "$@"
