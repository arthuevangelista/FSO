/* =======================================================================
 * APLICATIVO DE CHAT
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Versão: rev 1.06 (PC1)
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
 *    4_ Inserir chave mutex ou semáforos para a thread de recebimento
 * de mensagens. Do jeito que está implementada, ela poderá entrar em
 * condição de corrida caso duas threads tentem enviar uma mensagem para
 * o mesmo destinatário!
 *    5_ Retirar na linha 274 (próximo ao fim do segundo e primeiro if-else
 * do console) o pthread_join() pois isto irá bloquear o prosseguimento do
 * código caso alguma mensagem não seja enviada e tenha que esperar as 3
 * tentativas de envio! (Retirar depois que fizer o ajuste com semáforos)
 *    6_ Testar o comando "list" e a função listagemUsr
 *    7_ Testar o broadcast de mensagens quando o destinatário é "all"
 *    8_ Sinal de notificação de recebimento de msg é colocado todo início
 * de loop. Verificar se o código funciona sem aquilo! Ou seja, se recebe
 * mensagens ainda que não tenha sido reestabelecido o sinal todo laço.
 *    9_ Falta adicionar quem enviou qual mensagem e de quem veio o
 * Broadcast. Para implementar esta funcionalidade, deverá ser enviada,
 * junto com a mensagem, que está enviando ela! Ou seja, será alterada a
 * thread t2 (envio de msg) para que t1 (recebimento de msg) possa ser
 * alterada para receber este protocolo.
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
#include <dirent.h>
#include <time.h>

// include das libs para thread, fila de mensagens e sinais
#include <pthread.h>
#include <mqueue.h>
#include <signal.h>

// include das libs contendo definições de constantes
#include <errno.h> // error handling
#include <fcntl.h> // O_ etc
#include <sys/stat.h> // I_S etc
#include <sys/types.h> // ino_t

#define MAX_NUM_USR (10 + 2)  // + 1 => \0 && + 1 => \n
#define MAX_NUM_MSG (500 + 2) // + 1 => \0 && + 1 => \n
#define TENTA_ENVIO 3 // 3 tentativas de envio

// struct global para uso do sigaction e sigevent
struct sigaction act;
struct sigevent sev;

// typedef com a struct de parametros a serem enviados para thread
typedef struct pa_Th{
  char nom[MAX_NUM_USR];
  char dest[MAX_NUM_USR];
  char corpoMsg[MAX_NUM_MSG];
}pa_Th;

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
    case EBUSY:
      fprintf(stderr, "Outro processo já registrou para receber notificações desta fila de mensanges!\n");
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
      perror("Unknown Error");
      exit(EXIT_FAILURE);
      break;
  } // FIM DO SWITCH-CASE
} // FIM DA FUNÇÃO TRATA-ERRO

// =======================================================================
// Função para listagem de Usuários
// =======================================================================
void listagemUsr(){
  // Para lista de users online deve ser verificada a pasta /dev/mqueue
  int rq, errsv, j, i = 0;
  char temp[MAX_NUM_USR];
  DIR* mqDir;
  struct dirent * atual;

  mqDir = opendir("~/dev/mqueue"); errsv = errno;
  if(mqDir == NULL){
    fprintf(stderr, "Não há usuários online!\n");
    rq = closedir(mqDir);
    return;
  }


  fprintf(stderr, "Usuários online\n");

  while ((atual = readdir(mqDir)) != NULL){
    if (atual->d_type == DT_REG){
      // Verifica se é um arquivo regular e imprime o nome do usuário
      for(j = 6; j < strlen(atual->d_name); j++) temp[j] = atual->d_name[j];
      fprintf(stderr, "%s\n", temp); // no lugar de temp estava atual->d_name
      i++;
    } // FIM DO IF-ELSE DE CHECAGEM DE ARQUIVO REGULAR
  } // FIM DO WHILE DE LEITURA DO DIRETÓRIO

  fprintf(stderr, "Total de usuários disponíveis: %d\n", i);

  rq = closedir(mqDir);
} // FIM DA FUNÇÃO DE LISTAGEM DOS USUÁRIOS

// =======================================================================
// T1 => thread para recebimento de mensagens
// =======================================================================
static void t1(union sigval sv /*void* param*/){
  struct mq_attr attr;

  // union sigval *sv;
  // sv = (union signal *)malloc(sizeof(param));
  // sv = (union sigval *) param;

  mqd_t mqdes;
  int rq, errsv = 0;
  char *nomeFila;

  nomeFila = (char *)malloc(sizeof(char)*(MAX_NUM_USR + 6));
  nomeFila = ((char *)sv.sival_ptr);

  char* buffer;
  buffer = (char*)malloc(sizeof(char)*MAX_NUM_MSG);

  mqdes = mq_open(nomeFila, O_RDONLY | O_NONBLOCK);
  while ((rq == -1) && (errsv == EAGAIN)){
    // Enquanto a fila de mensagens não estiver vazia, realiza a leitura
    rq = mq_receive(mqdes, buffer, MAX_NUM_MSG, NULL);
    errsv = errno;
  } // FIM DO WHILE DE LEITURA DAS MENSAGENS

  fprintf(stderr, "%s: %s\n", "dest"/*quem enviou a msg*/,buffer);

  free(buffer);
  pthread_exit(NULL);
} // FIM DA THREAD DE RECEBIMENTO DE MENSAGENS

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
  int i, j, rq, errsv;

  struct timespec timout;
  timout.tv_sec += 2; // timer setado para 2 segundo
  timout.tv_nsec = 0;

  char nomeFila[(MAX_NUM_USR + 6)] = "/chat-";
  if(strcmp(paramThread->dest,"all") == 0){
  // Para enviar a todos, verificar a pasta /dev/mqueue
  // Assim como na função listagemUsr !
  char temp[MAX_NUM_USR];
  DIR* mqDir;
  struct dirent * atual;

  mqDir = opendir("~/dev/mqueue"); errsv = errno;
  if(mqDir == NULL) trataErro(errsv);

  while ((atual = readdir(mqDir)) != NULL){
    if (atual->d_type == DT_REG){
        // Verifica se é um arquivo regular e envia msg pro usuário
        // no lugar de temp estava atual->d_name
        mqdes = mq_open(atual->d_name, O_WRONLY | O_NONBLOCK); errsv = errno;
        if(mqdes == (mqd_t) - 1) trataErro(errsv);
        rq = mq_send(mqdes, paramThread->corpoMsg, strlen(paramThread->corpoMsg), (sysconf(_SC_MQ_PRIO_MAX)-1)); errsv = errno;
        if((rq == -1) && (errsv == EAGAIN)){
          // fecha fila atual e tenta enviar com mq_timedsend por 3 vezes
          rq = mq_close(mqdes);
          mqdes = mq_open(atual->d_name, O_WRONLY);
          for (i = 0; i < TENTA_ENVIO; i++){
            rq = mq_timedsend(mqdes, paramThread->corpoMsg, strlen(paramThread->corpoMsg), (sysconf(_SC_MQ_PRIO_MAX)-1), &timout);
            if(rq < 0) continue; // se não foi enviada, continue no laço for
          }
          // se ainda não foi enviada, envia msg de erro
          for(j = 6; j < strlen(atual->d_name); j++) temp[j] = atual->d_name[j];
          if(rq < 0) fprintf(stderr, "ERRO %s:%s:%s\n", paramThread->nom, temp, paramThread->corpoMsg);
        } // FIM DO IF-ELSE DE ERRO DE ENVIO
      } // FIM DO IF-ELSE DE CHECAGEM DE ARQUIVO REGULAR
    } // FIM DO WHILE DE LEITURA DO DIRETÓRIO

  rq = closedir(mqDir); errsv = errno;
  if(rq == -1) trataErro(errsv);

  // caso tudo esteja certo e de acordo
  free(paramThread);
  pthread_exit(NULL);
  }else{
    strcat(nomeFila,paramThread->dest);
    mqdes = mq_open(nomeFila, O_WRONLY | O_NONBLOCK); errsv = errno;
    if((mqdes == (mqd_t) - 1) && (errsv == ENOENT)){
        fprintf(stderr, "UNKNOWNUSER %s\n", paramThread->dest);
        free(paramThread);
        pthread_exit(NULL);
    } // FIM DO IF-ELSE DE UNKNOWN USER

    rq = mq_send(mqdes, paramThread->corpoMsg, strlen(paramThread->corpoMsg), (sysconf(_SC_MQ_PRIO_MAX)-1)); errsv = errno;
    if((rq == -1) && (errsv == EAGAIN)){
      // fecha fila atual e tenta enviar com mq_timedsend por 3 vezes
      rq = mq_close(mqdes);
      mqdes = mq_open(nomeFila, O_WRONLY);
      for (i = 0; i < TENTA_ENVIO; i++){
        rq = mq_timedsend(mqdes, paramThread->corpoMsg, strlen(paramThread->corpoMsg), (sysconf(_SC_MQ_PRIO_MAX)-1), &timout);
        if(rq < 0) continue; // se não foi enviada, continue no laço for
      }
      // se ainda não foi enviada, envia msg de erro
      if(rq < 0) fprintf(stderr, "ERRO %s:%s:%s\n", paramThread->nom, paramThread->dest, paramThread->corpoMsg);
    } // FIM DO IF-ELSE DE ERRO DE ENVIO

  // caso tudo esteja certo e de acordo
  free(paramThread);
  pthread_exit(NULL);
  } // FIM DO IF-ELSE DO BROADCAST

} // FIM DA THREAD DE ENVIO DE MENSAGENS

