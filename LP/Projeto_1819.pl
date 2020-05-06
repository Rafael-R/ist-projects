% Numero: 89532  -  Nome: Rafael Rodrigues

/*##############################################################################
                                    Projeto
                            Logica para Programacao
##############################################################################*/

:- [codigo_comum].

%###############################################################################

% Predicados auxiliares

zero(Elem) :- not(var(Elem)), Elem == 0.
    /* Predicado: zero/1
       ------------
       Input: Recebe um elemento (Elem).
       Output: Devolve true caso Elem seja igual a zero e false caso contrario.
    */


um(Elem) :- not(var(Elem)), Elem == 1.
    /* Predicado: um/1
       ------------
       Input: Recebe um elemento (Elem).
       Output: Devolve true caso Elem seja igual a um e false caso contrario.
    */


conta_elementos(L, Elem, N_Elems) :-
    /* Predicado: conta_elementos/3
       ------------
       Input: Recebe uma lista (L) e um tipo de elemento (Elem).
       Output: Devolve o numero de vezes que o elemento ocorre
               na lista recebida (N_Elems).
    */
    include(Elem, L, L_Elems),
    length(L_Elems, N_Elems).


soma_lista([], 0).
    /* Predicado: soma_lista/2
       ------------
       Input: Recebe uma lista.
       Output: Devolve o valor da soma dos elementos da lista fornecida.
    */
soma_lista([H|T], Soma) :-
   soma_lista(T, Resto),
   Soma is H + Resto.


soma_triplo([X, Y, Z], Soma) :-
    /* Predicado: soma_triplo/2
       ------------
       Input: Recebe uma lista de tres elementos.
       Output: Devolve o valor da soma dos elementos da lista fornecida.
    */
    exclude(var, [X, Y, Z], L),
    soma_lista(L, Soma).


preenche([], _, []).
    /* Predicado: preenche/3
       ------------
       Input: Recebe uma lista e um elemento (Elem).
       Output: Devolve a lista que resulta de substituir as
               variaveis pelo elemento fornecido.
    */
preenche([H|T], Elem, [Elem|T2]) :- var(H), preenche(T, Elem, T2).
preenche([H|T], Elem, [H|T2]) :- preenche(T, Elem, T2).


combinacao(0, _, []).
    /* Predicado: combinacao/3
       ------------
       Input: Recebe um numero (N) e uma lista (L).
       Output: Devolve as combinacoes, N a N, dos elementos de L.
    */
combinacao(N, L, [E|C_L_E]) :-
    N > 0,
    append(_, [E|L_apos_E], L),
    N_1 is N - 1,
    combinacao(N_1, L_apos_E, C_L_E).


diferente(Puz1, Puz2, (L, C)) :-
    /* Predicado: diferente/3
       ------------
       Input: Recebe dois puzzles (Puz1 e Puz2) e uma posicao ((L, C)).
       Output: Devolve true caso os elementos na mesma posicao dada, dos dois
               puzzles, seja diferente e falso caso contrario.
    */
    mat_ref(Puz1, (L, C), Cont1),
    mat_ref(Puz2, (L, C), Cont2),
    (var(Cont1), zero(Cont2)
        ;
     var(Cont1), um(Cont2)).


pos_var(Puz, (L, C)) :-
    /* Predicado: pos_var/2
       ------------
       Input: Recebe um puzzle (Puz) e uma posicao ((L, C)).
       Output: Devolve true caso o elemento na posicao dada seja uma
               variavel e falso caso contrario.
    */
    mat_ref(Puz, (L, C), Cont),
    var(Cont).

%###############################################################################

% Predicados para inicializacao de puzzles

aplica_R1_triplo([X, Y, Z], N_Triplo) :-
    /* Predicado: aplica_R1_triplo/2
       ------------
       Input: Recebe uma lista de tres elementos.
       Output: Devolve a lista (N_Triplo) resultante de aplicar a regra 1 a
               lista dada. Isto significa que se a lista tiver dois zeros/uns e
               uma variavel, esta deve ser preenchida com um/zero. Se a lista
               tiver tres zeros/uns, o predicado deve devolver false.
    */
    conta_elementos([X, Y, Z], var, Num_Vars),
    soma_triplo([X, Y, Z], Soma),
    (Num_Vars == 0, Soma > 0, Soma < 3,
        N_Triplo = [X, Y, Z], !;
     Num_Vars == 1, Soma == 2,
        preenche([X, Y, Z], 0, N_Triplo), !;
     Num_Vars == 1, Soma == 0,
        preenche([X, Y, Z], 1, N_Triplo), !;
     Num_Vars == 1, Soma == 1,
        N_Triplo = [X, Y, Z], !;
     member(Num_Vars, [2 ,3]),
        N_Triplo = [X, Y, Z]), !.

