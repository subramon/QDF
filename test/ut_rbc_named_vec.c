#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "rbc_helpers.h"
#include "rbc_pr.h"
#include "rbc_checkers.h"
#include "rbc_makers.h"
#include "split_str.h"
#include "rbc_named_vec.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  RBC_REC_TYPE rbc, rbc_str;
  memset(&rbc, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_str, 0, sizeof(RBC_REC_TYPE));
  char *keys = NULL;
  double *dvals = NULL;

  if ( argc != 1 ) { go_BYE(-1); }
  keys = malloc(128); 
  return_if_malloc_failed(keys); 
  memset(keys, 0, 128); 
  char *bak = keys;
  int n_keys = 0;


  strcpy(bak, "a"); bak += strlen("a") + 1; n_keys++;
  strcpy(bak, "bc"); bak += strlen("bc") + 1; n_keys++;
  strcpy(bak, "def"); bak += strlen("def") + 1; n_keys++;
  strcpy(bak, "ghij"); bak += strlen("ghij") + 1; n_keys++;
  dvals = malloc(n_keys * sizeof(double));
  for ( int i = 0; i < n_keys; i++ ) { 
    dvals[i] = i+1;
  }
  status = rbc_named_vec(keys, 128, dvals, n_keys, &rbc); cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  uint32_t len = 0;
  status = pr_json(&rbc, &rbc_str, &len, NULL); cBYE(status);
  printf("Created %s \n", (char *)rbc_str.data); 
  if ( strcmp((char *)rbc_str.data,
      "{\"a\" : 1, \"bc\" : 2, \"def\" : 3, \"ghij\" : 4}") != 0 ) {
    go_BYE(-1);
  }
  printf("Test %s completed successfully\n", argv[0]);
  
BYE:
  free_rbc(&rbc);
  free_rbc(&rbc_str);
  free_if_non_null(keys);
  free_if_non_null(dvals);
  return status;
}
