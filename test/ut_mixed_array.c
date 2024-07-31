#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "get_file_size.h"
#include "rbc_helpers.h"
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
  RBC_REC_TYPE *in_rbcs = NULL;

  char **svals = NULL; int n_svals = 4;
  double *dvals = NULL; int n_dvals = 10;
  int32_t *ivals = NULL; int n_ivals = 20;
  void **vals = NULL; int n_vals = 123;

  qtype_t *qtypes = NULL;

  RBC_REC_TYPE rbc_mixed;
  RBC_REC_TYPE rbc_SC_arr, rbc_F8_arr, rbc_I4_arr, rbc_df;
  RBC_REC_TYPE rbc_nil, rbc_true, rbc_false, rbc_num, rbc_str;

  memset(&rbc_mixed, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_SC_arr, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_F8_arr, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_I4_arr, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_df, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_nil, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_true, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_false, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_num, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_str, 0, sizeof(RBC_REC_TYPE));

  if ( argc != 1 ) { go_BYE(-1); } 
  // make j_nil
  status = make_nil(NULL, &rbc_nil); cBYE(status);
  // make j_boolean
  status = make_boolean(true, &rbc_true); cBYE(status);
  status = make_boolean(false, &rbc_false); cBYE(status);
  //-------------------
  // make_number
  status = make_number(123.4567, &rbc_num); cBYE(status);
  //-------------------
  // make_string
  status = make_string("123.4567", &rbc_str); cBYE(status);
  //------------------- Make a uniform SC array 
  svals = malloc(n_svals * sizeof(char *));
  return_if_malloc_failed(svals);
  for ( int i = 0; i < n_svals; i++ ) { 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    svals[i] = strdup(buf);
  }
  status = make_SC_array(svals, NULL, 0, n_svals, 0, &rbc_SC_arr); 
  cBYE(status);
  //------------------- Make a uniform F8 array 
  dvals = malloc(n_dvals * sizeof(double));
  for ( int i = 0; i < n_dvals; i++ ) { 
    dvals[i] = i+1;
  }
  status = make_num_array(dvals, n_dvals, 0, F8, &rbc_F8_arr); cBYE(status);
  //------------------- Make a uniform I4 array 
  ivals = malloc(n_ivals * sizeof(int32_t));
  for ( int i = 0; i < n_ivals; i++ ) { 
    ivals[i] = 100*(i+1);
  }
  status = make_num_array(ivals, n_ivals, 0, I4, &rbc_I4_arr); cBYE(status);
  //------------------- Make a data frame 
  vals = malloc(n_svals * sizeof(void *));
  memset(vals, 0, n_svals * sizeof(void *));

  qtypes = malloc(n_svals * sizeof(qtype_t));
  memset(qtypes, 0, n_svals * sizeof(qtype_t));
    // terrible hack but works for testing
  if ( n_svals > 4 ) { go_BYE(-1); }  
  qtypes[0] = I1;
  qtypes[1] = I8;
  qtypes[2] = F4;
  qtypes[3] = F8;
  vals[0] = malloc(n_vals * sizeof(int8_t));
  vals[1] = malloc(n_vals * sizeof(int64_t));
  vals[2] = malloc(n_vals * sizeof(float));
  vals[3] = malloc(n_vals * sizeof(double));
  for ( int i = 0; i < n_vals; i++ ) { 
    ((uint8_t  *)vals[0])[i] = i+1;
    ((uint64_t *)vals[1])[i] = 10*(i+1);
    ((float    *)vals[2])[i] = 100*(i+1);
    ((double   *)vals[3])[i] = 1000*(i+1);
  }
  //----------------------
  status = make_data_frame(svals, n_svals, NULL, vals, n_vals, 0, 
      qtypes, &rbc_df);
  cBYE(status);
  // set up input for mixed array
  int n_mixed = 9;
  in_rbcs = malloc(n_mixed * sizeof(RBC_REC_TYPE));
  in_rbcs[0] = rbc_nil; 
  in_rbcs[1] = rbc_true;
  in_rbcs[2] = rbc_false;
  in_rbcs[3] = rbc_num;
  in_rbcs[4] = rbc_str;
  in_rbcs[5] = rbc_SC_arr;
  in_rbcs[6] = rbc_F8_arr;
  in_rbcs[7] = rbc_I4_arr ;
  in_rbcs[8] = rbc_df;
  // call mixded array
  status = make_mixed_array_or_object(
      in_rbcs, NULL, n_mixed, &rbc_mixed); 
  cBYE(status);
  status = chk_rbc(&rbc_mixed); cBYE(status); 
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_rbc(&rbc_mixed);
  free_rbc(&rbc_SC_arr);
  free_rbc(&rbc_F8_arr);
  free_rbc(&rbc_I4_arr);
  free_rbc(&rbc_df);
  free_rbc(&rbc_nil);
  free_rbc(&rbc_true);
  free_rbc(&rbc_false);
  free_rbc(&rbc_num);
  free_rbc(&rbc_str);
  free_if_non_null(qtypes);
  free_if_non_null(in_rbcs);
  free_if_non_null(dvals);
  free_if_non_null(ivals);
  free_2d_array(&svals, n_svals);
  free_2d_array((char ***)&vals, n_svals);
  return status;
}
