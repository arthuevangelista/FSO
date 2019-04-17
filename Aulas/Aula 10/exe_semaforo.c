/* =======================================================================
 * EXERCÍCIO DE SEMÁFOROS
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Autor: Arthur Evangelista
 * Matrícula: 14/0016686
 *
 * Exercício sobre semáforos. Distribuição de tickets.
 * Altere o valor de OPT conforme:
 * OPT == 0 -> Para valores de tickets sequenciais (valor == posição)
 * OPT != 0 -> Para valores de tickets aleatórios (valor != posição)
 *
 * A função get_ticket inicia um semáforo, retorna uma variável local
 * ticket que recebeu o valor da variável global pi->ticket[], depois
 * zera esta var global, reposiciona o apontador de início de pilha
 * para uma posição abaixo e libera o semáforo. Desta forma, nenhuma
 * outra thread irá adquirir o mesmo valor que ela, a menos que este
 * valor seja devolvido para pilha.
 *
 * Caso não houvesse o valor de NMAX (como no caso de um sistema de
 * distribuição de um restaurante, onde não deve haver número limite
 * de consumidores para o sistema) o tipo de pilha a ser implementada
 * seria dinâmica e não estática.
 * =======================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define OPT 0
#define NMAX 10
#define NUM_THREAD 2

// Definição de tipo das structs
typedef struct pilha Pilha;
struct pilha{
  int pos;
  int ticket[NMAX];
};

// Variáveis Globais
volatile sem_t S;
volatile Pilha* pi;

// Subrotina da thread
void* threadCoop(void* param){
  int meu_ticket;
  int fid = (int) param;

  meu_ticket = get_ticket();
  if(meu_ticket < 0){
    fprintf(stderr, "%s\n", "Sem tickets disponíveis no momento!\n");
    pthread_exit(NULL);
  }else{
    fprintf(stderr, "Thread [%d]: Meu ticket é o número %d.\n", fid, meu_ticket);
    usleep(1000); // 1 segundo
    return_ticket(meu_ticket);
    fprintf(stderr, "Thread [%d]: Devolvendo meu ticket número %d!\n", fid, meu_ticket);
    pthread_exit(NULL);
  } // FIM DO IF
} // FIM DA SUBROTINA

// Função para aquisição do ticket
int get_ticket(){
  sem_wait(&S);
  int ticket;

  if(pi->pos > 0){
    ticket = pi->ticket[pi->pos];
    pi->ticket[pi->pos] = 0;
    pi->pos--;
  }else{
    ticket = -1;
  }
  sem_post(&S);
  return ticket;
} // FIM DA get_ticket

// Função para retorno do ticket
void return_ticket(int t){
  sem_wait(&S);
      pi->pos++;
      pi->ticket[pi->pos] = t;
  sem_post(&S);
} // FIM DO return_ticket

int main(){
  // Declaração da Thread
  pthread_t minhaThread[NUM_THREAD];

  // Declaração dos parâmetros a serem usados
  int fd, ti, fid;

  // Inicialização do semáforo
  sem_init(&S,0,1);

  // Inicialização da pilha
  pi = (Pilha*) malloc(sizeof(struct pilha));
  if(pi != NULL){
    pi->pos = NMAX;
  }else{
    fprintf(stderr, "%s\n", "Não foi possível inicializar a pilha!");
  }

  // Laço para geração dos tickets
  for(ti = 0; ti < NMAX; ti++){
    if(OPT){
      pi->ticket[ti] = rand()%NMAX;
    }else{
      pi->ticket[ti] = ti;
    }
  }

  // Apenas pelo estilo
  printf("\n\n\n\n");

  // Laço for para inicialização das threads
  for(fid = 1; fid <= NUM_THREAD; fid++){
    fd = pthread_create(&minhaThread[fid], NULL, &threadCoop, (void *)fid);
    if(fd){
      fprintf(stderr, "Erro ao iniciar a Thread [%d]\n", fid);
      exit(-1);
    } // FIM DO IF-ELSE
  } // FIM DO LAÇO FOR

  for(fid = 1; fid <= NUM_THREAD; fid++){
    pthread_join(minhaThread[fid], NULL);
  }

  fprintf(stderr, "\n\n%s\n", "Todas as threads foram finalizadas!");
  usleep(500);
  fprintf(stderr, "\n%s\n", "Encerrando o código.");

  free(pi);
  sem_destroy(&S);
  return 0;
}
