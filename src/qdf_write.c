#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_setters.h"
#include "qdf_write.h"

int
qdf_write(
    const QDF_REC_TYPE * const src, // INPUT 
    uint32_t offset, // INPUT
    bool truncate, // INPUT
    QDF_REC_TYPE * restrict dst // INPUT and OUTPUT 
   )
{
  int status = 0;

  //==============================
  mcr_chk_non_null(dst, -1); 
  char *dx = dst->data; 
  jtype_t djtype = get_jtype(dx); if ( djtype != j_array ) { go_BYE(-1); }
  qtype_t dqtype = get_qtype(dx); if ( dqtype == Q0 ) { go_BYE(-1); }
  uint32_t dsz = get_arr_size(dx); 
  uint32_t  dn = get_arr_len(dx); 
  uint32_t  dw = get_arr_width(dx); 
  //==============================
  mcr_chk_non_null(src, -1); 
  char *sx = src->data; 
  jtype_t sjtype = get_jtype(sx); if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx); if ( sqtype != dqtype ) { go_BYE(-1); }
  uint32_t  sn = get_arr_len(sx); 
  //==============================
  // Check if there is enough space
  if ( truncate ) { 
    if ( sn + offset > dsz ) { 
      sn  = dsz - offset;
    }
  }
  uint32_t new_dn = sn + offset;
  if ( new_dn > dsz ) { go_BYE(-1); }
  //==============================
  char *dptr = get_arr_ptr(dx); 
  const char * const sptr = get_arr_ptr(sx); 

  memcpy(dptr + (dw * offset), sptr, (dw * sn));
  // check if dn needs to be updated
  if ( new_dn > dn ) { set_arr_len(dx, new_dn); }
BYE:
  return status;
}
