#include <stdio.h>
#include <stlib.h>
#include <signal.h>

/*    Crie uma árvore de processos e implemente a comunicação entre eles.
Por exemplo:
A cria B, A cria C, B cria D, B cria E, D cria F.
C quer conversar com D, F quer conversar com C.    */

void sighandler(int signum){

} // FIM DO SIGNAL HANDLER

int main(){
// proc A
    if(fork() >= 0){ // cria proc B
        if(fork() >= 0){ // cria proc D

   	}else{

        }
    }else{

    }

	return 0;
} // FIM DA INT MAIN
