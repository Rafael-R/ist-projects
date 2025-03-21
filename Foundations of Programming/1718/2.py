from parte1 import e_palavra
from itertools import permutations
from string import ascii_uppercase

########################################################################################################################
# Nome: Rafael Sousa Rodrigues   Numero: 89532
########################################################################################################################

# ----------------------------------------------------------------------------------------------------------------------
# Tipos Abstratos de Dados
# ----------------------------------------------------------------------------------------------------------------------

# TAD palavra_potencial


alfabeto = list(ascii_uppercase)


def cria_palavra_potencial(cad_caracteres, tuplo_de_letras):
    """
    cria_palavra_potencial: cad. caracteres x tuplo de letras -> palavra_potencial
    Esta funcao recebe como argumentos uma cadeia de caracteres e um conjunto de
    letras (potencialmente com repeticoes) e devolve uma palavra_potencial.
    """
    def aux(cad_caracteres, tuplo_de_letras):
        cadeia = list(cad_caracteres)
        tuplo = list(tuplo_de_letras)
        contador = 0
        for i in range(len(cadeia)):
            for j in range(len(tuplo)):
                if cadeia[i] not in tuplo:
                    raise ValueError('cria_palavra_potencial:a palavra nao e valida.')
                elif cadeia[i] == tuplo[j]:                                # A funcao substitui por tracos, um a um,
                    cadeia[i] = '-'                                        # os elementos iguais na cadeia e no tuplo,
                    tuplo[j] = '-'                                         # de modo a nao haver repeticao de elemntos.
                    contador += 1
                    if contador == len(cadeia):
                        return cad_caracteres
                elif cadeia == (['-'] * len(cadeia)) and contador != len(cadeia):
                    raise ValueError('cria_palavra_potencial:a palavra nao e valida.')

    if isinstance(cad_caracteres, str) and isinstance(tuplo_de_letras, tuple):
        if not e_palavra_potencial(cad_caracteres) or not e_conjunto_palavras(list(tuplo_de_letras)):
            raise ValueError('cria_palavra_potencial:argumentos invalidos.')
        elif cad_caracteres == '' and e_conjunto_palavras(list(tuplo_de_letras)):
            return cad_caracteres
        else:
            return aux(cad_caracteres, tuplo_de_letras)
    else:
        raise ValueError('cria_palavra_potencial:argumentos invalidos.')


def palavra_tamanho(palavra_potencial):
    """
    palavra_tamanho: palavra_potencial -> inteiro
    Este selector recebe como argumento um elemento do tipo palavra_potencial e
    devolve o numero de letras da palavra.
    """
    return len(palavra_potencial)


def e_palavra_potencial(universal):
    """
    e_palavra_potencial: universal -> lógico
    Este reconhecedor recebe um unico argumento e devolve True caso esse argumento
    seja do tipo palavra_potencial, e False em caso contrario.
    """
    if not isinstance(universal, str):
        return False
    else:
        if universal == '':
            return True
        elif universal[0] in alfabeto:
            return e_palavra_potencial(universal[1:])
        else:
            return False


def palavras_potenciais_iguais(palavra_potencial1, palavra_potencial2):
    """
    palavras_potenciais_iguais: palavra_potencial x palavra_potencial -> lógico
    Este teste recebe como argumentos dois elementos do tipo palavra_potencial e devolve
    True caso esses argumentos representem a mesma palavra, e False em caso contrario.
    """
    return palavra_potencial1 == palavra_potencial2


def palavra_potencial_menor(palavra_potencial1, palavra_potencial2):
    """
    palavra_potencial_menor: palavra_potencial x palavra_potencial -> lógico
    Este teste recebe como argumentos dois elementos do tipo palavra_potencial e devolve
    True caso o primeiro argumento represente uma palavra alfabeticamente anterior
    a palavra representada pelo segundo argumento, e False em caso contrario.
    """
    return palavra_potencial1 < palavra_potencial2


def palavra_potencial_para_cadeia(palavra_potencial):
    """
    palavra_potencial_para_cadeia: palavra_potencial -> cad. caracteres
    Esta funcao recebe como argumento um elemento do tipo palavra_potencial e devolve
    uma cadeia de caracteres que a represente.
    """
    return str(palavra_potencial)


# TAD conjunto_palavras


def cria_conjunto_palavras():
    """
    cria_conjunto_palavras: -> conjunto_palavras
    Esta funcao corresponde ao construtor do tipo conjunto_palavras.
    Nao recebe argumentos e devolve um conjunto de palavras vazio.
    """
    conjunto_palavras = []
    return conjunto_palavras


def numero_palavras(conjunto_palavras):
    """
    numero_palavras: conjunto_palavras -> inteiro
    Este seletor recebe como argumento um elemento do tipo conjunto_palavras e
    devolve um inteiro correspondente ao numero de palavras guardadas.
    """
    return len(conjunto_palavras)


