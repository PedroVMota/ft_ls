#!/usr/bin/env bash
set -euo pipefail

# Directory where we'll dump all the outputs
TESTING_DIR="Object/Testing"
mkdir -p "$TESTING_DIR"

# Binaries under test
FT_LS_BIN="./ft_ls"
LS_BIN="ls"

# The directory to feed to both commands (default: current directory)
TEST_DIR="${1:-.}"

# Test‐cases: each is a string of flags
TEST_CASES=(
    ""                # no flags
    "-l"
    "-a"
    "-R"
    "-r"
    "-t"
    "-l -a"
    "-l -R"
    "-l -a -r -t"
)

echo "Running ft_ls vs ls tests on directory: $TEST_DIR"
echo

for FLAGS in "${TEST_CASES[@]}"; do
    # Make a safe name for the files (spaces → underscores; empty → "none")
    SAFE_NAME=$(echo "$FLAGS" | tr ' ' '_' | tr -cd '[:alnum:]_')
    [[ -z "$SAFE_NAME" ]] && SAFE_NAME="none"

    FT_OUT="$TESTING_DIR/ft_ls_${SAFE_NAME}.out"
    LS_OUT="$TESTING_DIR/ls_${SAFE_NAME}.out"

    # Run ft_ls and ls, capturing stdout+stderr
    if [[ -z "$FLAGS" ]]; then
        # no flags
        "$FT_LS_BIN" "$TEST_DIR"        > "$FT_OUT" 2>&1
        "$LS_BIN"    "$TEST_DIR"        > "$LS_OUT" 2>&1
    else
        # split flags into array so that "-l -a" becomes two args
        read -r -a FLAGS_ARR <<< "$FLAGS"
        "$FT_LS_BIN" "${FLAGS_ARR[@]}" "$TEST_DIR"   > "$FT_OUT" 2>&1
        "$LS_BIN"    "${FLAGS_ARR[@]}" "$TEST_DIR"   > "$LS_OUT" 2>&1
    fi

    # Compare
    if diff -q "$FT_OUT" "$LS_OUT" > /dev/null; then
        echo "PASS: [$FLAGS]"
    else
        echo "FAIL: [$FLAGS]"
        echo "---- diff ----"
        diff -u "$LS_OUT" "$FT_OUT" || true
        echo "---- ls output (${LS_OUT}) ----"
        cat "$LS_OUT"
        echo
        echo "---- ft_ls output (${FT_OUT}) ----"
        cat "$FT_OUT"
        echo
        # stop on first failure
        exit 1
    fi
done

echo
echo "All tests passed! Outputs are in $TESTING_DIR/"