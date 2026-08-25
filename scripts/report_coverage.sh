#!/usr/bin/env bash

set -euo pipefail

SCRIPT_PATH="${BASH_SOURCE:-$0}"
ROOT="$(realpath "$(dirname "$SCRIPT_PATH")/..")"

GCOV=(gcov -b -c -p -H)

find "$ROOT"/src/ -print0 -type f -name '*.c' | (xargs -0 "${GCOV[@]}" || true)
