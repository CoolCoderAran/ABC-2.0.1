#!/usr/bin/env bash

set -uo pipefail

SCRIPT_PATH="${BASH_SOURCE:-$0}"
ROOT="$(realpath "$(dirname "$SCRIPT_PATH")/..")"

echo $ROOT
cd "$ROOT/system_tests/" || exit 1

RED='\033[1;31m'
GRN='\033[1;32m'
ORN='\033[1;33m'
BLD='\033[1m'
NC='\033[0m' # No Color

SHOULD_COLOR=1

case "${1---color=auto}" in
--color=never)
    SHOULD_COLOR=0
    ;;
--color=auto | --color)
    if [[ -t 1 ]]; then
      # If stdout is a terminal, then color the output
      SHOULD_COLOR=1
    else
      # Otherwise, do not color the output
      SHOULD_COLOR=0
    fi
    ;;
--color=always)
    SHOULD_COLOR=1
    ;;
esac

if [[ "$SHOULD_COLOR" == 0 ]]; then
    RED=''
    GRN=''
    ORN=''
    BLD=''
    NC=''
fi

abc="$ROOT/src/abc"
origin_cd="$PWD"

echo ""
echo -e "${BLD}---------------- Integration Tests -----------------${NC}"

OK=1

PASSED=0
TOTAL=$(find "$origin_cd" -name "*.expected" | wc -l)
EXCLUDED=$(cat ./EXCLUDED.txt)
FAILING_TESTS=()
TOTAL_SKIPPED=0
TOTAL_FAILED=0

for entry in "$origin_cd"/*
do
    if ! [ -d "$entry" ]; then
        # Skip if entry is not a directory
        continue
    fi

    # setup
    entry_name=${entry##*/}
    if grep "^$entry_name"'$' <<<"$EXCLUDED" > /dev/null; then
        (( TOTAL-- ))
        (( TOTAL_SKIPPED++ ))
        echo -e "${ORN}Skipping $entry_name...${NC}"
        echo ""
        continue
    fi

    cd "$entry" || continue
    current_cd="$PWD"

    ws=""
    if [ -d "$entry_name" ]; then
        ws="$entry_name"
    fi

    ws_flag=""
    if [ -n "$ws" ]; then
        ws_flag="-w $ws"
    fi

    function process_test {
        test_file="$origin_cd/$2/$1.test"
        echo -e "${BLD} TESTING:  ${GRN}$1 ${NC}"

        output_file="$origin_cd/$2/$1.actual.stdout"
        error_file="$origin_cd/$2/$1.actual.stderr"
        echo ">> $abc -g $current_cd $ws_flag <$test_file > $output_file"

        # NOTE: $ws_flag needs to be split. Do not quote.
        # NOTE 2: the 2> command directs stderr to the error output file (needed for bad weather tests)
        $abc -g "$current_cd" $ws_flag <"$test_file" > "$output_file" 2> "$error_file"

        # pick between stdout and stderr based on which kind of test we are running
        if [[ "$2" = bw* ]]; then
            rm -f "$output_file"
            output_file="$error_file"
        else
            rm -f "$error_file"
        fi

        if [ ! -f "$origin_cd/$2/$1.expected" ]; then
            echo -e "${ORN}No $1.expected comparator file found. Skipping test.${NC}
            "
            (( TOTAL_SKIPPED++ ))
            continue
        else
            expected_file="$origin_cd/$2/$1.expected"
        fi

        if diff --strip-trailing-cr --unified --color=auto \
					"$output_file" "$expected_file"
        then
            (( PASSED++ ))
            echo -e "${GRN}Test passed!${NC}"
        else
            OK=0
            FAILING_TESTS+=("$1")
            (( TOTAL_FAILED++ ))
            echo ""
            echo -e "${RED}Files are NOT equal! Test failed!${NC}"
            echo -e "${ORN}See diff above.${NC}"
        fi
        # cleanup
        rm -f "wsgroup.abc"
        rm -r -f "first"
        rm -f "$output_file"
        cd $origin_cd
        echo ""
    }

    if [[ "$entry_name" = bw* ]]; then
        for tf in "$current_cd"/*.test
        do
            test_name=${tf##*/}
            test_name=${test_name%.*}
            process_test "$test_name" "$entry_name"
        done
    else
        process_test "$entry_name" "$entry_name"
    fi
done

if (( "$TOTAL_FAILED" != 0 )); then
    echo -e "${RED}FAILING TESTS:"
fi
for test in "${FAILING_TESTS[@]}"
do
    echo "   $test"
done
echo -e "${NC}"

echo -e "${BLD} ${PASSED}/"${TOTAL}" tests passed, ${TOTAL_FAILED} tests failed, ${TOTAL_SKIPPED} tests skipped. ${NC}"
if (( OK == 0 ));
then
    exit 1
else
    exit 0
fi
