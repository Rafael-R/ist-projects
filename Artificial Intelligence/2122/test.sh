#!/bin/bash
# Shell Script
# Projeto de Inteligência Artificial, 2021/2022

NC='\033[0m'
RED='\033[0;31m'
GREEN='\033[1;32m'

if [ $# != 1 ] ; then
    echo "Usage: $0 <tests_dir>"
    echo "Example: $0 tests"
    exit 1
fi

testdir=$1

passed=0
total=0

for test_in in $testdir/input_*; do

    test="${test_in#*_}"
    test_out="$testdir/output_$test"
    test_myout="$testdir/myoutput_$test"

    python3 takuzu.py < $test_in > $test_myout

    ((total++))
    DIFF=$(diff -b $test_out $test_myout)

    if [ "$DIFF" != "" ]; then
        echo -e "${RED}The output of test [${test##*/}] differs from expected.${NC}"
        echo "$DIFF"
    else
        ((passed++))
        echo -e "${GREEN}The output of test [${test##*/}] matches expected.${NC}"
        rm $test_myout
    fi

done

echo "- Passed $passed/$total (~$(( passed * 100 / total))%)"