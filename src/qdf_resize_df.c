#include "incs.h"
#include "qdf_struct.h"
#include "qtypes.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_helpers.h"
#include "qdf_resize_df.h"

int 
qdf_resize_df(
    QDF_REC_TYPE *ptr_old,
    char ** in_keys, // [in_n_keys]
    char ** in_str_qtypes, // [in_n_keys]
    uint32_t in_n_keys,
    uint32_t new_sz,
    QDF_REC_TYPE *ptr_new
    )
{
  int status = 0;
  uint32_t n_keys = 0; char **keys = NULL; // [n_keys] 
  uint32_t uitmp; char **str_qtypes = NULL; // [n_keys] 
                                            //--------------------------------------------------
  if ( ptr_old == NULL ) { go_BYE(-1); } 
  if ( ptr_new == NULL ) { go_BYE(-1); } 
  status = chk_qdf(ptr_old); cBYE(status);
  if ( !get_is_df(ptr_old->data) ) { go_BYE(-1) }
  if ( new_sz == 0 ) { go_BYE(-1); } 
  uint32_t old_n  = get_obj_arr_len(ptr_old->data);
  // uint32_t old_sz = get_obj_arr_size(ptr_old->data);
  if ( new_sz < old_n ) { go_BYE(-1); } 
  //--------------------------------------------------
  // find out keys in old qdf if not specified
  if ( in_keys == NULL ) {
    if ( in_str_qtypes != NULL ) { go_BYE(-1); }
    if ( in_n_keys != 0 ) { go_BYE(-1); } 

    status = get_keys_as_array(ptr_old, &keys, &n_keys); 
    cBYE(status);
    status = get_str_qtypes_as_array(ptr_old, &str_qtypes, &uitmp); 
    cBYE(status);
    if ( uitmp != n_keys ) { go_BYE(-1); }
  }
  else {
    if ( in_str_qtypes == NULL ) { go_BYE(-1); }
    if ( in_n_keys == 0 ) { go_BYE(-1); } 
    keys = in_keys;
    str_qtypes = in_str_qtypes;
    n_keys = in_n_keys;
  }
  // we are now ready to make a new data frame 
  status = make_empty_data_frame(keys, n_keys, str_qtypes,
      new_sz, ptr_new);
  cBYE(status);
  // iterate through each key and copy it from old to new 
  for ( uint32_t i = 0; i < n_keys; i++ ) {
    // START: get access to the source 
    QDF_REC_TYPE src_col; memset(&src_col, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_old, -1, keys[i], &src_col, NULL);cBYE(status);
    // basic checks
    jtype_t src_jtype = get_jtype(src_col.data); 
    if ( src_jtype != j_array ) { go_BYE(-1); }
    qtype_t src_qtype = get_qtype(src_col.data); 
    if ( src_qtype != get_c_qtype(str_qtypes[i]) ) { go_BYE(-1); } 
    //--------------------------
    uint32_t src_n  = get_arr_len(src_col.data);
    uint32_t src_w  = get_arr_width(src_col.data);
    char *src_ptr = get_arr_ptr(src_col.data);
    // STOP   get access to the source 
    // START: get access to the destination 
    QDF_REC_TYPE dst_col; memset(&dst_col, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_new, -1, keys[i], &dst_col, NULL);cBYE(status);
    // basic checks
    jtype_t dst_jtype = get_jtype(dst_col.data); 
    if ( dst_jtype != j_array ) { go_BYE(-1); }
    qtype_t dst_qtype = get_qtype(dst_col.data); 
    if ( dst_qtype != get_c_qtype(str_qtypes[i]) ) { go_BYE(-1); } 
    //--------------------------
    uint32_t dst_n  = get_arr_len(dst_col.data);
    uint32_t dst_w  = get_arr_width(dst_col.data);
    uint32_t dst_sz = get_arr_size(dst_col.data);
    char *dst_ptr = get_arr_ptr(dst_col.data);

    if ( dst_n  != 0 ) { go_BYE(-1); }
    if ( dst_w  != src_w ) { go_BYE(-1); }
    if ( dst_sz != new_sz ) { go_BYE(-1); }
    // STOP : get access to the destination 
    //-- now copy the data 
    memcpy(dst_ptr, src_ptr, src_n * src_w);
    //-- update occupancy 
    status = set_arr_len(dst_col.data, src_n); cBYE(status);
  } 
#ifdef DEBUG
  for ( uint32_t i = 0; i < n_keys; i++ ) {
    // START: get access to the source 
    QDF_REC_TYPE src_col; memset(&src_col, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_old, -1, keys[i], &src_col, NULL);cBYE(status);
    //--------------------------
    uint32_t src_n = get_arr_len(src_col.data);
    uint32_t src_w = get_arr_width(src_col.data);
    char *src_ptr = get_arr_ptr(src_col.data);
    // STOP   get access to the source 
    // START: get access to the destination 
    QDF_REC_TYPE dst_col; memset(&dst_col, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_new, -1, keys[i], &dst_col, NULL);cBYE(status);
    //--------------------------
    uint32_t dst_n  = get_arr_len(dst_col.data);
    uint32_t dst_w  = get_arr_width(dst_col.data);
    uint32_t dst_sz = get_arr_size(dst_col.data);
    char *dst_ptr = get_arr_ptr(dst_col.data);

    if ( dst_n != src_n ) { go_BYE(-1); } 
    if ( dst_w != src_w ) { go_BYE(-1); } 
    if ( dst_sz != new_sz ) { go_BYE(-1); } 
    // STOP : get access to the destination 
    if ( memcmp(dst_ptr, src_ptr, src_n * src_w) != 0 ) { 
      go_BYE(-1);
    }
  } 
#endif
  //-- update occupancy 
  uint32_t src_n = get_obj_arr_len(ptr_old->data);
  status = set_obj_arr_len(ptr_new->data, src_n); cBYE(status);
  
  //--------------------------------------------------
BYE:
  if ( in_keys == 0 ) { 
    if ( str_qtypes != NULL ) { 
      for ( uint32_t i = 0; i < n_keys; i++ ) { 
        free_if_non_null(str_qtypes[i]);
      }
      free_if_non_null(str_qtypes);
    }
    if ( keys != NULL ) { 
      for ( uint32_t i = 0; i < n_keys; i++ ) { 
        free_if_non_null(keys[i]);
      }
      free_if_non_null(keys);
    }
  }
  else {
    // you have just copied pointers. DO NOT FREE!
  }
  return status;
}
