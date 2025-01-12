#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_pr.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "split_str.h"
#include "free_2d_array.h"
#include "qdf_named_vec.h"
/* This is to capture the way Rserve returns a vector of the form 
 * { "x" : 12.34, y : "78.90", "z" : 12.12 }
 */

int
qdf_named_vec(
    char * const key_blob, // [n_blob] 
    uint32_t n_blob,
    const double * const dvals, // [n] 
    uint32_t n_vals,
    QDF_REC_TYPE * restrict ptr_out_qdf
    )
{
  int status = 0;
  QDF_REC_TYPE key_qdf; memset(&key_qdf, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE off_qdf; memset(&off_qdf, 0, sizeof(QDF_REC_TYPE));
  char *x = NULL; 
  char **keys = NULL;

  if ( key_blob == NULL ) { go_BYE(-1); }
  if ( n_blob == 0 ) { go_BYE(-1); }
  mcr_chk_null(ptr_out_qdf, -1); 
  status = split_str_nullc_sep(key_blob, n_blob, n_vals, &keys); 
  cBYE(status);
  status = make_SC_array((char ** const)keys, 
      NULL, 0, n_vals, 0, &key_qdf); cBYE(status); 
  status = make_num_array(NULL, n_vals, 0, I4, &off_qdf); cBYE(status); 
  // calculate len off out_qdf
  size_t hdr_len = 24; 
  size_t qdf_size = hdr_len; // make space for header 
  qdf_size += key_qdf.size;
  qdf_size += off_qdf.size;
  qdf_size += n_vals * 16; // size of a number as an QDF
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  return_if_malloc_failed(x);
  memset(x, 0, qdf_size); // TODO P4 Delete later, this is for Valgrind 
  ptr_out_qdf->data = x;
  ptr_out_qdf->size = qdf_size;
  char *bak_x = x;
  //-============
  set_jtype(x, j_object); 
  set_qtype(x, F8);
  set_obj_len(x, n_vals); 
  set_qdf_size(x, qdf_size); 
  //-============
  // add the keys 
  memcpy(x+hdr_len, key_qdf.data, key_qdf.size); 
  // position x appropriately 
  x = bak_x + hdr_len + key_qdf.size + off_qdf.size;
  int32_t *offptr = get_arr_ptr(off_qdf.data);
  // set the values
  for ( uint32_t i = 0; i < n_vals; i++ ) { 
    QDF_REC_TYPE num_qdf; memset(&num_qdf, 0, sizeof(QDF_REC_TYPE));
    status = make_number(dvals[i], &num_qdf); 
    memcpy(x, num_qdf.data, num_qdf.size);
    if ( num_qdf.size != 16 ) { go_BYE(-1); }
    offptr[i] = x - (char *)ptr_out_qdf->data; 
    x +=  num_qdf.size;
    free_qdf(&num_qdf);
  }
  if ( (x - bak_x) != (int)ptr_out_qdf->size ) { go_BYE(-1); }
  // add the offsets (after they have been set correctly in loop above 
  x = bak_x + hdr_len + key_qdf.size;
  memcpy(x, off_qdf.data, off_qdf.size); 
#ifdef DEBUG
  status = chk_qdf(ptr_out_qdf); cBYE(status);
#endif
BYE:
  free_2d_array(&keys, n_vals);
  free_qdf(&key_qdf);
  free_qdf(&off_qdf);
  return status;
}
