/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Aluno: Lucas de Lyra Monteiro */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5

/* Variaveis globais */
int x = 0;
int y = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
pthread_cond_t t_cond;



void *oiMaria (void *t) {
    printf("Oi, Maria!\n");

    pthread_mutex_lock(&x_mutex);
    x++;
    if (x == 2){
        pthread_cond_signal(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

void *oiJose (void *t) {
    printf("Oi, José!\n");

    pthread_mutex_lock(&x_mutex);
    x++;
    if (x == 2){
        pthread_cond_signal(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}


void *sentese (void *t) {

    pthread_mutex_lock(&x_mutex);
    while (x < 2) { 
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    y++;
    printf("Sente-se por favor.\n");
    if (y == 1) {
        pthread_cond_broadcast(&t_cond);
    }
    pthread_mutex_unlock(&x_mutex); 

    
    pthread_exit(NULL);
}

void *ateMaisJose (void *t) {

    pthread_mutex_lock(&x_mutex);
    while (y < 1) { 
        pthread_cond_wait(&t_cond, &x_mutex);
    }
    printf("Até mais, José!\n");
    pthread_mutex_unlock(&x_mutex); 
    pthread_exit(NULL);
}

void *ateMaisMaria (void *t) {

    pthread_mutex_lock(&x_mutex);
    while (y < 1) { 
        pthread_cond_wait(&t_cond, &x_mutex);
    }
    printf("Até mais, Maria!\n");
    pthread_mutex_unlock(&x_mutex); 
    pthread_exit(NULL);
}



/* Funcao principal */
int main(int argc, char *argv[]) {
    int i; 
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);
    pthread_cond_init(&t_cond, NULL);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, oiMaria, NULL);
    pthread_create(&threads[1], NULL, oiJose, NULL);
    pthread_create(&threads[2], NULL, sentese, NULL);
    pthread_create(&threads[3], NULL, ateMaisJose, NULL);
    pthread_create(&threads[4], NULL, ateMaisMaria, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
}
