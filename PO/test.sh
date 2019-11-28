#!/bin/bash
# Shell Script
# Projeto de Programação com Objetos 2019/2020

classpath="po-uuilib/po-uuilib.jar:project/m19-core/m19-core.jar:project/m19-app/m19-app.jar"

if [ $# != 1 ] ; then
    echo "Usage: $0 <tests_dir>"
    echo "Example: $0 tests"
    exit 1
fi

testdir="$1"
RED='\033[0;31m'
GREEN='\033[1;32m'
NC='\033[0m'

export CLASSPATH=$classpath

# make

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
        echo -e "${RED}The output of test ${test##*/} differs from expected.${NC}"
    else
        echo -e "${GREEN}The output of test [${test##*/}] matches expected.${NC}"
    fi

done

# make clean

rm works

unset CLASSPATH
