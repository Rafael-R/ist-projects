# Bake-off 3 - Escrita em Smartwatches

**Disponível:** 20 Abril 2020  
**Sessão de bake-off:** aula de laboratório da semana de 18 de Maio  
**Submissão via Twitter:** exclusivamente no dia 22 de Maio, até às 23h59  
**Desafio:** diminuir o tempo de escrita numa interface para smartwatches  
**Resultado esperado:** interface que minimize o tempo de escrita de texto numa área com 40mmx40mm  
**Avaliação:** 0-20 valores; 10 valores qualidade do processo de desenho, 10 valores dependentes do número médio de palavras por minuto  

## Desafio

O objetivo deste bake-off é desenhar e implementar uma técnica de escrita de texto em smartwatches. É disponibilizado o código-fonte em Processing que: mostra um teclado navegacional com duas direções e uma caixa de texto, indica qual a frase a transcrever (i.e. frase objetivo); quantifica o desempenho do utilizador imprimindo as palavras por minuto e taxa de erros. **Para vencer este bake-off têm de alterar o código fonte fornecido para que os utilizadores escrevam texto o mais rapidamente possível** - atenção, taxa de erros acima de 5% serão penalizadas.  

**Os caracteres disponíveis incluem apenas letras de 'a' - 'z', espaço ('_') e apagar (' ´ ')**. Não são necessários outros caracteres para transcrever as frases objetivo (maiúsculas, símbolos, números, acentos, etc.). Ao desenvolverem a vossa solução, não podem aceder à frase objetivo. Têm apenas acesso aos caracteres que já foram escritos pelo utilizador.  

**Têm de calcular e imprimir uma métrica adicional: Caracteres por Segundo**.

## Funcionamento

O bake-off é um desafio de design em aberto. É crucial que iniciem um **processo iterativo** de geração e teste de ideias desde o primeiro dia. A vossa solução tem de obedecer às seguintes **regras**:

1. **Não podem** alterar o tamanho do ecrã (40mm x 40mm) ou teclado (40mm x 30mm);
2. **Não podem** alterar o código que simula a ocultação e seleção com um dedo (a área de toque do dedo simula o 'fat finger problem' e a incerteza da seleção);
3. **Não podem** aceder à frase objetivo ou ao corpus de frases durante a escrita. Ou seja, a solução não sabe à priori o que o utilizador quer escrever, mas pode tentar prever;
4. **Não podem** usar mais nenhum botão para além do botão esquerdo do rato.

**As mudanças ao código fonte estão restringidas à área do teclado (40mm x 30mm). Mais, apenas os toques dentro desta área são válidos.**

É esperado que **discutam as vossas ideias com o docente do laboratório para garantirem que não quebram nenhuma das regras**.
