/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5
/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void *A (void *t) {
  int *my_id = (int*)t, i;

  printf("A: Comecei: thread %d\n", *my_id);
  
  while (*my_id <3) {
    pthread_mutex_lock(&x_mutex);
    
    if (*my_id ==1) {
      printf("Olá, Maria!");
      pthread_cond_signal(&x_cond);
    }
    else{
      printf("Olá, Jose!");
      pthread_cond_signal(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    break;
  }
  free(my_id);
  pthread_exit(NULL);
}

/* Thread que imprime x */
void *B (void *t) {
  int *my_id = (int*)t;

  printf("B: Comecei: thread %d\n", *my_id);

  pthread_mutex_lock(&x_mutex);
  while (*my_id!=3) { //para o caso de B ser sinalizada e antes de retornar a execucao o valor de x mudar!
  /*if ((x % 10) != 0) { /*** pode gerar resultados incorretos, por que??? ***/
     printf("B: thread %d x = %d, vai se bloquear...\n", *my_id, x);
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("B: thread %d, sinal recebido e mutex realocado. x = %d\n", *my_id, x);
  }
  /*...faz alguma coisa com x */   
  printf("Sente-se por favor");
  pthread_mutex_unlock(&x_mutex); 
  free(my_id);
  pthread_exit(NULL);
}

void *C (void *t) {
   int *my_id = (int*)t;

  printf("B: Comecei: thread %d\n", *my_id);

  pthread_mutex_lock(&x_mutex);
  while (*my_id<=3) { //para o caso de B ser sinalizada e antes de retornar a execucao o valor de x mudar!
  /*if ((x % 10) != 0) { /*** pode gerar resultados incorretos, por que??? ***/
     printf("B: thread %d x = %d, vai se bloquear...\n", *my_id, x);
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("B: thread %d, sinal recebido e mutex realocado. x = %d\n", *my_id, x);
  }
  /*...faz alguma coisa com x */   
    if (*my_id ==4) {
      printf("Tchau, Maria!");
      pthread_cond_signal(&x_cond);
    }
    else if (*my_id ==5){
      printf("Tchau, Jose!");
      pthread_cond_signal(&x_cond);
    }
  pthread_mutex_unlock(&x_mutex); 
  free(my_id);
  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  int *t1, *t2, *t3, *t4, *t5;
  pthread_t threads[NTHREADS];
  //aloca espaco para os identificadores das threads
  t1=malloc(sizeof(int)); 
  t2=malloc(sizeof(int)); 
  t3=malloc(sizeof(int)); 
  t4=malloc(sizeof(int)); 
  t5=malloc(sizeof(int)); 
  *t1=1, *t2=2, *t3=3, *t4=4, *t5=5;

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[1], NULL, A, (void *)t1);
  pthread_create(&threads[2], NULL, A, (void *)t2);
  pthread_create(&threads[3], NULL, B, (void *)t3);
  pthread_create(&threads[4], NULL, C, (void *)t4);
  pthread_create(&threads[5], NULL, C, (void *)t5);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("FIM.\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
