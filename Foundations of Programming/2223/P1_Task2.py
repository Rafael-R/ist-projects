def calcula_quocientes(v, n):
    quocientes = {}
    for partido in v:
        quocientes[partido] = []
        for i in range(n):
            quocientes[partido] += [v[partido] / (i + 1)]
    return quocientes

def atribui_mandatos(v, n):
    mandatos = []
    quocientes = calcula_quocientes(v, n)
    for i in range(n):
        partidos = []
        maiores_quocientes = []
        for partido in quocientes:
            partidos += [partido]
            maiores_quocientes += [quocientes[partido][0]]
        maior_quociente = max(maiores_quocientes)
        index = []
        for i in range(len(maiores_quocientes)):
            if maiores_quocientes[i] == maior_quociente:
                index += [i]
        if len(index) == 1:
            partido = partidos[index[0]]
            mandatos += partido
            quocientes[partido].remove(maior_quociente)
        else:
            votos = []
            for i in index:
                votos += [v[partidos[i]]]
            menos_votos = min(votos)
            for i in range(len(votos)):
                if votos[i] == menos_votos:
                    partido = partidos[index[i]]
            mandatos += partido
            quocientes[partido].remove(maior_quociente)
    return mandatos

def obtem_partidos(info):
    partidos = []
    for circulo in info:
        for partido in info[circulo]['votos']:
            if partido not in partidos:
                partidos += [partido]
    partidos.sort()
    return partidos

def obtem_resultado_eleicoes(info):
    partidos = obtem_partidos(info)
    total_votos = {}
    total_deputados = {}
    for partido in partidos:
        total_votos[partido] = 0
        total_deputados[partido] = 0
    for circulo in info:
        deputados = info[circulo]['deputados']
        votos = info[circulo]['votos']
        for partido in votos:
            total_votos[partido] += votos[partido]
        mandatos = atribui_mandatos(votos, deputados)
        for partido in mandatos:
            total_deputados[partido] += 1
    resultados = []
    for partido in partidos:
        resultados += [(partido, total_deputados[partido], total_votos[partido])]
    resultados.sort(key = lambda x: x[1:3], reverse=True)
    return resultados