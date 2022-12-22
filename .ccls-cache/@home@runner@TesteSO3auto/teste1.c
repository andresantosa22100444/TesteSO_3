#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  char tipo;
  int tempo;
} Peca;

void dormida(long tempo) {

  long tmp = tempo * 10;

  struct timespec ts;

  ts.tv_sec = tmp / 1000;

  ts.tv_nsec = (tmp - (ts.tv_sec * 1000)) * 1000000;

  nanosleep(&ts, NULL);
}

void *produzir(void *arg) {
  Peca *p = (Peca *)arg;

  dormida(p->tempo);

  printf("Produziu %c\n", p->tipo);

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  char tipo;
  int tempo;

  while (1) {
    printf("Insira comando:");
    scanf(" %c%d", &tipo, &tempo);

    if (tipo == 'q') {

      sleep(tempo);
      exit(0);
    }

    Peca p = {tipo, tempo};

    pthread_t t;
    pthread_create(&t, NULL, produzir, &p);
    pthread_join(t, NULL);
  }

  return 0;
}
