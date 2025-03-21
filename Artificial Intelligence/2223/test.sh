#!/bin/zsh
# Shell Script
# Projeto de Inteligência Artificial, 2022/2023

NC='\033[0m'
RED='\033[0;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'

if [ $# != 1 ] ; then
    echo "Usage: $0 <tests_dir>"
    echo "Example: $0 tests"
    exit 1
fi

testdir=$1

failed=()
passed=0
total=0

for test_in in $testdir/*.txt; do
    test="${test_in%.*}"
    test_out="$test.out"
    test_myout="$test.myout"

    python3 src/bimaru.py < $test_in > $test_myout

    ((total++))
    DIFF=$(diff -b $test_out $test_myout)

    if [ "$DIFF" != "" ]; then
        line=$(head -n 1 $test_myout)
        if [ "$line" = "No solution found" ]; then
            failed+=($test)
            echo -e "${RED}TEST ${test##*/}: ERROR.${NC}"
        else
            ((passed++))
            echo -e "${YELLOW}TEST ${test##*/}: DIFFER.${NC}"
        fi
    else
        ((passed++))
        echo -e "${GREEN}TEST ${test##*/}: SUCCESS.${NC}"
        rm $test_myout
    fi
done

echo "- Passed $passed/$total (~$(( passed * 100 / total))%)"
for value in "${failed[@]}"; do
    echo $value
done