#include "incs.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "macros.h"
#include "rs_mmap.h"
#include "configs.h"
#include "holiday.h"
#include "read_configs.h"
#include "free_configs.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  config_t C;
  FILE *ofp = NULL;
  holiday_enum_t *out_holidays = NULL;
  memset(&C, 0, sizeof(config_t));
  if ( argc != 4 ) { go_BYE(-1); }
  const char * const binfile = argv[1];
  const char * const outfile = argv[2];
  const char * const cfgfile = argv[3];

  status = read_configs(cfgfile, &C); cBYE(status);
  status = rs_mmap(binfile, &X, &nX, 1);
  // replace eoln with nullc
  int n_rows = 0;
  for ( size_t i = 0; i < nX; i++ ) { 
    if ( X[i] == '\n' ) { X[i] = '\0'; }
    if ( X[i] == '\0' ) { n_rows++; }
  }
  out_holidays = malloc(n_rows * sizeof(holiday_enum_t *));
  return_if_malloc_failed(out_holidays);

  status = holiday_xform(X, n_rows, out_holidays); cBYE(status);
  printf("Successfully completed %s %s, %s \n", argv[0], argv[1], argv[2]);
  ofp = fopen(outfile, "w");
  return_if_fopen_failed(ofp, outfile, "w");
  fprintf(ofp, "idx,num_holidays\n");
  char buf[1024];
  for ( int i = 0; i < n_rows; i++ ) { 
    memset(buf, 0, 1024);
    holiday_enum_t bmask = out_holidays[i];
    status = holiday_bmask_to_str(C.holiday_str, buf, 1024, bmask);
    int n_holidays = __builtin_popcountll(out_holidays[i]);
    fprintf(ofp, "%d,%u,%s\n", i, n_holidays, buf);

  }
BYE:
  free_configs(&C);
  fclose_if_non_null(ofp);
  free_if_non_null(out_holidays);
  if ( X != NULL ) { munmap(X, nX); }
  return status;
}
