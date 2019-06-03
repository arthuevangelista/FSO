/* =======================================================================
 * APLICATIVO DE CHAT
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Versão: rev 1.0000 (PC1)
 * Autor: Arthur Evangelista
 * Matrícula: 14/0016686
 *
 * =======================================================================
 * Para compilar este código, insira a linha abaixo
 * no terminal de sua distribuição linux:
 *
 * gcc main.c -o a.out -lrt -lpthread
 *
 * E em seguida execute a aplicação com:
 * ./a.out
 * =======================================================================
 * To-Do List para próximas versões:
 *    1_ Alterar nome do executável para:
 *      a.out => 140016686chat
 *    2_ ...
 * =======================================================================
 */

// =======================================================================
// INICIALICAÇÃO DAS BIBLIOTECAS EM COMUM
// =======================================================================

// include das libs do sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// include das libs para thread, fila de mensagens e sinais
#include <pthread.h>
#include <mqueue.h>
#include <signal.h>

// include das libs contendo definições de constantes
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_NUM_USR (10 + 1)
#define MAX_NUM_MSG (500 + 2) // + 1 => \O || + 1 => \n

/* macro elegante para resolução de erros. Fonte: mq_notify() */
#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

// =======================================================================
// Função principal main
// =======================================================================
int main(int argc, char const *argv[]){
  // Declaração das threads
  // pthread_t recebeMsg;
  // pthread_t enviaMsg;

  // Declaração do descritor da fila de mensagens
  // mqd_t mqdes;

  // Declaração das variáveis contendo strings
  char corpoMsg[MAX_NUM_MSG], nomeUsr[MAX_NUM_USR];
  char nomeFila[(MAX_NUM_USR + 5)] = "chat-";

  // limpa a tela
  system("clear");
  // versões seguintes implementar algo mais sofisticado que system clear

  printf("Seja bem-vindo ao aplicativo de Chat!\n\nInsira seu nome: ");
  fgets(nomeUsr,MAX_NUM_USR,stdin);

  system("clear");
  printf("Olá, %s, seja bem-vindo!\n", nomeUsr);
  printf("Entrando na sala de chat da turma.");
  printf("Aguarde um momento...\n\n");

  // ABERTURA DA FILA DE MENSAGENS
  // printf("Fila de mensagens aberta com sucesso!\n");
  strcat(nomeFila,nomeUsr); // concatena as duas strings
  fprintf(stderr, "%s\n", nomeFila);
  // mqdes = mq_open(, O_CREAT | O_RDWR);

  // CRIAÇÃO DAS THREADS PARA ENVIO E RECEPÇÃO DE MENSAGENS
  // printf("Threads criadas com sucesso!\n");

  system("clear");

  // printar janela do terminal bonita pra sala de chat global

  printf("===================================================\n");
  printf("%s: ", nomeUsr); fgets(corpoMsg,(MAX_NUM_MSG-1),stdin);
  printf("===================================================\n");
  printf("Digite \"list\" para listar todos os usuários disponíveis\n");
  printf("Digite \"sair\" para sair do aplicativo\n");
  // SIGINT = ctrl + c
  printf("===================================================\n");

  return 0;
}
