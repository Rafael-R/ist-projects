#!/bin/bash
# Shell Script
# Projeto de Sistemas Operativos 2019/2020

if [ $# != 1 ] ; then
    echo "Usage: $0 <tests_dir>"
    echo "Example: $0 tests"
    exit 1
fi

testdir="$1"

export CLASSPATH=po-uuilib/po-uuilib.jar:project/m19-core/m19-core.jar:project/m19-app/m19-app.jar

for test in $testdir/*.out; do

    test="${test%%.*}"
    test_import="$test.import"
    test_in="$test.in"
    test_outhyp="$test.outhyp"
    test_out="$test.out"

    if [[ -f "$test_import" ]]; then
        java -Dimport=$test_import -Din=$test_in -Dout=$test_outhyp m19.app.App
    else 
        java -Din=$test_in -Dout=$test_outhyp m19.app.App
    fi

    DIFF=$(diff -b $test_out $test_outhyp) 
    if [ "$DIFF" != "" ]; then
        echo "The output of test $test differs from expected"
    fi

done

rm works

unset CLASSPATH
