// Copies input to output but suppresses every line upto and including
// the first line that matches the pattern
// If input were
// abc
// defg
// hijkl
// mnopqr
// tsuvwxy
// and pattern were hijkl
// then output would be 
// mnopqr
// tsuvwxy
// In addition, lines starting with # are suppressed
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils_macros.h"
#include "mk_file.h"
#include "get_file_size.h"
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *ifp = NULL, *ofp = NULL;
#define MAXLEN 127
  char buf[MAXLEN+1];

  if ( argc != 4 ) { go_BYE(-1); }
  char *in_file_name  = argv[1]; 
  char *pattern       = argv[2]; 
  char *out_file_name = argv[3]; 

  if ( strcmp(in_file_name, out_file_name) == 0 ) { go_BYE(-1); }

  ifp = fopen(in_file_name, "r");
  return_if_fopen_failed(ifp,  in_file_name, "r");
  ofp = fopen(out_file_name, "w");
  return_if_fopen_failed(ofp,  out_file_name, "w");

  bool copy = false;
  for ( ; !feof(ifp); ) { 
    memset(buf, 0, MAXLEN+1);
    char *cptr = fgets(buf, MAXLEN, ifp);
    if ( feof(ifp) ) { break; } 
    if ( cptr == NULL ) { break; }
    if ( copy == false ) {
      cptr = strstr(buf, pattern);
      if ( cptr != NULL ) { copy = true; }
      continue;
    }
    if ( buf[0] == '#' ) { // comment line 
      continue;
    }
    fprintf(ofp, "%s", buf);
  }
BYE:
  fclose_if_non_null(ifp);
  fclose_if_non_null(ofp);

  return status;
}
