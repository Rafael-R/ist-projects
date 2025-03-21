#!/bin/bash
# Shell Script
# Projeto de Análise e Síntese de Algoritmos, 2023/2024

testdir=tests/generated

for test in $testdir/*.in; do

    test="${test%%.*}"
    test_in="$test.in"
    test_out="$test.out"

    start=$(date +%s)
    python3 src/main.py < $test_in > $test_out
    end=$(date +%s)
    echo "${test##*/}: $(($end-$start)) s"

done

for out in $testdir/*.out; do
    rm $out
done
