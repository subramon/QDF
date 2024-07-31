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
  size_t rbc_size;
  char **svals = NULL; uint32_t n_svals = 4;
  double *dvals = NULL; uint32_t n_dvals = 10;
  int32_t *ivals = NULL; uint32_t n_ivals = 20;
  void **vals = NULL;
  qtype_t *qtypes = NULL;
  RBC_REC_TYPE rbc;
  char ** files = NULL; uint32_t n_files = 0;
  RBC_REC_TYPE *ext_rbcs = NULL;
  int *kidxs = NULL;
  BUF_SPEC_TYPE buf_spec; 
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));

  if ( argc != 1 ) { go_BYE(-1); }
  // check j_nil
  memset(&rbc, 0,sizeof(RBC_REC_TYPE));
  status = make_nil(NULL, &rbc); cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  rbc_size = x_get_rbc_size(&rbc); 
  if ( rbc_size != 8 ) { go_BYE(-1); }
  free_rbc(&rbc);
  // check j_boolean
  
  status = make_boolean(true, &rbc); cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  bool bval = x_get_bool_val(&rbc); 
  if ( bval != true ) { go_BYE(-1); }
  rbc_size = x_get_rbc_size(&rbc); if ( rbc_size != 8 ) { go_BYE(-1); }
  free_rbc(&rbc);
  //-------------------
  status = make_boolean(false, &rbc); cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  bval = x_get_bool_val(&rbc); 
  if ( bval != false ) { go_BYE(-1); }
  rbc_size = x_get_rbc_size(&rbc); if ( rbc_size != 8 ) { go_BYE(-1); }
  free_rbc(&rbc);
  //-------------------
  // Check make_number
  status = make_number(123.4567, &rbc); cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  double dval = x_get_num_val(&rbc); 
  if ( dval != 123.4567 ) { go_BYE(-1); }
  rbc_size = x_get_rbc_size(&rbc); if ( rbc_size != 16 ) { go_BYE(-1); }
  free_rbc(&rbc);
  //-------------------
  // Check make_string
  status = make_string("123.4567", &rbc); cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  const char * const sval = x_get_str_val(&rbc); 
  if ( strcmp(sval, "123.4567") != 0 ) { go_BYE(-1); }
  rbc_size = x_get_rbc_size(&rbc); 
  if ( rbc_size != 32 ) { go_BYE(-1); }
  free_rbc(&rbc);
  //------------------- Make a uniform SC array 
  svals = malloc(n_svals * sizeof(char *));
  return_if_malloc_failed(svals);
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    svals[i] = strdup(buf);
  }
  status = make_SC_array((const char **)svals, NULL, 0, n_svals, 0, &rbc);
  cBYE(status);
  status = chk_uniform_array(rbc.data, SC); cBYE(status); 
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    RBC_SCLR_TYPE sclr; memset(&sclr, 0, sizeof(RBC_SCLR_TYPE));
    // test getting a string value given an index
    status = get_arr_val(rbc.data, i, &sclr, NULL); 
    if ( sclr.qtype != SC ) { go_BYE(-1); } 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    if ( strcmp(buf, sclr.val.str) != 0 ) { go_BYE(-1); }
    // TODO Test a setter/getter here
  }
  // test that for each input value, you get back its index
  rbc_size = x_get_rbc_size(&rbc); 
  if ( rbc_size != 56 ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    RBC_SCLR_TYPE sclr; memset(&sclr, 0, sizeof(RBC_SCLR_TYPE));
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    sclr.val.str = buf;
    int itmp; 
    status = get_arr_idx(rbc.data, &sclr, &itmp); 
    if ( itmp < 0 ) { go_BYE(-1); }
    if ( itmp != (int)i ) { go_BYE(-1); }
  }

  // printf("len = %d \n", len);
  free_rbc(&rbc);
  //------------------- Make a uniform F8 array 
  dvals = malloc(n_dvals * sizeof(double));
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    dvals[i] = i+1;
  }
  status = make_num_array(dvals, n_dvals, 0, F8, &rbc); cBYE(status);
  if ( x_get_jtype(&rbc) != j_array ) { go_BYE(-1); } 
  if ( x_get_qtype(&rbc) != F8 ) { go_BYE(-1); } 
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    RBC_SCLR_TYPE sclr; memset(&sclr, 0, sizeof(RBC_SCLR_TYPE));
    // test getting a numeric value given an index
    status = get_arr_val(rbc.data, i, &sclr, NULL); 
    if ( sclr.qtype != F8 ) { go_BYE(-1); }
    if ( sclr.val.f8 != dvals[i] ) { go_BYE(-1); }
    // test set and then get 
    sclr.val.f8 = dvals[i] * 100; sclr.qtype = F8;
    status = x_set_arr_val(&rbc, i, sclr);
    memset(&sclr, 0, sizeof(RBC_SCLR_TYPE));
    status = get_arr_val(rbc.data, i, &sclr, NULL); 
    if ( sclr.val.f8 != sclr.val.f8 ) { go_BYE(-1); }
  }
  uint32_t width = get_arr_width(rbc.data);
  if ( width != sizeof(double) ) { go_BYE(-1); }
  uint32_t arr_len = get_arr_len(rbc.data);
  if ( arr_len != n_dvals ) { go_BYE(-1); }
  uint32_t arr_size = get_arr_size(rbc.data);
  if ( arr_size != arr_len ) { go_BYE(-1); }
  // test the values by direct access
  const double * const dptr = get_arr_ptr(rbc.data); 
  if ( dptr == NULL ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    if ( (dvals[i]*100) != dptr[i] ) { go_BYE(-1); }
  }
  RBC_REC_TYPE tmp_rbc; memset(&tmp_rbc, 0, sizeof(RBC_REC_TYPE));
  status = x_get_arr_ptr(&rbc, &tmp_rbc);
  const double * const d2ptr = tmp_rbc.data;
  if ( d2ptr == NULL ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_dvals; i++ ) { 
    if ( (dvals[i]*100) != d2ptr[i] ) { go_BYE(-1); }
  }
  free_rbc(&rbc);
  free_rbc(&tmp_rbc);
  //------------------- Make a uniform I4 array 
  ivals = malloc(n_ivals * sizeof(int32_t));
  for ( uint32_t i = 0; i < n_ivals; i++ ) { 
    ivals[i] = 100*((int)i+1);
  }
  arr_size = n_ivals + 1;
  status = make_num_array(ivals, n_ivals, arr_size, I4, &rbc); cBYE(status);
  if ( x_get_jtype(&rbc) != j_array ) { go_BYE(-1); } 
  if ( x_get_qtype(&rbc) != I4 ) { go_BYE(-1); } 
  for ( uint32_t i = 0; i < n_ivals; i++ ) { 
    RBC_SCLR_TYPE sclr; memset(&sclr, 0, sizeof(RBC_SCLR_TYPE));
    status = get_arr_val(rbc.data, i, &sclr, NULL); 
    if ( sclr.qtype != I4 ) { go_BYE(-1); } 
    if ( sclr.val.i4 != ivals[i] ) { go_BYE(-1); }
  }
  if ( x_get_arr_width(&rbc) != sizeof(int32_t) ) { go_BYE(-1); }
  if ( get_arr_len(rbc.data) != n_ivals ) { go_BYE(-1); }
  if ( get_arr_size(rbc.data) != arr_size ) { go_BYE(-1); }
  free_rbc(&rbc);
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
  status = make_data_frame((const char **)svals, n_svals, NULL, 
      (const void **)vals, n_vals, obj_arr_size, qtypes, &rbc);
  cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  // Check that you have indeed created a data frame 
  if ( !x_get_is_df(&rbc) ) { go_BYE(-1); }
  uint32_t chk_n_rows = x_get_obj_arr_len(&rbc); 
  if ( chk_n_rows != n_vals ) { go_BYE(-1); }
  uint32_t chk_obj_arr_size = x_get_obj_arr_size(&rbc); 
  if ( chk_obj_arr_size != obj_arr_size ) { go_BYE(-1); }
  // Check that the data fraee contains the keys you expect
  bool b_keys_exist;
  status = are_keys_in_object(&rbc, (const char ** const)svals, 
      n_svals, &b_keys_exist); 
  cBYE(status);
  if ( !b_keys_exist ) { go_BYE(-1); }
  //--- Get num keys in data frame
  uint32_t n_keys = x_get_obj_len(&rbc); 
  if ( n_keys != n_svals ) { go_BYE(-1); }
  // Get keys rbc from dataframe rbc
  RBC_REC_TYPE keys_rbc; memset(&keys_rbc, 0, sizeof(RBC_REC_TYPE));
  status = get_keys(&rbc, &keys_rbc); cBYE(status);
  // TODO Check these keys 
  status = chk_rbc(&rbc); cBYE(status);
  //--- Get a key, get its index (or -1 if it does not exist)
  //-- Access the name of a key in a dataframe. 
  uint32_t chk_n_keys = x_get_arr_len(&keys_rbc);
  if ( chk_n_keys != n_keys ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    RBC_SCLR_TYPE sclr; memset(&sclr, 0, sizeof(RBC_SCLR_TYPE));
    status = get_arr_val(keys_rbc.data, i, &sclr, NULL); 
    if ( sclr.qtype != SC ) { go_BYE(-1); }
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    if ( strcmp(buf, sclr.val.str) != 0 ) { go_BYE(-1); }
  }
  // test is_key with positive cases
  kidxs = malloc(n_svals * sizeof(int));

  for ( uint32_t i = 0; i < n_svals; i++ ) { 
    char buf[32]; sprintf(buf, "Value_%d", i+1);
    status = is_val_in_SC_array(&keys_rbc, buf, kidxs+i);
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
  status = is_val_in_SC_array(&keys_rbc, "bogus_key", kidxs); 
  if ( *kidxs != -1 ) { go_BYE(-1); }
  // test pr_df_as_csv
  status = pr_df_as_csv(&rbc, NULL, 0, "_df.csv"); cBYE(status);
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
    status = bindmp(&rbc, files[i]); cBYE(status);
    int64_t l = get_file_size(files[i]); if ( l <= 0 ) { go_BYE(-1); } 
  }
  status = load_rbcs_from_files(files, n_files, false, &ext_rbcs); 
  cBYE(status);
  free_rbcs(&ext_rbcs, "munmap", n_files); 
  free_rbc(&rbc);
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
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
