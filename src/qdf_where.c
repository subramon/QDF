#include "incs.h"
#include "free_2d_array.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_pr.h"
#include "qdf_xhelpers.h"
#include "qdf_makers.h"
#include "qdf_checkers.h"
#include "qdf_clone.h"
#include "qdf_setters.h"
#include "qdf_f_to_s.h"
#include "qdf_where.h"

// Returns number of rows selected by where clause
// A return of < 0 indicates an error 
int
qdf_where(
    QDF_REC_TYPE *ptr_src, // INPUT df consisting of numeric arrays
    const QDF_REC_TYPE *const where, // numeric I1 array (1=> good, 0 => null)
    QDF_REC_TYPE *ptr_dst, // OUTPUT: same as INPUT 
    uint32_t *ptr_num_good
   )
{
  int status = 0;
  char ** cols = NULL;  // [n_cols]
  uint32_t n_cols = 0; 
  qtype_t * qtypes = NULL; // [n_cols]
  uint32_t * widths = NULL; // [n_cols]
  //==============================
  mcr_chk_non_null(ptr_src, -1); 
  mcr_chk_null(ptr_dst, -1); 
  mcr_chk_non_null(where, -1); 

  // START examine where clause
  char *wx = where->data; 
  // "where" should not be part of "ptr_dst"
  if ( ( wx > (char *)ptr_src->data ) && 
      ( wx < (char *)ptr_src->data + ptr_src->size ) ) {
    go_BYE(-1);
  }
  jtype_t wjtype = get_jtype(wx); if ( wjtype != j_array ) {  go_BYE(-1); }
  uint32_t num_where    = get_arr_len(wx); 
  qtype_t wqtype = get_qtype(wx); if ( wqtype != I1 ) { go_BYE(-1); } 
  const char * const wptr = get_arr_ptr(wx); 
  if ( wptr == NULL ) { go_BYE(-1); }
  // STOP  examine where clause
  // START find out how many items left after where has been applied 
  SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(where, "sum", NULL, &sclr); cBYE(status);
  double dtemp; status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status);
  uint32_t num_good = (uint32_t)dtemp;
  *ptr_num_good = num_good; 
  // STOP  find out how many items left after where has been applied 
  if ( num_good  > num_where ) { go_BYE(-1); } 
  // If all elements good => just clone input and return 
  if ( num_good  == num_where ) { 
    status = qdf_clone(ptr_src, ptr_dst); cBYE(status); goto BYE; 
  }
  // If all elements bad, return empty QDF
  if ( num_good == 0 ) { 
    memset(ptr_dst, 0, sizeof(QDF_REC_TYPE)); goto BYE;
  }
  //==============================

  // START: Access input dataframe 
  char *src_x = ptr_src->data; 
  jtype_t src_jtype = get_jtype(src_x); 
  if ( src_jtype != j_object ) { go_BYE(-1); } 
  if ( !get_is_df(src_x) ) { go_BYE(-1); }
  uint32_t num_src = get_obj_arr_len(src_x); 
  // where clause and dataframe should have same number of rows 
  if ( num_where != num_src ) { go_BYE(-1); } 
  //==============================
  status = get_keys_as_array(ptr_src, &cols, &n_cols); cBYE(status);
  qtypes = malloc(n_cols * sizeof(qtype_t));
  widths = malloc(n_cols * sizeof(uint32_t));
  for ( uint32_t j = 0; j < n_cols; j++ ) {
    QDF_REC_TYPE tmp_qdf; memset(&tmp_qdf, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_src,  -1, cols[j], &tmp_qdf, NULL); 
    cBYE(status);
    qtypes[j] = get_qtype(tmp_qdf.data); 
    widths[j] = get_arr_width(tmp_qdf.data); 
  }
  status = make_empty_data_frame(cols, n_cols, qtypes, widths, num_good,
      ptr_dst); 
  cBYE(status);
#ifdef DEBUG
  status = chk_qdf(ptr_dst); cBYE(status);
#endif
  //==============================
  for ( uint32_t i = 0; i < n_cols; i++ ) {  // for each col in df
    // get access to source 
    QDF_REC_TYPE src_col; memset(&src_col, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_src, -1, cols[i], &src_col, NULL); 
    cBYE(status);
#ifdef DEBUG
    status = chk_qdf(&src_col); cBYE(status);
#endif
    char *src_ptr    = get_arr_ptr(src_col.data); 
    if ( src_ptr == NULL ) { go_BYE(-1); }
    jtype_t col_jtype = get_jtype(src_col.data);
    if ( col_jtype != j_array ) { go_BYE(-1); }
    qtype_t col_qtype = get_qtype(src_col.data);
    uint32_t col_n   = get_arr_len(src_col.data);
    if ( col_n == 0 ) { go_BYE(-1); } 
    // TODO P2 Handle above case, useful for place holder columns
    // get access to destination
    QDF_REC_TYPE dst_col; memset(&dst_col, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_dst, -1, cols[i], &dst_col, NULL); 
    cBYE(status);
#ifdef DEBUG
    status = chk_qdf(&dst_col); cBYE(status);
#endif
    char *dst_ptr    = get_arr_ptr(dst_col.data); 
    if ( dst_ptr == NULL ) { 
      go_BYE(-1); }


    //-------------------------------------------
    uint32_t dst_idx = 0;
    switch ( col_qtype ) {
      // TODO P3 This should be auto-generated
      case F4 : 
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((float *)dst_ptr)[dst_idx++] = ((float *)src_ptr)[src_idx];
          }
        }
        break;
      case F8 : 
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((double *)dst_ptr)[dst_idx++] = ((double *)src_ptr)[src_idx];
          }
        }
        break;
      case BL : 
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((bool *)dst_ptr)[dst_idx++] = ((bool *)src_ptr)[src_idx];
          }
        }
        break;
      case I1 : 
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((int8_t *)dst_ptr)[dst_idx++] = ((int8_t *)src_ptr)[src_idx];
          }
        }
        break;
      case I2 : 
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((int16_t *)dst_ptr)[dst_idx++] = ((int16_t *)src_ptr)[src_idx];
          }
        }
        break;
      case I4 :
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((int32_t *)dst_ptr)[dst_idx++] = ((int32_t *)src_ptr)[src_idx];
          }
        }
        break;
      case I8 :
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((int64_t *)dst_ptr)[dst_idx++] = ((int64_t *)src_ptr)[src_idx];
          }
        }
        break;
      case TM1 : 
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            ((tm_t *)dst_ptr)[dst_idx++] = ((tm_t *)src_ptr)[src_idx];
          }
        }
        break;
      case SC : 
        for ( uint32_t src_idx = 0; src_idx < num_src; src_idx++ ) { 
          if ( wptr[src_idx] == 1 ) { 
            memcpy(
                dst_ptr+(dst_idx*widths[i]), 
                src_ptr+(src_idx*widths[i]), 
                widths[i]);
            dst_idx++;
          }
        }
        break;
      default : 
        fprintf(stderr, "To be implemented\n"); go_BYE(-1); 
        break;
    }
    set_arr_len(dst_col.data, num_good); // IMPORTANT
    if ( dst_idx != num_good ) { go_BYE(-1); }
  }
  x_set_obj_arr_len(ptr_dst, num_good); // IMPORTANT
BYE:
  free_2d_array(&cols, n_cols);
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  return status; 
}
