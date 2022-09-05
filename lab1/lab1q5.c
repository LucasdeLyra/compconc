/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Aumenta em 10% todo os itens de uma array usando threads em C e a funcao que espera as threads terminarem */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

#define NTHREADS  2 //total de threads a serem criadas
#define SIZEARRAY 10000 //tamanho da array
double randomlist[SIZEARRAY]; //array com numeros aleatorios


void imprime(double *array, int tamanhoArray) {
  int i;
  for (i=0; i < tamanhoArray; i++) {
      printf("%d - %f\n", i, array[i]);
  }
}

void fillArray (double *array, int tamanhoArray){
  int i;
  for(i=0; i < tamanhoArray; i++){
      array[i] = rand();
  }
}

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
    int idThread, nThreads;
} t_Args;

//funcao executada pelas threads
void *MultiplyTenPercent (void *arg) {
    t_Args *args = (t_Args *) arg;
    int i;
    printf("Sou a thread %d de %d threads\n", args->idThread, args->nThreads);
    for (i=SIZEARRAY*args->idThread; i<SIZEARRAY/NTHREADS; i++){
        randomlist[i] = 1.1 * randomlist[i];
    }
    free(arg); //aqui pode liberar a alocacao feita na main

    pthread_exit(NULL);
}



//funcao principal do programa
int main() {
    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    int thread; //variavel auxiliar

    t_Args *arg; //receberá os argumentos para a thread


    srand(time(NULL));
    fillArray(randomlist, SIZEARRAY); //preenche a array com numeros aleatorios
    //imprime(randomlist, SIZEARRAY);
    //printf("%f\n", randomlist[0]);
    for(thread=0; thread<NTHREADS; thread++) {
        printf("--Aloca e preenche argumentos para thread %d\n", thread);
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = thread; 
        arg->nThreads = NTHREADS;
        
        printf("--Cria a thread %d\n", thread);
        if (pthread_create(&tid_sistema[thread], NULL, MultiplyTenPercent, (void*) arg)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
      } 
    }
    //imprime(randomlist, SIZEARRAY);
    //printf("%f\n", randomlist[0]);
    printf("--Thread principal terminou\n");
    pthread_exit(NULL);
}
