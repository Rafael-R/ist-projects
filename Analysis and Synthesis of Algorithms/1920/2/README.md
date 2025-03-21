# 2º Projecto

## Descrição do Problema

Por causa do COVID-19, o Eng. António Caracol foi encarregue de projetar um sistema que
permita aos cidadãos de Manhattan deslocar-se a um supermercado sem se cruzarem com nenhum outro cidadão. Estão disponíveis no mapa da cidade, as localizações dos supermercados,
que se situam todos em esquinas e as moradas dos cidadãos que, para este efeito, se situam
também nas esquinas.
Tratando-se de Manhattan, as ruas têm um arranjo em quadriculado absolutamente regular,
e considera-se que em todas as ruas se circula em ambos os sentidos. As avenidas estão numeradas de 1 a M e são na direção NORTE-SUL, enquanto que as ruas estão numeradas de 1 a N
e são na direção ESTE-OESTE. Os cruzamentos são definidos por um par de números, sendo
que o par (A,B) corresponde ao cruzamento da Avenida A com a rua B.
Dados um conjunto de supermercados abertos e de cidadãos que querem fazer compras
a uma dada hora, o sistema deverá determinar qual o número máximo de cidadãos que pode
deslocar-se a um supermercado, sem correr o risco de se encontrar com outro cidadão, numa
rua, avenida ou cruzamento, inicial, intermédio ou final do seu percurso.
Podem existir dois supermercados no mesmo cruzamento, mas apenas um deles poderá ser
usado numa solução, para evitar contactos nesse local. Dois ou mais cidadãos podem morar no
mesmo cruzamento, mas apenas um deles poderá sair à rua de cada vez, os que ficam em casa
não levantam problemas de contágio. Da mesma forma, se um ou mais cidadãos morarem num
cruzamento mas não sairem à rua, o cruzamento pode ser usado por outro cidadão para passar
ou aceder a um supermercado, nesse ou noutro cruzamento.

## Input

O ficheiro de entrada contém informação sobre o número de ruas e avenidas, bem como sobre
o número e morada de cidadãos e supermercados. O input é definido da seguinte forma:

- Uma linha com o número de avenidas e o número de ruas, M e N;
- Uma linha com o número de supermercados e o número de cidadãos, S e C;
- S linhas, cada uma com a localização de cada supermercado, definida pelo número da
avenida e pelo número da rua do respetivo cruzamento;
- C linhas, cada uma com a morada de cada cidadão, definida pelo número da avenida e
pelo número da rua do respetivo cruzamento.

## Output

O output do programa deverá indicar um inteiro representando o número máximo de cidadãos
que podem deslocar-se aos supermercados sem se encontrarem.

## Exemplos

### input 1

5 5  
7 7  
1 1  
1 3  
2 4  
1 2  
1 4  
3 4  
3 5  
2 1  
2 3  
2 2  
3 2  
2 5  
4 4  
5 3  

### output 1

6

### input 2

3 4  
3 3  
1 4  
2 4  
3 3  
1 1  
1 2  
2 2  

### output 2

3