// =======================================================================
// Função principal main
// =======================================================================
void main(int argc, char const *argv[]){
  // Declaração das threads
  pthread_t recebeMsg;
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

  // ABERTURA DA FILA DE MENSAGENS
  strcat(nomeFila,nomeUsr); // concatena as duas strings
  mqdes = mq_open(nomeFila, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IWOTH, NULL);
  errsv = errno;
  if(mqdes == (mqd_t) - 1) trataErro(errsv);

  system("clear");
  printf("===================================================\n");
  printf("Digite \"list\" para listar todos os usuários disponíveis\n");
  printf("Digite \"sair\" para sair do aplicativo\n");
  printf("===================================================\n");
  while(1){
    // INICIALICAÇÃO DA THREAD DE RECEBIMENTO DE MENSAGENS
    // É colocado um sinal de notificação assíncrona para cada recebimento de msg
    // Novamente esse sinal é colocado (verificar se funfa sem isso)
    sev.sigev_notify = SIGEV_THREAD; // Tipo de notificação (chama uma thread)
    sev.sigev_notify_function = t1; // Nome da thread a ser chamada
    sev.sigev_notify_attributes = NULL; // Attr (geralmente é nulo mesmo)
    sev.sigev_value.sival_ptr = &nomeFila; // argumento a ser passado para thread

    // LAME AS F**K (limpeza das strings a cada loop)
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
        pthread_cancel(enviaMsg);
        pthread_cancel(recebeMsg);
        free(paramThread);
        exit(EXIT_SUCCESS);
    }else{
    	if(strcmp(destUsr,"list") == 0){
        listagemUsr();
    	}else{
        // Caso seja apenas um destinatário comum, chama thread de envio de msg
        fprintf(stderr, "Mensagem: "); fgets(corpoMsg,(MAX_NUM_MSG-1),stdin);
	      strcpy(paramThread->nom,nomeUsr);
        strcpy(paramThread->dest,destUsr);
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