%-------------------------------------------------------------------------------

aplica_R1_fila_aux([X, Y, Z], N_Fila) :- aplica_R1_triplo([X, Y, Z], N_Fila).
    /* Predicado: aplica_R1_fila_aux/2
       ------------
       Input: Recebe uma lista, que representa uma linha/coluna.
       Output: Devolve a lista (N_Fila) resultante de aplicar a regra 1 a lista,
               uma so vez. Isto significa que a lista deve ser percorrida uma
               vez, do inicio para o fim, aplicando o predicado aplica_R1_triplo
               a cada sub-fila de 3 elementos. Se a lista tiver tres zeros/uns
               seguidos, o predicado devolve false.
    */
aplica_R1_fila_aux([X, Y, Z | R], N_Fila) :-
    aplica_R1_triplo([X, Y, Z], [NX, NY, NZ]),
    aplica_R1_fila_aux([NY, NZ | R], N_Fila_Temp), !,
    N_Fila = [NX | N_Fila_Temp].

%-------------------------------------------------------------------------------

aplica_R1_fila(Fila, N_Fila) :-
    /* Predicado: aplica_R1_fila/2
       ------------
       Input: Recebe uma lista (Fila), que representa uma linha/coluna.
       Output: Devolve a lista (N_Fila) resultante de aplicar a regra 1 a lista.
               Isto significa que todas as posicoes vazias da lista que possam
               ser preenchidas para respeitar a regra 1 se encontram preenchidas
               em N_Fila. Se a lista tiver tres zeros/uns seguidos, o
               predicado devolve false.
    */
    aplica_R1_fila_aux(Fila, N_Fila_Temp),
    (Fila == N_Fila_Temp,
        N_Fila = N_Fila_Temp, !;
     aplica_R1_fila(N_Fila_Temp, N_Fila)).

%-------------------------------------------------------------------------------

aplica_R2_fila(Fila, N_Fila) :-
    /* Predicado: aplica_R2_fila/2
       ------------
       Input: Recebe uma lista (Fila), que representa uma linha/coluna.
       Output: Devolve a lista (N_Fila) resultante de aplicar a regra 2 a lista.
               Seja N metade do numero de elementos da lista. Aplicar a regra 2
               significa que se a lista ja contiver N zeros/uns, todas as
               posicoes vazias da lista devem ser preenchidas com uns/zeros.
               Se o numero de zeros/uns ultrapassa N, o predicado devolve false.
    */
    length(Fila, Dim),
    N is Dim / 2,
    conta_elementos(Fila, zero, Num_Zeros),
    conta_elementos(Fila, um, Num_Uns),
    N >= Num_Zeros, N >= Num_Uns,
    (Num_Zeros == N,
        preenche(Fila, 1, N_Fila), !;
     Num_Uns == N,
        preenche(Fila, 0, N_Fila), !;
     N_Fila = Fila).

%-------------------------------------------------------------------------------

aplica_R1_R2_fila(Fila, N_Fila) :-
    /* Predicado: aplica_R2_fila/2
       ------------
       Input: Recebe uma lista (Fila), que representa uma linha/coluna.
       Output: Devolve a lista (N_Fila) resultante de aplicar as regras 1 e 2 a
               lista, por esta ordem.
    */
    aplica_R1_fila(Fila, N_Fila1), !,
    aplica_R2_fila(N_Fila1, N_Fila).

%-------------------------------------------------------------------------------

aplica_R1_R2_puzzle_aux([], []).
    /* Predicado: aplica_R1_R2_puzzle_aux/2
    ------------
       Input: Recebe uma lista, que representa uma linha/coluna.
       Output: Devolve uma lista resultante de aplicar as regras 1 e 2 a lista.
    */
aplica_R1_R2_puzzle_aux([H|T], [NH|T2]) :-
    aplica_R1_R2_fila(H, NH),
    aplica_R1_R2_puzzle_aux(T, T2).


aplica_R1_R2_puzzle(Puz, N_Puz) :-
    /* Predicado: aplica_R1_R2_puzzle/2
       ------------
       Input: Recebe um puzzle (Puz).
       Output: Devolve o puzzle (N_Puz) resultante de aplicar as regras 1 e 2,
               as linhas e as colunas de Puz, por esta ordem.
    */
    aplica_R1_R2_puzzle_aux(Puz, N_Puz1),
    mat_transposta(N_Puz1, N_Puz1T),
    aplica_R1_R2_puzzle_aux(N_Puz1T, N_Puz2T),
    mat_transposta(N_Puz2T, N_Puz).

