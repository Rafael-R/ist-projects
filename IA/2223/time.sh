#!/bin/bash
# Shell Script
# Projeto de Inteligência Artificial, 2021/2022

if [ $# != 1 ] ; then
    echo "Usage: $0 <tests_dir>"
    echo "Example: $0 tests"
    exit 1
fi

testdir=$1

echo "---------------------"

for test_in in $testdir/*.txt; do

    test="${test_in%.*}"

    echo "${test##*/}"
    output=$(time python3 src/bimaru.py < $test_in)
    echo "$output" | tail -0
    echo "---------------------"

done
