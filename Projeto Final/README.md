**=======================================================================**
 __APLICATIVO DE CHAT__
**=======================================================================**
 Universidade de Brasília
 campus Gama

 Versão: rev 1.63 (PC1)
 Autor: Arthur Evangelista
 Matrícula: 14/0016686

**=======================================================================**
Para compilar este código, insira a linha abaixo
no terminal de sua distribuição linux:

gcc main.c -o a.out -lrt -lpthread

ou use o makefile

E em seguida execute a aplicação com:
 ./a.out
=======================================================================
## Informações relevantes:
* Implementado o envio de mensagens (+-)
* Implementado o comando "sair"
* Implementado o comando "list"
* Implementado sinal de notificação para recebimento de mensagens (+-)
* Falta adicionar quem enviou qual mensagem e de quem veio o
Broadcast. Para implementar esta funcionalidade, deverá ser enviada,
junto com a mensagem, o indivíduo locutor. Ou seja, será alterada a
thread t2 (envio de msg) para que t1 (recebimento de msg) possa ser
alterada para receber este protocolo.
* Falta resolução de bugs e segfault inesperado melhor explicado a seguir.

## Problemas conhecidos:
   * Sob certas condições (ainda não descobertas quais) o código
finaliza com segfault ou double free. Acredita-se que seja algum problema
na implementação do envio de mensagens.
   * A thread de recebimento de mensagem não possui comportamento adequado
(não printa a mensagem na tela como seria de se esperar). Acredita-se que
uma thread deva ser chamada para ler todas as mensagens antes do loop
principal para limpar a fila de msgs antes que mq_notify seja chamado.
   * Inserir chave mutex ou semáforos para a thread de envio/recebimento
de mensagens. Do jeito que está implementada, ela poderá entrar em
condição de corrida caso duas threads tentem enviar uma mensagem para
o mesmo destinatário!
   * Retirar na linha 383 (próximo ao fim do segundo e primeiro if-else
do console) o pthread_join() pois isto irá bloquear o prosseguimento do
código caso alguma mensagem não seja enviada e tenha que esperar as 3
tentativas de envio!

##Dificuldades de implementação
As duas maiores dificuldades de implementação foi a listagem de usuários (e por$

* Listagem e Broadcast
        * A implementação das duas funcionalidades foi dificultada pela busca
pela manipulação dos arquivos no diretório em que a fila de mensagens fica
aberta. Foi descoberto que o diretório /dev/mqueue possuia os arquivos de
fila de mensagens e com a biblioteca dirent.h é possível contar os arquivos
e printar seus respectivos nomes (que, com algumas manipulações de strings
(ainda não implementadas) podem se tornar os nomes dos usuários online)

* Recebimento de Mensagens
        * Esta thread tem dado muito trabalho. Até a implementação dela,
tudo parecia funcionar bem e não possuia segfault ou double free. Foi
implementada uma thread que seria chamada toda vez que mq_notify notificasse
sobre uma mensagem nova. Esta função mq_notify recebe o descritor de fila de
mensagens em conjunto com uma estrutura de dados do tipo sigevent.
Primeiramente, nesta implementação, toda vez que a thread é chamada ela
realiza a leitura de todas as mensagens novas até que a fila de mensagens
esteja vazia. Isto foi realizado para evitar que o número máximo de threads
seja atingido com facilidade (do caso contrário, toda msg nova iria
chamar uma thread nova). Em segundo lugar, ela não apresenta o comportamento
esperado de apresentar todas as mensagens. Na realidade ela não apresenta
nenhuma msg, apenas a string "dest" que mais adiante será substituído pelo nome
de quem envio a string. Isso quando não ocorre segfault ou double free antes
mesmo que esta thread receba qualquer sinal.

**=====================================================================**