%-------------------------------------------------------------------------------

inicializa(Puz, N_Puz) :-
    /* Predicado: inicializa/2
       ------------
       Input: Recebe um puzzle (Puz).
       Output: Devolve o puzzle (N_Puz) resultante de inicializar o puzzle Puz.
               Inicializar um puzzle consiste em aplicar as regras 1 e 2, a
               cada linha e coluna, ate nao serem preenchidas novas posicoes.
    */
    aplica_R1_R2_puzzle(Puz, N_PuzR12),
    (dif(Puz, N_PuzR12),
        inicializa(N_PuzR12, N_Puz), !;
     not(dif(Puz, N_PuzR12)),
        N_Puz = N_PuzR12).

%###############################################################################

% Predicado para a verificacao da regra 3

verifica_R3_aux([]).
    /* Predicado: verifica_R3_aux/1
       ------------
       Input: Recebe uma lista com todas as combinacoes possiveis de
              linhas/colunas, duas a duas, de um puzzle.
       Output: Devolve false caso alguma das combinacoes tenha duas
               linhas/colunas iguais, ou true em caso contrario.
    */
verifica_R3_aux([[Comb1, Comb2]|T]) :-
    conta_elementos(Comb1, var, Num_Vars1),
    conta_elementos(Comb2, var, Num_Vars2),
    (Num_Vars1 == 0, Num_Vars2 == 0, not(Comb1 = Comb2),
        verifica_R3_aux(T);
    Num_Vars1 > 0 ; Num_Vars2 > 0,
        verifica_R3_aux(T)).


verifica_R3(Puz) :-
    /* Predicado: verifica_R3/1
       ------------
       Input: Recebe um puzzle (Puz).
       Output: Devolve true caso o puzzle respeite a regra 3 ou
               false em caso contrario.
    */
    findall(Comb, combinacao(2, Puz, Comb), L_Combs),
    verifica_R3_aux(L_Combs), !,
    mat_transposta(Puz, Puz1),
    findall(Comb, combinacao(2, Puz1, Comb), C_Combs),
    verifica_R3_aux(C_Combs), !,
    mat_transposta(Puz1, Puz).

%###############################################################################

% Predicado para a propagacao de mudancas

propaga_posicoes([], N_Puz, N_Puz).
    /* Predicado: propaga_posicoes/3
    ------------
       Input: Recebe uma lista de posicoes (Posicoes) e um puzzle (Puz).
       Output: Devolve um puzzle (N_Puz) resultande de propagar,
               recursivamente, as mudancas das posicoes.
    */
propaga_posicoes([(L, C)|T], Puz, N_Puz) :-
    nth1(L, Puz, Linha),
    aplica_R1_R2_fila(Linha, N_Linha),
    mat_elementos_coluna(Puz, C, Coluna),
    aplica_R1_R2_fila(Coluna, N_Coluna),
    mat_muda_linha(Puz, L, N_Linha, N_PuzL),
    mat_muda_coluna(N_PuzL, C, N_Coluna, N_PuzLC),
    findall((X, Y), (diferente(Puz, N_PuzLC, (X, Y))), N_PosT),
    append(N_PosT, T, N_Pos),
    propaga_posicoes(N_Pos, N_PuzLC, N_Puz).

%###############################################################################

% Predicado resolve

resolve_aux(Puz, Sol) :-
    findall((X, Y), (pos_var(Puz, (X, Y))), Pos_Vars),
    length(Pos_Vars, Num_Vars),
    (Num_Vars == 0,
       Sol = Puz, !;
    Num_Vars > 0,
        random_between(1, Num_Vars, Index),
        nth1(Index, Pos_Vars, Var),
        (mat_muda_posicao(Puz, Var, 0, N_Puz0),
         propaga_posicoes([Var], N_Puz0, N_Puz00),
         verifica_R3(N_Puz00),
            resolve_aux(N_Puz00, Sol), !;
         mat_muda_posicao(Puz, Var, 1, N_Puz1),
         propaga_posicoes([Var], N_Puz1, N_Puz11),
         verifica_R3(N_Puz11),
            resolve_aux(N_Puz11, Sol), !)).


resolve(Puz, Sol) :-
    /* Predicado: resolve/2
       ------------
       Input: Recebe um puzzle (Puz).
       Output: Devolve um puzzle (Sol) que e uma solucao para o puzzle.
    */
    inicializa(Puz, N_PuzI),
    verifica_R3(N_PuzI), !,
    resolve_aux(N_PuzI, Sol).

%###############################################################################
