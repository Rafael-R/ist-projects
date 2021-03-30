#!/bin/bash
# Shell Script
# Projeto de Análise e Síntese de Algoritmos, 2019/2020

if [ $# != 1 ] ; then
    echo "Usage: $0 <tests_dir>"
    echo "Example: $0 tests"
    exit 1
fi

testdir=$1

echo "Compiling..."
echo "--------------------------"
make > /dev/null || exit 1

for test_in in $testdir/*.in; do

    test="${test_in%%.*}"

    echo "${test##*/}"
    output=$(time ./main < $test_in)
    echo "$output" | tail -0
    echo "--------------------------"

done

echo "Cleaning..."
make clean > /dev/null
