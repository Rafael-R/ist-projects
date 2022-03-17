# Numero: 86532   Nome: Rafael Sousa Rodrigues
# ---------------------------------------------------------------------------- #

################################################################################
#                                Segundo Projeto                               #
#                          FUNDAMENTOS DA PROGRAMACAO                          #
################################################################################

# ---------------------------------------------------------------------------- #
#                           TIPOS ABSTRATOS DE DADOS                           #
# ---------------------------------------------------------------------------- #


# Celula

def cria_celula(v):
    """ cria_celula: {1, 0, −1} -> celula
    cria_celula(v), recebe o valor do estado de uma celula do qubit
    (0, representa inativo, 1, ativo e -1 incerto) e devolve uma celula
    com esse valor. Caso o argumento nao seja valido gera um erro."""

    if v == 0:
        return {'estado': v}
    elif v == 1:
        return {'estado': v}
    elif v == -1:
        return {'estado': v}
    else:
        raise ValueError('cria_celula: argumento invalido.')


def obter_valor(c):
    """ obter_valor: celula -> {1, 0, -1}
    obter_valor(c) devolve o valor associado a c."""

    return c['estado']


def inverte_estado(c):
    """ inverte_estado: celula -> celula
    inverte(c) devolve a celula resultante de inverter o estado da celula
    que e seu argumento: uma celula inativa torna-se ativa e vice versa;
    uma celula no estado incerto mantem-se no mesmo estado."""

    if obter_valor(c) == 0:
        c['estado'] = 1
    elif obter_valor(c) == 1:
        c['estado'] = 0
    return c


def eh_celula(arg):
    """ eh_celula: universal -> logico
    eh_celula(arg) devolve verdadeiro apenas no no caso do seu argumento
    ser do tipo celula."""

    return arg in [cria_celula(0), cria_celula(1), cria_celula(-1)]


def celulas_iguais(c1, c2):
    """ celulas_iguais: celula x celula -> logico
    celulas_iguais(c1, c2) devolve verdadeiro apenas se c1 e c2 sao celulas
    no mesmo estado."""

    if eh_celula(c1) and eh_celula(c2) and c1 == c2:
        return True
    else:
        return False


def celula_para_str(c):
    """ celula_para_str: celula -> cad. caracteres
    celula_para_str(c) devolve uma cadeia de caracteres que representa a
    celula que e seu argumento: o estado ativo e representado por '1'; o
    estado inativo e representado por '0' e o estado incerto e representado
    por 'x'."""

    if obter_valor(c) == 0:
        return '0'
    elif obter_valor(c) == 1:
        return '1'
    else:
        return 'x'


# Coordenada

def cria_coordenada(l, c):
    """ cria_coordenada: N x N -> coordenada (N = natural)
    cria_coordenada(l, c) devolve a coordenada correspondente a linha l e
    a coluna c. Caso os argumentos nao sejam validos gera um erro."""

    if l in [0, 1, 2] and c in [0, 1, 2]:
        return (l, c)
    else:
        raise ValueError('cria_coordenada: argumentos invalidos.')


def coordenada_linha(c):
    """ coordenada_linha: coordenada -> N (N = natural)
    coordenada_linha(c) devolve o natural correspondente a linha da
    coordenada c."""

    return c[0]


def coordenada_coluna(c):
    """ coordenada_linha: coordenada -> N (N = natural)
    coordenada_linha(c) devolve o natural correspondente a coluna da
    coordenada c."""

    return c[1]


def eh_coordenada(arg):
    """ eh_coordenada: universal -> logico
    eh_coordenada(arg) devolve verdadeiro apenas no caso em que o seu
    argumento e do tipo coordenada."""

    return isinstance(arg, tuple) and len(arg) == 2 and \
        coordenada_linha(arg) in [0, 1, 2] and \
        coordenada_coluna(arg) in [0, 1, 2]


def coordenadas_iguais(c1, c2):
    """coordenadas_iguais: coordenada x coordenada -> logico
    coordenadas_iguais(c1, c2) devolve verdadeiro apenas se c1 e c2
    representem coordenadas iguais, ou seja representam a mesma posicao."""

    if eh_coordenada(c1) and eh_coordenada(c2) and c1 == c2:
        return True
    else:
        return False


