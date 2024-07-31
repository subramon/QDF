#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "get_file_size.h"
#include "rbc_helpers.h"
#include "rbc_xhelpers.h"
#include "rbc_checkers.h"
#include "rbc_external.h"
#include "rbc_setters.h"
#include "rbc_pr.h"
#include "num_lines.h"
#include "rbc_makers.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  size_t rbc_size; double dtemp;
  RBC_REC_TYPE rbc, str_rbc;
  uint32_t str_len = 0;
  double *dvals = NULL; int n_dvals = 3;
  int *ivals = NULL; int n_ivals = 10;
  char **svals = NULL; int n_svals = 2;
  void **vals = NULL; int n_vals = 3; 
  qtype_t *qtypes = NULL;
  memset(&rbc, 0, sizeof(RBC_REC_TYPE));
  memset(&str_rbc, 0, sizeof(RBC_REC_TYPE));
  if ( argc != 1 ) { go_BYE(-1); }
  // check j_nil
  status = make_nil(NULL, &rbc); cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  rbc_size = x_get_rbc_size(&rbc); if ( rbc_size != 8 ) { go_BYE(-1); }
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  if ( strcmp(str_rbc.data, "null") != 0 ) { go_BYE(-1); } 
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;

  // check j_boolean
  status = make_boolean(true, &rbc); cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  if ( strcmp(str_rbc.data, "true") != 0 ) { go_BYE(-1); } 
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
  //-------------------
  status = make_boolean(false, &rbc); cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  if ( strcmp(str_rbc.data, "false") != 0 ) { go_BYE(-1); } 
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
  //-------------------
  //-------------------
  // Check make_number
  status = make_number(123.4567, &rbc); cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  dtemp = atof(str_rbc.data); 
  if ( dtemp != 123.4567 ) { go_BYE(-1); } 
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
  //-------------------
  // Check make_string
  status = make_string("123.4567", &rbc); cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  if ( strcmp(str_rbc.data, "\"123.4567\"") != 0 ) { go_BYE(-1); }
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
  //------------------- Make a uniform SC array 
  svals = malloc(n_svals * sizeof(char *));
  return_if_malloc_failed(svals);
  for ( int i = 0; i < n_svals; i++ ) { 
    char *x = NULL;
    x = malloc(32); memset(x, 0, 32);
    sprintf(x, "V_%d", i+1);
    svals[i] = x; 
  }
  status = make_SC_array(svals, NULL, 0, n_svals, 0, &rbc); cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  if ( strcmp(str_rbc.data, "[\"V_1\", \"V_2\"]") != 0 ) { go_BYE(-1); }
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
  //------------------- Make a uniform F8 array 
  dvals = malloc(n_dvals * sizeof(double));
  for ( int i = 0; i < n_dvals; i++ ) { dvals[i] = i+1; }
  status = make_num_array(dvals, n_dvals, 0, F8, &rbc); cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  if ( strcmp(str_rbc.data, "[1, 2, 3]") != 0 ) { go_BYE(-1); }
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
  //------------------- Make a uniform I4 array 
  ivals = malloc(n_ivals * sizeof(int32_t));
  for ( int i = 0; i < n_ivals; i++ ) { ivals[i] = 100*(i+1); }
  status = make_num_array(ivals, n_ivals, 0, I4, &rbc); cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  if ( strcmp(str_rbc.data, 
        "[100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]") 
      != 0 ) { go_BYE(-1); }
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
  //------------------- Make a data frame 
  vals = malloc(n_svals * sizeof(void *));
  memset(vals, 0, n_svals * sizeof(void *));

  qtypes = malloc(n_svals * sizeof(qtype_t));
  memset(qtypes, 0, n_svals * sizeof(qtype_t));
    // terrible hack but works for testing
  if ( n_svals != 2 ) { go_BYE(-1); }  
  qtypes[0] = I1;
  qtypes[1] = F4;
  vals[0] = malloc(n_vals * sizeof(int8_t));
  vals[1] = malloc(n_vals * sizeof(float));
  for ( int i = 0; i < n_vals; i++ ) { 
    ((uint8_t  *)vals[0])[i] = i+1;
    ((float    *)vals[1])[i] = 100*(i+1);
  }
  //----------------------
  status = make_data_frame(svals, n_svals, NULL, vals, n_vals, 0, 
      qtypes, &rbc);
  cBYE(status);
  status = pr_json(&rbc, &str_rbc, &str_len, NULL); cBYE(status);
  // TODO P1 FIX FIX FIX 
  if ( strcmp(str_rbc.data, 
        "{\"V_1\" : [1, 2, 3], \"V_2\" : [100, 200, 300]}")
      != 0 ) { go_BYE(-1); }
  free_rbc(&rbc); free_rbc(&str_rbc); str_len = 0;
 
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(dvals);
  free_if_non_null(ivals);
  free_if_non_null(qtypes);
  free_2d_array(&svals, n_svals);
  free_2d_array((char ***)&vals, n_svals);
  return status;
}
