#include "exemplo.h"

// Subrotina da thread
void* threadCoop(void* param){
  int meu_ticket;
  int fid = (intptr_t) param;

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