def subconjunto_por_tamanho(conjunto_palavras, inteiro):
    """
    subconjunto_por_tamanho: conjunto_palavras x inteiro -> lista
    Este seletor recebe como argumentos um elemento do tipo conjunto_palavras e um
    inteiro n, e devolve uma lista com as palavras_potenciais de tamanho n contidas
    no conjunto de palavras.
    """
    subconjunto = []
    for i in range(numero_palavras(conjunto_palavras)):
        if palavra_tamanho(conjunto_palavras[i]) == inteiro:
            subconjunto += [conjunto_palavras[i]]
    return subconjunto


def acrescenta_palavra(conjunto_palavras, palavra_potencial):
    """
    acrescenta_palavra: conjunto_palavras x palavra_potencial ->
    Este modificador recebe como argumentos um elemento do tipo conjunto_palavras
    e uma palavra_potencial, e tem como efeito juntar a palavra ao conjunto de
    palavras, caso esta ainda nao pertenca ao conjunto.
    """
    if not e_conjunto_palavras(conjunto_palavras) or not e_palavra_potencial(palavra_potencial):
        raise ValueError('acrescenta_palavra:argumentos invalidos.')
    elif palavra_potencial not in conjunto_palavras:
        conjunto_palavras += [palavra_potencial]


def e_conjunto_palavras(universal):
    """
    e_conjunto_palavras: universal -> lógico
    Este reconhecedor recebe um unico argumento e devolve True caso esse
    argumento seja do tipo conjunto_palavras, e False em caso contrario.
    """
    if not isinstance(universal, list):
        return False
    else:
        if universal == []:
            return True
        elif e_palavra_potencial(universal[0]):
            return e_conjunto_palavras(universal[1:])
        else:
            return False


def conjuntos_palavras_iguais(conjunto_palavras1, conjunto_palavras2):
    """
    conjuntos_palavras_iguais: conjunto_palavras x conjunto_palavras -> lógico
    Este teste recebe como argumentos dois elementos do tipo conjunto_palavras e devolve
    True caso os dois argumentos contenham as mesmas palavras, e False caso contrario.
    """
    return sorted(conjunto_palavras1) == sorted(conjunto_palavras2)


def conjunto_palavras_para_cadeia(conjunto_palavras):
    """
    conjunto_palavras_para_cadeia: conjunto_palavras -> cad. caracteres
    Esta funcao recebe como argumento um elemento do tipo conjunto_palavras e devolve
    uma cadeia de caracteres que o represente. As palavras sao enumeradas por ordem
    crescente do seu tamanho, e para cada tamanho sao ordenadas alfabeticamente,
    """
    conjunto = sorted(conjunto_palavras, key=len)
    res = {}
    cadeia = ''
    if conjunto != []:
        for i in range(palavra_tamanho(conjunto[-1]) + 1):                          # i varia de 0 ao tamanho da maior
            lista = sorted(subconjunto_por_tamanho(conjunto_palavras, i))           # palavra do conjunto, gerando assim
            lista_str = ''                                                          # um subconjunto para todos os
            if lista != [] and numero_palavras(lista) == 1:                         # tamanhos possiveis, que sao depois
                res[i] = '[' + lista[0] + ']'                                       # adicionados a um dicionario.
            elif lista != [] and numero_palavras(lista) > 1:
                for j in range(numero_palavras(lista) - 1):
                    lista_str += palavra_potencial_para_cadeia(lista[j]) + ', '
                res[i] = '[' + lista_str + lista[-1] + ']'
            else:
                res[i] = ''
        j = 0
        while j < palavra_tamanho(conjunto[-1]):                                    # O dicionario e depois transformado
            if res[j] == '':                                                        # numa string.
                j += 1
            else:
                cadeia += str(j) + '->' + str(res[j]) + ';'
                j += 1
        final = palavra_tamanho(conjunto[-1])
        cadeia_final = '[' + cadeia + str(final) + '->' + str(res[final]) + ']'
        return cadeia_final
    elif conjunto == []:                                                              # Se o conjunto inicial for vazio,
        cadeia_final = '[]'                                                           # e devolvida uma string com uma
    return cadeia_final                                                               # lista vazia.


# TAD jogador


def cria_jogador(cad_caracteres):
    """
    cria_jogador: cad. caracteres -> jogador
    Esta funcao corresponde ao construtor do tipo jogador. Recebe como argumento uma
    cadeia de caracteres correspondente ao nome do jogador, e devolve um jogador.
    """
    if not isinstance(cad_caracteres, str):
        raise ValueError('cria_jogador:argumento invalido.')
    else:
        return {'JOGADOR': cad_caracteres, 'PONTOS': 0, 'VALIDAS': [], 'INVALIDAS': []}


