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
  }
  return calculo;
}

int main(int argc, char **argv) {

  int numeroDeIteracoes = 0;
  int nrMagico = 0;
  int processos = 0;

  puts("Quantas iteracoes?");
  scanf(" %d", &numeroDeIteracoes);

  puts("Qual o numero magico??");
  scanf(" %d", &nrMagico);

  if (numeroDeIteracoes < 5000) {
    processos = 2;
  }
  if (numeroDeIteracoes > 5000 && numeroDeIteracoes > 10000) {
    processos = 3;
  } else {
    processos = 4;
  }
  int i;
  pid_t childPid, pid;
  int status = 0;

  for (i = 0; i < processos; ++i) {
    if (fork() == 0) {
      int cenas = numeroDeIteracoes / 2;
      for (int c = 1; c <= cenas; c++) {

        printf("%d \n", c);
      }

      exit(0);
    }
  }
while ((pid = wait(NULL)) > 0); //Wait for all child to complete
}
