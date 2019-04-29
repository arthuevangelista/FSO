/* =======================================================================
 * EXERCÍCIO DE SEMÁFOROS
 * =======================================================================
 * Universidade de Brasília
 * campus Gama
 *
 * Autor: Arthur Evangelista
 * Matrícula: 14/0016686
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

#include "exemplo.h"

#define NMAX 3
#define NUM_THREAD 6

// Definição de tipo das structs
typedef struct pilha Pilha;
struct pilha{
  int pos;
  int ticket[NMAX];
};

// Variáveis Globais
sem_t S;
Pilha* pi;

// Função para aquisição do ticket
int get_ticket(){
  sem_wait(&S);
  int ticket;

  if(pi->pos > 0){
    ticket = pi->ticket[pi->pos];
//    pi->ticket[pi->pos] = 0;
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
  int fd, ti;
  int fid;

  // Inicialização do semáforo
  sem_init(&S,0,1);

  // Inicialização da pilha
  sem_wait(&S);
  pi = (Pilha*) malloc(sizeof(struct pilha));
  if(pi != NULL){
    pi->pos = NMAX;
  }else{
    fprintf(stderr, "%s\n", "Não foi possível inicializar a pilha!");
  }

  // Laço para geração dos tickets
  for(ti = 0; ti < NMAX; ti++){
      pi->ticket[ti] = rand()%1000;
  }
  sem_post(&S);
  printf("\n");

  // Laço for para inicialização das threads
  for(fid = 0; fid < NUM_THREAD; fid++){
    fd = pthread_create(&minhaThread[fid], NULL, &threadCoop, (void *) (intptr_t) fid);
    if(fd){
      fprintf(stderr, "Erro ao iniciar a Thread [%d]\n", fid);
      exit(-1);
    } // FIM DO IF-ELSE
  } // FIM DO LAÇO FOR

  for(fid = 0; fid < NUM_THREAD; fid++){
    pthread_join(minhaThread[fid], NULL);
  }

  fprintf(stderr, "\n\n%s\n", "Todas as threads foram finalizadas!");
  usleep(500);
  fprintf(stderr, "\n%s\n", "Encerrando o código.");

  free(pi);
  sem_destroy(&S);
  return 0;
}
