#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_setters.h"

// following only for uniform arrays 
int
x_set_arr_val(
    QDF_REC_TYPE *ptr_qdf,
    uint32_t idx,
    SCLR_REC_TYPE sclr
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  const void *x = ptr_qdf->data;
  const qdf_array_hdr_t *xp = (const qdf_array_hdr_t *)x;
  if ( xp->jtype != j_array ) { go_BYE(-1); }
  if ( xp->qtype != sclr.qtype ) { go_BYE(-1); }

  uint32_t width   = xp->width;
  uint32_t arr_len = xp->arr_len;

  if ( idx >= arr_len ) { go_BYE(-1); }
  void *arr_ptr = get_arr_ptr(x); 

  switch ( xp->qtype ) {
    case Q0 : go_BYE(-1); break; 
    case I1  : ((int8_t *)arr_ptr)[idx]   = sclr.val.i1; break; 
    case I2  : ((int16_t *)arr_ptr)[idx]  = sclr.val.i2;  break; 
    case I4  : ((int32_t *)arr_ptr)[idx]  = sclr.val.i4;  break; 
    case I8  : ((int64_t *)arr_ptr)[idx]  = sclr.val.i8;  break; 

    case UI1 : ((uint8_t *)arr_ptr)[idx]  = sclr.val.ui1;  break; 
    case UI2 : ((uint16_t *)arr_ptr)[idx] = sclr.val.ui2;  break; 
    case UI4 : ((uint32_t *)arr_ptr)[idx] = sclr.val.ui4;  break; 
    case UI8 : ((uint64_t *)arr_ptr)[idx] = sclr.val.ui8;  break; 

    case F4  : ((float *)arr_ptr)[idx]    = sclr.val.f4;  break; 
    case F8  : ((double *)arr_ptr)[idx]   = sclr.val.f8; break; 

    case SC : 
              {
                uint32_t len = (uint32_t)strlen(sclr.val.str);
                if ( len >= width ) { go_BYE(-1); }
                strcpy((char *)arr_ptr, sclr.val.str);
              }
              break;
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
int
x_set_arr_all(
    QDF_REC_TYPE *ptr_qdf, 
    const char * const change_col, 
    const char * const change_val
    )
{
  int status = 0;
  QDF_REC_TYPE lqdf; memset(&lqdf, 0, sizeof(QDF_REC_TYPE));

  if ( ptr_qdf == NULL ) { go_BYE(-1); }
  if ( ptr_qdf->is_read_only ) { go_BYE(-1); }
  if ( change_col == NULL ) { go_BYE(-1); }
  if ( change_val == NULL ) { go_BYE(-1); }
  status = get_key_val(ptr_qdf, -1, change_col, &lqdf, NULL); 
  if ( status < 0 ) { go_BYE(-2); } // Indicates no such column in QDF

  jtype_t jtype = x_get_jtype(&lqdf); 
  if ( jtype != j_array ) { go_BYE(-1); } 
  qtype_t qtype = x_get_qtype(&lqdf);
  uint32_t n = get_arr_len(lqdf.data); 
  switch ( qtype ) {  // TODO P3 Handle other cases as well 
    case F4 : 
      {
        char *endptr = NULL;
        float val = strtof(change_val, &endptr);
        if ( *endptr != '\0' ) { go_BYE(-2); }
        // Note the -2 that indicates that the change_val was bogus
        float * const X = (float * const) get_arr_ptr(lqdf.data); 
        for ( uint32_t i = 0; i < n; i++ ) {
          X[i] = val;
        }
      }
      break;
    case F8 : 
      {
        char *endptr = NULL;
        double val = strtod(change_val, &endptr);
        if ( *endptr != '\0' ) { go_BYE(-2); }
        // Note the -2 that indicates that the change_val was bogus
        double * const X = (double * const) get_arr_ptr(lqdf.data); 
        for ( uint32_t i = 0; i < n; i++ ) {
          X[i] = val;
        }
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
