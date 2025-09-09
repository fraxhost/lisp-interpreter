#!/bin/bash

# Compile program
gcc -o main main.c

# Base paths
RESOURCE_DIR_IN="./resources/sprint1/input"
RESOURCE_DIR_EX="./resources/sprint1/expected"
RESOURCE_DIR_OUT="./resources/sprint1/output"

# File to store the final test report
REPORT_FILE="./resources/sprint1/test_report.txt"

TOTAL=20
PASS=0

# Clear previous report
echo "===== Test Report =====" > "$REPORT_FILE"
echo >> "$REPORT_FILE"

for i in $(seq 1 $TOTAL); do
    INPUT="$RESOURCE_DIR_IN/tc${i}.txt"
    EXPECTED="$RESOURCE_DIR_EX/tc${i}.txt"
    OUTPUT="$RESOURCE_DIR_OUT/tc${i}.txt"

    echo "============================" >> "$REPORT_FILE"
    echo "Running test $i..." >> "$REPORT_FILE"

    # Run program and capture output
    ./main "$INPUT" 2>&1 > "$OUTPUT"

    if [ ! -f "$EXPECTED" ]; then
        echo "⚠️ Expected file $EXPECTED not found. Skipping test $i." >> "$REPORT_FILE"
        continue
    fi

    EXPECTED_CONTENT=$(cat "$EXPECTED")
    GOT_CONTENT=$(cat "$OUTPUT")

    echo "Input:" >> "$REPORT_FILE"
    cat "$INPUT" >> "$REPORT_FILE"
    echo >> "$REPORT_FILE"

    echo "Expected:" >> "$REPORT_FILE"
    echo "$EXPECTED_CONTENT" >> "$REPORT_FILE"

    echo "Got:" >> "$REPORT_FILE"
    echo "$GOT_CONTENT" >> "$REPORT_FILE"

    if diff -q "$OUTPUT" "$EXPECTED" >/dev/null; then
        echo "✅ Test $i PASSED" >> "$REPORT_FILE"
        PASS=$((PASS + 1))
    else
        echo "❌ Test $i FAILED" >> "$REPORT_FILE"
    fi
    echo >> "$REPORT_FILE"
done

echo "==== Summary ====" >> "$REPORT_FILE"
echo "Passed $PASS out of $TOTAL tests" >> "$REPORT_FILE"
