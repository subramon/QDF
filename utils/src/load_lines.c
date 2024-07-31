#include "incs.h"
#include "num_lines.h"
#include "auxil.h"
#include "load_lines.h"

#define BUFLEN 4095 // TODO P4 un hard code 
int
load_lines(
    const char * const infile,
    char ***ptr_X,
    int *ptr_nX
    )
{
  int status = 0;
  char **X = NULL; int nX = 0;
  FILE *fp = NULL; 
  char buf[BUFLEN+1];
  nX = num_lines(infile); if ( nX < 0 ) { go_BYE(-1); }
  X = malloc(nX * sizeof(char *));
  return_if_malloc_failed(X);
  memset(X, 0, nX * sizeof(char *));
  fp = fopen(infile, "r");
  return_if_fopen_failed(fp, infile, "r");
  for ( int i = 0; i < nX; i++ ) { 
    memset(buf, 0, BUFLEN+1);
    char *cptr = fgets(buf, BUFLEN, fp);
    if ( cptr == NULL ) { go_BYE(-1); }
    if ( buf[BUFLEN] != '\0' ) { go_BYE(-1); } // buf too small 
    X[i] = strdup(cptr); 
    int len = strlen(X[i]);
    for ( int j = len-1; j >= 0; j-- ) {
      if ( X[i][j] == '\n' ) { X[i][j] = '\0'; break; }
    }
  }

  *ptr_X = X;
  *ptr_nX = nX;
BYE:
  fclose_if_non_null(fp);
  return status;
}
#undef UNIT_TEST
#ifdef UNIT_TEST
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char **X = NULL; int nX = 0;
  status = load_lines("load_lines.c", &X, &nX); cBYE(status);
  for ( int i = 0; i < nX; i++ ) { 
    fprintf(stdout, "%d:%s \n", i, X[i]);
  }
  fprintf(stdout, "%d\n", nX);
BYE:
  free_2d_array(&X, nX);
  return status;
}
#endif
