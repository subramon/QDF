// Cuts input file into several output files
// Each output file has a max number of liones provided byuser 

#include "incs.h"
#include "utils_macros.h"
#include "rs_mmap.h"
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *ofp = NULL, *ifp = NULL;
  char outfile[1024]; memset(outfile, 0, 1024);
  if ( argc != 4 ) { go_BYE(-1); }  
  const char * const infile     = argv[1];
  const char * const out_prefix = argv[2];
  int num_lines  = atoi(argv[3]);
  if ( num_lines <= 0 ) { go_BYE(-1); }

  ifp = fopen(infile, "r");
  return_if_fopen_failed(ifp, infile, "r");
  int nF = 0; // number of output files 
  sprintf(outfile, "%s%d", out_prefix, ++nF);
  ofp = fopen(outfile, "w");
  return_if_fopen_failed(ofp, outfile, "w");
  int nL = 0; // number of lines in output file 
  for ( int i = 0; ; i++ ) { 
    if ( feof(ifp) ) { break; }
    // read a line at a time from Hive data set into X[nX]
    char *X = NULL; size_t nX = 0;
    size_t nr = getline(&X, &nX, ifp); 
    int len = 0;
    for ( int i = 0; i < nX; len++, i++ ) { 
      if ( X[i] == '\n' ) { len++; break; }
    }
    if ( nr <= 0 ) { break; }
    if ( nL >= num_lines ) {
      fclose_if_non_null(ofp);
      sprintf(outfile, "%s%d", out_prefix, ++nF);
      ofp = fopen(outfile, "w");
      return_if_fopen_failed(ofp, outfile, "w");
      nL = 0;
    }
    fwrite(X, len, 1, ofp);
    nL++;
    if ( (i%1000000) == 0 ) {
      printf("Processed %d lines \n", i);
    }
  }
BYE:
  fclose_if_non_null(ifp);
  fclose_if_non_null(ofp);
  return status;
}
