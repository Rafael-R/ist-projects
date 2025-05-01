# 1º Projeto

## Descrição do Problema

O projecto é composto pelos dois problemas que se descrevem em baixo.

### Problema 1

Dada uma sequência $\vec{x} = \langle x_0, x_1, \ldots, x_k\rangle$ de inteiros, pretende calcular-se o tamanho da maior subsequência estritamente crescente de $\vec{x}$, bem como o número de subsequências estritamente crescentes de tamanho máximo. Por exemplo, a sequência $\vec{x} = \langle 1, 2, 6, 3, 7\rangle$ tem duas subsequências estritamente crescentes de tamanho máximo igual a 4: $\vec{s}_1 = \langle 1, 2, 6, 7\rangle$ e $\vec{s}_2 = \langle 1, 2, 3, 7\rangle$.

### Problema 2

Dadas duas sequências $\vec{x} = \langle x_0, x_1, \ldots, x_k\rangle$ e $\vec{y} = \langle y_0, y_1, \ldots, y_l\rangle$ de inteiros, pretende calcular-se apenas o tamanho da maior subsequência __comum__ estritamente crescente entre $\vec{x}$ e $\vec{y}$. Por exemplo, as sequências $\vec{x} = \langle 1, 2, 6, 3, 7\rangle$ e $\vec{y} = \langle1, 2, 4, 7, 3\rangle$ têm duas subsequências comuns estritamente crescentes de tamanho máximo igual a 3: $\vec{s}_1 = \langle1, 2, 3\rangle$ e $\vec{s}_2 = \langle1, 2, 7\rangle$.

## Input

O ficheiro de entrada contém a informação relativa ao problema a resolver e às sequências de inteiros correspondentes, e é definido da seguinte forma:

- uma linha contendo um inteiro que indica o problema a resolver: o inteiro $n$ corresponde ao problema $n$;
- $n$ linhas contendo cada uma delas uma sequência de inteiros separados por um único
espaço em branco e não contendo qualquer outro caracter, a não ser o fim de linha.

## Output

Para o Problema 1, o programa deverá escrever no output dois inteiros $t$ e $c$ separados por um espaço, onde $t$ corresponde ao tamanho da maior subsequência que respeita as restrições do problema e $c$ corresponde ao número de subsequências de tamanho máximo.

Para o Problema 2, o programa apenas deverá escrever no output um inteiro $t$ correspondente ao tamanho da maior subsequência que respeita as restrições do problema.

## Exemplo

### Input 1

```
1
1 2 6 3 7
```

### Output 1

```
4 2
```

### Input 2

```
2
1 2 6 3 7
1 2 4 7 3
```

### Output 2

```
3
```

## Implementação

A implementação do projeto deverá ser feita preferencialmente usando as linguagens de programação C ou C++. Submissões nas linguagens Java/Python também serão aceites, embora
fortemente desaconselhadas. Alunos que o escolham fazer devem estar cientes de que submissões em Java/Python podem não passar todos os testes mesmo implementando o algoritmo
correto.

O tempo necessário para implementar este projeto é inferior a 15 horas.

### Parâmetros de compilação

```shell
C++: g++ -std=c++11 -O3 -Wall file.cpp -lm
C: gcc -O3 -ansi -Wall file.c -lm
Javac: javac File.java
Java: java -Xss32m -Xmx256m -classpath . File
Python: python3 file.py
```
