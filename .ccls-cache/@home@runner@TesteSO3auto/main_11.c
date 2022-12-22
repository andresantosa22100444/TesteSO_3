#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

long tempoEspera = 0;
sem_t *sem_body, *sem_roda, *sem_motor;
pthread_mutex_t mutexBody = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexRoda = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMotor = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
  char componenteAConstruir;
  long tempoEspera;
} tarefa;

int body[4] = {0, 0, 0, 0};
char esperaBody[10] = {};
int esperaBodyCount = 0;
int esperaBodyCountIni = 0;

int roda[3] = {0, 0, 0};
char esperaRoda[10] = {};
int esperaRodaCount = 0;
int esperaRodaCountIni = 0;

int motor[4] = {0, 0, 0, 0};
char esperaMotor[10] = {};
int esperaMotorCount = 0;
int esperaMotorCountIni = 0;

int carro[3] = {0, 0, 0};
sem_t sem;

void fflushInOut() {
  fflush(stdout);
  fflush(stdin);
}

/******************************************************************************
 * *
 * FUNCAO dormida *
 * *
 * Esta funcao suspende a execucao do progama durante 10 no máximo *
 * Parametro tempo - Valor de espera em 10*milisegundos *
 * *
 ******************************************************************************/
void dormida(long tempo) {
  long tmp = tempo * 10;
  struct timespec ts;
  ts.tv_sec = tmp / 1000;
  ts.tv_nsec = (tmp - (ts.tv_sec * 1000)) * 1000000;
  nanosleep(&ts, NULL);
}

int validaLetra(char letra) {
  if (letra == 'e' || letra == 'j' || letra == 'p' || letra == 'c' ||
      letra == 'u' || letra == 't' || letra == 'o' || letra == 'v' ||
      letra == 'l' || letra == 'x' || letra == 'r' || letra == 'q') {
    return 1;
  }
  return 0;
}

void marcaComponenteConcluida(char componente) {
  switch (componente) {
  case 'e':
    body[0]++;
    break;
  case 'j':
    body[1]++;
    break;
  case 'p':
    body[2]++;
    break;
  case 'c':
    body[3]++;
    break;
  case 'u':
    roda[0]++;
    break;
  case 't':
    roda[1]++;
    break;
  case 'o':
    roda[2]++;
    break;
  case 'v':
    motor[0]++;
    break;
  case 'l':
    motor[1]++;
    break;
  case 'x':
    motor[2]++;
    break;
  case 'r':
    motor[3]++;
    break;
  case 'C':
    carro[0]++;
    break;
  case 'R':
    carro[1]++;
    break;
  case 'M':
    carro[2]++;
    break;
  }
}

void validaFimConstrucao() {
  if (body[0] == 1 && body[1] == 5 && body[2] == 4 && body[3] == 1) {
    memset(body, 0, sizeof(body));
    printf("Produziu C\n");
    fflushInOut();
  }

  if (roda[0] == 4 && roda[1] == 4 && roda[2] == 16) {
    memset(roda, 0, sizeof(roda));
    printf("Produziu R\n");
    fflushInOut();
  }

  if (motor[0] == 16 && motor[1] == 4 && motor[2] == 1 && motor[3] == 2) {
    memset(motor, 0, sizeof(motor));
    printf("Produziu M\n");
    fflushInOut();
  }

  if (carro[0] == 1 && carro[1] == 4 && carro[2] == 1) {
    memset(carro, 0, sizeof(carro));
    printf("Produziu A\n");
    fflushInOut();
  }
}

void adicionaListaEspera(char componente) {
  switch (componente) {
  case 'e':
  case 'j':
  case 'p':
  case 'c': {
    esperaBody[esperaBodyCount] = componente;
    esperaBodyCount++;
    break;
  }

  case 'u':
  case 't':
  case 'o': {
    esperaRoda[esperaRodaCount] = componente;
    esperaRodaCount++;
    break;
  }

  case 'v':
  case 'l':
  case 'x':
  case 'r': {
    esperaMotor[esperaMotorCount] = componente;
    esperaMotorCount++;
    break;
  }
  }
}

