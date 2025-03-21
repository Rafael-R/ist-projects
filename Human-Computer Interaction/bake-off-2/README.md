# Bake-off 2 - Seleção de Alvos e Fatores Humanos

**Disponível:** 23 Março 2020  
**Sessão de bake-off:** aula de laboratório da semana de 20 de Abril  
**Submissão via Twitter:** exclusivamente no dia 24 de Abril, até às 23h59  
**Desafio:** diminuir o tempo de seleção de alvos numa interface abstrata  
**Resultado esperado:** interface que minimize o tempo de seleção de alvos circulares numa grelha de 4 x 4  
**Avaliação:** 0-20 valores; 10 valores para qualidade do processo de desenho, 10 valores dependentes do tempo de seleção médio dos alvos

## Desafio

O objetivo deste bake-off é diminuir o tempo de seleção de alvos numa interface abstrata. É disponibilizado o código-fonte em Processing que: (1) mostra uma grelha de 4x4 alvos; (2) indica qual o alvo a selecionar; (3) quantifica o desempenho do utilizador imprimindo o tempo de seleção (em ms) e taxa de sucesso (0% - 100%). **Para vencer este bake-off têm de alterar o código fonte fornecido para que os utilizadores selecionem os alvos o mais rapidamente possível -** atenção, soluções com taxas de sucesso abaixo de 95% serão penalizadas!

Ao desenvolverem a vossa solução, podem aceder à lista de alvos a selecionar. No entanto, **só podem aceder ao alvo atual (i), próximo alvo (i+1) e alvo anterior (i-1)**.

Têm de calcular e imprimir uma métrica adicional: Fitts Index of Performance (Índice de Dificuldade). Devem usar a fórmula proposta por Mackenzie,  log2 (distância-ao-alvo / largura-do-alvo + 1) - ver aula teórica 8 e secção 2.1.4 do Livro - O Movimento. Devem imprimir um valor para cada seleção bem sucedida.

## Funcionamento

O bake-off é um desafio de design em aberto. É crucial que iniciem um **processo iterativo** de geração e teste de ideias desde o primeiro dia. A vossa solução tem de obedecer às seguintes **regras**:

- **_Não podem_** colocar alvos invisíveis ou impossíveis de selecionar;
- **_Não podem_** alterar a hitbox dos alvos dependendo do alvo a selecionar; isto é, alterações às hitboxs terão de ter o mesmo comportamento para todos os alvos;
- **_Não podem_** fazer alterações ao movimento do cursor que seja dependente do alvo a selecionar; isto é, alterações ao movimento do cursor terão de ter o mesmo comportamento para todos os alvos;
- **_Não podem_** usar hardware adicional para além do rato;
- **_Não podem_** modificar o código que calcula as métricas de desempenho existentes;

É esperado que **discutam as vossas ideias com o docente do laboratório para garantirem que não quebram nenhuma das regras.**

Lembrem-se, o vosso objetivo de desenho é **minimizar o tempo de seleção** (ver aulas de Fatores Humanos, Capítulo 2 - Nós, os Humanos - e secção 10.3 - Avaliação Preditiva).
