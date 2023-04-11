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
#define TAMANHOLISTA 10000 //tamanho da array

//imprime uma array de doubles
void imprime(double *array, int tamanhoArray) {
  int i;
  for (i=0; i < tamanhoArray; i++) {
      printf("%d - %f\n", i, array[i]);
  }
}


// preenche um array com números randômicos
void preenche_lista (double *array, int tamanhoArray){
  int i;
  for(i=0; i < tamanhoArray; i++){
      array[i] = i;
  }
}


//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
    int idThread, nThreads;
    double *rlist;
} t_Args;


//função sequencial
void multiplica_dois (double *array){
    for (int i=0; i<TAMANHOLISTA; i++){
        array[i] = array[i]*2;
    }
}


//funcao executada pelas threads
void *multiplica_dois_threads (void *arg) {
    t_Args *args = (t_Args *) arg;
    int i;
    printf("Sou a thread %d de %d threads\n", args->idThread+1, args->nThreads);

    for (i=TAMANHOLISTA/NTHREADS*args->idThread; i<(TAMANHOLISTA/NTHREADS)*(args->idThread+1); i++){
        args->rlist[i] = args->rlist[i]*2;
    }
    free(arg); //aqui pode liberar a alocacao feita na main

    pthread_exit(NULL);
}


// testa se dois doubles são iguais, vendo se o valor absoluto da diferença entre eles é maior que 10^-6
int correto(double *arrayThreads, double *array){
    for (int i = 0; i<TAMANHOLISTA; i++){
        if (abs(array[i]-arrayThreads[i]) > 0.000001){ //se a diferença é maior que 10^-6 deu ruim
            printf("Erro no item %d\n", i);
            return 1;
        }
    }
    printf("Passou em tudo! Yey :3\n");
    return 0;
}


//funcao principal do programa
int main() {
    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    double randomlist[TAMANHOLISTA], copyarray[TAMANHOLISTA]; //array com numeros aleatorios
    int thread; //variavel auxiliar

    t_Args *arg; //receberá os argumentos para a thread


    srand(time(NULL));
    preenche_lista(randomlist, TAMANHOLISTA); //preenche a array com numeros aleatorios

    // copia os itens da array preenchida para uma outra array para testar depois
    for (int i = 0; i<TAMANHOLISTA; i++){
        copyarray[i] = randomlist[i];
    }

    //printf("%f%f", copyarray[0], randomlist[0]);
    for(thread=0; thread<NTHREADS; thread++) {
        printf("--Aloca e preenche argumentos para thread %d\n", thread+1);
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = thread; 
        arg->nThreads = NTHREADS;
        arg->rlist = randomlist;
        
        printf("--Cria a thread %d\n", thread+1);
        if (pthread_create(&tid_sistema[thread], NULL, multiplica_dois_threads, (void*) arg)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
      } 
    }

    multiplica_dois(copyarray); //multiplica a array copiada sequencialmente
    correto(copyarray, randomlist); //testa se está correto

    //imprime(randomlist, TAMANHOLISTA); teste antigo para ver se está correto

    printf("--Thread principal terminou\n");
    pthread_exit(NULL);
}
