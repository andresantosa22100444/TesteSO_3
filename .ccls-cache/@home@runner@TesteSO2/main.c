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

/******************************************************************************
 *                                                                             *
 *                              FUNCAO randNumGen                              *
 *                                                                             *
 *                         Gerador de numero aleatorios                        *
 *                                                                             *
 ******************************************************************************/
double randNumGen() {
  int random_value = rand(); // Gera numero aleatorio
  // Mete o valor gerado entre 0 and 1
  double unit_random = random_value / (double)RAND_MAX;
  return unit_random;
}

/******************************************************************************
 *                                                                             *
 *                              FUNCAO fazCalculo                              *
 *                                                                             *
 * Algoritmo ficticio de teste                                                 *
 *                                                                             *
 * Parametros:                                                                 *
 * it_inicio - Numero onde comecam as iteracoes                                *
 * it_fim - Numero onde terminam as iteracoes                                  *
 * num_magico - Numero que se adicionar em cada iteracao                       *
 *                                                                             *
 ******************************************************************************/
long fazCalculo(long it_inicio, long it_fim, long num_magico) {
  double calculo = 0; // resultado do calculo
  double x, y;        // variaveis que irao guardar numeros aleatorios
  // Formula de calculo fornecido
  for (long i = it_inicio; i < it_fim; i++) {
    // gera um numero random dependente da iteracao
    srand(i);
    x = randNumGen();
    y = randNumGen();
    double result = sqrtf((x * x) + (y * y));
    if (result < 1)
      calculo += num_magico;
    // printf("Itera????o %ld calculo =%f\n", i, calculo);
  }

  return calculo;
}

long *criarmemoria() {
  long *ptr;
  int ret;
  int fd = shm_open("/shm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("shm");
    exit(1);
  }
  ret = ftruncate(fd, SHM_SIZE);
  if (ret == -1) {
    perror("shm");
    exit(2);
  }
  ptr = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    perror("shm-mmap");
    exit(3);
  }
  return ptr;
}

int main(int argc, char **argv) {

  int numeroDeIteracoes = 0;
  int nrMagico = 0;
  int processos = 0;
  long *memory = criarmemoria();

  puts("Quantas iteracoes?");
  scanf(" %d", &numeroDeIteracoes);

  puts("Qual o numero magico?");
  scanf(" %d", &nrMagico);

  if (numeroDeIteracoes > 2350000  && numeroDeIteracoes < 3500000) {
    processos = 2;
  }
  if (numeroDeIteracoes >= 3500000 && numeroDeIteracoes <= 3650000) {
    processos = 3;
  }
  if (numeroDeIteracoes > 3650000) {
    processos = 4;
  } else if (processos == 0) {
    processos = 1;
  }
  // processos = 4;
  pid_t pids[processos];
  int status = 0;
  long t_ini = 0;
  long t_div = (numeroDeIteracoes / processos);
  long t_fim = t_div;
  time_t begin = time(NULL);



  for (int p = 0; p < processos; p++) {
    pid_t cur_pid = fork();
    long temp;
    if (cur_pid < 0) {
      exit(EXIT_FAILURE);
    } else if (cur_pid > 0) {
      /* Parent process */

    } else {
      /* Child process */
      pids[p] = cur_pid;
      //printf("Processo nr%d\n", p);
      *memory += fazCalculo(t_ini, t_fim, nrMagico);

      exit(EXIT_SUCCESS);
    }
    t_ini = t_fim;
    t_fim += t_div;
  }

  for (int i = 0; i < processos; i++) {
    wait(&pids[i]);
  }

  /*for (int p = 0; p < processos; ++p) {
    t_processo = clock();
    if ((pids[p] = fork()) < 0) {
      perror("fork");
      abort();
    } else if (pids[p] == 0) {
      printf("Processo %d come??ou\n", p);
      t_processo = clock();
      *memory += fazCalculo(t_ini, t_fim, nrMagico);

      exit(0);
    }
  }*/

  printf("Valor calculado: %ld\n", *memory);
  time_t end = time(NULL);
  // printf("The elapsed time is %ld seconds", (end - begin));

  *memory = shm_unlink("/shm");
  return 0;
  exit(EXIT_SUCCESS);
}
