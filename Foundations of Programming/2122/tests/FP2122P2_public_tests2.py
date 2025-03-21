def teste211():
    total_score = 0
    fun_name = TAD_posicao_public

    p1 = cria_posicao(-1, 2)
    # cria_posicao: argumentos invalidos

    p1 = cria_posicao(2, 3)
    p2 = cria_posicao(7, 0)
    posicoes_iguais(p1, p2)
    # False

    p1 = cria_posicao(2, 3)
    posicao_para_str(p1) == '(2, 3)'
    # True

    p2 = cria_posicao(7, 0)
    t = obter_posicoes_adjacentes(p2)
    tuple(posicao_para_str(p) for p in t)
    # ('(8, 0)', '(7, 1)', '(6, 0)')

    p2 = cria_posicao(7, 0)
    t = obter_posicoes_adjacentes(p2)
    tuple(posicao_para_str(p) for p in ordenar_posicoes(t))
    # ('(6, 0)', '(8, 0)', '(7, 1)')

    return



def teste212():
    total_score = 0
    fun_name = TAD_animal_public

    cria_animal('rabbit', -5, 0)
    # cria_animal: argumentos invalidos

    r1 = cria_animal('rabbit', 5, 0)
    animal_para_str(r1)
    # rabbit [0/5]

    f1 = cria_animal('fox', 20, 10)
    animal_para_str(f1)
    # fox [0/20;0/10]

    r1 = cria_animal('rabbit', 5, 0)
    animal_para_char(r1)
    # r

    f1 = cria_animal('fox', 20, 10)
    animal_para_char(f1)
    # F

    f1 = cria_animal('fox', 20, 10)
    f2 = cria_copia_animal(f1)
    f2 = aumenta_idade(aumenta_idade(f2))
    f2 = aumenta_fome(f2)
    animal_para_str(f1)
    # fox [0/20;0/10]

    f1 = cria_animal('fox', 20, 10)
    f2 = cria_copia_animal(f1)
    f2 = aumenta_idade(aumenta_idade(f2))
    f2 = aumenta_fome(f2)
    animal_para_str(f2)
    # fox [2/20;1/10]

    f1 = cria_animal('fox', 20, 10)
    f2 = cria_copia_animal(f1)
    f2 = aumenta_idade(aumenta_idade(f2))
    f2 = aumenta_fome(f2)
    animais_iguais(f1, f2)
    # False

    f1 = cria_animal('fox', 20, 10)
    f2 = cria_copia_animal(f1)
    f2 = aumenta_idade(aumenta_idade(f2))
    f2 = aumenta_fome(f2)
    f3 = reproduz_animal(f2)
    animal_para_str(f2)
    # fox [0/20;1/10]

    f1 = cria_animal('fox', 20, 10)
    f2 = cria_copia_animal(f1)
    f2 = aumenta_idade(aumenta_idade(f2))
    f2 = aumenta_fome(f2)
    f3 = reproduz_animal(f2)
    animal_para_str(f3)
    # fox [0/20;0/10]

    return


