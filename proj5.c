#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t slotCheio, slotVazio; // Condições
sem_t mutexProd, mutexCons; // Exclusão mútua
int Buffer[N];
int count = 0;

void Insere(int item) {
    static int in = 0;
    int i = 0;
    // Aguarda slot vazio
    sem_wait(&slotVazio);
    // Exclusão mútua entre produtores
    sem_wait(&mutexProd);
    while (i <= N) {
        Buffer[in] = item;
        in = (in + 1) % N;
        i++;
        count++;
    }
    sem_post(&mutexProd);
    // Sinaliza um slot cheio
    sem_post(&slotCheio);
}

int Retira(void) {
    int item;
    static int out = 0;
    // Aguarda slot cheio
    sem_wait(&slotCheio);
    // Exclusão mútua entre consumidores
    sem_wait(&mutexCons);
    item = Buffer[out];
    out = (out + 1) % N;
    count--;
    sem_post(&mutexCons);
    // Sinaliza um slot vazio
    sem_post(&slotVazio);
    return item;
}

void *produtor(void *arg) {
    int elemento;
    if (count == 0) {
        Insere(elemento);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int elemento;
    while (count > 0) {
        elemento = Retira();
        // Consome o elemento....
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int N = 10; // Número de elementos no buffer
    int numThreads = N; // Número de threads produtoras e consumidoras
    pthread_t tid[numThreads];
    int *id[numThreads], t;

    for (t = 0; t < numThreads; t++) {
        if ((id[t] = malloc(sizeof(int))) == NULL) {
            pthread_exit(NULL);
            return 1;
        }
        *id[t] = t + 1;
    }

    // Inicia os semáforos
    sem_init(&mutexCons, 0, 1);
    sem_init(&mutexProd, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, N);

    for (t = 0; t < numThreads; t++) {
        if (pthread_create(&tid[t], NULL, produtor, (void *)id[t])) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

