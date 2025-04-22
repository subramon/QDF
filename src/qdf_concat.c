#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_concat.h"
// concatenates in_qdfs into out_qdf
// Assumptiion: in_qdfs are all data frames
// Further, they all contain the keys specified 
int
qdf_concat(
    QDF_REC_TYPE * const in_qdfs,  // [n_qdfs]  INPUT 
    uint32_t n_qdfs, // INPUT 
    char ** const keys, // [n_keys] INPUT 
    uint32_t n_keys, // INPUT
    QDF_REC_TYPE *ptr_out_qdf
    )
{
  int status = 0;
  uint32_t *n_rows = NULL; // [n_qdfs]
  qtype_t *qtypes = NULL; // [n_keys]
  uint32_t *widths = NULL; // [n_keys]
  // Check parameters
  if ( n_qdfs <= 0 ) { go_BYE(-1); }
  if ( n_keys <= 0 ) { go_BYE(-1); }
  if ( keys == NULL ) { go_BYE(-1); }
  //-- mallocs
  qtypes = malloc(n_keys * sizeof(qtype_t));
  return_if_malloc_failed(qtypes);
  memset(qtypes, 0,  n_keys * sizeof(qtype_t));

  widths = malloc(n_keys * sizeof(uint32_t));
  return_if_malloc_failed(widths);
  memset(widths, 0,  n_keys * sizeof(uint32_t));

  n_rows = malloc(n_qdfs * sizeof(uint32_t));
  return_if_malloc_failed(n_rows);
  memset(n_rows, 0,  n_qdfs * sizeof(uint32_t));
  // get num_rows of each input QDF and confirm that it is a data frame
  uint32_t total_n_rows = 0;
  for ( uint32_t i = 0; i < n_qdfs; i++ )  {
    // get length of i^{th} data frame
    bool b_is_df = x_get_is_df(&(in_qdfs[i])); 
    if ( !b_is_df ) { go_BYE(-1); }
    n_rows[i] = x_get_obj_arr_len(&(in_qdfs[i])); 
    total_n_rows += n_rows[i];
  }
  //---
  // Check that all input qdfs have desired keys
  // For each key, it must have same qtype across all QDFs
  //-------------------------------------------
  for ( uint32_t j = 0; j < n_keys; j++ ) {
    qtype_t qtype_j = Q0;
    uint32_t width_j = 0; 
    for ( uint32_t i = 0; i < n_qdfs; i++ )  {
      //--------------------------------
      // get type of column with key = key in i^{th} input QDF
      QDF_REC_TYPE tmp_qdf; memset(&tmp_qdf, 0, sizeof(QDF_REC_TYPE));
      status = get_key_val(&(in_qdfs[i]), -1, keys[j], &tmp_qdf, NULL); 
      cBYE(status);
      //-----------------------------------------------
      qtype_t qtype =  get_qtype(tmp_qdf.data); 
      if ( i == 0 ) { 
        qtype_j = qtype;
      }
      else {
        if ( qtype_j != qtype ) { go_BYE(-1); }
      }
      //-----------------------------------------------
      uint32_t width =  get_arr_width(tmp_qdf.data); 
      if ( i == 0 ) { 
        width_j = width;
      }
      else {
        if ( width_j != width ) { go_BYE(-1); }
      }
    }
    qtypes[j] = qtype_j;
    widths[j] = width_j;
  }
  if ( total_n_rows == 0 ) { go_BYE(-1); } 
  // we now have information to create an empty output QDF
  status = make_empty_data_frame(keys, n_keys, qtypes, widths, 
      total_n_rows, ptr_out_qdf);
  cBYE(status);
  // but we still have to copy data from input to output
  for ( uint32_t j = 0; j < n_keys; j++ ) { 
    QDF_REC_TYPE dst_qdf; memset(&dst_qdf, 0, sizeof(QDF_REC_TYPE));
    const char * const key = keys[j];
    qtype_t qtype = qtypes[j];
    uint32_t width = widths[j];
    if ( width == 0 ) { 
      go_BYE(-1); }

    // figure out destination for writes 
    status = get_key_val(ptr_out_qdf, -1, key, &dst_qdf, NULL); 
    cBYE(status);
    status = chk_qdf(&dst_qdf); cBYE(status);
    char * dstptr = get_arr_ptr(dst_qdf.data); 

    for ( uint32_t i = 0; i < n_qdfs; i++ ) { 
      // figure out source for writes 
      QDF_REC_TYPE src_qdf; memset(&src_qdf, 0, sizeof(QDF_REC_TYPE));
      status = get_key_val(&(in_qdfs[i]), -1, key, &src_qdf, NULL); 
      cBYE(status);
      const char * const srcptr = get_arr_ptr(src_qdf.data);
      if ( srcptr == NULL ) { go_BYE(-1); }
      memcpy(dstptr, srcptr, n_rows[i] * width);
      dstptr += (n_rows[i] * width);
      status = set_obj_arr_len(dst_qdf.data, total_n_rows);
    }
  }
  status = set_obj_arr_len(ptr_out_qdf->data, total_n_rows);
  cBYE(status);
BYE:
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_if_non_null(n_rows);
  return status;
}
