#!/bin/bash
# Shell Script
# Projeto de Análise e Síntese de Algoritmos, 2020/2021

if [ $# != 1 ] ; then
    echo "Usage: $0 <generator_file>"
    echo "Example: $0 tests"
    exit 1
fi

generator=$1

echo "Compiling..."
echo "--------------------------"
make > /dev/null || exit 1

cost=(10 20 40)
size=(1000 2000 3000 4000 5000 6000 7000 8000 9000 10000)

if [ ! -d generated/ ]; then
    mkdir -p generated;
else
    rm -rf generated/*
fi

for c in ${cost[@]}; do
    for s in ${size[@]}; do
        test="test_c${c}_s${s}.in"
        ./${generator} ${s} ${c} > generated/${test}
        echo "Generated $test"
    done
done

echo "Cleaning..."
make clean > /dev/null