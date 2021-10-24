# Guião de Demonstração

## 1. Preparação do sistema

Para testar o sistema e todos os seus componentes, é necessário preparar um ambiente com dados para proceder à verificação dos testes.

### 1.1. Lançar o _registry_

Para lançar o _ZooKeeper_, ir à pasta `zookeeper/bin` e correr o comando  
`./zkServer.sh start` (Linux) ou `zkServer.cmd` (Windows).

É possível também lançar a consola de interação com o _ZooKeeper_, novamente na pasta `zookeeper/bin` e correr `./zkCli.sh` (Linux) ou `zkCli.cmd` (Windows).

### 1.2. Compilar o projeto

Primeiramente, é necessário compilar e instalar todos os módulos e suas dependências -- _rec_, _hub_, _app_, etc.
Para isso, basta ir à pasta _root_ do projeto e correr o seguinte comando:

```sh
$ mvn clean install -DskipTests
```

### 1.3. Lançar e testar as réplicas do servidor _rec_

Para proceder aos testes é preciso, em primeiro lugar, lançar duas réplicas do servidor _rec_.
Para isso basta ir à pasta _rec_ e executar:

```sh
$ rec localhost 2181 localhost 8091 1
```

De seguida abrir outra consola na mesma pasta e executar:

```sh
$ rec localhost 2181 localhost 8092 2
```

**Nota:** Para poder correr o script _app_ diretamente é necessário fazer `mvn install` e adicionar ao _PATH_ ou
utilizar diretamente os executáveis gerados na pasta `target/appassembler/bin/`.

Estes comandos irão colocar uma das réplicas do servidor _rec_ no endereço _localhost_ e na porta _8091_ e a outra 
réplica no endereço _localhost_ e na porta _8092_.

Para confirmar o funcionamento das réplicas com um _ping_, fazer:

```sh
$ cd rec-tester
$ mvn compile exec:java
```

Para executar toda a bateria de testes de integração, fazer:

```sh
$ mvn verify
```

Todos os testes devem ser executados sem erros.

### 1.4. Lançar e testar o servidor _hub_

