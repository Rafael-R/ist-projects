% Numero: 89532  -  Nome: Rafael Rodrigues

%-------------------------------------------------------------------------------
%                                   Projeto
%                           Logica para Programacao
%-------------------------------------------------------------------------------

:- [codigo_comum].
:- [puzzles_publicos].

%-------------------------------------------------------------------------------

% Predicados auxiliares

zero(Num) :- not(var(Num)), Num == 0.

um(Num) :- not(var(Num)), Num == 1.

conta_elementos(L, Elem, Cont) :-
    include(Elem, L, L_Elems),
    length(L_Elems, Cont).


soma_lista([], 0).
soma_lista([H|T], Soma) :-
   soma_lista(T, Resto),
   Soma is H + Resto.


soma_triplo([X, Y, Z], Soma) :-
    exclude(var, [X, Y, Z], L),
    soma_lista(L, Soma).


preenche([], _, []).
preenche([H|T], Num, [Num|T2]) :- var(H), preenche(T, Num, T2).
preenche([H|T], Num, [H|T2]) :- preenche(T, Num, T2).


combinacao(0, _, []).

combinacao(N, L, [E|C_L_E]) :-
    N > 0,
    append(_, [E|L_apos_E], L),
    N_1 is N - 1,
    combinacao(N_1, L_apos_E, C_L_E).



% Predicados para inicializacao de puzzles

aplica_R1_triplo([X, Y, Z], N_Triplo) :-
    conta_elementos([X, Y, Z], var, Num_Vars),
    soma_triplo([X, Y, Z], Soma),
    (Num_Vars == 0, Soma > 0, Soma < 3, N_Triplo = [X, Y, Z]
        ;
     Num_Vars == 1, Soma == 2, preenche([X, Y, Z], 0, N_Triplo)
        ;
     Num_Vars == 1, Soma == 0, preenche([X, Y, Z], 1, N_Triplo)
        ;
     Num_Vars == 1, Soma == 1, N_Triplo = [X, Y, Z]
        ;
     member(Num_Vars, [2 ,3]), N_Triplo = [X, Y, Z]).


aplica_R1_fila_aux([X, Y, Z], N_Fila) :- aplica_R1_triplo([X, Y, Z], N_Fila).

aplica_R1_fila_aux([X, Y, Z | R], N_Fila) :-
    aplica_R1_triplo([X, Y, Z], [NX, NY, NZ]),
    aplica_R1_fila_aux([NY, NZ | R], N_Fila_Temp),
    N_Fila = [NX | N_Fila_Temp].


aplica_R1_fila(Fila, N_Fila) :-
    aplica_R1_fila_aux(Fila, N_Fila_Temp),
    (Fila == N_Fila_Temp, N_Fila = N_Fila_Temp
        ;
     aplica_R1_fila(N_Fila_Temp, N_Fila)).


aplica_R2_fila(Fila, N_Fila) :-
    length(Fila, Dim),
    N is Dim / 2,
    conta_elementos(Fila, zero, Num_Zeros),
    conta_elementos(Fila, um, Num_Uns),
    N >= Num_Zeros,
    N >= Num_Uns,
    (Num_Zeros == N, preenche(Fila, 1, N_Fila)
        ;
     Num_Uns == N, preenche(Fila, 0, N_Fila)
        ;
     N_Fila = Fila).


aplica_R1_R2_fila(Fila, N_Fila) :-
    aplica_R1_fila(Fila, N_Fila1), !,
    aplica_R2_fila(N_Fila1, N_Fila).


aplica_R1_R2_puzzle_aux([], []).

aplica_R1_R2_puzzle_aux([H|T], [NH|T2]) :-
    aplica_R1_R2_fila(H, NH),
    aplica_R1_R2_puzzle_aux(T, T2).


aplica_R1_R2_puzzle(Puz, N_Puz) :-
    aplica_R1_R2_puzzle_aux(Puz, N_Puz1),
    mat_transposta(N_Puz1, N_Puz1T),
    aplica_R1_R2_puzzle_aux(N_Puz1T, N_Puz2T),
    mat_transposta(N_Puz2T, N_Puz).


inicializa(Puz, N_Puz) :-
    aplica_R1_R2_puzzle(Puz, N_Puz),
    (Puz == N_Puz
        ;
     dif(Puz, N_Puz), inicializa(N_Puz, N_Puz)).



% Predicado para a verificacao da regra 3

verifica_R3_aux([H|T2]) :-
    /*conta_elementos(Comb1, var, Num_Vars1),
    conta_elementos(Comb2, var, Num_Vars2),*/
    write(H),
    write('-'),
    /*write(Comb2),
    write('     '),*/
    verifica_R3_aux(T2).


verifica_R3(Puz) :-
    findall(Comb, combinacao(2, Puz, Comb), L_Combs),
    verifica_R3_aux(L_Combs),
    mat_transposta(Puz, Puz1),
    findall(Comb, combinacao(2, Puz1, Comb), C_Combs),
    verifica_R3_aux(C_Combs).



% Predicado para a propagacao de mudancas

propaga_posicoes().



% Predicado resolve

resolve().



%-------------------------------------------------------------------------------