def teste213():
    total_score = 0
    fun_name = TAD_prado_public

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
    an2 = (cria_animal('lynx', 20, 15),)
    pos = tuple(cria_posicao(p[0], p[1]) for p in ((5, 1), (7, 2), (10, 1), (6, 1)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    obter_tamanho_x(prado), obter_tamanho_y(prado)
    # (12, 5)

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
    an2 = (cria_animal('lynx', 20, 15),)
    pos = tuple(cria_posicao(p[0], p[1]) for p in ((5, 1), (7, 2), (10, 1), (6, 1)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    prado_para_str(prado)
    # +----------+\\n|....rL...r|\\n|...@@.r...|\\n|..........|\\n+----------+

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
    an2 = (cria_animal('lynx', 20, 15),)
    pos = tuple(cria_posicao(p[0], p[1]) for p in ((5, 1), (7, 2), (10, 1), (6, 1)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    p1 = cria_posicao(7, 2)
    p2 = cria_posicao(9, 3)
    prado = mover_animal(prado, p1, p2)
    prado_para_str(prado)
    # +----------+\\n|....rL...r|\\n|...@@.....|\\n|........r.|\\n+----------+

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
    an2 = (cria_animal('lynx', 20, 15),)
    pos = tuple(cria_posicao(p[0], p[1]) for p in ((5, 1), (7, 2), (10, 1), (6, 1)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    obter_valor_numerico(prado, cria_posicao(9, 3))
    # 45

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
    an2 = (cria_animal('lynx', 20, 15),)
    pos = tuple(cria_posicao(p[0], p[1]) for p in ((5, 1), (9, 3), (10, 1), (6, 1)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    posicao_para_str(obter_movimento(prado, cria_posicao(5, 1)))
    # (4, 1)

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
    an2 = (cria_animal('lynx', 20, 15),)
    pos = tuple(cria_posicao(p[0], p[1]) for p in ((5, 1), (9, 3), (10, 1), (6, 1)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    posicao_para_str(obter_movimento(prado, cria_posicao(6, 1)))
    # (5, 1)

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('rabbit', 5, 0) for i in range(3))
    an2 = (cria_animal('lynx', 20, 15),)
    pos = tuple(cria_posicao(p[0], p[1]) for p in ((5, 1), (9, 3), (10, 1), (6, 1)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    posicao_para_str(obter_movimento(prado, cria_posicao(10, 1)))
    # (10, 2)

    return


def teste221():

    total_score = 0
    fun_name = geracao_public

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('sheep', 2, 0) for i in range(3))
    an2 = (cria_animal('wolf', 10, 3),)
    pos = tuple(cria_posicao(p[0], p[1])  for p in ((2, 2), (4, 3), (10, 2), (3, 2)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    prado_para_str(prado)
    # +----------+\\n|..........|\\n|.sW@@....s|\\n|...s......|\\n+----------+

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('sheep', 2, 0) for i in range(3))
    an2 = (cria_animal('wolf', 10, 3),)
    pos = tuple(cria_posicao(p[0], p[1])  for p in ((2, 2), (4, 3), (10, 2), (3, 2)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    prado_para_str(geracao(prado))
    # +----------+\\n|..W.......|\\n|s..@@.....|\\n|....s....s|\\n+----------+

    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('sheep', 2, 0) for i in range(3))
    an2 = (cria_animal('wolf', 10, 3),)
    pos = tuple(cria_posicao(p[0], p[1])  for p in ((2, 2), (4, 3), (10, 2), (3, 2)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    prado_para_str(geracao(geracao(prado)))
    # +----------+\\n|...W......|\\n|ss.@@....s|\\n|...ss....s|\\n+----------+


    dim = cria_posicao(11, 4)
    obs = (cria_posicao(4, 2), cria_posicao(5, 2))
    an1 = tuple(cria_animal('sheep', 2, 0) for i in range(3))
    an2 = (cria_animal('wolf', 10, 3),)
    pos = tuple(cria_posicao(p[0], p[1])  for p in ((2, 2), (4, 3), (10, 2), (3, 2)))
    prado = cria_prado(dim, obs, an1 + an2, pos)
    prado_para_str(geracao(geracao(geracao(prado))))
    # +----------+\\n|.........s|\\n|...@@....s|\\n|ssss......|\\n+----------+


    return


def teste222():
    total_score = 0
    fun_name = simula_ecossistema_public

    path = '/home/fpshak/data/contests/FP2122P2/'
    simula_ecossistema(path + 'public_test_config.txt', 20, False)
    # Predadores: 1 vs Presas: 3 (Gen. 0)\n+----------+\n|..........|\n|.mL@@....m|\n|...m......|\n+----------+\nPredadores: 0 vs Presas: 28 (Gen. 20)\n+----------+\n|mmmmmmmmmm|\n|mmm@@mmmmm|\n|mmmmmmmmmm|\n+----------+\n(0, 28)

    path = '/home/fpshak/data/contests/FP2122P2/'
    simula_ecossistema(path + 'public_test_config.txt', 20, True)
    # Predadores: 1 vs Presas: 3 (Gen. 0)\n+----------+\n|..........|\n|.mL@@....m|\n|...m......|\n+----------+\nPredadores: 1 vs Presas: 6 (Gen. 2)\n+----------+\n|...L......|\n|mm.@@....m|\n|...mm....m|\n+----------+\nPredadores: 0 vs Presas: 6 (Gen. 3)\n+----------+\n|.........m|\n|...@@....m|\n|mmmm......|\n+----------+\nPredadores: 0 vs Presas: 12 (Gen. 4)\n+----------+\n|........mm|\n|mmm@@....m|\n|mmmmm....m|\n+----------+\nPredadores: 0 vs Presas: 18 (Gen. 6)\n+----------+\n|mmm....mmm|\n|mmm@@..mmm|\n|mmmmm....m|\n+----------+\nPredadores: 0 vs Presas: 20 (Gen. 7)\n+----------+\n|mmmm..mmmm|\n|mmm@@..mmm|\n|mmmm.m.m..|\n+----------+\nPredadores: 0 vs Presas: 28 (Gen. 8)\n+----------+\n|mmmmmmmmmm|\n|mmm@@mmmmm|\n|mmmmmmmmmm|\n+----------+\n(0, 28)

    return
