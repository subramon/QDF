// TODO P1: Need to deal with null values properly
// Currently, I am just setting to 0
#include "incs.h"
#include "cmem_struct.h"
#include "qdf_struct.h"
#include "qdf.h"
#include "q_to_QDF.h"

int
q_to_QDF(
    char ** col_names, //  [n_cols][...]
    int * widths, //  [n_cols][...]
    char ** qtypes, //  [n_cols][...]
    bool  *is_load, //  [n_cols]
    void **vals, // [n_cols][n_rows] input 
    bool **nn, // [n_cols][n_rows] input 
    int n_rows, // output 
    int n_cols, // input 
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  char *keys = NULL;
  if ( nn != NULL ) { go_BYE(-1); } // TODO Not implemented

  // concatenate keys 
  int len_keys = 0;
  for ( int i = 0; i < n_cols; i++ ) {
    len_keys += strlen(col_names[i]) + 1;
  }
  keys = malloc(len_keys);
  return_if_malloc_failed(keys);
  memset(keys, 0, len_keys);
  char *alt_keys = keys;
  int actual_num_cols = 0;
  for ( int i = 0; i < n_cols; i++ ) {
    if ( ( is_load != NULL ) && ( is_load[i] == false ) ) { continue; }
    int c_qtype = get_c_qtype(qtypes[i]);
    if ( c_qtype == Q0 ) { 
      go_BYE(-1); }
    if ( strcmp(qtypes[i], "XX") == 0 ) { continue; } 
    if ( strcmp(qtypes[i], "SV") == 0 ) { 
      continue; // TODO UNDO 
    }
    else {
      if ( widths[i] == 0 ) { 
        go_BYE(-1); } 
    }
    actual_num_cols++;
    int len = strlen(col_names[i]);
    memcpy(alt_keys, col_names[i], len);
    if ( actual_num_cols == 0 ) { go_BYE(-1); }
    alt_keys += (len+1);
  }
  if ( actual_num_cols == 0 ) { go_BYE(-1); }
  status = make_hdr_object(keys, len_keys, actual_num_cols, ptr_qdf); 
  cBYE(status);
  int actual_col_idx = 0;
  for ( int i = 0; i < n_cols; i++ ) {
    if ( ( is_load != NULL ) && ( is_load[i] == false ) ) { continue; }
    QDF_REC_TYPE  qdf1;
    memset(&qdf1,  0, sizeof(QDF_REC_TYPE));
    int c_qtype = get_c_qtype(qtypes[i]);
    if ( strcmp(qtypes[i], "SV") == 0 ) { continue; } // TODO UNDO 
    // XXX status = make_hdr_array(n_rows, &qdf1); cBYE(status);
    // I don't think I need make_hdr_array, make_num_array will do
    void *vals_i = NULL;
    if ( vals != NULL ) { 
      vals_i = vals[i];
    }
    status = make_num_array(vals_i, c_qtype, widths[i], n_rows, &qdf1);
    cBYE(status);
    status = append(ptr_qdf, &qdf1, actual_col_idx); cBYE(status);
    actual_col_idx++;
    free_if_non_null(qdf1.data);
  }
  actual_col_idx++;
BYE:
  free_if_non_null(keys);
  return status;
}
