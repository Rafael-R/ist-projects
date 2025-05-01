# 1º Projeto

## Descrição do Problema

O Professor João Caracol tem um fascínio por dominós e passa tardes a fazer longas sequências de dominós. Depois chama os vizinhos para todos observarem os dominós a cairem em sequência quando o professor deita alguns dos dominós abaixo.

Como passatempo decidiu desenvolver um algoritmo para conseguir determinar qual o número mínimo de dominós que tem de deitar abaixo com a mão, de forma a garantir que todos os dominós caiem. Adicionalmente, ficou curioso em saber qual o número de peças pertencente à  
maior sequência de dominós a cair, de cada vez que ele deita abaixo com a mão um dominó.

## Input

O ficheiro de entrada contém a informação sobre as sequências de dominós, e é definido da
seguinte forma:

- Uma linha contendo dois inteiros: o número $n$ de peças de dominó $(n ≥ 2)$, e o número
de dependências $m$ a indicar $(m ≥ 0)$;
- Uma lista em que cada linha $i$ contém dois inteiros $x$ e $y$ indicando que se o dominó $x$ cair, então o dominó $y$ também cai.

Quaisquer inteiros numa linha estão separados por no máximo um espaço em branco, não
contendo qualquer outro caracter, a não ser o fim de linha.

Assuma que os grafos de input são um DAG (grafo dirigido acíclico).

## Output

O programa deverá escrever no output dois inteiros $k$ e $l$ separados por um espaço, onde $k$ corresponde ao número mínimo de intervenções necessárias para garantir que todos os dominós caem e $l$ corresponde ao tamanho da maior sequência de dominós a cair.

## Exemplo

### Input

```
7 8
3 4
3 2
4 6
4 5
6 2
6 5
5 7
2 7
```

### Output

```
2 5
```

## Implementação

A implementação do projeto deverá ser feita preferencialmente usando as linguagens de programação C ou C++. Submissões nas linguagens Java/Python também serão aceites, embora fortemente desaconselhadas. Alunos que o escolham fazer devem estar cientes de que submissões em Java/Python podem não passar todos os testes mesmo implementando o algoritmo correto. Mais se observa que soluções recursivas podem esgotar o limite da pilha quando executadas sobre os testes de maior tamanho, pelo que se recomenda a implementação de um algoritmo iterativo.

O tempo necessário para implementar este projeto é inferior a 15 horas.

### Parâmetros de compilação

```shell
C++: g++ -std=c++11 -O3 -Wall file.cpp -lm
C: gcc -O3 -ansi -Wall file.c -lm
Javac: javac File.java
Java: java -Xss32m -Xmx256m -classpath . File
Python: python3 file.py
```
