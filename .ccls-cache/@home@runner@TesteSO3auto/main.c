#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  char tipo;
  int tempo;
  int sem;
} Peca;

int valvulas, cilindros, caixas, radiadores, rodas, carrocarias, motores,
    automoveis;
sem_t mutex_valvulas, mutex_cilindros, mutex_caixas, mutex_radiadores,
    mutex_rodas, mutex_carrocarias, mutex_motores, mutex_automoveis;

sem_t mutex[16];

void dormida(long tempo) {
  long tmp = tempo * 10;

  struct timespec ts;

  ts.tv_sec = tmp / 1000;

  ts.tv_nsec = (tmp - (ts.tv_sec * 1000)) * 1000000;

  nanosleep(&ts, NULL);
}

void *produzir(void *arg) {
  Peca *p = (Peca *)arg;

  // printf("tempo: %d", p->tempo);
  dormida(p->tempo);
  sem_wait(&mutex[p->sem]);
  printf("Produziu %c\n", p->tipo);
  sem_post(&mutex[p->sem]);
  pthread_t thread;

  pthread_exit(NULL);
}

void *fabrica(void *arg) {
  Peca *p = (Peca *)arg;

  // printf("tempo: %d", p->tempo);
  // dormida(p->tempo);

  printf("Produziu %c\n", p->tipo);
  pthread_t thread[16];

  switch (p->tipo) {
  case 'e':
    p->sem = 1;
    pthread_create(&thread[1], NULL, produzir, &p);
    pthread_join(thread[1],NULL);
    break;
  case 'j':
    p->sem = 2;
    pthread_create(&thread[2], NULL, produzir, &p);
    pthread_join(thread[1],NULL);
    break;
  case 'p':
    p->sem = 3;
    pthread_create(&thread[3], NULL, produzir, &p);
    break;
  case 'c':
    p->sem = 4;
    pthread_create(&thread[4], NULL, produzir, &p);
    break;
  case 'u':
    p->sem = 5;
    pthread_create(&thread[5], NULL, produzir, &p);
    break;
  case 't':
    p->sem = 6;
    pthread_create(&thread[6], NULL, produzir, &p);
    break;
  case 'o':
    p->sem = 7;
    pthread_create(&thread[7], NULL, produzir, &p);
    break;
  case 'v':
    p->sem = 8;
    pthread_create(&thread[8], NULL, produzir, &p);
    break;
  case 'l':
    p->sem = 9;
    pthread_create(&thread[9], NULL, produzir, &p);
    break;
  case 'x':
    p->sem = 10;
    pthread_create(&thread[10], NULL, produzir, &p);
    break;
  case 'r':
    p->sem = 11;
    pthread_create(&thread[11], NULL, produzir, &p);
    break;
  }

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  char tipo;
  int tempo;
  for (int i = 1; i < 16; ++i) {
    sem_init(&mutex[i], 0, 1);
  }

  pthread_t thread;
  while (1) {
    char tipo;
    int tempo;
    // printf("Insira comando:\n");
    scanf(" %c %d", &tipo, &tempo);
    // printf("Tipo: %c\nTempo: %d\n", tipo, tempo);

    if ((tipo == 'e' || tipo == 'j' || tipo == 'p' || tipo == 'c' ||
         tipo == 'u' || tipo == 't' || tipo == 'o' || tipo == 'v' ||
         tipo == 'l' || tipo == 'x' || tipo == 'r') &&
        tempo > 0 && tempo < 999) {

      Peca p = {tipo, tempo};
      pthread_create(&thread, NULL, fabrica, &p);

    }

    else if (tipo == 'q') {
      dormida(tempo);
      return 0;
    }

    else {
      printf("Comando invalido\n");
    }
  }
  pthread_exit(NULL);
  return 0;
}