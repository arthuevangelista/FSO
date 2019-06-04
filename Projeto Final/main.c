/* =======================================================================
 * APLICATIVO DE CHAT
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Versão: rev 1.03 (PC1)
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
 *      a) a.out => 140016686chat
 *    2_ Implementar algo mais sofisticado que system clear
 *    3_ Printar janela do terminal bonita pra sala de chat global
 *      a) Talvez utilizar a biblioteca ncurses (testar linking com -lncurses)
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
#include <time.h>

// include das libs para thread, fila de mensagens e sinais
#include <pthread.h>
#include <mqueue.h>
#include <signal.h>

// include das libs contendo definições de constantes
#include <errno.h> // error handling
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_NUM_USR (10 + 2)  // + 1 => \O
#define MAX_NUM_MSG (500 + 2) // + 1 => \O

// struct global para uso do sigaction
struct sigaction act;

// typedef com a struct de parametros a serem enviados para thread
struct parTh{
  char nom[MAX_NUM_USR];
  char dest[MAX_NUM_USR];
  char corpoMsg[MAX_NUM_MSG];
};

typedef struct parTh pa_Th;

// =======================================================================
// T1 => thread para recebimento de mensagens
// =======================================================================
void* t1(void* param){

}

// =======================================================================
// T2 => thread para envio de mensagens
// =======================================================================
void* t2(void* param){
  // IMPLEMENTAR CHAVE MUTEX OU SEMÁFORO PARA EVITAR CONDIÇÃO DE CORRIDA
  // ENTRE DOIS OU MAIS THREADS TENTADO ACESSAR A MESMA FILA DE MENSAGENS
  pa_Th* paramThread;

  paramThread = (pa_Th*)malloc(sizeof(pa_Th));
  paramThread = (pa_Th *) param;

  mqd_t mqdes;
  int i, rq, errsv;

  struct timespec timout;
  timout.tv_sec += 2; // timer setado para 2 segundo
  timout.tv_nsec = 0;

  char nomeFila[(MAX_NUM_USR + 6)] = "/chat-";
  if(strcmp(paramThread->dest,"all") == 0){
  // chama a thread para enviar para todos (essa comparação é melhor se feita dentro da thread de envio)
  // caso tudo esteja certo e de acordo
  pthread_exit(NULL);
  }else{
    strcat(nomeFila,paramThread->dest);
    mqdes = mq_open(nomeFila, O_WRONLY); errsv = errno;
    if(mqdes == (mqd_t) - 1){
      if(errsv == ENOENT){
        fprintf(stderr, "UNKNOWNUSER %s\n", paramThread->dest);
        pthread_exit(NULL);
      } // FIM DO SEGUNDO IF-ELSE DE UNKNOWN USER
    }else{
    for (i = 0; i < 3; i++){ // 3 tentativas de envio
      rq = mq_timedsend(mqdes, paramThread->corpoMsg, MAX_NUM_MSG, (sysconf(_SC_MQ_PRIO_MAX)-1), &timout);
      if(rq < 0) continue; // se não foi enviada, continue no laço for
    } // FIM DO LAÇO FOR DE TENTATIVAS DE ENVIO
    if(rq < 0){
       // se ainda não foi enviada, envia msg de erro
       fprintf(stderr, "ERRO %s:%s:%s\n", paramThread->nom, paramThread->dest, paramThread->corpoMsg);
    } // FIM DO ENVIO DA MENSAGEM DE ERRO DE ENVIO
   } // FIM DO PRIMEIRO IF-ELSE DE UNKNOWN USER
  // caso tudo esteja certo e de acordo
  pthread_exit(NULL);
  } // FIM DO IF-ELSE DO BROADCAST

} /* FIM DA THREAD DE ENVIO DE MENSAGENS */

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
    case EAGAIN:
      fprintf(stderr, "O número máximo de threads imposto pelo sistema foi atingido!\n");
      fprintf(stderr, "Entre em contato com o administrador do sistema! :)\n");
      exit(EXIT_FAILURE);
      break;
    case ESRCH:
      fprintf(stderr, "A thread a ser cancelada já foi finalizada!\n");
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
  pthread_t enviaMsg;

  // Declaração do descritor da fila de mensagens
  mqd_t mqdes;

  // set das flags para o sinal de interrupção SIGINT = ctrl + c
  act.sa_sigaction = trataSinal;
  act.sa_flags = SA_SIGINFO; // info sobre o sinal

  // Variáveis do tipo int para salvar códigos de erro
  int i, rq, errsv;

  // Declaração das variáveis contendo strings
  char corpoMsg[MAX_NUM_MSG], nomeUsr[MAX_NUM_USR], destUsr[MAX_NUM_USR];
  char nomeFila[(MAX_NUM_USR + 6)] = "/chat-";

  // Declaração da struct de parametros a serem enviados para thread
  pa_Th* paramThread;
  paramThread = (pa_Th*)malloc(sizeof(pa_Th));

  // Direcionamento para o devido tratamento dos sinais
  sigaction(SIGINT, &act, NULL);

  // limpa a tela
  system("clear");
  printf("Seja bem-vindo ao aplicativo de Chat!\n\nInsira seu nome: ");
  fgets(nomeUsr,MAX_NUM_USR,stdin);

  // Remove o \n no final da string inserida
  if( nomeUsr[strlen(nomeUsr)-1] == '\n' ) nomeUsr[strlen(nomeUsr)-1] = 0;

  system("clear");
  printf("Olá, %s, seja bem-vindo!\n", nomeUsr);
  printf("Entrando na sala de chat da turma. ");
  printf("Aguarde um momento...\n\n");

  // ABERTURA DA FILA DE MENSAGENS
  strcat(nomeFila,nomeUsr); // concatena as duas strings
  mqdes = mq_open(nomeFila, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IWOTH, NULL);
  errsv = errno;
  if(mqdes == (mqd_t) - 1){
    trataErro(errsv);
  }else{
    printf("Fila de mensagens aberta com sucesso!\n");
  }

  while(1){
    system("clear");
    printf("===================================================\n");
    printf("Digite \"list\" para listar todos os usuários disponíveis\n");
    printf("Digite \"sair\" para sair do aplicativo\n");
    printf("===================================================\n");

// LAME AS F**K
    for(i = 0; i < strlen(destUsr); i++)
	    destUsr[i] = '\0';
	    paramThread->dest[i] = '\0';
    for(i = 0; i < strlen(corpoMsg); i++)
	    corpoMsg[i] = '\0';
	    paramThread->corpoMsg[i] = '\0';

    // Inserção de destinatário
    fprintf(stderr,"%s:",nomeUsr); fgets(destUsr,(MAX_NUM_USR-1),stdin);
    // Remove o \n no final da string inserida
    if( destUsr[strlen(destUsr)-1] == '\n' ) destUsr[strlen(destUsr)-1] = 0;
    // Comparação do console
    if(strcmp(destUsr,"sair") == 0){
        rq = mq_close(mqdes); errsv = errno;
        if(rq == -1) trataErro(errsv);
        rq = mq_unlink(nomeFila); errsv = errno;
        if(rq == -1) trataErro(errsv);
        rq = pthread_cancel(enviaMsg);
        // rq = pthread_cancel(recebeMsg); errsv = errno;
        // if(rq != 0) trataErro(errsv);
        exit(EXIT_SUCCESS);
    }else{
    	if(strcmp(destUsr,"list") == 0){
        listagemUsr();
    	}else{
        // Caso seja apenas um destinatário comum, echama thread de envio de msg
        fprintf(stderr, "Mensagem: "); fgets(corpoMsg,(MAX_NUM_MSG-1),stdin);
	strcpy(paramThread->nom,nomeUsr);
	strcpy(paramThread->dest,destUsr);
printf("%s\n%s\n", destUsr, paramThread->dest);
	strcpy(paramThread->corpoMsg,corpoMsg);
        rq = pthread_create(&enviaMsg, NULL, &t2, (void *) paramThread);
        errsv = errno;
        if(rq) trataErro(errsv);
        // Aguarda o retorno da thread de envio para retornar ao loop
        // mudar isso aqui => função blockante !!!
        pthread_join(enviaMsg, NULL);
    	} // FIM DO SEGUNDO IF-ELSE DO CONSOLE
    } // FIM DO PRIMEIRO IF-ELSE DO CONSOLE
  } // FIM DO LOOP INFINITO DO CHAT
} // FIM DA FUNÇÃO MAIN
