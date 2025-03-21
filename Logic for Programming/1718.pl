% Numero: 89532  -  Nome: Rafael Rodrigues

/* -----------------------------------------------------------------------------

                                Primeiro Projeto
                            Logica para Programacao

----------------------------------------------------------------------------- */

:- include('exemplos_puzzles').

/* ------------------------- PREDICADOS AUXILIARES ---------------------------*/

propaga(Puz, Pos, Posicoes) :-
    /*
     * Predicado: propaga
     * ------------
     * Input: Recebe um puzzle (Puz) e uma posicao (Pos).
     * Output: Devolve a lista Posicoes que contem as posicoes que devem
     *         ser preenchidas para que essa posicao possa ser preenchida.
     */
    obtem_termometros(Puz, Termometros),
    procura_termometro(Termometros, Pos, Temporario),
    reverse(Temporario, Invertida),
    remove_posicoes(Pos, Invertida, Posicoes).

/* -------------------------------------------------------------------------- */

nao_altera_linhas_anteriores(Posicoes, L, Ja_Preenchidas) :-
    /*
     * Predicado: nao_altera_linhas_anteriores
     * ------------
     * Input: Recebe uma lista de posicoes (Posicoes), que representam uma
     *        possibilidade de preenchimento para uma dada linha (L), todas as
     *        posicoes desta lista pertencendo a linhas anteriores a L,
     *        pertencem a lista de posicoes ja preenchidas (Ja_Preenchidas).
     * Output: Devolve true caso as Posicoes nao preencham nenhuma posicao
     *         vazia nas linhas anteriores a L.
     *         Devolve false em caso contrario.
     */
    sort(Posicoes, Sorted),
    reverse(Sorted, Inverted),
    filtra_posicoes(Inverted, L, Filtrado), !,
    subset(Filtrado, Ja_Preenchidas).

/* -------------------------------------------------------------------------- */

verifica_parcial(Puz, Ja_Preenchidas, Dim, Poss) :-
    /*
     * Predicado: nao_altera_linhas_anteriores
     * ------------
     * Input: Recebe um puzzle (Puz), uma lista com as posicoes ja preenchidas
     *        anteriormente (Ja_Preenchidas), a dimensao do puzzle (Dim) e
     *        uma lista de posicoes que representao uma potencial possibilidade
     *        para preencher uma linha (Poss).
     * Output: Devolve true caso Poss nao viole os totais das colunas, isto é,
     *         se Poss for escolhida, nenhuma coluna fica com um numero de
     *         posições preenchidas superior ao seu total.
     *         Devolve false em caso contrario.
     */
    obtem_total_colunas(Puz, Total_Colunas),
    union(Ja_Preenchidas, Poss, Posicoes),
    conta_colunas(Total_Colunas, Posicoes, 1, Dim).

/* --------------------- PREDICADO possibilidades_linha --------------------- */

possibilidades_linha(
    Puz, Posicoes_linha, Total, Ja_Preenchidas, Possibilidades_L) :-
    /*
     * Predicado: possibilidades_linha
     * ------------
     * Input: Recebe um puzzle (Puz), uma lista com as posicoes da linha em
     *        questao (Posicoes_linha), o numero total de posicoes a preencher
     *        na linha em questao (Total) e uma lista das posicoes ja
     *        preenchidas anteriormente (Ja_Preenchidas).
     * Output: Devolve a lista Possibilidades_L que contem as possibilidades
     *         para preencher a linha em questao.
     */
    findall(Poss, combinacao(Total, Posicoes_linha, Poss), Posss),
    obtem_propaga(Posss, Puz, Propagadas),
    obtem_total_linhas(Puz, Total_Linhas),
    length(Total_Linhas, Dim),
    obtem_linha(Posicoes_linha, Linha),
    filtra(Propagadas, Puz, Linha, Total_Linhas, Dim, Ja_Preenchidas, X),
    sort(X, Possibilidades_L).



obtem_propaga([], _, []).

obtem_propaga([H|T], Puz, Posss) :-
    obtem_propaga_aux(H, Puz, Poss),
    obtem_propaga(T, Puz, Posss1),
    union(Posss1, [Poss], Posss).

obtem_propaga_aux([], _, _).

obtem_propaga_aux([H|T], Puz, Poss) :-
    propaga(Puz, H, Posicoes),
    obtem_propaga_aux(T, Puz, Poss1),
    union(Poss1, Posicoes, Poss).


filtra([], _, _, _, _, _, []).

filtra([H|T], Puz, Linha, Total_Linhas, Dim, Ja_Preenchidas, NovaLista) :-
    conta_linhas(Total_Linhas, H, 1, Dim),
    verifica_parcial(Puz, Ja_Preenchidas, Dim, H),
    nao_altera_linhas_anteriores(H, Linha, Ja_Preenchidas),
    filtra_posicoes(Ja_Preenchidas, Linha, Preenchidas_Linha),
    subset(Preenchidas_Linha, H),
    sort(H, Possibilidade),
    filtra(T, Puz, Linha, Total_Linhas, Dim, Ja_Preenchidas, NovaLista1), !,
    union(NovaLista1, [Possibilidade], NovaLista).

