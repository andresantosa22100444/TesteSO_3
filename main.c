#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 100
/*
to do:
1. sem_queue
2. problemas do loop



*/

typedef struct {
  char tipo;
  int tempo;
  int sem;
} Peca;

#define BUFFER_SIZE 100

typedef struct {
  Peca buffer[BUFFER_SIZE];
  int inicio;
  int fim;
  int tamanho;
  int quantidade;
} BufferCircular;
/*BufferCircular buff_escape, buff_janela, buff_porta, buff_chassi, buff_pneus,
    buff_jantes, buff_porcas, buff_valvulas, buff_cilindros, buff_caixa,
    buff_radiadores, buff_queue;*/
BufferCircular buffPecas[12];
pthread_mutex_t lock[12];

sem_t *sem_escape, *sem_janela, *sem_porta, *sem_chassi, *sem_pneus,
    *sem_jantes, *sem_porcas, *sem_valvulas, *sem_cilindros, *sem_caixa,
    *sem_radiadores, *sem_queue;

pthread_t tfabrica[12];

void inicializarBuffer(BufferCircular *b) {
  b->inicio = 0;
  b->fim = 0;
  b->tamanho = BUFFER_SIZE;
  b->quantidade = 0;
}

int bufferVazio(BufferCircular *b) { return (b->quantidade == 0); }

int bufferCheio(BufferCircular *b) { return (b->quantidade == b->tamanho); }

void inserirPeca(BufferCircular *b, Peca p) {
  if (bufferCheio(b)) {
    // Tratar erro de buffer cheio
  } else {
    sem_wait(sem_queue);
    b->buffer[b->fim] = p;
    b->fim = (b->fim + 1) % b->tamanho;
    b->quantidade++;
    sem_post(sem_queue);
  }
}

Peca removerPeca(BufferCircular *b) {
  Peca p;
  if (bufferVazio(b)) {
    // Tratar erro de buffer vazio
  } else {
    sem_wait(sem_queue);
    p = b->buffer[b->inicio];
    b->inicio = (b->inicio + 1) % b->tamanho;
    b->quantidade--;
    sem_post(sem_queue);
  }
  return p;
}

void dormida(long tempo) {
  long tmp = tempo * 10;

  struct timespec ts;

  ts.tv_sec = tmp / 1000;

  ts.tv_nsec = (tmp - (ts.tv_sec * 1000)) * 1000000;

  nanosleep(&ts, NULL);
}

void *produzir(void *arg) {
  int i = *(int *)arg;
  while (!bufferVazio(&buffPecas[i])) {
    pthread_mutex_lock(&lock[i]);
    Peca p = removerPeca(&buffPecas[i]);
    dormida(p.tempo);
    printf("Produziu %c \n", p.tipo);
    pthread_mutex_unlock(&lock[i]);
    pthread_exit(NULL);
  };
  // printf("tempo: %d\n Peca %c\n", p->tempo, p->tipo);
  // printf("Thread %d\n", p->sem);
}

void fabrica() {
  for (int i = 0; i < 11; i++) {
    pthread_create(&tfabrica[1], NULL, produzir, &i);
  }
}

void pecaBuff(Peca p) {
  switch (p.tipo) {
  case 'e':
    inserirPeca(&buffPecas[1], p);
    break;
  case 'j':
    inserirPeca(&buffPecas[2], p);
    break;
  case 'p':
    inserirPeca(&buffPecas[3], p);
    break;
  case 'c':
    inserirPeca(&buffPecas[4], p);
    break;
  case 'u':
    inserirPeca(&buffPecas[5], p);
    break;
  case 't':
    inserirPeca(&buffPecas[6], p);
    break;
  case 'o':
    inserirPeca(&buffPecas[7], p);
    break;
  case 'v':
    inserirPeca(&buffPecas[8], p);
    break;
  case 'l':
    inserirPeca(&buffPecas[9], p);
    break;
  case 'x':
    inserirPeca(&buffPecas[10], p);
    break;
  case 'r':
    inserirPeca(&buffPecas[11], p);
    break;
  default:
    break;
  }
}

// Creates threads to deal with printing text

int main(int argc, char **argv) {
  // Faz unlinks
  sem_unlink("/sem_escape");
  sem_unlink("/sem_janela");
  sem_unlink("/sem_porta");
  sem_unlink("/sem_chassi");
  sem_unlink("/sem_pneus");
  sem_unlink("/sem_jantes");
  sem_unlink("/sem_porcas");
  sem_unlink("/sem_valvulas");
  sem_unlink("/sem_cilindros");
  sem_unlink("/sem_caixa");
  sem_unlink("/sem_radiadores");
  sem_unlink("/sem_queue");

  // Inicia os sem
  sem_escape = sem_open("/sem_escape", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_janela = sem_open("/sem_janela", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_porta = sem_open("/sem_porta", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_chassi = sem_open("/sem_chassi", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_pneus = sem_open("/sem_pneus", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_jantes = sem_open("/sem_jantes", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_porcas = sem_open("/sem_porcas", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_valvulas = sem_open("/sem_valvulas", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_cilindros = sem_open("/sem_cilindros", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_caixa = sem_open("/sem_caixa", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_radiadores = sem_open("/sem_radiadores", O_CREAT, S_IRUSR | S_IWUSR, 1);
  sem_queue = sem_open("/sem_queue", O_CREAT, S_IRUSR | S_IWUSR, 1);

  // Iniciar buff
  /*inicializarBuffer(&buff_escape);
  inicializarBuffer(&buff_janela);
  inicializarBuffer(&buff_porta);
  inicializarBuffer(&buff_chassi);
  inicializarBuffer(&buff_pneus);
  inicializarBuffer(&buff_jantes);
  inicializarBuffer(&buff_porcas);
  inicializarBuffer(&buff_valvulas);
  inicializarBuffer(&buff_cilindros);
  inicializarBuffer(&buff_caixa);
  inicializarBuffer(&buff_radiadores);
  inicializarBuffer(&buff_queue);*/
  for (int i = 0; i < 12; i++) {
    inicializarBuffer(&buffPecas[i]);
  }

  // printf("Insira comando:\n");
  char tipo;
  int tempo;

  do {
    fflush(stdout);
    fflush(stdin);
    scanf(" %c %d", &tipo, &tempo);
    // printf("Tipo: %c\nTempo: %d\n", tipo, tempo);
    if ((tipo == 'e' || tipo == 'j' || tipo == 'p' || tipo == 'c' ||
         tipo == 'u' || tipo == 't' || tipo == 'o' || tipo == 'v' ||
         tipo == 'l' || tipo == 'x' || tipo == 'r') &&
        tempo > 0 && tempo <= 999) {

      Peca agrs = (Peca){tipo, tempo};
      // inserirPeca(&buff_queue, agrs);
      pecaBuff(agrs);

      fabrica();

    }

    else if (tipo == 'q') {
      dormida(tempo);
      return 0;
    }

    else {
      printf("Comando invalido\n");
    }
  } while (1);

  return 0;
}