#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_setters.h"
#include "qdf_append.h"

// If there is no space in dst qdf for src qdf, then we 
// return status == -2 and set num_to_expand to how much dst needs to 
// increase to fit src 
// If check_for_space is set to false (dangerous but do-able), then
// we assume that dst has enough space for src. Do this with care!

int
qdf_append_df(
    QDF_REC_TYPE * restrict dst,
    const QDF_REC_TYPE *const src,
    uint32_t *ptr_num_to_expand 
   )
{
  int status = 0;

  mcr_chk_non_null(src, -1); 
  //==============================
  mcr_chk_non_null(dst, -1);  // TODO Is this too aggressive?
  char *dx = dst->data; 
  if ( !get_is_df(dx) ) { go_BYE(-1); } 
  uint32_t dn  = get_obj_arr_len(dx); 
  uint32_t dsz = get_obj_arr_size(dx); 
  //==============================
  mcr_chk_non_null(src, -1);  // TODO Is this too aggressive?
  char *sx = src->data; 
  if ( !get_is_df(sx) ) { go_BYE(-1); } 
  uint32_t sn  = get_obj_arr_len(sx); 
  //==============================
  // return if insufficient space 
  if ( sn + dn > dsz ) { 
    *ptr_num_to_expand = ((sn + dn) - dsz );
    return -2; 
  }
  *ptr_num_to_expand = 0; 
  //==============================
  // Iterate over all keys in dst
  QDF_REC_TYPE keys_qdf; memset(&keys_qdf, 0, sizeof(QDF_REC_TYPE));
  status = get_keys(dst, &keys_qdf); cBYE(status);
  //-------------------------
  uint32_t n_keys = get_obj_len(dx); 
  for ( uint32_t i = 0; i < n_keys; i++ ) { // for each key in src 
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    status = get_arr_val(keys_qdf.data, i, &sclr, NULL);  cBYE(status);
    const char * const key = sclr.val.str;
    if ( key == NULL ) { go_BYE(-1); }

    QDF_REC_TYPE dcol; memset(&dcol, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(dst, -1, key, &dcol, NULL); cBYE(status); 
    qtype_t dqtype = x_get_qtype(&dcol); 
    uint32_t ldn   = x_get_arr_len(&dcol); 
    uint32_t ldsz  = x_get_arr_size(&dcol); 
    uint32_t ldw   = x_get_arr_width(&dcol); 
    char *dptr     = get_arr_ptr(dcol.data);
    // now get source column 
    QDF_REC_TYPE scol; memset(&scol, 0, sizeof(QDF_REC_TYPE));
    bool b_is_key; status = is_key(src, key, &b_is_key); cBYE(status);
    if ( !b_is_key ) {
      // this column not available in source. Skipping...
      continue;
      // TODO P1 Important: Think about this 
      // We have assumed that all columns in dataframe have
      // same length AND same size
    }
    status = get_key_val(src, -1, key, &scol, NULL); cBYE(status);
    qtype_t sqtype = x_get_qtype(&scol); 
    uint32_t lsn   = x_get_arr_len(&scol); 
    uint32_t lsw   = x_get_arr_width(&scol); 
    const char * const sptr = get_arr_ptr(scol.data);
    // some checks 
    if ( sqtype != dqtype ) { go_BYE(-1); } 
    if ( ldw != lsw ) { go_BYE(-1); }
    if ( ldn != dn ) { go_BYE(-1); }
    if ( lsn != sn ) { go_BYE(-1); }
    if ( ( ldn + lsn ) > ldsz ) { go_BYE(-1); } 

    memcpy(dptr + (ldn*ldw), sptr, (lsn*lsw));
    // update occupancy of this column of the dataframe
    set_arr_len(dcol.data, dn + sn);
  }
  //-- Update occupancy in dst_qdf dataframe 
  set_obj_arr_len(dx, dn + sn);
BYE:
  return status;
}
