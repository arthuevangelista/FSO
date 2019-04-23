/* =======================================================================
 * EXERCÍCIO DE FILAS DE MENSAGENS
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Autor: Arthur Evangelista
 * Matrícula: 14/0016686
 *
 * Apenas um exemplo de passagem de mensagem por fila de mensagem
 * Este código utiliza processos
 *
 * gcc exemplo.c -lrt -o out
 *
 * get_ticket
 - abre a fila
 - manda msg pedindo ticket
 - espera a resposta
 - fecha a fila
 - return ticket
 *
 * return_ticket
 - abre a fila
 - manda msg com o ticket devolvido t
 - fecha a fila
 *
 * servidor
 - abre e cria a fila
 - loop infinito
 - espera msg
 - se é pede ticket
    entrega um ticket()
      escolhe um ticket dea fila/pilha de ticket
      envia msg com o ticket
 - senão
    devolve um ticket()
      coloca o ticket na fila/pilha de tickets
 *
 * =======================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>

#define PASTA "/my_queue"

int main(int argc, char const *argv[]) {
  mqd_t queue; // descritor da fila de msg
  struct mq_attr attr; // atributos da fila
  int msg; // define q as msg serão num inteiros

// Definição dos atributos das mensagens
attr.mq_maxmsg = 10; // num max de msgs
attr.mq_msgsize = sizeof(msg); // cada msg vai ter o tamanho de um int
attr.mq_flags = 0;

  queue = mq_open(PASTA, O_RDWR|O_CREAT, 0666, &attr); // cria fila de mensagens. Se falar, retorna -1

  mq_send(queue, (void *) &msg, sizeof(msg), 0) // retorna -1 pra erro
  // enviará a msg desejada. O cast para void* é necessário
  mq_receive(queue, (void *) &msg, sizeof(msg), 0) // retorna -1 pra erro

  if(fork()){ // Processo filho
    mq_send("mensagem a ser enviada");

  }else{ // Processo pai'
    mq_receive("msg a ser recebida");
  }

  mq_close(fd);
  return 0;
}
