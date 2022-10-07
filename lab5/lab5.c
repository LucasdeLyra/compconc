/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: a thread B so pode imprimir "Bye" depois que duas threads A imprimirem  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

/* Variaveis globais */
int x = 0;
int y = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
pthread_cond_t t_cond;



void *bemvindo (void *t) {
    int boba1, boba2;
    
    /* faz alguma coisa para gastar tempo... */
    boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
    printf("Seja bem-vindo!\n");

    pthread_mutex_lock(&x_mutex);
    x++;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}


void *sentese (void *t) {

    pthread_mutex_lock(&x_mutex);
    if (x == 0) { 
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    y++;
    if (y == 2) {
        pthread_cond_signal(&t_cond);
    }

    pthread_mutex_unlock(&x_mutex); 

    printf("Sente-se por favor.\n");
    pthread_exit(NULL);
}


void *avontade (void *t) {

    pthread_mutex_lock(&x_mutex);
    if (x == 0) { 
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    y++;
    if (y == 2) {
        pthread_cond_signal(&t_cond);
    }
    pthread_mutex_unlock(&x_mutex); 

    printf("Fique a vontade.\n");

    pthread_exit(NULL);
}


void *voltesempre (void *t) {

    pthread_mutex_lock(&x_mutex);
    if (y < 2) { 
        pthread_cond_wait(&t_cond, &x_mutex);
    }
    printf("Volte sempre!\n");
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
    pthread_create(&threads[0], NULL, voltesempre, NULL);
    pthread_create(&threads[1], NULL, avontade, NULL);
    pthread_create(&threads[2], NULL, sentese, NULL);
    pthread_create(&threads[3], NULL, bemvindo, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
}
