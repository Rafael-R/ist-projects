#!/bin/bash
# Shell Script
# Projeto de Análise e Síntese de Algoritmos, 2019/2020

NC='\033[0m'
RED='\033[0;31m'
GREEN='\033[1;32m'

if [ $# != 1 ] ; then
    echo "Usage: $0 <tests_dir>"
    echo "Example: $0 tests"
    exit 1
fi

testdir=$1

echo "Compiling..."
echo "-----------------------------------------------------------"
make > /dev/null || exit 1

passed=0
total=0

for test in $testdir/*.out; do

    test="${test%%.*}"
    test_in="$test.in"
    test_out="$test.out"
    test_myout="$test.myout"

    ./main < $test_in > $test_myout

    ((total++))
    DIFF=$(diff -b $test_out $test_myout)

    if [ "$DIFF" != "" ]; then
        echo -e "${RED}The output of test [${test##*/}] differs from expected.${NC}"
        echo "$DIFF"
    else
        ((passed++))
        echo -e "${GREEN}The output of test [${test##*/}] matches expected.${NC}"
    fi

done

echo "- Passed $passed/$total (~$(( passed * 100 / total))%)"
echo "-----------------------------------------------------------"
echo "Cleaning..."
make clean > /dev/null
for myout in $testdir/*.myout; do
    rm $myout
done
