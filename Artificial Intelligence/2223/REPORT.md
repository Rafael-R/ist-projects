# Report

## Classe Board

A classe Board é composta por uma matriz onde guarda os valores já preenchidos na sua respetiva posição, tem quatro listas onde guarda o número de partes de barco que faltam e o número de posições vazias, por linha e por coluna. Além disso tem um dicionário onde guarda as posições dos barcos já descobertos e uma lista onde guarda posições de partes de barco que ainda não pertencem a nenhum barco descoberto.

## Leitura de input

Começo por fazer a leitura do input para criar um Board.
Ao fazer a leitura do input guardamos as partes de barco na lista hints antes de as inserirmos na matriz utilizando o método set_value.

O método set_value para além de introduzir o valor, preenche também as posições adjacentes com água, consoante o valor, e verifica se essa posição pertence a um barco. Caso pertença remove as posições do barco da lista de hints.

Depois de inserir todas as hints utilizo o método process para preencher com água linhas e colunas em que não seja possível inserir mais partes de barco, e preencher com partes de barco linhas e colunas onde não seja possível inserir mais água. Este método é executado recursivamente até não serem efetuadas mais alterações.

## Técnica de pesquisa

Inicialmente optei por usar DFS para a pesquisa pois pareceu-me ser a técnica mais intuitiva para perceber o que acontecia em cada nó, com o intuito de alterar posteriormente, mas visto que consegui obter a nota máxima no Mooshak com esta técnica acabei por não o fazer, não tendo por isso definido nenhuma heurística.

### Actions

O método actions começa por fazer uma cópia do Board atual.

1° Passo: Verifica para cada posição na lista de hints quais as posições adjacentes que podem ser preenchidas, inserindo um valor indefinido e guardando essas posições numa lista. Preencher estas posições é útil para o passo seguinte.

2° Passo: De seguida processa o board à semelhança do que foi feito no método process, mas desta vez apenas marca os valores como indefinidos e acrescenta as posições à lista de indefinidos.

3° Passo: Percorre a lista de posições indefinidas e tenta adivinhar que valores poderiam estar naquela posição utilizando o método guess. Se o método guess retornar um único valor este é adicionado á lista de posições corretas. Caso retorne múltiplas posições estas são adicionadas à lista de possibilidades. Caso não retorne nada apaga o valor indefinido da matriz, pois posições vazias serão avaliadas mais tarde.

4° Passo: Se a lista de posições corretas não for vazia retorna este conjunto de ações para ser executado de uma vez, ou no caso em que é vazia retorna a lista de possibilidades. Caso esta lista também seja vazia passa ao passo seguinte.

5° Passo: Verifica em que posições poderia inserir um barco, do maior para o mais pequeno, retorna uma lista com estas possibilidades.

Se não encontrar nenhuma jogada válida significa que este caminho não é valido.

### Result

O método result começa por fazer uma cópia do Board do estado que recebe. A esta cópia aplica a ação ou ações recebidas.
Depois de aplicar todas as ações processa o board utilizando o método process. Retorna um novo estado com este Board.

### Goal test

Verifica que todas as linhas e colunas têm o número certo de partes de barco e água preenchidas e que o número de barcos de cada dimensão corresponde ao esperado.
