#construtores

def cria_posicao(x, y):
    if isinstance(x, int) and x >= 0 and isinstance(y, int) and y >= 0:
        return (x, y)
    else:
        raise ValueError("cria_posicao: argumentos invalidos")


def cria_copia_posicao(posicao):
    return posicao

#seletores

def obter_pos_x(p):
    return p[0]


def obter_pos_y(p):
    return p[1]

#reconhecedor

def eh_posicao(arg):
    if isinstance(arg, tuple) and len(arg) == 2 and \
        isinstance(arg[0], int) and arg[0] > 0 and \
        isinstance(arg[1], int) and arg[1] > 0:
        return True
    else:
        return False

#teste

def posicoes_iguais(p1, p2):
    if eh_posicao(p1) and eh_posicao(p2) and p1 == p2:
        return True

#transformador

def posicao_para_str(p):
    return str(p)

#extra

def obter_posicoes_adjacentes(p):
    x = obter_pos_x(p)
    y = obter_pos_y(p)
    adjacente = ()
    if y > 0:
        adjacente = adjacente + ((x, y - 1),)
    adjacente = adjacente + ((x + 1, y),) + ((x, y + 1),)
    if x > 0:
        adjacente = adjacente + ((x - 1, y),)
    return adjacente


def ordenar_posicoes(t):
    return sorted(t, key=lambda tuple: (tuple[1], tuple[0]) )


# construtores

def cria_animal(s, r, a):
    if isinstance(s, str) and s != '' and \
        isinstance(r, int) and r > 0 and \
        isinstance(a, int) and a >= 0:
        animal = {
        "especie": s,
        "idade": 0,
        "freq_reproducao": r,
        "fome": 0,
        "freq_alimentacao": a
        }
        return animal
    else:
        raise ValueError("cria_animal: argumentos invalidos")
    


def cria_copia_animal(a):
    return a.copy()


#selectores

def obter_especie(a):
    return a["especie"]


def obter_freq_reproducao(a):
    return a["freq_reproducao"]


def obter_freq_alimentacao(a):
    return a["freq_alimentacao"]


def obter_idade(a):
    return a["idade"]

def obter_fome(a):
    return a["fome"]


# modificadores

def aumenta_idade(a):
    a["idade"] = obter_idade(a) + 1
    return a

def reset_idade(a):
    a["idade"] = 0
    return a

def aumenta_fome(a):
    if obter_freq_alimentacao(a) > 0:
        a["fome"] = obter_fome(a) + 1
        return a

def reset_fome(a):
    if obter_freq_alimentacao(a) > 0:
        a["fome"] = 0
        return a


# reconhecedor

def eh_animal(arg):
    if isinstance(arg, dict) and len(arg) == 5 and \
        "especie" in arg.keys() and \
        "idade" in arg.keys() and \
        "freq_reproducao" in arg.keys() and \
        "fome" in arg.keys() and \
        "freq_alimentacao" in arg.keys():
        return True
    else:
        return False

def eh_predador(arg):
    if eh_animal(arg) and obter_freq_alimentacao(arg) > 0:
        return True
    else:
        return False

def eh_presa(arg):
    if eh_animal(arg) and obter_freq_alimentacao(arg) == 0:
        return True
    else:
        return False


# teste

def animais_iguais(a1, a2):
    if eh_animal(a1) and eh_animal(a2) and a1 == a2:
        return True


# transformadores

def animal_para_char(a):
    if eh_predador(a):
        return obter_especie(a)[0].upper()
    else:
        return obter_especie(a)[0].lower()

def animal_para_str(a):
    if eh_predador(a):
        return "{} [{}/{};{}/{}]".format(obter_especie(a),
                                        obter_idade(a),
                                        obter_freq_reproducao(a),
                                        obter_fome(a),
                                        obter_freq_alimentacao(a))
    else:
        return "{} [{}/{}]".format(obter_especie(a),
                                        obter_idade(a),
                                        obter_freq_reproducao(a))


# alto nivel

def eh_animal_fertil(a):
    if obter_idade(a) == obter_freq_reproducao(a):
        return True
    else:
        return False

def eh_animal_faminto(a):
    if eh_predador(a):
        if obter_fome(a) >= obter_freq_alimentacao(a):
            return True
    else:
        return False

def reproduz_animal(a):
    reset_idade(a)
    cria = cria_copia_animal(a)
    reset_fome(cria)
    return cria


# construtor

def cria_prado(d, r, a, p):
    if eh_posicao(d) and \
        isinstance(r, tuple) and (eh_posicao(arg) for arg in r) and len(r) >= 0 and \
        isinstance(a, tuple) and (eh_animal(arg) for arg in a) and len(a) >= 1 and \
        isinstance(p, tuple) and (eh_posicao(arg) for arg in p) and len(p) == len(a):
        prado = {
        "posicao": d,
        "rochedos": r,
        "animais": a,
        "posicoes": p,
        }
        return prado
    else:
        raise ValueError("cria_prado: argumentos invalidos")


