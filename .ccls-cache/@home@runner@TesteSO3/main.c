/**************************
 *                         *
 *       Constantes        *
 *                         *
 **************************/
#define NUM_PROCESSES 1
#define TOT_COUNT 1000
#define TEM_ERRO -1
#define CALCULO 0
#define FILHO 0
#define FEITO 1
#define SHM_SIZE 1024

/**************************
 *                         *
 *         Includes        *
 *                         *
 **************************/
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int done = 2;

void dormida(long tempo) {

  long tmp = tempo * 10;

  struct timespec ts;

  ts.tv_sec = tmp / 1000;

  ts.tv_nsec = (tmp - (ts.tv_sec * 1000)) * 1000000;

  nanosleep(&ts, NULL);
}

void *thread(void *arg) {
  long tempo = atoi(arg);
  dormida(tempo);
  printf("Produziu %s \n", arg);
  return NULL;
}

void *manageThread(char tipo[], long tempo) {
  pthread_t thread_id;
  dormida(tempo);
  pthread_create(&thread_id, NULL, thread, "\0");
}

int main(int argc, char **argv) {

  pthread_t fabrica;

  char userInput[100] = {"\0"};
  pthread_t thread_id;

  while (userInput[0] != 'q') {
    puts("Insira comando:");
    sleep(1);
    scanf("%s", userInput);

    char tipo[100];
    pthread_create(&fabrica, NULL, thread, userInput);
  }
  pthread_exit(NULL);

  return 0;
  exit(EXIT_SUCCESS);
}
