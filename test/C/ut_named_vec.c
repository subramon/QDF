#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_pr.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "split_str.h"
#include "qdf_named_vec.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  QDF_REC_TYPE qdf, qdf_str;
  memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_str, 0, sizeof(QDF_REC_TYPE));
  char *keys = NULL;
  double *dvals = NULL;

  if ( argc != 1 ) { go_BYE(-1); }
  keys = malloc(128); 
  return_if_malloc_failed(keys); 
  memset(keys, 0, 128); 
  char *bak = keys;
  uint32_t n_keys = 0;


  strcpy(bak, "a"); bak += strlen("a") + 1; n_keys++;
  strcpy(bak, "bc"); bak += strlen("bc") + 1; n_keys++;
  strcpy(bak, "def"); bak += strlen("def") + 1; n_keys++;
  strcpy(bak, "ghij"); bak += strlen("ghij") + 1; n_keys++;
  dvals = malloc(n_keys * sizeof(double));
  for ( uint32_t  i = 0; i < n_keys; i++ ) { 
    dvals[i] = i+1;
  }
  status = qdf_named_vec(keys, 128, dvals, n_keys, &qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  uint32_t len = 0;
  status = pr_json(&qdf, &qdf_str, &len, NULL); cBYE(status);
  printf("Created %s \n", (char *)qdf_str.data); 
  if ( strcmp((char *)qdf_str.data,
      "{\"a\" : 1, \"bc\" : 2, \"def\" : 3, \"ghij\" : 4}") != 0 ) {
    go_BYE(-1);
  }
  printf("SUCCESS Test %s completed successfully\n", argv[0]);
  
BYE:
  free_qdf(&qdf);
  free_qdf(&qdf_str);
  free_if_non_null(keys);
  free_if_non_null(dvals);
  return status;
}