filtra([_|T], Puz, Linha, Total_Linhas, Dim, Ja_Preenchidas, NovaLista) :-
    filtra(T, Puz, Linha, Total_Linhas, Dim, Ja_Preenchidas, NovaLista).


/* --------------------------- PREDICADO resolve ---------------------------- */

resolve(Puz, Solucao) :-
    /*
     * Predicado: resolve
     * ------------
     * Input: Recebe um puzzle (Puz).
     * Output: Devolve a lista Solucao que contem as posicoes a
     *         preencher no puzzle para obter uma solucao.
     */
    obtem_total_linhas(Puz, Total_Linhas),
    length(Total_Linhas, Num_Linhas),
    resolve_aux(Puz, Total_Linhas, Num_Linhas, 1, [], Solucao).



gera_posicoes(_, Total_Coluna, Contador, _) :-
    Contador > Total_Coluna.

gera_posicoes(Linha, Total_Coluna, Contador, Lista) :-
    Contador =< Total_Coluna,
    Posicao = (Linha, Contador),
    Contador1 is Contador + 1,
    gera_posicoes(Linha, Total_Coluna, Contador1, NovaLista),
    append(NovaLista, [Posicao], Lista).


resolve_aux(_, _, Num_Linhas, Contador, Ja_Preenchidas, Solucao) :-
    Contador > Num_Linhas,
    sort(Ja_Preenchidas, Solucao).

resolve_aux(Puz, Total_Linhas, Num_Linhas, Contador, Ja_Preenchidas, Solucao) :-
    Contador =< Num_Linhas,
    gera_posicoes(Contador, Num_Linhas, 1, Posicoes),
    sort(Posicoes, Posicoes_linha),
    nth1(Contador, Total_Linhas, Total),
    possibilidades_linha(Puz, Posicoes_linha, Total, Ja_Preenchidas, Possibilidades), !,
    member(Possibilidade, Possibilidades),
    union(Possibilidade, Ja_Preenchidas, NovaLista),
    Contador1 is Contador + 1,
    resolve_aux(Puz, Total_Linhas, Num_Linhas, Contador1, NovaLista, Solucao).


/* -------------------------------------------------------------------------- */

obtem_termometros([Termometros, _, _], Termometros).

obtem_total_linhas([_, Total_Linhas, _], Total_Linhas).

obtem_total_colunas([_, _, Total_Colunas], Total_Colunas).

obtem_linha([(L,_)|_], L).


procura_termometro([H|_], Posicao, NovaLista) :-
    member(Posicao, H),
    append([], H, NovaLista).

procura_termometro([_|T], Posicao, NovaLista) :-
    procura_termometro(T, Posicao, NovaLista).


remove_posicoes(Posicao, Lista, NovaLista) :-
    nth0(0, Lista, Posicao),
    sort(Lista, NovaLista).

remove_posicoes(Posicao, [_|T], NovaLista) :-
    remove_posicoes(Posicao, T, NovaLista).


filtra_posicoes([], _, []).

filtra_posicoes(Lista, Linha, NovaLista) :-
    obtem_linha(Lista, Index_Linha),
    Index_Linha < Linha,
    append([], Lista, NovaLista).

filtra_posicoes([_|T], Linha, NovaLista) :-
    filtra_posicoes(T, Linha, NovaLista).


conta_linhas([], _, _, _).

conta_linhas([H|T], Lista, Linha, Dim) :-
    Linha =< Dim,
    conta_linhas_aux(Lista, Linha, Num),
    Num =< H,
    Linha1 is Linha + 1,
    conta_linhas(T, Lista, Linha1, Dim).

conta_linhas_aux([], _, 0).

conta_linhas_aux([(X,_)|T], X, Contador) :-
    conta_linhas_aux(T, X, Contador1),
    Contador is Contador1 + 1.

conta_linhas_aux([(X,_)|T], Z, Contador) :-
    conta_linhas_aux(T, Z, Contador),
    X \= Z.


conta_colunas([], _, _, _).

conta_colunas([H|T], Lista, Coluna, Dim) :-
    Coluna =< Dim,
    conta_colunas_aux(Lista, Coluna, Num),
    Num =< H,
    Coluna1 is Coluna + 1,
    conta_colunas(T, Lista, Coluna1, Dim).

conta_colunas_aux([], _, 0).

conta_colunas_aux([(_,X)|T], X, Contador) :-
    conta_colunas_aux(T, X, Contador1),
    Contador is Contador1 + 1.

conta_colunas_aux([(_,X)|T], Z, Contador) :-
    conta_colunas_aux(T, Z, Contador),
    X \= Z.


combinacao(0, _, []).

combinacao(N, L, [E|C_L_E]) :-
    N > 0,
    append(_, [E|L_apos_E], L),
    N_1 is N - 1,
    combinacao(N_1, L_apos_E, C_L_E).

/* -------------------------------------------------------------------------- */
