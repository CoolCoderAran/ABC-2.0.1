#!/usr/bin/env bash

set -euo pipefail

function with-header-guard() {
  HEADER_DEFINE="$(echo "ABC_${1%%.h}_HEADER" | tr -s '[:lower:]' '[:upper:]' | tr -s '/_\-\.' '____')"

  cat <<EOF
#ifndef $HEADER_DEFINE
#define $HEADER_DEFINE

EOF

  cat "$1"

  cat <<EOF

#endif  /* HEADER GUARD */
EOF
}

with-header-guard "$1" | sponge "$1"
