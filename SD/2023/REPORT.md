# Sistemas Distribuídos 2022/2023

## Relatório Terceira Entrega do Projecto

**Grupo A55**

Nesta terceira entrega, procurámos implementar a arquitectura gossip para dar
garantias de coerência fraca e alta disponibilidade ao DistLedger.

Começámos por definir um vector clock ligado ao cliente (User), que é uma lista
com três posições. Cada posição corresponde ao timestamp específico de uma réplica.

Do lado das réplicas definimos também o seu próprio vector clock que estas mantêm, 
com o seu timestamp e o das outras réplicas que conheça.

Ao efectuar um pedido de escrita o cliente envia o seu vector clock à respectiva réplica.
Ao receber este pedido a réplica irá primeiro ver se o pedido não é duplicado, 
descartando-o caso o seja. De seguida a réplica incrementa o seu timestamp, prepara a operação 
e adiciona-a ao ledger com a indicação de que não foi executada ainda.
A réplica responde de volta ao cliente, incluindo o seu timestamp actualizado no vector clock 
do cliente.
De seguida, a réplica executa a operação se o vector clock do cliente for menor ou igual ao seu.
Caso contrário, entra em espera, até ter um gossip com actualizações que de outra réplica que lhe 
permita avançar.

Quando recebe um pedido de leitura, a réplica verifica a relação causal do pedido, verificando se 
o vector clock do cliente é menor ou igual que o próprio.
Se não existir relação causal, é enviada a resposta imediatamente.
Caso contrário, a réplica entra em espera, até receber gossip com actualização.

Quando recebe resposta a um pedido de escrita, o cliente actualiza o seu vector clock com o novo vector 
clock enviado pela réplica. Este vector clock é único.

O admin pode a qualquer momento efectuar um pedido de gossip a uma das réplicas.
Assim que uma réplica recebe esse pedido irá enviar à outra o seu ledger, para que a outra se 
possa actualizar com as operações que ainda não viu.
Quando uma réplica recebe um gossip irá ver se, por cada operação recebida, esta não é duplicada.

A réplica deveria também actualizar o seu vector clock de acordo com as operações que adiciona ao ledger 
e executa.

Infelizmente não conseguimos concluír esta última etapa para obter um funcionamento correcto e de acordo 
com o algoritmo Gossip.
