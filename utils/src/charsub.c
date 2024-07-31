// YOU NEED to provide integer version of character
// So, newline is 10
// replaces a character by another in file 

#include "incs.h"
#include "rs_mmap.h"
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  if ( argc != 4 ) { go_BYE(-1); }  
  const char * const infile = argv[1];
  
  int itmp = atoi(argv[2]);
  if ( ( itmp < 0 ) || ( itmp > 127 ) ) { go_BYE(-1); }
  char oldchar = (char)itmp;
  
  itmp = atoi(argv[3]);
  if ( ( itmp < 0 ) || ( itmp > 127 ) ) { go_BYE(-1); }
  char newchar = (char)itmp;

  status = rs_mmap(infile, &X, &nX, 1); cBYE(status);
  int num_subs = 0;
  for ( size_t i = 0; i < nX; i++ ) { 
    if ( X[i] == oldchar ) { 
      X[i] = newchar;
      num_subs++;
    }
  }
  printf("Processed file %s with %d substitutions \n", infile, num_subs);



BYE:
  rs_munmap(X, nX);
  return status;
}