def coordenada_para_str(c):
    """ coordenada_para_str: coordenada -> cad. caracteres
    coordenada_para_str(c) devolve a cadeia de caracteres que representa o seu
    argumento. A coordenada correspondente a linha l e coluna c e representada
    pela cadeia de carateres '(l, c)'."""

    return '(' + str(coordenada_linha(c)) + \
           ', ' + str(coordenada_coluna(c)) + ')'


# Tabuleiro

def tabuleiro_inicial():
    """ tabuleiro_inicial: {} -> tabuleiro
    tabuleiro_inicial() devolve o tabuleiro que representa o seu estado
    inicial do jogo."""

    return [[cria_celula(-1), cria_celula(-1), cria_celula(-1)],
            [cria_celula(0), cria_celula(0), cria_celula(-1)],
            [cria_celula(-1), cria_celula(0), cria_celula(-1)]]


def str_para_tabuleiro(s):
    """ str_para_tabuleiro: cad. caracteres -> tabuleiro
    str_para_tabuleiro(s) devolve o tabuleiro correspondente a cadeia de
    caracteres que e seu argumento."""

    if not isinstance(s, str):
        raise ValueError('str_para_tabuleiro: argumento invalido.')
    else:
        t = eval(s)
        if not isinstance(t, tuple) or len(t) != 3 or \
           not isinstance(t[0], tuple) or len(t[0]) != 3 or \
           not isinstance(t[1], tuple) or len(t[1]) != 3 or \
           not isinstance(t[2], tuple) or len(t[2]) != 2:
            raise ValueError('str_para_tabuleiro: argumento invalido.')

    tab = [[cria_celula(t[0][0]), cria_celula(t[0][1]), cria_celula(t[0][2])],
           [cria_celula(t[1][0]), cria_celula(t[1][1]), cria_celula(t[1][2])],
           [cria_celula(-1), cria_celula(t[2][0]), cria_celula(t[2][1])]]

    if eh_tabuleiro(tab):
        return tab
    else:
        raise ValueError('str_para_tabuleiro: argumento invalido.')


def tabuleiro_dimensao(t):
    """ tabuleiro_dimensao: tabuleiro -> N (N = natural)
    tabuleiro_dimensao(t) devolve o natural correspondente ao numero de linhas
    (e, consequentemente, tambem ao numero de colunas) existentes em t."""

    return len(t)


def tabuleiro_celula(t, coor):
    """ tabuleiro x coordenada -> celula
    tabuleiro_celula(t, coor) devolve a celula presente na coordenada coor
    do tabuleiro t."""

    return t[coordenada_linha(coor)][coordenada_coluna(coor)]


def tabuleiro_substitui_celula(t, cel, coor):
    """tabuleiro_substitui_celula: tabuleiro x celula x coordenada -> tabuleiro
    tabuleiro_substitui_celula(t, cel, coor) devolve o tabuleiro que resulta de
    substituir a celula existente na coordenada coor do tabuleiro, pela nova
    celula. Caso os argumentos nao sejam validos gera um erro."""

    if eh_tabuleiro(t) and eh_celula(cel) and eh_coordenada(coor):
        t[coordenada_linha(coor)][coordenada_coluna(coor)] = cel
        return t
    else:
        raise ValueError('tabuleiro_substitui_celula: argumentos invalidos.')


def tabuleiro_inverte_estado(t, coor):
    """tabuleiro_inverte_estado: tabuleiro x coordenada -> tabuleiro
    tabuleiro_inverte_estado(t, coor) devolve o tabuleiro que resulta de
    inverter o estado da celula presente na coordenada coor do tabuleiro.
    Caso os argumentos nao sejam validos gera um erro."""

    if eh_tabuleiro(t) and eh_coordenada(coor):
        t[coordenada_linha(coor)][coordenada_coluna(coor)] = \
            inverte_estado(tabuleiro_celula(t, coor))
        return t
    else:
        raise ValueError('tabuleiro_inverte_estado: argumentos invalidos.')


