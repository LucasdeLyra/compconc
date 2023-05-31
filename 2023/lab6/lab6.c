#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TAMANHO 10
#define NUMPRODUTORES 5
#define NUMCONSUMIDORES 5

int Buffer[TAMANHO];
int count = 0, in = 0, out = 0;
sem_t slot_cheio, slot_vazio, mutexCons, mutexProd;


void imprimeBuffer(int tamanho){
	for(int i = 0; i < tamanho; i++) {
        printf("%d ", Buffer[i]);
    }
	printf("\n");
}


void Insere(int itens[], int id){
	sem_wait(&slot_vazio);

	sem_wait(&mutexProd);
        printf("Produtor[%d] rodando\n", id);
        for(int i = 0; i < TAMANHO; i++) { //copia vetor de entrada para o buffer
            Buffer[i] = itens[i];
        }
        count = TAMANHO;
        in = (in + 1) % TAMANHO;
        printf("Produtor[%d] inseriu\n", id);
        imprimeBuffer(TAMANHO);
    sem_post(&mutexProd);

    for (int i = 0; i < TAMANHO; i++){
        sem_post(&slot_cheio);
    }
	

}


int Retira(int id){
	int item;
	sem_wait(&slot_cheio);
        sem_wait(&mutexCons);
            printf("Consumidor[%d] rodando\n", id-NUMPRODUTORES); //coloquei pra imprimir dessa forma pq eu estava confuso sem achar o consumidor 1
            item = Buffer[out];
            Buffer[out] = -1;
            out = (out + 1) % TAMANHO;
            count--;
            printf("Consumidor[%d] consumiu %d\n", id-NUMPRODUTORES, item);
            imprimeBuffer(TAMANHO);
            if (count == 0) {
                sem_post(&slot_vazio);
            }
        sem_post(&mutexCons);


	return item;
}

void * produtor(void * arg){
    int elemento[TAMANHO];
	int *id = (int *) arg;
    int pos = 0;
    while (1) {
    //produz um elemento....
        if (pos < TAMANHO) {
            elemento[pos] = 10;
            pos++;
        }
        else if (pos == TAMANHO){
            Insere(elemento, * id);
            pos = 0;
        }
    }
	free(arg);
	pthread_exit(NULL);
}

void * consumidor(void * arg){
    int item;
	int *id = (int *) arg;
	while(1){
		item = Retira(*id);
	}
	free(arg);
	pthread_exit(NULL);
}

int main(){
	pthread_t tid[NUMPRODUTORES + NUMCONSUMIDORES];
	int *id[NUMPRODUTORES + NUMCONSUMIDORES];

    
	sem_init(&mutexCons, 0, 1);
	sem_init(&mutexProd, 0, 1);
	sem_init(&slot_cheio, 0, 0);
	sem_init(&slot_vazio, 0, 1);

	for(int i = 0; i < NUMPRODUTORES + NUMCONSUMIDORES; i++){
		id[i] = malloc(sizeof(int));
		if(id[i] == NULL) {
            exit(1);
        }    
		*id[i]= i + 1;
	}
	for(int i = 0; i < TAMANHO; i++) {
        Buffer[i] = 0;
    }
		
        
	for(int i = 0; i < NUMPRODUTORES; i++) {
		if(pthread_create(&tid[i], NULL, produtor, (void *) id[i])) {
            exit(1);
        } 
    }
	for(int i = 0; i < NUMCONSUMIDORES; i++){
		if(pthread_create(&tid[i+NUMPRODUTORES], NULL, consumidor, (void *) id[i+NUMPRODUTORES])) {
            exit(1);
        }
    }


	pthread_exit(NULL);
    sem_destroy(&slot_cheio);
    sem_destroy(&slot_vazio);
    sem_destroy(&mutexCons);
    sem_destroy(&mutexProd);
    for (int i = 0; i < NUMPRODUTORES + NUMCONSUMIDORES; i++){
        free(id[i]);
    }
    free(id);
	return 0;
}
