AMS 2022/2023 – Enunciado do Projeto

---

# Serviço PICKTRU – Representação Arquitetural a Desenvolver

Pretende-se desenvolver, segundo o UoD fornecido, uma representação arquitetural do produto PICKTRU a partir da qual seja possível criar duas vistas, uma para informar partes interessadas ao nível do negócio e outra para informar partes interessadas ao nível da tecnologia.

*Cenários não referidos explicitamente no UoD, ou que não se possam deduzir dele, mas que uma conclusão de senso comum aponte de  que num cenário de uso real deveriam ser considerados, devem ser apresentados para discussão, idealmente no canal no serviço  Moodle existente para este efeito, ou presencialmente nas aulas.*

**Em concreto, os diagramas a desenvolver devem ser:**

1. (30%) Vista de Negócio
    1. **(10%) Diagrama de Vista Geral do Negócio:** Diagrama em linguagem ArchiMate, alinhado com as recomendações para um "Layered Viewpoint", fazendo uso apenas de elementos das camadas "Business" e "Application".
    2. **(10%) Diagrama do Processo P-SET:** Diagrama, em linguagem BPMN, do processo P-SET, representando o fluxo privado de atividades do D-PRJ da PICK.
    3. **(10% Entrega1) Diagrama do Processo P-ON:** Diagrama, em linguagem BPMN, do processo P-ON, representando todas as colaborações envolvendo entre si as várias equipas da PICK, as colaborações destas com o cliente, e as atividades que sejam causa ou que resultem diretamente dessas colaborações.
2. (60%) Vista de Tecnologia e (10%) Integração de Modelos
    1. **(10%) Integração dos modelos** da "Vista de Negócio" (i.e., diagramas 1.i, 1.ii, 1.iii atualizados após o feedback da primeira entrega) na ferramenta "Enterprise Architect".
    2. **(10%) Diagramas de Casos de Uso** (em UML), representando em dois diagramas independentes, mas alinhados entre si, os casos de uso dos sistemas RUN e STORE.
    3. **(20%) Diagrama de Classes** (em UML) do modelo de domínio da aplicação RUN. Deve ser considerado neste diagrama, e entre outros elementos, uma classe que represente de forma genérica cada artigo à venda numa loja, tendo ainda o cuidado de se declararem explicitamente enumerados e tipos não primitivos da linguagem UML que sejam representados no diagrama.
    4. **(10%) Diagrama de Máquina de Estados** (em UML) que represente o ciclo de vida dos objetos que, no modelo de domínio da aplicação RUN, representem as zonas dos artigos à venda numa loja.
    5. **(10%) Diagrama de Blocos** (em SysML) que represente o sistema STORE, tendo o cuidado incluir explicitamente todos os tipos não primitivos da linguagem SysML que sejam representados no diagrama.
    6. **(10%) Diagrama Interno de Blocos** (em SysML) que represente o sistema STORE. Este diagrama deve estar alinhado com o
diagrama de bloco (produzido em 2.v) e com o diagrama de casos de uso da aplicação SMART, que faz parte do mesmo sistema.

As entregas do projeto devem consistir em:

- Todas a entregas: Ficheiro PDF com uma tabela com a identificação do grupo na primeira página, indicando para cada aluno o número estimado de horas de trabalho direto para a entrega, seguindo-se todos os diagramas na ordem indicada na definição da entrega.
- E1 - Entrega 1:
  - Ficheiro PDF com todos os diagramas da "Vista de Negócio".
  - Ficheiro comprimido (idealmente em formato ".zip") contendo os dois ficheiros fonte das ferramentas Archi e Camunda usados para a criação de cada um dos diagramas.
- E2 – Entrega 2:
  - Ficheiro PDF com todos os diagramas do projeto (diagramas 2.i a 2.vi).
  - Ficheiro fonte da ferramenta EA com os dados de todos os diagramas do projeto.
