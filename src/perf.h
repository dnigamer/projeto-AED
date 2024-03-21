// implementação de funções para monitorização de performance e contagem de instruções
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <sys\timeb.h>
#else
#include <sys/timeb.h>
#endif

int CONTA;
struct timeb TSTART, TEND;

void start_clock() { ftime(&TSTART); }

void stop_clock() {
  ftime(&TEND);
  int diff = (int)(1000.0 * (TEND.time - TSTART.time) + (TEND.millitm - TSTART.millitm));
  printf("\n ---> Tempo de execução: %u ms <---\n", diff);
}
