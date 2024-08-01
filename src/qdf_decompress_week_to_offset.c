#include "incs.h"
#include "qdf_struct.h"
#include "date_to_hol_struct.h"
#include "auxil.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "tm2time.h"
#include "qdf_decompress_week_to_offset.h"
int
decompress_week_to_offset(
    const QDF_REC_TYPE * const s1,
    const QDF_REC_TYPE * const s2,
    const date_hol_t * const H, // [nH]
    uint32_t nH, 
    QDF_REC_TYPE * restrict dst
    )
{
  int status = 0;
  uint32_t offset;
  mcr_chk_non_null(s1, -1); 
  char *s1x = (void *)s1->data;

  jtype_t s1_jtype = get_jtype(s1x); if (s1_jtype != j_array) {go_BYE(-1); }
  qtype_t s1_qtype = get_qtype(s1x); 
  if ( ( s1_qtype != UI1 ) && ( s1_qtype != UI2 ) ) { go_BYE(-1); }

  uint32_t s1n = get_arr_len(s1x); 

  const void * const s1ptr = get_arr_ptr(s1x); 

  status = make_num_array(NULL, s1n, 0, TM1,  dst); cBYE(status);
  tm_t *TM1dst = get_arr_ptr(dst->data); 

  struct tm t1;
  tm_t basetm; status = x_get_date_val(s2, &basetm); cBYE(status);
  t_assign(&t1, &basetm); 
  time_t tmp = tm2time(&t1); 
  if ( tmp < 0 ) { go_BYE(-1); }
  uint32_t basetime = (uint32_t)tmp; 
  uint32_t baseidx = 0; bool found = false; 
  for ( uint32_t i = 0; i < nH; i++ ) { 
    if ( H[i].t_epoch == basetime ) { baseidx = i; found = true; break; }
  }
  if ( !found ) { go_BYE(-1); }

  for ( uint32_t i = 0; i < s1n; i++ ) { 
    switch ( s1_qtype ) { 
      case UI1 : offset = ((const uint8_t * const)s1ptr)[i]; break;
      case UI2 : offset = ((const uint16_t * const)s1ptr)[i]; break;
      default : go_BYE(-1); break;
    }
    TM1dst[i] = H[baseidx+offset].mytm;
    // convert ltime to tm_t 
  }
BYE:
  return status;
}
