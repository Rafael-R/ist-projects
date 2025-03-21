# 1º Projeto

## Descrição do Problema

O Professor João Caracol desenvolveu um algoritmo para prever as notas do projeto dos alunos da disciplina ASA tendo por base a nota que obtiveram no primeiro teste. Assim, o Professor João Caracol associou a cada aluno uma dada estimativa de nota. Acontece que o Professor João Caracol não considerou o facto de que os alunos partilham frequentemente soluções entre si. Alertado para esse problema por um colega mais experiente, o Professor João Caracol quer agora recalcular as suas estimativas associando a cada aluno uma nova estimativa que tem em conta relações de amizade entre alunos. A previsão de nota para um aluno A corresponderá agora ao máximo das previsões originais associadas a alunos aos quais A tem acesso transitivamente via relações de amizade. Ou seja, dado que os alunos partilham tanto as suas soluções como as soluções dos amigos, um aluno A terá acesso às soluções dos amigos, dos amigos dos amigos, dos amigos dos amigos dos amigos e assim sucessivamente. Note que as relações de amizade não são necessariamente simétricas.

## Input

O ficheiro de entrada contém a informação sobre as estimativas de nota dos alunos e sobre a relações de amizade. O input é definido da seguinte forma:

- Uma linha com o número de alunos inscritos na disciplina, N (N ≥ 2), e o número de relações de amizade entre alunos, M (M ≥ 1), separados por uma vírgula; 
- Uma lista de N linhas, em que cada linha i contém um inteiro ni correspondente à estimativa de nota original do aluno cujo número coincide com o índice da linha i (i ∈ [1...N]);
- Uma lista de M linhas, em que cada linha contém dois inteiros u e v (separados por um espaço em branco) indicando que o aluno u tem como amigo o aluno v (o que não significa que v tenha u como amigo).

## Output

O programa deverá escrever no output uma lista de N linhas, em que cada linha contém a estimativa de nota recalculada do aluno cujo número coincide com o índice da linha.

## Exemplo

### Input

9,12  
11  
12  
13  
14  
15  
16  
17  
18  
19  
1 2  
1 3  
2 4  
2 5  
3 4  
4 1  
4 8  
5 6  
6 7  
7 5  
8 9  
9 8  

### Output

19  
19  
19  
19  
17  
17  
17  
19  
19  
