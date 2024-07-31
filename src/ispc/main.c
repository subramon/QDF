#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "if_a_then_not_b.h"
#include "alt_if_a_then_not_b.h"

static int
sortcompare(const void *p1, const void *p2)
{
  uint64_t *v1 = (uint64_t *)p1;
  uint64_t *v2 = (uint64_t *)p2;
  if ( *v1 < *v2 ) { 
    return -1;
  }
  else {
    return 1;
  }
}

uint64_t
RDTSC(
    void
    )
{
  unsigned int lo, hi;
  asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}
int
main(
    int argc,
    char **argv
    )
{
  int N = 4096;
  uint64_t X[N];
  int nT = 128; // number of trials
  int n_discard = 4;
  nT += (2*n_discard);
  uint64_t results[nT];

  int mode = 1;
  if ( argc == 1 ) { 
    mode = 1;
  }
  else if ( argc == 2 ) {
    if ( strcmp(argv[1], "gcc") == 0 ) { 
      mode = 1;
    }
    else if ( strcmp(argv[1], "ispc") == 0 ) { 
      mode = 2;
    }
    else {
      printf("error\n"); exit(1);
    }
  }
  else {
    printf("error\n"); exit(1);
  }

  srand48(RDTSC());
  for ( int j = 0; j < nT; j++ ) { 
    for ( int i = 0; i < N; i++ ) { 
      X[i] = lrand48();
    }
    int a = lrand48() % 64;
    int b = lrand48() % 64;
    uint64_t t_start = RDTSC();
    if ( mode == 1 ) { 
      if_a_then_not_b(X, N, a, b);
    }
    else {
      alt_if_a_then_not_b(X, N, a, b);
    }
    uint64_t t_stop  = RDTSC();
    results[j] = (t_stop - t_start);
  }
  qsort(results, nT, sizeof(uint64_t), sortcompare);
  double sum = 0;
  for ( int j = n_discard; j < nT-n_discard; j++ ) { 
    sum += results[j];
  }
  for ( int j = 1; j < nT; j++ ) { 
    if ( results[j] < results[j-1] ) {
      printf("error\n"); exit(-1);
    }
  }
  int n = nT - (2*n_discard);
  printf("t/%d = %lf\n", n, sum/n);
}
