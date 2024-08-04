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
  size_t qdf_size;
  char **svals = NULL; uint32_t n_svals = 4;
  double *dvals = NULL; uint32_t n_dvals = 10;
  int32_t *ivals = NULL; uint32_t n_ivals = 20;
  void **vals = NULL;
  qtype_t *qtypes = NULL;
  QDF_REC_TYPE qdf;
  char ** files = NULL; uint32_t n_files = 0;
  QDF_REC_TYPE *ext_qdfs = NULL;
  int *kidxs = NULL;
  BUF_SPEC_TYPE buf_spec; 
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));

  if ( argc != 1 ) { go_BYE(-1); }
  // check j_nil
  memset(&qdf, 0,sizeof(QDF_REC_TYPE));
  status = make_nil(NULL, &qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  qdf_size = x_get_qdf_size(&qdf); 
  if ( qdf_size != 8 ) { go_BYE(-1); }
  free_qdf(&qdf);
  // check j_boolean
  
  status = make_boolean(true, &qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  bool bval = x_get_bool_val(&qdf); 
  if ( bval != true ) { go_BYE(-1); }
  qdf_size = x_get_qdf_size(&qdf); if ( qdf_size != 8 ) { go_BYE(-1); }
  free_qdf(&qdf);
  //-------------------
  status = make_boolean(false, &qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  bval = x_get_bool_val(&qdf); 
  if ( bval != false ) { go_BYE(-1); }
  qdf_size = x_get_qdf_size(&qdf); if ( qdf_size != 8 ) { go_BYE(-1); }
  free_qdf(&qdf);
  //-------------------
  // Check make_number
  status = make_number(123.4567, &qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  double dval = x_get_num_val(&qdf); 
  if ( dval != 123.4567 ) { go_BYE(-1); }
  qdf_size = x_get_qdf_size(&qdf); if ( qdf_size != 16 ) { go_BYE(-1); }
  free_qdf(&qdf);
  //-------------------
  // Check make_string
  status = make_string("123.4567", &qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  const char * const sval = x_get_str_val(&qdf); 
  if ( strcmp(sval, "123.4567") != 0 ) { go_BYE(-1); }
  qdf_size = x_get_qdf_size(&qdf); 
  if ( qdf_size != 32 ) { go_BYE(-1); }
  free_qdf(&qdf);
  //------------------- Make a uniform SC array 
  svals = malloc(n_svals * sizeof(char *));
  return_if_malloc_failed(svals);
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    svals[i] = strdup(buf);
  }
  status = make_SC_array(svals, NULL, 0, n_svals, 0, &qdf);
  cBYE(status);
  status = chk_uniform_array(qdf.data, SC); cBYE(status); 
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    // test getting a string value given an index
    status = get_arr_val(qdf.data, i, &sclr, NULL); 
    if ( sclr.qtype != SC ) { go_BYE(-1); } 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    if ( strcmp(buf, sclr.val.str) != 0 ) { go_BYE(-1); }
    // TODO Test a setter/getter here
  }
  // test that for each input value, you get back its index
  qdf_size = x_get_qdf_size(&qdf); 
  if ( qdf_size != 56 ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    sclr.val.str = buf;
    int itmp; 
    status = get_arr_idx(qdf.data, &sclr, &itmp); 
    if ( itmp < 0 ) { go_BYE(-1); }
    if ( itmp != (int)i ) { go_BYE(-1); }
  }

  // printf("len = %d \n", len);
  free_qdf(&qdf);
  //------------------- Make a uniform F8 array 
  dvals = malloc(n_dvals * sizeof(double));
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    dvals[i] = i+1;
  }
  status = make_num_array(dvals, n_dvals, 0, F8, &qdf); cBYE(status);
  if ( x_get_jtype(&qdf) != j_array ) { go_BYE(-1); } 
  if ( x_get_qtype(&qdf) != F8 ) { go_BYE(-1); } 
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    // test getting a numeric value given an index
    status = get_arr_val(qdf.data, i, &sclr, NULL); 
    if ( sclr.qtype != F8 ) { go_BYE(-1); }
    if ( sclr.val.f8 != dvals[i] ) { go_BYE(-1); }
    // test set and then get 
    sclr.val.f8 = dvals[i] * 100; sclr.qtype = F8;
    status = x_set_arr_val(&qdf, i, sclr);
    memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    status = get_arr_val(qdf.data, i, &sclr, NULL); 
    if ( sclr.val.f8 != sclr.val.f8 ) { go_BYE(-1); }
  }
  uint32_t width = get_arr_width(qdf.data);
  if ( width != sizeof(double) ) { go_BYE(-1); }
  uint32_t arr_len = get_arr_len(qdf.data);
  if ( arr_len != n_dvals ) { go_BYE(-1); }
  uint32_t arr_size = get_arr_size(qdf.data);
  if ( arr_size != arr_len ) { go_BYE(-1); }
  // test the values by direct access
  const double * const dptr = get_arr_ptr(qdf.data); 
  if ( dptr == NULL ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    if ( (dvals[i]*100) != dptr[i] ) { go_BYE(-1); }
  }
  QDF_REC_TYPE tmp_qdf; memset(&tmp_qdf, 0, sizeof(QDF_REC_TYPE));
  status = x_get_arr_ptr(&qdf, &tmp_qdf);
  const double * const d2ptr = tmp_qdf.data;
  if ( d2ptr == NULL ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    if ( (dvals[i]*100) != d2ptr[i] ) { go_BYE(-1); }
  }
  free_qdf(&qdf);
  free_qdf(&tmp_qdf);
  //------------------- Make a uniform I4 array 
  ivals = malloc(n_ivals * sizeof(int32_t));
  for ( uint32_t i = 0; i < n_ivals; i++ ) { 
    ivals[i] = 100*((int)i+1);
  }
  arr_size = n_ivals + 1;
  status = make_num_array(ivals, n_ivals, arr_size, I4, &qdf); cBYE(status);
  if ( x_get_jtype(&qdf) != j_array ) { go_BYE(-1); } 
  if ( x_get_qtype(&qdf) != I4 ) { go_BYE(-1); } 
  for ( uint32_t i = 0; i < n_ivals; i++ ) { 
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    status = get_arr_val(qdf.data, i, &sclr, NULL); 
    if ( sclr.qtype != I4 ) { go_BYE(-1); } 
    if ( sclr.val.i4 != ivals[i] ) { go_BYE(-1); }
  }
  if ( x_get_arr_width(&qdf) != sizeof(int32_t) ) { go_BYE(-1); }
  if ( get_arr_len(qdf.data) != n_ivals ) { go_BYE(-1); }
  if ( get_arr_size(qdf.data) != arr_size ) { go_BYE(-1); }
  free_qdf(&qdf);
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
  uint32_t n_vals = 123;
  vals[0] = malloc(n_vals * sizeof(int8_t));
  vals[1] = malloc(n_vals * sizeof(int64_t));
  vals[2] = malloc(n_vals * sizeof(float));
  vals[3] = malloc(n_vals * sizeof(double));
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    ((int8_t  *)vals[0])[i] = (int8_t)(i+1);
    ((int64_t *)vals[1])[i] = (int64_t)(10*(i+1));
    ((float   *)vals[2])[i] = (float)(100*(i+1));
    ((double  *)vals[3])[i] = (double)(1000*(i+1));
  }
  //----------------------
  uint32_t obj_arr_size = n_vals + 1;
  status = make_data_frame(svals, n_svals, NULL, 
      vals, n_vals, obj_arr_size, qtypes, &qdf);
  cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  // Check that you have indeed created a data frame 
  if ( !x_get_is_df(&qdf) ) { go_BYE(-1); }
  uint32_t chk_n_rows = x_get_obj_arr_len(&qdf); 
  if ( chk_n_rows != n_vals ) { go_BYE(-1); }
  uint32_t chk_obj_arr_size = x_get_obj_arr_size(&qdf); 
  if ( chk_obj_arr_size != obj_arr_size ) { go_BYE(-1); }
  // Check that the data fraee contains the keys you expect
  bool b_keys_exist;
  status = are_keys_in_object(&qdf, svals, n_svals, &b_keys_exist); 
  cBYE(status);
  if ( !b_keys_exist ) { go_BYE(-1); }
  //--- Get num keys in data frame
  uint32_t n_keys = x_get_obj_len(&qdf); 
  if ( n_keys != n_svals ) { go_BYE(-1); }
  // Get keys qdf from dataframe qdf
  QDF_REC_TYPE keys_qdf; memset(&keys_qdf, 0, sizeof(QDF_REC_TYPE));
  status = get_keys(&qdf, &keys_qdf); cBYE(status);
  // TODO Check these keys 
  status = chk_qdf(&qdf); cBYE(status);
  //--- Get a key, get its index (or -1 if it does not exist)
  //-- Access the name of a key in a dataframe. 
  uint32_t chk_n_keys = x_get_arr_len(&keys_qdf);
  if ( chk_n_keys != n_keys ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    status = get_arr_val(keys_qdf.data, i, &sclr, NULL); 
    if ( sclr.qtype != SC ) { go_BYE(-1); }
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    if ( strcmp(buf, sclr.val.str) != 0 ) { go_BYE(-1); }
  }
  // test is_key with positive cases
  kidxs = malloc(n_svals * sizeof(int));

  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    status = is_val_in_SC_array(&keys_qdf, buf, kidxs+i);
  }
  // check kidxs
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    if ( kidxs[i] < 0 )  { go_BYE(-1); }
    if ( (uint32_t)kidxs[i] >= n_svals ) { go_BYE(-1); }
  }
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    for ( uint32_t j = i+1; j < n_svals; j++ ) { 
      if ( kidxs[i] == kidxs[j] ) { go_BYE(-1); }
    }
  }

  // test is_key with negative case
  status = is_val_in_SC_array(&keys_qdf, "bogus_key", kidxs); 
  if ( *kidxs != -1 ) { go_BYE(-1); }
  // test pr_df_as_csv
  status = pr_df_as_csv(&qdf, NULL, 0, "_df.csv"); cBYE(status);
  int itmp = num_lines("_df.csv");  if ( itmp < 0 ) { go_BYE(-1); }
  uint32_t nl = (uint32_t)itmp;
  if ( nl-1 != n_vals ) { go_BYE(-1); }  // -1 for header
  // TODO Test with other ways of specifying keys 
  //---------------------------------------------
  // Test bindmp and load back 
  n_files = 4;
  files = malloc(n_files * sizeof(char *));
  memset(files, 0,  n_files * sizeof(char *));
  for ( uint32_t i = 0; i < n_files; i++ ) { 
    char buf[32]; sprintf(buf, "_File_%d", i+1);
    files[i] = strdup(buf); 
    status = bindmp(&qdf, files[i]); cBYE(status);
    int64_t l = get_file_size(files[i]); if ( l <= 0 ) { go_BYE(-1); } 
  }
  status = load_qdfs_from_files(files, n_files, false, &ext_qdfs); 
  cBYE(status);
  free_qdfs(&ext_qdfs, "munmap", n_files); 
  free_qdf(&qdf);
  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(qtypes);
  free_if_non_null(dvals);
  free_if_non_null(ivals);
  free_if_non_null(kidxs);
  free_2d_array(&svals, n_svals);
  free_2d_array((char ***)&vals, n_svals);
  free_2d_array(&files, n_files);
  return status;
}
