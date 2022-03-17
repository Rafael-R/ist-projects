# Nome: Rafael Sousa Rodrigues   Numero: 89532

# ------------------------------------------------------------------------------
# GRAMÁTICA GURU - Linguagem
# ------------------------------------------------------------------------------


def palavra(pal):
    def palavra_aux(pal):
        if silaba_final(pal):                                                 # Palavra pode ser apenas uma silaba final
            return True
        elif len(pal) != 0:                                                   # A funcao auxiliar vai removendo as
            for i in range(6):                                                # silabas ate encontrar uma silaba final.
                if silaba(pal[0:i]) and palavra_aux(pal[i:]):                 # Remove a silaba e repete o processo
                    return True                                               # com o que resta da cadeia de caracteres.
            return False
        else:
            return False

    return monossilabo(pal) or palavra_aux(pal)


def silaba(sil):
    return vogal(sil) or silaba_2(sil) or silaba_3(sil) or silaba_4(sil) or \
        silaba_5(sil)


def silaba_final(sil_fin):
    return monossilabo_2(sil_fin) or monossilabo_3(sil_fin) or \
        silaba_4(sil_fin) or silaba_5(sil_fin)


def silaba_5(sil_5):
    return par_consoantes(sil_5[:2]) and vogal(sil_5[2:3]) and sil_5[3:] == 'NS'


def silaba_4(sil_4):
    return par_vogais(sil_4[:2]) and sil_4[2:] == 'NS' or \
        consoante(sil_4[:1]) and vogal(sil_4[1:2]) and sil_4[2:] == 'NS' or \
        consoante(sil_4[:1]) and vogal(sil_4[1:2]) and sil_4[2:] == 'IS' or \
        par_consoantes(sil_4[:2]) and par_vogais(sil_4[2:]) or \
        consoante(sil_4[:1]) and par_vogais(sil_4[1:3]) and \
        consoante_final(sil_4[3:])


def silaba_3(sil_3):
    return sil_3 in ['QUA', 'QUE', 'QUI', 'GUE', 'GUI'] or \
        vogal(sil_3[:1]) and sil_3[1:] == 'NS' or \
        consoante(sil_3[:1]) and par_vogais(sil_3[1:]) or \
        consoante(sil_3[:1]) and vogal(sil_3[1:2]) and \
        consoante_final(sil_3[2:]) or \
        par_vogais(sil_3[:2]) and consoante_final(sil_3[2:]) or \
        par_consoantes(sil_3[:2]) and vogal(sil_3[2:])


def silaba_2(sil_2):
    return par_vogais(sil_2) or consoante(sil_2[:1]) and vogal(sil_2[1:]) or \
        vogal(sil_2[:1]) and consoante_final(sil_2[1:])


def monossilabo(mon):
    return vogal_palavra(mon) or monossilabo_2(mon) or monossilabo_3(mon)


def monossilabo_3(mon_3):
    return consoante(mon_3[:1]) and vogal(mon_3[1:2]) and \
        consoante_terminal(mon_3[2:]) or \
        consoante(mon_3[:1]) and ditongo(mon_3[1:]) or \
        par_vogais(mon_3[:2]) and consoante_terminal(mon_3[2:])


def monossilabo_2(mon_2):
    return mon_2 in ['AR', 'IR', 'EM', 'UM'] or \
        vogal(mon_2[:1]) and mon_2[1:] == 'S' or \
        ditongo_palavra(mon_2) or \
        consoante_freq(mon_2[:1]) and vogal(mon_2[1:])


def par_consoantes(par_con):
    return par_con in ['BR', 'CR', 'FR', 'GR', 'PR', 'TR', 'VR', 'BL', 'CL',
                       'FL', 'GL', 'PL']


def consoante(con):
    return con in ['B', 'C', 'D', 'F', 'G', 'H', 'J', 'L', 'M', 'N', 'P', 'Q',
                   'R', 'S', 'T', 'V', 'X', 'Z']


def consoante_final(con_fin):
    return con_fin in ['N', 'P'] or consoante_terminal(con_fin)


def consoante_terminal(con_ter):
    return con_ter in ['L', 'M', 'R', 'S', 'X', 'Z']


def consoante_freq(con_fre):
    return con_fre in ['D', 'L', 'M', 'N', 'P', 'R', 'S', 'T', 'V']


def par_vogais(par_vog):
    return ditongo(par_vog) or par_vog in ['IA', 'IO']


def ditongo(dit):
    return dit in ['AE', 'AU', 'EI', 'OE', 'OI', 'IU'] or ditongo_palavra(dit)


def ditongo_palavra(dit_pal):
    return dit_pal in ['AI', 'AO', 'EU', 'OU']


def vogal(vog):
    return vog in ['I', 'U'] or vogal_palavra(vog)


def vogal_palavra(vog_pal):
    return artigo_def(vog_pal) or vog_pal == 'E'


def artigo_def(art_def):
    return art_def in ['A', 'O']


# ------------------------------------------------------------------------------
# FUNÇÕES IMPLEMENTADAS
# ------------------------------------------------------------------------------


def e_silaba(cad_caracteres):
    """Verifica se a cadeia de caracteres e uma silaba.
    Se a cadeia de caracteres formar uma silaba valida de acordo com a
    gramatica dada, a funcao devolve True, senao devolve False."""
    if not isinstance(cad_caracteres, str) or cad_caracteres == '':
        raise ValueError('e_silaba:argumento invalido')
    else:
        return silaba(cad_caracteres)


def e_monossilabo(cad_caracteres):
    """Verifica se a cadeia de caracteres e um monossilabo.
    Se a cadeia de caracteres formar um monossilabo valido de acordo com a
    gramatica dada, a funcao devolve True, senao devolve False."""
    if not isinstance(cad_caracteres, str) or cad_caracteres == '':
        raise ValueError('e_monossilabo:argumento invalido')
    else:
        return monossilabo(cad_caracteres)


def e_palavra(cad_caracteres):
    """Verifica se a cadeia de caracteres e uma palavra.
    Se a cadeia de caracteres formar uma palavra valida de acordo com a
    gramatica dada, a funcao devolve True, senao devolve False."""
    if not isinstance(cad_caracteres, str) or cad_caracteres == '':
        raise ValueError('e_palavra:argumento invalido')
    else:
        return palavra(cad_caracteres)

# ------------------------------------------------------------------------------
