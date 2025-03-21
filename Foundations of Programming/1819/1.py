# Numero: 86532   Nome: Rafael Sousa Rodrigues
#-------------------------------------------------------------------------------

################################################################################
#                               Primeiro Projeto                               #
#                          FUNDAMENTOS DA PROGRAMACAO                          #
################################################################################

# ---------------------------------------------------------------------------- #
#                          REPRESENTACAO DO TABULEIRO                          #
# ---------------------------------------------------------------------------- #


def eh_tabuleiro(universal):
    """eh_tabuleiro: universal -> booleano
    Esta funcao recebe um argumento de qualquer tipo e devolve True se o seu
    argumento corresponde a um tabuleiro e False caso contrario."""
    if not isinstance(universal, tuple) or len(universal) != 3 \
       or not isinstance(universal[0], tuple) or len(universal[0]) != 3 \
       or not isinstance(universal[1], tuple) or len(universal[1]) != 3 \
       or not isinstance(universal[2], tuple) or len(universal[2]) != 2:
        return False
    else:
        estados = [-1, 0, 1]
        for i in range(len(universal)):
            for j in range(len(universal[i])):
                if universal[i][j] not in estados:
                    return False
        return True


def tabuleiro_str(tabuleiro):
    """tabuleiro_str: tabuleiro -> cad. caracteres
    Esta funcao recebe um tabuleiro e devolve a cadeia de caracteres que o
    representa."""
    if not eh_tabuleiro(tabuleiro):
        raise ValueError('tabuleiro_str: argumento invalido')
    else:
        return '+-------+\n|...' + representa(tabuleiro[0][2]) + \
               '...|\n|..' + representa(tabuleiro[0][1]) + \
               '.' + representa(tabuleiro[1][2]) + \
               '..|\n|.' + representa(tabuleiro[0][0])  + \
               '.' + representa(tabuleiro[1][1]) + \
               '.' + representa(tabuleiro[2][1]) + \
               '.|\n|..' + representa(tabuleiro[1][0]) + \
               '.' + representa(tabuleiro[2][0]) + '..|\n+-------+'


def tabuleiros_iguais(tabuleiro1, tabuleiro2):
    """tabuleiros_iguais: tabuleiro	x tabuleiro	-> booleano
    Esta funcao recebe dois tabuleiros e devolve True se os tabuleiros sao
    iguais e False caso contrario."""
    if not eh_tabuleiro(tabuleiro1) or not eh_tabuleiro(tabuleiro2):
        raise ValueError('tabuleiros_iguais: um dos argumentos nao e tabuleiro')
    else:
        return tabuleiro1 == tabuleiro2


# ---------------------------------------------------------------------------- #
#                          REPRESENTACAO DO TABULEIRO                          #
# ---------------------------------------------------------------------------- #


def porta_x(tabuleiro, caracter):
    """porta_x:	tabuleiro x {"E", "D"} -> tabuleiro
    Esta funcao recebe um tabuleiro e um caracter ("E" ou "D") e devolve um
    novo tabuleiro resultante de aplicar a porta X ao qubit da esquerda ou de
    direita, conforme o caracter seja "E" ou "D", respetivamente."""
    if eh_tabuleiro(tabuleiro) and caracter == 'E':
        tuplo2 = ()
        for i in tabuleiro[1]:
            tuplo2 += (inverte(i),)
        return (tabuleiro[0], tuplo2, tabuleiro[2])
    elif eh_tabuleiro(tabuleiro) and caracter == 'D':
        return ((tabuleiro[0][0], inverte(tabuleiro[0][1]), tabuleiro[0][2]), \
               (tabuleiro[1][0], inverte(tabuleiro[1][1]), tabuleiro[1][2]), \
               (inverte(tabuleiro[2][0]), tabuleiro[2][1]))
    else:
        raise ValueError('porta_x: um dos argumentos e invalido')


def porta_z(tabuleiro, caracter):
    """porta_z:	tabuleiro x {"E", "D"} -> tabuleiro
    Esta funcao recebe um tabuleiro e um caracter ("E" ou "D") e devolve um
    novo tabuleiro resultante de aplicar a porta Z ao qubit da esquerda ou da
    direita, conforme o caracter seja "E" ou "D", respetivamente."""
    if eh_tabuleiro(tabuleiro) and caracter == 'E':
        tuplo1 = ()
        for i in tabuleiro[0]:
            tuplo1 += (inverte(i),)
        return (tuplo1, tabuleiro[1], tabuleiro[2])
    elif eh_tabuleiro(tabuleiro) and caracter == 'D':
        return ((tabuleiro[0][0], tabuleiro[0][1], inverte(tabuleiro[0][2])), \
               (tabuleiro[1][0], tabuleiro[1][1], inverte(tabuleiro[1][2])), \
               (tabuleiro[2][0], inverte(tabuleiro[2][1])))
    else:
        raise ValueError('porta_z: um dos argumentos e invalido')


def porta_h(tabuleiro, caracter):
    """porta_h: tabuleiro x {"E", "D"} -> tabuleiro
    Esta funcao recebe um tabuleiro e um caracter ("E" ou "D") e devolve um novo
    tabuleiro resultante de aplicar a porta H ao qubit da esquerda ou da
    direita, conforme o caracter seja "E" ou "D", respetivamente."""
    if eh_tabuleiro(tabuleiro) and caracter == 'E':
        return (tabuleiro[1], tabuleiro[0], tabuleiro[2])
    elif eh_tabuleiro(tabuleiro) and caracter == 'D':
        return ((tabuleiro[0][0], tabuleiro[0][2], tabuleiro[0][1]), \
               (tabuleiro[1][0], tabuleiro[1][2], tabuleiro[1][1]), \
               (tabuleiro[2][1], tabuleiro[2][0]))
    else:
        raise ValueError('porta_h: um dos argumentos e invalido')


# ---------------------------------------------------------------------------- #
#                              FUNCOES AUXILIARES                              #
# ---------------------------------------------------------------------------- #


def representa(estado):
    """rep: estado -> string
    Esta funcao recebe o estado de uma celula e devolve uma string, que contem
    um simbolo, que representa graficamente o estado da celula."""
    if estado == 0:
        return '0'
    elif estado == 1:
        return '1'
    else:
        return 'x'


def inverte(estado):
    """troca: estado -> estado
    Esta funcao recebe o estado de uma celula e devolve o estado inverso,
    caso o estado nao seja incerto."""
    if estado == 0:
        return 1
    elif estado == 1:
        return 0
    else:
        return estado


# ---------------------------------------------------------------------------- #
