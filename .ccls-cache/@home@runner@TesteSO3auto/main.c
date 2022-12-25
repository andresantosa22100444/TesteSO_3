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
BufferCircular buff_escape, buff_janela, buff_porta, buff_chassi, buff_pneus,
    buff_jantes, buff_porcas, buff_valvulas, buff_cilindros, buff_caixa,
    buff_radiadores, buff_queue;

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
  Peca *p = (Peca *)arg;

  // printf("tempo: %d\n Peca %c\n", p->tempo, p->tipo);
  // printf("Thread %d\n", p->sem);
  dormida(p->tempo);
  printf("Produziu %c \n", p->tipo);
  pthread_exit(NULL);
}

void fabrica() {
  while (!bufferVazio(&buff_queue)) {

    Peca p = removerPeca(&buff_queue);
    switch (p.tipo) {
    case 'e':
      p = removerPeca(&buff_escape);
      sem_wait(sem_janela);
      // dormida(p.tempo);
      pthread_create(&tfabrica[1], NULL, produzir, &p);
      sem_post(sem_janela);

      break;
    case 'j':
      p = removerPeca(&buff_janela);
      sem_wait(sem_janela);
      // dormida(p.tempo);
      pthread_create(&tfabrica[2], NULL, produzir, &p);
      sem_post(sem_janela);
      break;
    case 'p':
      p = removerPeca(&buff_porta);
      sem_wait(sem_porta);
      // dormida(p.tempo);
      pthread_create(&tfabrica[3], NULL, produzir, &p);
      sem_post(sem_porta);
      break;
    case 'c':
      p = removerPeca(&buff_chassi);
      sem_wait(sem_chassi);
      // dormida(p.tempo);
      pthread_create(&tfabrica[4], NULL, produzir, &p);
      sem_post(sem_chassi);
      break;
    case 'u':
      p = removerPeca(&buff_pneus);
      sem_wait(sem_pneus);
      // dormida(p.tempo);
      pthread_create(&tfabrica[5], NULL, produzir, &p);
      sem_post(sem_pneus);
      break;
    case 't':
      p = removerPeca(&buff_jantes);
      sem_wait(sem_jantes);
      // dormida(p.tempo);
      pthread_create(&tfabrica[6], NULL, produzir, &p);
      sem_post(sem_jantes);
      break;
    case 'o':
      p = removerPeca(&buff_porcas);
      sem_wait(sem_porcas);
      // dormida(p.tempo);
      pthread_create(&tfabrica[7], NULL, produzir, &p);
      sem_post(sem_porcas);
      break;
    case 'v':
      p = removerPeca(&buff_valvulas);
      sem_wait(sem_valvulas);
      // dormida(p.tempo);
      pthread_create(&tfabrica[8], NULL, produzir, &p);
      sem_post(sem_valvulas);
      break;
    case 'l':
      p = removerPeca(&buff_cilindros);
      sem_wait(sem_cilindros);
      // dormida(p.tempo);
      pthread_create(&tfabrica[9], NULL, produzir, &p);
      sem_post(sem_cilindros);
      break;
    case 'x':
      p = removerPeca(&buff_caixa);
      sem_wait(sem_caixa);
      // dormida(p.tempo);
      pthread_create(&tfabrica[10], NULL, produzir, &p);
      sem_post(sem_caixa);
      break;
    case 'r':
      p = removerPeca(&buff_radiadores);
      sem_wait(sem_radiadores);
      // dormida(p.tempo);
      pthread_create(&tfabrica[11], NULL, produzir, &p);
      sem_post(sem_radiadores);

      break;
    default:
      break;
    }
  }
}

void pecaBuff(Peca p) {
  switch (p.tipo) {
  case 'e':
    inserirPeca(&buff_escape, p);
    break;
  case 'j':
    inserirPeca(&buff_janela, p);
    break;
  case 'p':
    inserirPeca(&buff_porta, p);
    break;
  case 'c':
    inserirPeca(&buff_chassi, p);
    break;
  case 'u':
    inserirPeca(&buff_pneus, p);
    break;
  case 't':
    inserirPeca(&buff_jantes, p);
    break;
  case 'o':
    inserirPeca(&buff_porcas, p);
    break;
  case 'v':
    inserirPeca(&buff_valvulas, p);
    break;
  case 'l':
    inserirPeca(&buff_cilindros, p);
    break;
  case 'x':
    inserirPeca(&buff_caixa, p);
    break;
  case 'r':
    inserirPeca(&buff_radiadores, p);
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
  inicializarBuffer(&buff_escape);
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
  inicializarBuffer(&buff_queue);

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
        tempo > 0 && tempo < 999) {

      Peca agrs = (Peca){tipo, tempo};
      inserirPeca(&buff_queue, agrs);
      // pecaBuff(agrs);

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