def jogador_nome(jogador):
    """
    jogador_nome: jogador -> cad. caracteres
    Este selector recebe como argumento um elemento do tipo jogador,
    e devolve o nome do jogador
    """
    return jogador['JOGADOR']


def jogador_pontuacao(jogador):
    """
    jogador_pontuacao: jogador -> inteiro
    Este selector recebe como argumento um elemento do tipo jogador,
    e devolve a pontuacao obtida pelo jogador ate ao momento.
    """
    return jogador['PONTOS']


def jogador_palavras_validas(jogador):
    """
    jogador_palavras_validas: jogador -> conjunto_palavras
    Este selector recebe como argumento um elemento do tipo jogador, e devolve
    o conjunto de palavras validas propostas pelo jogador ate ao momento.
    """
    return jogador['VALIDAS']


def jogador_palavras_invalidas(jogador):
    """
    jogador_palavras_invalidas: jogador -> conjunto_palavras
    Este selector recebe como argumento um elemento do tipo jogador, e devolve o
    conjunto de palavras invalidas propostas pelo jogador ate ao momento.
    """
    return jogador['INVALIDAS']


def adiciona_palavra_valida(jogador, palavra_potencial):
    """
    adiciona_palavra_valida: jogador x palavra_potencial ->
    Este modificador recebe como argumentos um elemento do tipo jogador e uma
    palavra_potencial p, e tem como efeito adicionar a palavra p ao conjunto de palavras
    validas propostas pelo jogador, e atualizar a pontuacao do jogador convenientemente.
    """
    if not e_jogador(jogador) or not e_palavra_potencial(palavra_potencial):
        raise ValueError('adiciona_palavra_valida:argumentos invalidos.')
    else:
        if palavra_potencial not in jogador['VALIDAS']:
            jogador['PONTOS'] += palavra_tamanho(palavra_potencial)
            jogador['VALIDAS'] += [palavra_potencial]


def adiciona_palavra_invalida(jogador, palavra_potencial):
    """
    adiciona_palavra_invalida: jogador x palavra_potencial ->
    Este modificador recebe como argumentos um elemento do tipo jogador e uma
    palavra_potencial p, e tem como efeito adicionar a palavra p ao conjunto de palavras
    invalidas propostas pelo jogador, e atualizar a pontuacao do jogador convenientemente.
    """
    if not e_jogador(jogador) or not e_palavra_potencial(palavra_potencial):
        raise ValueError('adiciona_palavra_invalida:argumentos invalidos.')
    else:
        if palavra_potencial not in jogador['INVALIDAS']:
            jogador['PONTOS'] -= palavra_tamanho(palavra_potencial)
            jogador['INVALIDAS'] += [palavra_potencial]


def e_jogador(universal):
    """
    e_jogador: universal -> lógico
    Este reconhecedor recebe um unico argumento e devolve True caso
    esse argumento seja do tipo jogador, e False em caso contrario.
    """
    if isinstance(universal, dict) and len(universal) == 4 and \
            isinstance(universal['JOGADOR'], str) and isinstance(universal['PONTOS'], int) and \
            e_conjunto_palavras(universal['VALIDAS']) and e_conjunto_palavras(universal['INVALIDAS']):
        return True
    else:
        return False


def jogador_para_cadeia(jogador):
    """
    jogador_para_cadeia: jogador -> cad. caracteres
    Esta funcao recebe como argumento um elemento do tipo jogador e devolve uma
    cadeia de caracteres que o represente. Cada jogador e descrito pelo seu nome,
    seguido pela pontuacao obtida e pelos conjuntos de palavras validas e invalidas.
    """
    jogador_para_cadeia = 'JOGADOR ' + str(jogador['JOGADOR']) + \
                          ' PONTOS=' + str(jogador['PONTOS']) + \
                          ' VALIDAS=' + conjunto_palavras_para_cadeia(jogador['VALIDAS']) + \
                          ' INVALIDAS=' + conjunto_palavras_para_cadeia(jogador['INVALIDAS'])
    return jogador_para_cadeia


# ----------------------------------------------------------------------------------------------------------------------
# Funcoes Adicionais
# ----------------------------------------------------------------------------------------------------------------------