Para proceder aos testes, é preciso em primeiro lugar lançar duas réplicas do servidor _rec_ seguindo
os passos indicados no ponto
[1.3](https://git.rnl.tecnico.ulisboa.pt/SD-20-21-2/A03-Bicloin/src/branch/main/demo#1-3-lan%C3%A7ar-e-testar-o-_rec_).

De seguida, é preciso lançar o servidor _hub_.
Para isso basta ir à pasta _hub_ e executar:

```sh
$ mvn compile exec:java
```

Este comando vai colocar o servidor _hub_ no endereço _localhost_ e na porta _8081_.

Para confirmar o funcionamento do servidor com um _ping_, fazer:

```sh
$ cd hub-tester
$ mvn compile exec:java
```

Para executar toda a bateria de testes de integração, fazer:

```sh
$ mvn verify
```

Todos os testes devem ser executados sem erros.

### 1.5. _App_

Iniciar a aplicação com a utilizadora alice:

```sh
$ app localhost 2181 alice +35191102030 38.7380 -9.3000
```

**Nota:** Para poder correr o script _app_ diretamente é necessário fazer `mvn install` e adicionar ao _PATH_ ou 
utilizar diretamente os executáveis gerados na pasta `target/appassembler/bin/`.

Abrir outra consola, e iniciar a aplicação com o utilizador bruno.

Depois de lançar todos os componentes, tal como descrito acima, já temos o que é necessário para usar o sistema através dos comandos.

## 2. Teste dos comandos

Nesta secção vamos correr os comandos necessários para testar todas as operações do sistema.
Cada subsecção é respetiva a cada operação presente no _hub_.

### 2.1. _balance_

#### 2.1.1 caso normal:

```sh
# input:
> balance

# output:
alise 0 BIC
```

### 2.2 _top-up_

#### 2.2.1 caso normal:

```sh
# input:
> top-up 1

# output:
alice 10 BIC
```

#### 2.2.2 montante inválido:

```sh
# input:
> top-up 25

# output:
INVALID_ARGUMENT: Invalid amount.
```

#### 2.2.3 input inválido:

```sh
#input
> top-up        # caso 1
> top-up top    # caso 2

# output
'top-up <amount>': increases your current balance by <amount>*10 BIC    # caso 1
Usage: top-up <num>, <num> has to be an integer.                        # caso 2
```

### 2.3 _tag_

#### 2.3.1 caso normal:

```sh
# input
> tag 38.7376 -9.3031 loc1

# output
OK
```

#### 2.3.2 input inválido:

```sh
#input
> tag                         # caso 1
> tag 38.7376                 # caso 1
> tag 38.7376 -9.3031         # caso 1
> tag loc1 38.7376 -9.3031    # caso 2

# output
'tag <lat> <long> <name>': creates a tag with name <name>, latitude <lat> and longitude <long>    # caso 1
Usage: tag <lat> <long> <name>, <lat> and <long> must be doubles.                                 # caso 2
```

### 2.4 _move_

#### 2.4.1 caso normal:

```sh
# input:
> move 38.7376 -9.3031        # caso 1
> tag 38.7376 -9.3031 loc1
> move loc1                   # caso 2

# output:
alice em https://www.google.com/maps/place/38.7376,-9.3031    # caso 1
OK
alice em https://www.google.com/maps/place/38.7376,-9.3031    # caso 2
```

#### 2.4.2 input inválido

```sh
# input:
> move                  # caso 1
> move string string    # caso 2

# output:
'move <tag_name>': moves your position to <tag_name> position if it exists
'move <lat> <long>': moves your position to latitude <lat> and longitude <long>   # caso 1
Usage: move <lat> <long>, <lat> and <long> must be doubles.                       # caso 2
```

### 2.5 _at_

#### 2.5.1 caso normal:

```sh
# input:
> move 38.7376 -9.3031
> at

# output:
alice em https://www.google.com/maps/place/38.7376,-9.3031
alice em https://www.google.com/maps/place/38.7376,-9.3031
```

### 2.6 _scan_

#### 2.6.1 caso normal:

```sh
# input:
> move 38.7376 -9.3031
> scan 2

# output:
alice em https://www.google.com/maps/place/38.7376,-9.3031
istt, lat 38.7372, -9.3023 long, 20 docas, 4 BIC prémio, 12 bicicletas, a 82 metros
stao, lat 38.6867, -9.3124 long, 30 docas, 3 BIC prémio, 20 bicicletas, a 5717 metros
```

#### 2.6.2 input inválido:

```sh
# input:
> scan        # caso 1
> scan scan   # caso 2

# output:
'scan <num>': returns information on the <num> closest stations or the information off all stations if <num> > number of stations   # caso 1
Usage: scan <num_stations>, <num_stations> has to be an integer.                                                                    # caso 2
```

### 2.7 _info_

#### 2.7.1 caso normal:

```sh
# input:
> info istt

# output:
IST Taguspark, lat 38.7372, -9.3023 long, 20 docas, 4 BIC prémio, 12 bicicletas, 22 levantamentos, 7 devoluções, https://www.google.com/maps/place/38.7372,-9.3023
```

#### 2.7.2 estação inválida:

```sh
# input:
> info isttt

# output:
INVALID_ARGUMENT: Invalid station
```

#### 2.7.3 input inválido:

```sh
#input
> info

# output
'info <station_id>': returns information on the station with id <station_id>
```

### 2.8 _bike-up_

#### 2.8.1 caso normal:

```sh
# input:
> top-up 10
> move 38.7376 -9.3031
> bike-up istt

# output:
alice 100 BIC
alice em https://www.google.com/maps/place/38.7376,-9.3031
OK
```

#### 2.8.3 estação inválida:

```sh
# input:_
> bike-up isttt

# output:
INVALID_ARGUMENT: Invalid station.
```

#### 2.8.4 pedido de bike-up a mais de 200m da estação:

```sh
# input:_
> move 38.6867 -9.3117
> bike-up istt

# output:
alice em https://www.google.com/maps/place/38.6867,-9.3117
ERRO fora de alcance
```

#### 2.8.5 pedido de bike-up com um saldo inferior a 10 bicloins:

```sh
# input:
> balance
> bike-up istt

# output:
alice 0 BIC
ERRO bicloins insuficientes
```

#### 2.8.6 pedido de bike-up quando o utilizador já tem uma bicicleta:

```sh
# input:
> bike-up istt
> bike-up istt

# output:
OK
ERRO já tem uma bicicleta
```

#### 2.8.7 pedido de bike-up sem bicicletas disponíveis na estação:

```sh
# input:
> move 38.7097 -9.1336
> bike-up istt

# output:
alice em https://www.google.com/maps/place/38.7097,-9.1336
ERRO a estação não tem bicicletas
```

### 2.9 _bike-down_

#### 2.9.1 caso normal:

```sh
# input:_
> top-up 10
> move 38.7376 -9.3031
> bike-up istt
> bike-down istt

# output:
alice 100 BIC
alice em https://www.google.com/maps/place/38.7376,-9.3031
OK
OK
```

#### 2.9.3 estação inválida:

```sh
# input:
> bike-down isttt

# output:
INVALID_ARGUMENT: Invalid station.
```

#### 2.9.4 pedido de bike-down a mais de 200m da estação:

```sh
# input:
> move 38.6867 -9.3117
> bike-down istt

# output:
alice em https://www.google.com/maps/place/38.6867,-9.3117
ERRO fora de alcance
```

#### 2.9.6 pedido de bike-down quando o utilizador não tem uma bicicleta:

```sh
# input:
> move 38.7376 -9.3031
> bike-down istt

# output:
alice em https://www.google.com/maps/place/38.7376,-9.3031
ERRO não tem uma bicicleta
```

#### 2.9.7 pedido de bike-down sem docas disponíveis na estação:

```sh
# input:
> move 38.7372 -9.3023
> bike-up istt
> move 38.7376 -9.1545
> bike-down gulb

# output:
alice em https://www.google.com/maps/place/38.7372,-9.3023
OK
alice em https://www.google.com/maps/place/38.7376,-9.1545
ERRO a estação não tem docas livres
```

### 2.10 _ping_

#### 2.10.1 caso normal:

```sh
# input:
> ping

# output:
Hello app!
```

### 2.11 _sys_status_

#### 2.11.1 caso normal: 
(utilizando duas réplicas do servidor rec)

```sh
# input:
> sys_status

# output:
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 UP
```

## 3. Testes da Replicação

#### 3.1 Caso normal sem faltas:

```sh
# input app:
> sys_status
> top-up 10

# output app:
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 UP
alice 100 BIC

# output hub:
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Received reply from replica 2.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Received read response from replica 2.
Frontend received answer with tag 0.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Received read response from replica 2.
Frontend received answer with tag 0.
Contacting replica 1 at localhost:8091. Sending write request...
Contacting replica 2 at localhost:8092. Sending write request...
Received write response from replica 1.
Received write response from replica 2.


#utput rec:
Received ping request
Received read request for user: alice
Received read request for user: alice
Received write request for user: alice
Data written
```

#### 3.2 Congelar e resumir o processo do rec2 com SIGTSTP e SIGCONT:

```sh
# input na app:
> sys_status

# input rec2:
$ SIGSTP

# input na app:
> sys_status
> top-up 10

# input rec2:
$ SIGCONT

# input na app:
> sys_status
> balance

# output app:
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 DOWN
alice 100 BIC
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 UP
alice 100 BIC

# output hub:
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Received reply from replica 2.
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Couldn't get ping response from replica 2.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Couldn't get read response from replica 2.
Frontend received answer with tag 0.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Couldn't get read response from replica 2.
Frontend received answer with tag 0.
Contacting replica 1 at localhost:8091. Sending write request...
Contacting replica 2 at localhost:8092. Sending write request...
Received write response from replica 1.
Couldn't get write response from replica 2.
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Received reply from replica 2.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Received read response from replica 2.
Frontend received answer with tag 1.

# output rec2:
Received ping request
^Z
[1]  + 36233 suspended  ./target/appassembler/bin/rec localhost 2181 localhost 8092 2

$ fg
[1]  + 36233 continued  ./target/appassembler/bin/rec localhost 2181 localhost 8092 2
Received ping request
Received read request for user: alice
```

#### 3.3 Terminar o processo do rec2 com SIGTERM:

```sh
# input na app:
> sys_status
> top-up 10

# input rec2:
$ SIGKTERM

#input na app:
> sys_status
> balance

# output app:
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 UP
alice 100 BIC
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP
alice 100 BIC

# output hub:
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Received reply from replica 2.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Received read response from replica 2.
Frontend received answer with tag 1.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Received read response from replica 2.
Frontend received answer with tag 1.
Contacting replica 1 at localhost:8091. Sending write request...
Contacting replica 2 at localhost:8092. Sending write request...
Received write response from replica 1.
Received write response from replica 2.
Contacting replica 1 at localhost:8091. Sending ping request...
Received reply from replica 1.
Contacting replica 1 at localhost:8091. Sending read request...
Received read response from replica 1.
Frontend received answer with tag 2.

# output rec2:
Received ping request
Received read request for user: alice
Received read request for user: alice
Received write request for user: alice
Data written
^CCaught signal
Unbinding rec from ZooKeeper
```

#### 3.4 Terminar o processo do rec2 com SIGKILL:

```sh
# input na app:_
> sys_status
> top-up 10

# input rec2:
$ SIGKILL

# input na app:
> sys_status
> balance

# output app:
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 UP
alice 100 BIC
/grpc/bicloin/hub/1 UP | /grpc/bicloin/rec/1 UP | /grpc/bicloin/rec/2 DOWN
alice 100 BIC

# output hub:
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Received reply from replica 2.
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Couldn't get ping response from replica 2.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Couldn't get read response from replica 2.
Frontend received answer with tag 0.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Couldn't get read response from replica 2.
Frontend received answer with tag 0.
Contacting replica 1 at localhost:8091. Sending write request...
Contacting replica 2 at localhost:8092. Sending write request...
Received write response from replica 1.
Couldn't get write response from replica 2.
Contacting replica 1 at localhost:8091. Sending ping request...
Contacting replica 2 at localhost:8092. Sending ping request...
Received reply from replica 1.
Couldn't get ping response from replica 2.
Contacting replica 1 at localhost:8091. Sending read request...
Contacting replica 2 at localhost:8092. Sending read request...
Received read response from replica 1.
Couldn't get read response from replica 2.
Frontend received answer with tag 1.

# output rec:
Received ping request
Received read request for user: alice
Received read request for user: alice
Received write request for user: alice
Data written
[1]    rec_pid killed     rec localhost .....
```

## 4. Considerações Finais

Estes testes não cobrem tudo, pelo que devem ter sempre em conta os testes de integração e o código.
