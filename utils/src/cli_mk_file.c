#include <stdio.h>
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
  char *endptr;
  if ( argc != 3 ) { go_BYE(-1); }
  char *file_name = argv[1]; 
  size_t file_size = strtoll(argv[2], &endptr, 10); 
  if ( *endptr != '\0' ) { go_BYE(-1); }
  status = mk_file(NULL, file_name, file_size);
  size_t chk_file_size = get_file_size(file_name);
  if ( chk_file_size != file_size ) { go_BYE(-1); }
BYE:
  return status;
}
