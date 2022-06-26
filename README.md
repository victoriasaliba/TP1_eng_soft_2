# Trabalho Prático 1 - Engenharia de Software 2

Isabelle Vieira Barbosa, Leandro Marques Venceslau de Souza, Victória Rocha Saliba

## Explicação do sistema

Trata-se de um sistema modelo cliente-servidor que simula a
interação entre uma Estação Remota (o servidor) e a Central de Monitoramento (o cliente). O sistema permite a comunicação entre esses dois processos, através de Sockets. 
A Estação Remota atende às mensagens da Central de Monitoramento, que pode solicitar ações
como:
- Instalar um ou mais sensores;
- Remover um ou mais sensores;
- Consultar equipamento;
- Consultar variável de processo de um ou mais sensores.

As versões do cliente e do servidor utilizam o protocolo TCP, sendo possível utilizar tanto o IPv4 quanto o
IPv6.


## Explicação das tecnologias usadas

O sistema foi implementado com a linguagem C, com foco em bibliotecas como _string.h_, _stdlib.h_, _stdio.h_ 
e outras focadas na parte dos sockets como _arpa/inet.h_, _unistd.h_, _sys/socket.h_, _sys/types.h_ e _inttypes.h_. 

Os testes unitários foram feitos através do framework [Sput-Unit](https://www.use-strict.de/sput-unit-testing/), e podem ser rodados com `make testes-unitarios`.

Já os testes de sistema simulam a entrada do usuário (programa cliente) e compara o retorno do servidor com a saída esperada. Para isso, foi utilizado um script, que pode ser rodado com `make testes-sistema`.

Para rodar utilizamos dois terminais Ubuntu 18.04.6 LTS.


## Exemplo de uso

- Execute o Makefile com `make`
- Em seguida, execute o programa do servidor no terminal com `./server v6 51511` (para IPv6) ou `./server v4 51511` (para IPv4)
- Em outro terminal, execute `./client ::1 51511` (IPv6) ou `./client 127.0.0.1 51511` (IPv4). A conexão deve ser iniciada.
- No terminal do cliente, digite os comandos:
  - Adicionar sensores: `add sensor [SensorId] in [equipmentId]`
      , exemplo: `add sensor 01 02 03 04 in 01`
  - Remover sensores: `remove sensor [SensorId] in [equipmentId]`
      , exemplo: `remove sensor 01 03 in 01`
  - Consultar equipamento: `list sensors in [equipmentId]`
      , exemplo: `list sensors in 01`
  - Consultar variável de processo: `read [SensorId] in [equipmentId]`
      , exemplo: `read 02 04 in 01`
- Para encerrar o programa, basta enviar `kill` a qualquer momento.
