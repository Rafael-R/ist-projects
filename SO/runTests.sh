#!/bin/bash
# Shell Script
# Projeto de Sistemas Operativos 2019/2020

if [ $# != 4 ] ; then
    echo "Usage: ${0} <inputdir> <outputdir> <maxthreads> <numbuckets>"
    echo "Example: ${0} tests_in tests_out 4 2"
    exit 1
fi

inputdir="${1}"
outputdir="${2}"
maxthreads="${3}"
numbuckets="${4}"

for test_in in ${inputdir}/*.txt; do

    test="${test_in##*/}"

    for numthreads in $(seq 1 ${maxthreads}); do

        echo "InputFile=${test} NumThreads=${numthreads}"
        test_out="${outputdir}/${test%.txt}-${numthreads}.txt"

        if [ ${numthreads} == 1 ]; then
            ./tecnicofs-nosync ${test_in} ${test_out} ${numthreads} 1 | tail -n 1
        else
            ./tecnicofs-mutex ${test_in} ${test_out} ${numthreads} ${numbuckets} | tail -n 1
        fi

    done
done