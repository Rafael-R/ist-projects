#!/bin/bash
# Shell Script
# Projeto de Análise e Síntese de Algoritmos, 2023/2024

amounts=(10000 20000 30000 40000 50000 60000 70000 80000 90000 100000)
Tcmin=10
Tcmax=50
Tlmax=50
Pok=20
seed=1

for amount in ${amounts[@]}; do

    T=$amount
    P=$(($amount / 5))
    ./tests/generated/generator $T $P $Tcmin $Tcmax $Tlmax $Pok $seed \
    > tests/generated/gen_${T}_${P}.in 

done