def cria_copia_prado(m):
    return m.copy()


# seletores

def obter_tamanho_x(m):
    return m["posicao"][0] + 1

def obter_tamanho_y(m):
    return m["posicao"][1] + 1

def obter_numero_predadores(m):
    contador = 0
    for animal in m["animais"]:
        if eh_predador(animal):
            contador += 1
    return contador

def obter_numero_presas(m):
    contador = 0
    for animal in m["animais"]:
        if eh_presa(animal):
            contador += 1
    return contador

def obter_posicao_animais(m):
    return ordenar_posicoes(m["posicoes"])

def obter_animal(m, p):
    indice = 0
    for posicao in m["posicoes"]:
        if posicoes_iguais(posicao, p):
            return m["animais"][indice]
        indice += 1


# modificadores

def eliminar_animal(m, p):
    indice = 0
    for posicao in m["posicoes"]:
        if posicoes_iguais(posicao, p):
            m["animais"] = m["animais"][:indice - 1] +  m["animais"][indice:]
            m["posicoes"] = m["posicoes"][:indice - 1] +  m["posicoes"][indice:]
            return m
        indice += 1    

def mover_animal(m, p1, p2):
    indice = 0
    for posicao in m["posicoes"]:
        if posicoes_iguais(posicao, p1):
            m["posicoes"] = m["posicoes"][:indice - 1] + (p2,) + m["posicoes"][indice:]
            return m
        indice += 1

def inserir_animal(m, a, p):
    m["animais"] = m["animais"] + (a,)
    m["posicoes"] = m["posicoes"] + (p,)
    return m


# reconhecedores

def eh_prado(arg):
    if isinstance(arg, dict) and len(arg) == 4 and \
        "posicao" in arg.keys() and \
        "rochedos" in arg.keys() and \
        "animais" in arg.keys() and \
        "posicoes" in arg.keys():
        return True
    else:
        return False

def eh_posicao_animal(m, p):
    if p in m["posicoes"]:
        return True
    else:
        return False

def eh_posicao_obstaculo(m, p):
    if p in m["rochedos"] or \
        p[0] == 0 or p[0] == m["posicao"][0] or \
        p[1] == 0 or p[1] == m["posicao"][1]:
        return True
    else:
        return False

def eh_posicao_livre(m, p):
    if p in m["posicoes"] or p in m["rochedos"] or \
        p[0] == 0 or p[0] == m["posicao"][0] or \
        p[1] == 0 or p[1] == m["posicao"][1]:
        return False
    else:
        return True

def prados_iguais(p1, p2):
    if eh_prado(p1) and eh_prado(p2) and p1 == p2:
        return True


# transformador

def prado_para_str(m):
    prado = ''
    tamanho_x = obter_tamanho_x(m)
    tamanho_y = obter_tamanho_y(m)
    for y in range(tamanho_y):
        for x in range(tamanho_x):
            p = (x, y)
            if (x == 0 or x == tamanho_x - 1) and (y == 0 or y == tamanho_y - 1):
                prado += '+'
            elif y == 0 or y == tamanho_y - 1:
                prado += '-'
            elif x == 0 or x == tamanho_x - 1:
                prado += '|'   
            elif eh_posicao_animal(m, p):
                print(p)
                prado += (animal_para_char(obter_animal(m, p)))
            elif eh_posicao_obstaculo(m, p):
                prado += '@'
            elif eh_posicao_livre(m, p):
                prado += '.'
        prado += '\n'
    return prado[:-1]


# alto nivel

def obter_valor_numerico(m, p):
    return p[1] * obter_tamanho_x(m) + p[0]

def obter_movimento(m, p):
    n = obter_valor_numerico(m, p)
    num_p = len(obter_posicoes_adjacentes(p))
    return p




# TESTE (apagar)

dim = cria_posicao(11, 4)
obs = (cria_posicao(4,2), cria_posicao(5,2))
an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
an2 = (cria_animal('lynx', 20, 15),)
pos = tuple(cria_posicao(p[0],p[1]) for p in ((5,1),(7,2),(10,1),(6,1)))
prado = cria_prado(dim, obs, an1+an2, pos)
print(prado["animais"][3])
print(obter_tamanho_x(prado), obter_tamanho_y(prado))
print(prado_para_str(prado))
p1 = cria_posicao(7,2)
p2 = cria_posicao(9,3)
prado = mover_animal(prado, p1, p2)
print(prado_para_str(prado))
print(obter_valor_numerico(prado, cria_posicao(9,3)))
posicao_para_str(obter_movimento(prado, cria_posicao(5,1)))
posicao_para_str(obter_movimento(prado, cria_posicao(6,1)))
posicao_para_str(obter_movimento(prado, cria_posicao(10,1)))