def gera_todas_palavras_validas(tuplo_de_letras):
    """
    gera_todas_palavras_validas: tuplo de letras -> conjunto_palavras
    Esta funcao recebe como argumento um tuplo de letras (possivelmente com
    repeticoes) para formar palavras e devolve um conjunto de palavras,
    contendo todas as palavras geradas a partir das letras dadas ou de
    subconjuntos das mesmas, que sao validas de acordo com a gramatica.
    """
    combinacoes = []
    palavra = ''
    lista = []
    lista_final = []
    for i in range(1, len(tuplo_de_letras) + 1):                               # Gera uma lista em que os seus elementos
        for combinacao in permutations(tuplo_de_letras, i):                    # sao tuplos, cada um representa uma
            combinacoes += [combinacao]                                        # de todas as combinacoes possiveis de
            for j in range(len(combinacoes)):                                  # fazer com as letras do tuplo inicial.
                palavra = ''
                for l in range(len(combinacoes[j])):
                    palavra += combinacoes[j][l]                               # Cada um desses tuplos e depois
            lista += [palavra]                                                 # convertido numa string que e adicionada
        for m in range(len(lista)):                                            # a uma lista. Essa lista e filtrada de
            if e_palavra(lista[m]):                                            # modo a devolver apenas as palavras
                palavra = cria_palavra_potencial(lista[m], tuplo_de_letras)    # que respeitam a gramatica do GURU
                acrescenta_palavra(lista_final, palavra)
    return lista_final


def guru_mj(tuplo_de_letras):
    """
    guru_mj: tuplo de letras ->
    Esta funcao corresponde a funcao principal do jogo e permite jogar um
    jogo completo de Palavra GURU Multi Jogador. Recebe como argumento um tuplo
    de letras correspondente ao conjunto de letras a usar na formacao das
    palavras, e apresenta o jogador vencedor, ou a indicacao de empate, caso
    exista mais do que um jogador com a melhor pontuacao.
    """
    print('Descubra todas as palavras geradas a partir das letras:')
    print(tuplo_de_letras)
    print('Introduza o nome dos jogadores (-1 para terminar)...')
    n = 1
    jogadores = {}
    while n != -1:
        jogador = str(input('JOGADOR ' + str(n) + ' -> '))
        if jogador == '-1':
            n = -1
        else:
            jogadores[n] = cria_jogador(jogador)
            n += 1
    k = 1
    conjunto_palavras = gera_todas_palavras_validas(tuplo_de_letras)
    palavras_descobertas = []
    falta_descobrir = numero_palavras(conjunto_palavras)
    while falta_descobrir > 0:
        for i in jogadores:
            if falta_descobrir > 0:
                print('JOGADA', k, '- Falta descobrir', falta_descobrir, 'palavras')
                jogada = str(input('JOGADOR ' + jogadores[i]['JOGADOR'] + ' -> '))
                if jogada in conjunto_palavras:
                    print(jogada, '- palavra VALIDA')
                    if jogada not in palavras_descobertas:
                        adiciona_palavra_valida(jogadores[i], jogada)
                        palavras_descobertas += [jogada]
                        falta_descobrir -= 1
                    k += 1
                else:
                    if cria_palavra_potencial(jogada, tuplo_de_letras) != jogada:
                        return cria_palavra_potencial(jogada, tuplo_de_letras)
                    else:
                        print(jogada, '- palavra INVALIDA')
                        adiciona_palavra_invalida(jogadores[i], jogada)
                    k += 1
    pontuacoes = []
    for i in jogadores:                                                                    # Cria uma lista com as
        pontuacoes += [jogadores[i]['PONTOS']]                                             # pontuacoes de todos os
    pontuacoes_ordenadas = sorted(pontuacoes)                                              # jogadores e ordena-as.
    if len(pontuacoes_ordenadas) == 1:                                                     # Se apenas houver um jogador
        vencedor = jogadores[1]['JOGADOR']                                                 # ele vence automaticamente.
        print('FIM DE JOGO! O jogo terminou com a vitoria do jogador', vencedor, 'com',
              pontuacoes_ordenadas[0], 'pontos.')
    else:
        if pontuacoes_ordenadas[-1] == pontuacoes_ordenadas[-2]:                        # Se as duas melhores pontuacoes
            print('FIM DE JOGO! O jogo terminou em empate.')                            # forem iguais houve empate.
        else:
            i = 1                                                                        # Se nao houver empate o
            vencedor = jogadores[1]                                                      # programa escolhe o vencedor
            while i < len(jogadores):                                                    # atraves de um ciclo while que
                if vencedor['PONTOS'] < jogadores[i + 1]['PONTOS']:                      # faz varias comparacoes ate
                    vencedor = jogadores[i + 1]                                          # descobrir o jogador com a
                    i += 1                                                               # maior pontuacao
                elif vencedor['PONTOS'] > jogadores[i + 1]['PONTOS']:
                    i += 1
            print('FIM DE JOGO! O jogo terminou com a vitoria do jogador', vencedor['JOGADOR'], 'com',
                  pontuacoes_ordenadas[-1], 'pontos.')
    for i in jogadores:
        print(jogador_para_cadeia(jogadores[i]))


# ----------------------------------------------------------------------------------------------------------------------
