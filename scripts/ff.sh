#/usr/bin/env bash

set -euo pipefail

grep --color=auto -A3 -Er "^([a-zA-Z0-9_-]+\s+)*[^a-zA-Z0-9]*\b$1\(" src/
