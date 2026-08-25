#!/usr/bin/env bash

set -euo pipefail

(ctags --output-format=json --c-kinds=f "$1" |
  jq -r '.pattern' |
  sed 's/^\/^/Forward /' |
  sed 's/ {[^$]*$\/$/;/' |
  grep -v 'Visible' |
  tee /dev/stderr |
  wl-copy
 ) 2>/dev/stdout
