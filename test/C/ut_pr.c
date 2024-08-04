#include "incs.h"
#include "free_2d_array.h"
#include "qdf_struct.h"
#include "get_file_size.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_external.h"
#include "qdf_setters.h"
#include "qdf_pr.h"
#include "num_lines.h"
#include "qdf_makers.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  size_t qdf_size; double dtemp;
  QDF_REC_TYPE qdf, str_qdf;
  uint32_t str_len = 0;
  double *dvals = NULL; uint32_t n_dvals = 3;
  int *ivals = NULL; uint32_t n_ivals = 10;
  char **svals = NULL; uint32_t n_svals = 2;
  void **vals = NULL; uint32_t n_vals = 3; 
  qtype_t *qtypes = NULL;
  memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  memset(&str_qdf, 0, sizeof(QDF_REC_TYPE));
  if ( argc != 1 ) { go_BYE(-1); }
  // check j_nil
  status = make_nil(NULL, &qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  qdf_size = x_get_qdf_size(&qdf); if ( qdf_size != 8 ) { go_BYE(-1); }
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  if ( strcmp(str_qdf.data, "null") != 0 ) { go_BYE(-1); } 
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;

  // check j_boolean
  status = make_boolean(true, &qdf); cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  if ( strcmp(str_qdf.data, "true") != 0 ) { go_BYE(-1); } 
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
  //-------------------
  status = make_boolean(false, &qdf); cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  if ( strcmp(str_qdf.data, "false") != 0 ) { go_BYE(-1); } 
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
  //-------------------
  //-------------------
  // Check make_number
  status = make_number(123.4567, &qdf); cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  dtemp = atof(str_qdf.data); 
  if ( dtemp != 123.4567 ) { go_BYE(-1); } 
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
  //-------------------
  // Check make_string
  status = make_string("123.4567", &qdf); cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  if ( strcmp(str_qdf.data, "\"123.4567\"") != 0 ) { go_BYE(-1); }
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
  //------------------- Make a uniform SC array 
  svals = malloc(n_svals * sizeof(char *));
  return_if_malloc_failed(svals);
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    char *x = NULL;
    x = malloc(32); memset(x, 0, 32);
    sprintf(x, "V_%d", i+1);
    svals[i] = x; 
  }
  status = make_SC_array(svals, NULL, 0, n_svals, 0, &qdf); 
  cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  if ( strcmp(str_qdf.data, "[\"V_1\", \"V_2\"]") != 0 ) { go_BYE(-1); }
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
  //------------------- Make a uniform F8 array 
  dvals = malloc(n_dvals * sizeof(double));
  for ( uint32_t i = 0; i < n_dvals; i++ ) { dvals[i] = i+1; }
  status = make_num_array(dvals, n_dvals, 0, F8, &qdf); cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  if ( strcmp(str_qdf.data, "[1, 2, 3]") != 0 ) { go_BYE(-1); }
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
  //------------------- Make a uniform I4 array 
  ivals = malloc(n_ivals * sizeof(int32_t));
  for ( uint32_t i = 0; i < n_ivals; i++ ) { ivals[i] = (int)(100*(i+1)); }
  status = make_num_array(ivals, n_ivals, 0, I4, &qdf); cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  if ( strcmp(str_qdf.data, 
        "[100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]") 
      != 0 ) { go_BYE(-1); }
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
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
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((uint8_t  *)vals[0])[i] = (uint8_t)(i+1);
    ((float    *)vals[1])[i] = (float)(100.0*(i+1));
  }
  //----------------------
  status = make_data_frame(svals, n_svals, NULL, 
      (const void **)vals, n_vals, 0, qtypes, &qdf);
  cBYE(status);
  status = pr_json(&qdf, &str_qdf, &str_len, NULL); cBYE(status);
  // TODO P1 FIX FIX FIX 
  if ( strcmp(str_qdf.data, 
        "{\"V_1\" : [1, 2, 3], \"V_2\" : [100, 200, 300]}")
      != 0 ) { go_BYE(-1); }
  free_qdf(&qdf); free_qdf(&str_qdf); str_len = 0;
 
  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(dvals);
  free_if_non_null(ivals);
  free_if_non_null(qtypes);
  free_2d_array(&svals, n_svals);
  free_2d_array((char ***)&vals, n_svals);
  return status;
}
