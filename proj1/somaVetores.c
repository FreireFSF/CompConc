
#include <stdio.h>
#include <pthread.h>

int vetores[10000];

void *mutiplica_por_dois(void *thread_id) {
  printf("Thread %l\n", (long) thread_id);
   for ( int id = *(int*)thread_id; id < 10000; id += 2) {
        vetores[id] *= 2;
    }
    pthread_exit(NULL);
}

int main() {
  pthread_t threads[2];
  int thread_args[2];

    for (int i = 1; i < 10000; i+=1) {
        vetores[i] = 1;
    }

    long i = 0 ;
  for (i; i < 2; i++) {
    thread_args[i] = i;
    pthread_create(&threads[i], NULL, mutiplica_por_dois, (void*)&thread_args[i]);
  }

  for (int i = 0; i < 2; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Resultado:\n");
    for (int i = 0; i < 10000; i++) {
        printf("%d ", vetores[i]);
    }

  return 0;
}
