/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Lucas de Lyra Monteiro - 121039714*/
/* Codigo: Aumenta em 10% todo os itens de uma array usando threads em C e a funcao que espera as threads terminarem */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

#define NTHREADS  2 //total de threads a serem criadas
#define SIZEARRAY 10000 //tamanho da array


//imprime uma array de doubles
void imprime(double *array, int tamanhoArray) {
  int i;
  for (i=0; i < tamanhoArray; i++) {
      printf("%d - %f\n", i, array[i]);
  }
}


// preenche um array com números randômicos
void fillArray (double *array, int tamanhoArray){
  int i;
  for(i=0; i < tamanhoArray; i++){
      array[i] = rand();
  }
}


//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
    int idThread, nThreads;
    double *rlist;
} t_Args;


//função sequencial
void MultiplyTenPercent (double *array){
    for (int i=0; i<SIZEARRAY; i++){
        array[i] = array[i]*1.1;
    }
}


//funcao executada pelas threads
void *MultiplyTenPercentThreads (void *arg) {
    t_Args *args = (t_Args *) arg;
    int i;
    printf("Sou a thread %d de %d threads\n", args->idThread, args->nThreads);
    for (i=SIZEARRAY*args->idThread; i<SIZEARRAY/NTHREADS; i++){
        args->rlist[i] = 1.1 * args->rlist[i];
    }
    free(arg); //aqui pode liberar a alocacao feita na main

    pthread_exit(NULL);
}


// testa se dois doubles são iguais, vendo se a diferença entre eles está entre -10^-6 e 10^-6 
int isCorrect(double *arrayThreads, double *array){
    for (int i = 0; i<SIZEARRAY; i++){
        if (array[0]-arrayThreads[0]>0.000001 && array[0]-arrayThreads[0]<-0.000001){
            printf("Errado no item %d\n", i);
            return 0;
        }
    }
    printf("Passou em tudo! Yey :3\n");
    return 1;
}


//funcao principal do programa
int main() {
    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    double randomlist[SIZEARRAY], copyarray[SIZEARRAY]; //array com numeros aleatorios
    int thread; //variavel auxiliar

    t_Args *arg; //receberá os argumentos para a thread


    srand(time(NULL));
    fillArray(randomlist, SIZEARRAY); //preenche a array com numeros aleatorios

    // copia os itens da array preenchida para uma outra array para testar depois
    for (int i = 0; i<SIZEARRAY; i++){
        copyarray[i] = randomlist[i];
    }

    //printf("%f%f", copyarray[0], randomlist[0]);
    for(thread=0; thread<NTHREADS; thread++) {
        printf("--Aloca e preenche argumentos para thread %d\n", thread);
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = thread; 
        arg->nThreads = NTHREADS;
        arg->rlist = randomlist;
        
        printf("--Cria a thread %d\n", thread);
        if (pthread_create(&tid_sistema[thread], NULL, MultiplyTenPercentThreads, (void*) arg)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
      } 
    }

    MultiplyTenPercent(copyarray); //multiplica a array copiada sequencialmente
    isCorrect(copyarray, randomlist); //testa se está correto

    printf("--Thread principal terminou\n");
    pthread_exit(NULL);
}
