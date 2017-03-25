#!/bin/zsh

#  tests.sh
#  assignment-one
#
#  Created by Josef Dolezal on 25/03/2017.
#  Copyright © 2017 Josef Dolezal. All rights reserved.

# This scripts takes all in_XY files and puts them as input
# for BINARY program. Output from program is redirected to output file
# and compared with out_XY. If the diff is empty, test succeeds, otherwise the test is marked
# as failed - expected and actual outputs are printed and tests are canceled.

# Get number of input files
BINARY=./main.o
TEMP_OUTPUT=output
TESTS_FOLDER=tests
TESTS_COUNT=$(( $(ls -lA "$TESTS_FOLDER" | tail +2 | wc -l)/2 ))

echo "\nFound $TESTS_COUNT tests.\n"

for i in {1..$TESTS_COUNT}; do
    CURRENT_TEST="$TESTS_FOLDER/in_$i"
    EXPECTED_OUTPUT="$TESTS_FOLDER/out_$i"

    echo "\nRunning test $i.."
    "$BINARY" < "$CURRENT_TEST" > "$TEMP_OUTPUT"

    diff "$EXPECTED_OUTPUT" "$TEMP_OUTPUT" > /dev/null

    if [ $? -ne 0 ] ; then
        echo "  Test $i failed."
        echo "  Expected output ($EXPECTED_OUTPUT):"
        cat "$EXPECTED_OUTPUT"
        echo "  Actual output:"
        cat "$TEMP_OUTPUT"
        echo "  Exiting tests."
        exit 1
    fi;

    echo "  Test $i succeeded"
done;

rm "$TEMP_OUTPUT"