void *threadBody(void *arg) {
  tarefa *args = (tarefa *)arg;
  char componente = args->componenteAConstruir;
  long tempo = args->tempoEspera;

  marcaComponenteConcluida(componente);
  dormida(tempo);
  printf("Produziu %c\n", esperaBody[esperaBodyCountIni]);
  esperaBodyCountIni++;
  fflushInOut();
  validaFimConstrucao();
  sem_post(sem_body);
  // pthread_mutex_unlock(&mutexBody);
  pthread_exit(NULL);
}

void fazPecaBody(long tempoEspera) {
  if (esperaBodyCountIni != esperaBodyCount) {
    pthread_t thread_id;

    sem_wait(sem_body);
    // pthread_mutex_lock(&mutexBody);
    tarefa args = {esperaBody[esperaBodyCountIni], tempoEspera};
    pthread_create(&thread_id, NULL, threadBody, &args);
  }
}

void *threadRoda(void *arg) {
  tarefa *args = (tarefa *)arg;
  char componente = args->componenteAConstruir;
  long tempo = args->tempoEspera;

  marcaComponenteConcluida(componente);
  dormida(tempo);
  printf("Produziu %c\n", esperaRoda[esperaRodaCountIni]);
  esperaRodaCountIni++;
  fflushInOut();
  validaFimConstrucao();
  sem_post(sem_roda);
  // pthread_mutex_unlock(&mutexRoda);
  pthread_exit(NULL);
}

void fazPecaRoda(long tempoEspera) {
  if (esperaRodaCountIni != esperaRodaCount) {
    pthread_t thread_id;

    sem_wait(sem_roda);
    // pthread_mutex_lock(&mutexRoda);
    tarefa args = {esperaRoda[esperaRodaCountIni], tempoEspera};

    pthread_create(&thread_id, NULL, threadRoda, &args);
  }
}

void *threadMotor(void *arg) {
  tarefa *args = (tarefa *)arg;
  char componente = args->componenteAConstruir;
  long tempo = args->tempoEspera;

  marcaComponenteConcluida(componente);
  dormida(tempo);
  printf("Produziu %c\n", esperaMotor[esperaMotorCountIni]);
  esperaMotorCountIni++;
  fflushInOut();
  validaFimConstrucao();
  sem_post(sem_motor);
  // pthread_mutex_unlock(&mutexMotor);
  pthread_exit(NULL);
}

void fazPecaMotor(long tempoEspera) {
  if (esperaMotorCountIni != esperaMotorCount) {
    pthread_t thread_id;

    sem_wait(sem_motor);
    // pthread_mutex_lock(&mutexMotor);
    tarefa args = {esperaMotor[esperaMotorCountIni], tempoEspera};

    pthread_create(&thread_id, NULL, threadMotor, &args);
  }
}

int main(int argc, char **argv) {
  // int main(void) {

  // e,j,p,c,u,t,o,v,l,x,r e q.
  // printf("%d\n", validaLetra('w'));

  char componente = ' ';
  int count = 0;

  sem_init(&sem, 0, 2);

  sem_unlink("/fazPecaBody");
  sem_body = sem_open("/fazPecaBody", O_CREAT, S_IRUSR | S_IWUSR, 50);

  sem_unlink("/fazPecaMotor");
  sem_motor = sem_open("/fazPecaMotor", O_CREAT, S_IRUSR | S_IWUSR, 50);

  sem_unlink("/fazPecaRoda");
  sem_roda = sem_open("/fazPecaRoda", O_CREAT, S_IRUSR | S_IWUSR, 50);

  do {
    pthread_t thread_id;
    count++;
    tempoEspera = 0;

    /*sem_consumidor_body =
        sem_open("/fazPecaBody", O_CREAT, S_IRUSR | S_IWUSR, 0);*/

    printf("Insira comando:\n");
    fflushInOut();
    scanf(" %c %ld", &componente, &tempoEspera);

    // termina o programa
    if (componente == 'q') {
      dormida(tempoEspera);
      // pthread_exit(NULL);
      return 0;
    }

    if (!validaLetra(componente) || tempoEspera > 999 || tempoEspera <= 0) {
      printf("Comando invalido\n");
      fflushInOut();
    } else {

      adicionaListaEspera(componente);
      fazPecaBody(tempoEspera);
      fazPecaRoda(tempoEspera);
      fazPecaMotor(tempoEspera);
    }
  } while (1);
}