def eh_tabuleiro(arg):
    """eh_tabuleiro: universal -> logico
    eh_tabuleiro(arg) devolve verdadeiro apenas no caso de arg ser
    do tipo tabuleiro."""

    if not isinstance(arg, list) or tabuleiro_dimensao(arg) != 3 or \
       not isinstance(arg[0], list) or tabuleiro_dimensao(arg[0]) != 3 or \
       not isinstance(arg[1], list) or tabuleiro_dimensao(arg[1]) != 3 or \
       not isinstance(arg[2], list) or tabuleiro_dimensao(arg[2]) != 3:
        return False
    else:
        for i in range(3):
            for j in range(3):
                if not eh_celula(tabuleiro_celula(arg, cria_coordenada(i, j))):
                    return False
        return True


def tabuleiros_iguais(t1, t2):
    """tabuleiros_iguais: tabuleiro x tabuleiro -> logico
    tabuleiros_iguais(t1, t2) devolve verdadeiro apenas no caso de t1 e t2 forem
    tabuleiros que contenham celulas iguais em cada uma das coordenadas."""

    if eh_tabuleiro(t1) and eh_tabuleiro(t2) and t1 == t2:
        return True
    else:
        return False


def tabuleiro_para_str(t):
    """tabuleiro_para_str: tabuleiro -> cad. caracteres
    tabuleiro_para_str(t) devolve a cadeia de caracteres
    que representa o tabuleiro que é seu argumento."""

    return '+-------+\n|...' + \
        celula_para_str(tabuleiro_celula(t, cria_coordenada(0, 2))) + \
        '...|\n|..' + \
        celula_para_str(tabuleiro_celula(t, cria_coordenada(0, 1))) + \
        '.' + celula_para_str(tabuleiro_celula(t, cria_coordenada(1, 2))) + \
        '..|\n|.' + \
        celula_para_str(tabuleiro_celula(t, cria_coordenada(0, 0))) + \
        '.' + celula_para_str(tabuleiro_celula(t, cria_coordenada(1, 1))) + \
        '.' + celula_para_str(tabuleiro_celula(t, cria_coordenada(2, 2))) + \
        '.|\n|..' + \
        celula_para_str(tabuleiro_celula(t, cria_coordenada(1, 0))) + \
        '.' + celula_para_str(tabuleiro_celula(t, cria_coordenada(2, 1))) + \
        '..|\n+-------+'


def porta_x(t, p):
    """porta_x: tabuleiro x {'E', 'D'} -> tabuleiro
    porta_x(t, p) devolve o tabuleiro resultante de aplicar a porta X a celula
    inferior do qubit da esquerda, ou da direita, conforme p seja 'E' ou 'D',
    respetivamente. Caso um dos argumentos nao seja valido gera um erro."""

    if eh_tabuleiro(t) and p == 'E':
        return inverte_linha(t, 1)
    elif eh_tabuleiro(t) and p == 'D':
        return inverte_coluna(t, 1)
    else:
        raise ValueError('porta_x: argumentos invalidos.')


def porta_z(t, p):
    """porta_z: tabuleiro x {'E', 'D'} -> tabuleiro
    porta_z(t, p) devolve o tabuleiro resultante de aplicar a porta Z a celula
    inferior do qubit da esquerda, ou da direita, conforme p seja 'E' ou 'D',
    respetivamente. Caso um dos argumentos nao seja valido gera um erro."""

    if eh_tabuleiro(t) and p == 'E':
        return inverte_linha(t, 0)
    elif eh_tabuleiro(t) and p == 'D':
        return inverte_coluna(t, 2)
    else:
        raise ValueError('porta_z: argumentos invalidos.')


