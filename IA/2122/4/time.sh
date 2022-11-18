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

for test_in in $testdir/input_*; do

    test="${test_in#*_}"

    echo "${test##*/}"
    output=$(time python3 takuzu.py < $test_in)
    echo "$output" | tail -0
    echo "---------------------"

done
