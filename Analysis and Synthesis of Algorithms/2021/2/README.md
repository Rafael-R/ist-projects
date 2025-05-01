# 2º Projeto

## Descrição do Problema

Em computação distribuída, o problema de atribuição de um conjunto de processos a um con-
junto de processadores é um problema complexo, mesmo quando considerados apenas dois
processadores, devido a overheads computacionais, como por exemplo: custos de execução de
cada processo em cada processador (este pode ter diferentes capacidades - velocidade, cache, etc); e custos de comunicação entre os vários processos.
Considere que pretende correr um programa $P$ = {$p_1, . . . , p_n$}, constituído por um conjunto de n processos. Considere ainda que é conhecido o custo $X_i$ de correr cada processo $p_i$ no processador $X$, e o custo $Y_i$ de correr cada processo $p_i$ no processador $Y$.

Considere adicionalmente o custo de comunicação $c_{ij}$ entre dois processos $p_i$ e $p_j$, sempre que $p_i \ne p_j$. Assuma que não há custo de comunicação $(c_{ij} = 0)$ entre processos que corram num mesmo processador.

Seja $P_X \subseteq P$ o conjunto de processos que correm no processador $X$, e $P_Y \subseteq P$ o conjunto de processos que correm no processador $Y$, tal que $P_X \cap P_Y = 0$, proponha e implemente um algoritmo eficiente que minimize o custo total de execução do programa $P$, sem testar todas as combinações de atribuições possíveis.

$$
\begin{aligned}
min\lbrace \sum_{i\in P_X}X_i + \sum_{i\in P_Y}Y_i + \sum_{(i,j)\in P_X\times P_Y}c_{ij} \rbrace
\end{aligned}
$$

Considere que a soma custos de execução $(\sum_{i\in P_X}X_i + \sum_{i\in P_Y}Y_i) \in O(n)$.

### Exemplo

Custos de execução de cada processo $p_i$

| $i$ | 1 | 2  | 3  | 4 |
|----|---|----|----|---|
| $X_i$ | 6 | 5  | 10 | 4 |
| $_Yi$ | 4 | 10 | 3  | 8 |

Custos de comunicação $c_{ij}$ entre dois processos $(p_i, p_j)$

| $c_{ij}$ | 1 | 2 | 3 | 4 |
|----------|---|---|---|---|
| 1        | 0 | 5 | 0 | 0 |
| 2        | 5 | 0 | 6 | 2 |
| 3        | 0 | 6 | 0 | 1 |
| 4        | 0 | 2 | 1 | 0 |

### Solução de custo mínimo

$A_X = \lbrace p_4 \rbrace$

$A_Y = \lbrace p_1, p_2, p_3 \rbrace$

$$
\begin{aligned}
custo & = \sum_{i\in A_X}X_i + \sum_{i\in A_Y}Y_i + \sum_{(i,j)\in A_X\times A_Y}c_{ij} \\
    & = X_4 +Y_1 +Y_2 +Y_3 + c_{24} + c_{34}\\
    & = 4 + 4 + 10 + 3 + 2 + 1 \\
    & = 24
\end{aligned}
$$

## Input

O ficheiro de entrada contém informação sobre o número de processos, bem como a informação
relativa ao custos de execução e de comunicação.

O input é definido da seguinte forma:

- Uma linha contendo dois inteiros: o número n de processos $(n ≥ 2)$; e o número $k$ de
entradas diferentes de $0$ na matriz de custos de comunicação $(k ≥ 0)$, apenas na diagonal superior visto que a matriz é simétrica;  
- $n$ linhas com dois inteiros com os custos de execução de $p_i$ no processador $X$ e $Y$ , respetivamente;
- $k$ linhas contendo 3 inteiros: o identificador $i(≤ n)$ do processo $p_i$; o identificador $j(≤ n)$ do processo $p_j$; e o custo de comunicação $c_{ij}$. Dado que a matriz é simétrica, $c_{ij} = c_{ji}$.

## Output

O output da sua solução deverá indicar apenas um inteiro representando o valor do custo mínimo, respeitando as restrições acima.

## Exemplo

### Input

```
4 4
6 4
5 10
10 3
4 8
1 2 5
2 3 6
2 4 2
3 4 1
```

### Output

```
24
```

## Implementação

A implementação do projeto deverá ser feita preferencialmente usando as linguagens de programação C ou C++. Submissões nas linguagens Java/Python também serão aceites, embora fortemente desaconselhadas. Alunos que o escolham fazer devem estar cientes de que submissões em Java/Python podem não passar todos os testes mesmo implementando o algoritmo correto, devido a aspetos de implementação e overhead
s da VM/interpretador.
O tempo necessário para implementar este projeto é inferior a 15 horas.

### Parâmetros de compilação

```shell
C++: g++ -std=c++11 -O3 -Wall file.cpp -lm
C: gcc -O3 -ansi -Wall file.c -lm
Javac: javac File.java
Java: java -Xss32m -Xmx256m -classpath . File
Python: python3 file.py
```
