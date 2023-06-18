/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Lucas de Lyra Monteiro 121039714
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define x 5 //numero de threads escritoras
#define y 5 //numero de threads leitoras
#define z 5 //numero de threads escritoras e leitoras

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo
int var=0; //variavel global central

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

//entrada leitura
void InicLeit (int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);
    while(escr > 0) {
        printf("L[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_leit, &mutex);
        printf("L[%d] desbloqueou\n", id);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit (int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] terminou de ler\n", id);
    leit--;
    if(leit==0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    while((leit>0) || (escr>0)) {
        printf("E[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_escr, &mutex);
        printf("E[%d] desbloqueou\n", id);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] terminou de escrever\n", id);
    escr--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}


//thread leitora
void * T1 (void * arg) {
    int *id = (int *) arg;
    while(1) {
        InicEscr(*id);
        var++;
        printf("Escritora %d esta escrevendo\n", *id);
        FimEscr(*id);
        sleep(1);
    } 
    free(arg);
    pthread_exit(NULL);
}

//thread leitora
void * T2 (void * arg) {
    int *id = (int *) arg;
    while(1) {
        InicLeit(*id);
        printf("Leitora %d esta lendo\n", *id);
        printf("O valor de var é: %d\n", var);
        if (var%2 == 0) {
            printf("Var é par\n");
        } else {
            printf("Var é ímpar\n");
        }
        FimLeit(*id);
        sleep(1);
    } 
    free(arg);
    pthread_exit(NULL);
}

//thread escritora-leitora
void * T3 (void * arg) {
    int *id = (int *) arg;
    int j=0;

    while(1) {
        InicLeit(*id);
        printf("Leitora-Escritora %d esta lendo\n", *id);
        printf("O valor de var é: %d\n", var);
        FimLeit(*id);

        InicEscr(*id);
        printf("Leitora-Escritora %d esta escrevendo\n", *id);
        var = *id;
        while (j<10000){
            j++;
        }
        FimEscr(*id);
        sleep(1);
    } 
    free(arg);
    pthread_exit(NULL);
}

//funcao principal
int main(void) {
    //identificadores das threads
    pthread_t tid[y+x+z];
    int id[y+x+z];

    //inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);

    //cria as threads escritoras
    for(int i=0; i<x; i++) {
        id[i+y] = i+1;
        if(pthread_create(&tid[i+y], NULL, T1, (void *) &id[i+y])) exit(-1);
    }

    //cria as threads leitoras
    for(int i=0; i<y; i++) {
        id[i] = 100+i+1;
        if(pthread_create(&tid[i], NULL, T2, (void *) &id[i])) exit(-1);
    } 

    //cria as threads leitoras-escritoras
    for(int i=0; i<z; i++) {
        id[i+y+x] = 1000+i+1;
        if(pthread_create(&tid[i+y+x], NULL, T3, (void *) &id[i+y+x])) exit(-1);
    }

    pthread_exit(NULL);
    return 0;
}

/*
A forma de identificar as diferentes threads é o número de dígitos, mas não pensei em nada melhor,
eu sei que falha se tiverem mais de 100 T1, por exemplo, vai dar ruim, 
mas eu espero que isso não esteja rodando num computador de 101 núcleos
*/