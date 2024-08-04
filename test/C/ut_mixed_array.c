#include "incs.h"
#include "qdf_struct.h"
#include "free_2d_array.h"
#include "get_file_size.h"
#include "qdf_helpers.h"
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
  QDF_REC_TYPE *in_qdfs = NULL;

  char **svals = NULL; uint32_t n_svals = 4;
  double *dvals = NULL; uint32_t n_dvals = 10;
  int32_t *ivals = NULL; uint32_t n_ivals = 20;
  void **vals = NULL; uint32_t n_vals = 123;

  qtype_t *qtypes = NULL;

  QDF_REC_TYPE qdf_mixed;
  QDF_REC_TYPE qdf_SC_arr, qdf_F8_arr, qdf_I4_arr, qdf_df;
  QDF_REC_TYPE qdf_nil, qdf_true, qdf_false, qdf_num, qdf_str;

  memset(&qdf_mixed, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_SC_arr, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_F8_arr, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_I4_arr, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_df, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_nil, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_true, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_false, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_num, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_str, 0, sizeof(QDF_REC_TYPE));

  if ( argc != 1 ) { go_BYE(-1); } 
  // make j_nil
  status = make_nil(NULL, &qdf_nil); cBYE(status);
  // make j_boolean
  status = make_boolean(true, &qdf_true); cBYE(status);
  status = make_boolean(false, &qdf_false); cBYE(status);
  //-------------------
  // make_number
  status = make_number(123.4567, &qdf_num); cBYE(status);
  //-------------------
  // make_string
  status = make_string("123.4567", &qdf_str); cBYE(status);
  //------------------- Make a uniform SC array 
  svals = malloc(n_svals * sizeof(char *));
  return_if_malloc_failed(svals);
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    svals[i] = strdup(buf);
  }
  status = make_SC_array(svals, NULL, 0, n_svals, 0, &qdf_SC_arr); 
  cBYE(status);
  //------------------- Make a uniform F8 array 
  dvals = malloc(n_dvals * sizeof(double));
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    dvals[i] = i+1;
  }
  status = make_num_array(dvals, n_dvals, 0, F8, &qdf_F8_arr); cBYE(status);
  //------------------- Make a uniform I4 array 
  ivals = malloc(n_ivals * sizeof(int32_t));
  for ( uint32_t i = 0; i < n_ivals; i++ ) { 
    ivals[i] = (int32_t)(100*(i+1));
  }
  status = make_num_array(ivals, n_ivals, 0, I4, &qdf_I4_arr); cBYE(status);
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
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((uint8_t  *)vals[0])[i] = (uint8_t)(i+1);
    ((uint64_t *)vals[1])[i] = (uint64_t)(10*(i+1));
    ((float    *)vals[2])[i] = (float)(100*(i+1));
    ((double   *)vals[3])[i] = (double)(1000*(i+1));
  }
  //----------------------
  status = make_data_frame(svals, n_svals, NULL, vals, n_vals, 0, 
      qtypes, &qdf_df);
  cBYE(status);
  // set up input for mixed array
  uint32_t n_mixed = 9;
  in_qdfs = malloc(n_mixed * sizeof(QDF_REC_TYPE));
  in_qdfs[0] = qdf_nil; 
  in_qdfs[1] = qdf_true;
  in_qdfs[2] = qdf_false;
  in_qdfs[3] = qdf_num;
  in_qdfs[4] = qdf_str;
  in_qdfs[5] = qdf_SC_arr;
  in_qdfs[6] = qdf_F8_arr;
  in_qdfs[7] = qdf_I4_arr ;
  in_qdfs[8] = qdf_df;
  // call mixded array
  status = make_mixed_array_or_object(
      in_qdfs, NULL, n_mixed, &qdf_mixed); 
  cBYE(status);
  status = chk_qdf(&qdf_mixed); cBYE(status); 
  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);
BYE:
  free_qdf(&qdf_mixed);
  free_qdf(&qdf_SC_arr);
  free_qdf(&qdf_F8_arr);
  free_qdf(&qdf_I4_arr);
  free_qdf(&qdf_df);
  free_qdf(&qdf_nil);
  free_qdf(&qdf_true);
  free_qdf(&qdf_false);
  free_qdf(&qdf_num);
  free_qdf(&qdf_str);
  free_if_non_null(qtypes);
  free_if_non_null(in_qdfs);
  free_if_non_null(dvals);
  free_if_non_null(ivals);
  free_2d_array(&svals, n_svals);
  free_2d_array((char ***)&vals, n_svals);
  return status;
}
