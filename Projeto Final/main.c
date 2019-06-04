/* =======================================================================
 * APLICATIVO DE CHAT
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Versão: rev 1.01 (PC1)
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
#include <errno.h> // error handling
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_NUM_USR (10 + 1)  // + 1 => \O
#define MAX_NUM_MSG (500 + 1) // + 1 => \O

// struct global para uso do sigaction
struct sigaction act;

// typedef com a struct de parametros a serem enviados para thread
typedef struct parTh pa_Th{
  char dest[(MAX_NUM_USR + 6)];
  char corpoMsg[(MAX_NUM_MSG + 1)]; // +1 => \n
};

// =======================================================================
// T1 => thread para recebimento de mensagens
// =======================================================================
void* recebeMsg(void* param){

}

// =======================================================================
// T2 => thread para envio de mensagens
// =======================================================================
void* enviaMsg(void* param){

}

// =======================================================================
// Função para tratamento do sinal SIGINT
// =======================================================================
void trataSinal(int signum, siginfo_t* info, void* ptr){
  system("clear");
  fprintf(stderr, "Recebido o sinal %d.\n", signum);
  fprintf(stderr, "Para finalizar o programa, digite \"sair\" no console.\n");
  fprintf(stderr, "\nPressione qualquer tecla para continuar... ");
  getchar();
}

// =======================================================================
// Função para listagem de Usuários
// =======================================================================
void listagemUsr(){
  fprintf(stderr, "Faz nada por enquanto.\n");
}

// =======================================================================
// Função de tratamento de erros
// =======================================================================
void trataErro(int errsv){
  switch (errsv) {
    case EACCES:
      fprintf(stderr, "Esta fila já existe e você não tem permissão para acessá-la!\n");
      exit(EXIT_FAILURE);
      break;
    case EEXIST:
      fprintf(stderr, "Um usuário com este nome já existe!\n");
      exit(EXIT_FAILURE);
      break;
    case EINVAL:
      fprintf(stderr, "O nome de usuário é inválido!\n");
      exit(EXIT_FAILURE);
      break;
    case EPERM:
      fprintf(stderr, "Operação não permitida ao usuário!\n");
      exit(EXIT_FAILURE);
      break;
    case EBADF:
      fprintf(stderr, "O descritor da fila de mensagens é inválido!\n");
      exit(EXIT_FAILURE);
      break;
    case ENOENT:
      fprintf(stderr, "Não existe uma fila de mensagens com este nome!\n");
      exit(EXIT_FAILURE);
      break;
    default:
      fprintf(stderr, "Erro desconhecido!\n");
      perror("Unknown Error:");
      exit(EXIT_FAILURE);
      break;
  } /* FIM DO SWITCH-CASE */
} /* FIM DA FUNÇÃO TRATA-ERRO */

// =======================================================================
// Função principal main
// =======================================================================
void main(int argc, char const *argv[]){
  // Declaração das threads
  //pthread_t recebeMsg;
  //pthread_t enviaMsg;

  // Declaração do descritor da fila de mensagens
  mqd_t mqdes;

  // set das flags para o sinal de interrupção SIGINT = ctrl + c
  act.sa_sigaction = trataSinal;
  act.sa_flags = SA_SIGINFO; // info sobre o sinal

  // Variáveis do tipo int para salvar códigos de erro
  int rq, errsv;

  // Declaração das variáveis contendo strings
  char corpoMsg[MAX_NUM_MSG], nomeUsr[MAX_NUM_USR], destUsr[MAX_NUM_USR];
  char nomeFila[(MAX_NUM_USR + 6)] = "/chat-";

  // Declaração da struct de parametros a serem enviados para thread
  pa_Th* paramThread;

  // Direcionamento para o devido tratamento dos sinais
  sigaction(SIGINT, &act, NULL);

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
  strcat(nomeFila,nomeUsr); // concatena as duas strings
  fprintf(stderr, "%s\n", nomeFila);
  mqdes = mq_open(nomeFila, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IWOTH, NULL);
  errsv = errno;
  if(mqdes == (mqd_t) - 1){
    trataErro(errsv);
  }else{
    printf("Fila de mensagens aberta com sucesso!\n");
  }

  // CRIAÇÃO DAS THREADS PARA ENVIO E RECEPÇÃO DE MENSAGENS
  // printf("Threads criadas com sucesso!\n");

  while(1){
    system("clear");
    // printar janela do terminal bonita pra sala de chat global

    printf("===================================================\n");
    printf("Digite \"list\" para listar todos os usuários disponíveis\n");
    printf("Digite \"sair\" para sair do aplicativo\n");
    printf("===================================================\n");
    printf("%s: ", nomeUsr); fgets(corpoMsg,(MAX_NUM_MSG-1),stdin);
    switch (corpoMsg) {
      case "sair":
        rq = mq_close(mqdes); errsv = errno;
        if(rq == -1) trataErro(errsv);
        rq = mq_unlink(nomeFila); errsv = errno;
        if(rq == -1) trataErro(errsv);
        //pthread_kill();
        //pthread_kill();
        exit(EXIT_SUCCESS);
      case "list":
        listagemUsr();
        break;
      default:
      // Caso seja apenas uma mensagem comum, chama thread de envio de msg
        //pthread_create();
        break;
    } // FIM DO SWITCH-CASE DO CORPO DA MSG
  } // FIM DO LOOP INFINITO DO CHAT
} // FIM DA FUNÇÃO MAIN