def porta_h(t, p):
    """porta_h: tabuleiro x {'E', 'D'} -> tabuleiro
    porta_h(t, p) devolve o tabuleiro resultante de aplicar a porta H a celula
    inferior do qubit da esquerda, ou da direita, conforme p seja 'E' ou 'D',
    respetivamente. Caso um dos argumentos nao seja valido gera um erro."""

    if eh_tabuleiro(t) and p == 'E':
        troca_celulas(t, cria_coordenada(0, 0), cria_coordenada(1, 0))
        troca_celulas(t, cria_coordenada(0, 1), cria_coordenada(1, 1))
        troca_celulas(t, cria_coordenada(0, 2), cria_coordenada(1, 2))
        return t
    elif eh_tabuleiro(t) and p == 'D':
        troca_celulas(t, cria_coordenada(0, 2), cria_coordenada(0, 1))
        troca_celulas(t, cria_coordenada(1, 2), cria_coordenada(1, 1))
        troca_celulas(t, cria_coordenada(2, 2), cria_coordenada(2, 1))
        return t
    else:
        raise ValueError('porta_h: argumentos invalidos.')


# ---------------------------------------------------------------------------- #
#                              FUNCOES ADICIONAIS                              #
# ---------------------------------------------------------------------------- #


def hello_quantum(descricao):
    """ hello_quantum: cad. caracteres -> logico
    Função principal que permite jogar um jogo completo de Hello Quantum.
    A função hello_quantum recebe uma cadeia de caracteres contendo a
    descrição do tabuleiro objetivo e o número máximo de jogadas. A função
    devolve verdadeiro se o jogador conseguir transformar o tabuleiro inicial
    no tabuleiro objetivo, não ultrapassando o número de jogadas indicado e
    devolve falso em caso contrário."""

    i = 0
    while descricao[i - 1] != ':':
        i -= 1
    max_jogadas = int(descricao[i:])
    objetivo = str_para_tabuleiro(descricao[:i - 1])

    tabuleiro = tabuleiro_inicial()
    print('Bem-vindo ao Hello Quantum!\n'
          + 'O seu objetivo e chegar ao tabuleiro:\n'
          + tabuleiro_para_str(objetivo) + '\n'
          + 'Comecando com o tabuleiro que se segue:\n'
          + tabuleiro_para_str(tabuleiro))

    jogadas = 0
    while jogadas < max_jogadas and not tabuleiros_iguais(tabuleiro, objetivo):
        jogadas += 1
        porta = input('Escolha uma porta para aplicar (X, Z ou H): ')
        lado = input('Escolha um qubit para analisar (E ou D): ')
        if porta == 'X':
            porta_x(tabuleiro, lado)
        elif porta == 'Z':
            porta_z(tabuleiro, lado)
        elif porta == 'H':
            porta_h(tabuleiro, lado)
        print(tabuleiro_para_str(tabuleiro))

    if tabuleiros_iguais(tabuleiro, objetivo):
        print('Parabens, conseguiu converter o tabuleiro em '
              + str(jogadas) + ' jogadas!')
        return True
    else:
        return False


# ---------------------------------------------------------------------------- #
#                              FUNCOES AUXILIARES                              #
# ---------------------------------------------------------------------------- #


def inverte_linha(t, linha):
    """inverte_linha: tabuleiro x linha -> tabuleiro
    inverte_linha(t, linha) devolve o tabuleiro resultante de inverter o
    estado de todas as celulas de uma dada linha."""

    for i in range(3):
        tabuleiro_inverte_estado(t, cria_coordenada(linha, i))
    return t


def inverte_coluna(t, coluna):
    """inverte_coluna: tabuleiro x coluna -> tabuleiro
    inverte_coluna(t, coluna) devolve o tabuleiro resultante de inverter o
    estado de todas as celulas de uma dada coluna."""

    for i in range(3):
        tabuleiro_inverte_estado(t, cria_coordenada(i, coluna))
    return t


def troca_celulas(t, coor1, coor2):
    """troca_celulas: tabuleiro x coordenada x coordenada -> tabuleiro
    troca_celulas(t, coor1, coor2) devolve o tabuleiro resultante de trocar
    as celulas nas coordenadas dadas."""

    temp = tabuleiro_celula(t, coor1)
    tabuleiro_substitui_celula(t, tabuleiro_celula(t, coor2), coor1)
    tabuleiro_substitui_celula(t, temp, coor2)
    return t


# ---------------------------------------------------------------------------- #
