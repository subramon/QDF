#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_setters.h"
#include "qdf_copy.h"

int
qdf_copy_array(
    QDF_REC_TYPE *dst, // modified in place 
    const QDF_REC_TYPE * const src
   )
{
  int status = 0;

  //==============================
  if ( dst == NULL ) { go_BYE(-1); }
  mcr_chk_non_null(dst, -1);
  mcr_chk_non_null(src, -1);
  void *dx = dst->data; 

  jtype_t dst_jtype = get_jtype(dx); 
  if ( dst_jtype != j_array ) { go_BYE(-1); } 
  qtype_t dst_qtype = get_qtype(dx); 
  if ( dst_qtype == Q0 ) { go_BYE(-1); } 

  uint32_t dn   = get_arr_len(dx);   // OKAY if ( dn == 0 ) { go_BYE(-1); }
  uint32_t dsz  = get_arr_size(dx);  if ( dsz < dn ) { go_BYE(-1); }
  uint32_t dw   = get_arr_width(dx); if ( dw == 0 ) { go_BYE(-1); }
  void *dst_ptr = get_arr_ptr(dx);

  memset(dst_ptr, 0, dsz*dw); // wipe out contents of dst
  //==============================
  void *sx = src->data; 

  jtype_t src_jtype = get_jtype(sx); 
  if ( src_jtype != j_array ) { go_BYE(-1); } 
  qtype_t src_qtype = get_qtype(sx); 
  if ( src_qtype == Q0 ) { go_BYE(-1); } 

  uint32_t sn = get_arr_len(sx); if ( sn <= 0 ) { go_BYE(-1); }
  uint32_t sw = get_arr_width(sx);  
  const char * const src_ptr = get_arr_ptr(sx); 
  //==============================
  if ( dsz      < sn  ) { go_BYE(-1); }
  if ( ( dw     != sw       ) ||
       ( dst_qtype != src_qtype ) ) { 
    status = coerce(dst_ptr, dst_qtype, src_ptr, src_qtype, sn);
  }
  else {
    memcpy(dst_ptr, src_ptr, sn*dw);
  }
  // set number of elements in destination
  set_arr_len(dx, sn);  cBYE(status);
BYE:
  return status;
}

// TODO P3: This code should be auto-generated from a template 
int
coerce(
    void *dst, 
    qtype_t dst_qtype,
    const void * const src,
    qtype_t src_qtype,
    uint32_t sn
    )
{
  int status = 0;
  int8_t   *I1dst; const int8_t  * I1src;
  int16_t  *I2dst; const int16_t * I2src;
  int32_t  *I4dst; const int32_t * I4src;
  int64_t  *I8dst; const int64_t * I8src;
  float    *F4dst; const float   * F4src;
  double   *F8dst; const double  * F8src;
  uint64_t *UI8dst;

  if ( dst == NULL ) { go_BYE(-1); }
  if ( src == NULL ) { go_BYE(-1); }
  if ( sn   <= 0    ) { go_BYE(-1); }

  I1dst = (int8_t *)dst;
  I2dst = (int16_t * const)dst;
  I4dst = (int32_t * const)dst;
  I8dst = (int64_t * const)dst;
  F4dst = (float * const)dst;
  F8dst = (double * const)dst;
  UI8dst = (uint64_t * const)dst;

  I1src = (const int8_t * const)src;
  I2src = (const int16_t * const)src;
  I4src = (const int32_t * const)src;
  I8src = (const int64_t * const)src;
  F4src = (const float * const)src;
  F8src = (const double * const)src;

  switch ( dst_qtype ) {
    case I1 : 
      switch ( src_qtype ) {
        case I1 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I1dst[i] = I1src[i]; }
          break;
        case I2 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I1dst[i] = (int8_t)I2src[i]; }
          break;
        case I4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I1dst[i] = (int8_t)I4src[i]; }
          break;
        case I8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I1dst[i] = (int8_t)I8src[i]; }
          break;
        case F4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I1dst[i] = (int8_t)F4src[i]; }
          break;
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I1dst[i] = (int8_t)F8src[i]; }
          break;
        default :
          go_BYE(-1);
          break;
      }
      break;
    case I2 : 
      switch ( src_qtype ) {
        case I1 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I2dst[i] = (int16_t)I1src[i]; }
          break;
        case I2 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I2dst[i] = I2src[i]; }
          break;
        case I4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I2dst[i] = (int16_t)I4src[i]; }
          break;
        case I8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I2dst[i] = (int16_t)I8src[i]; }
          break;
        case F4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I2dst[i] = (int16_t)F4src[i]; }
          break;
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I2dst[i] = (int16_t)F8src[i]; }
          break;
        default :
          go_BYE(-1);
          break;
      }
      break;
    case I4 : 
      switch ( src_qtype ) {
        case I1 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I4dst[i] = I1src[i]; }
          break;
        case I2 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I4dst[i] = I2src[i]; }
          break;
        case I4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I4dst[i] = I4src[i]; }
          break;
        case I8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I4dst[i] = (int32_t)I8src[i]; }
          break;
        case F4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I4dst[i] = (int32_t)F4src[i]; }
          break;
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I4dst[i] = (int32_t)F8src[i]; }
          break;
        default :
          go_BYE(-1);
          break;
      }
      break;
    case I8 : 
      switch ( src_qtype ) {
        case I1 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I8dst[i] = I1src[i]; }
          break;
        case I2 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I8dst[i] = I2src[i]; }
          break;
        case I4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I8dst[i] = I4src[i]; }
          break;
        case I8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I8dst[i] = I8src[i]; }
          break;
        case F4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I8dst[i] = (int8_t)F4src[i]; }
          break;
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { I8dst[i] = (int8_t)F8src[i]; }
          break;
        default :
          go_BYE(-1);
          break;
      }
      break;
    case F4 : 
      switch ( src_qtype ) {
        case I1 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F4dst[i] = I1src[i]; }
          break;
        case I2 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F4dst[i] = I2src[i]; }
          break;
        case I4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F4dst[i] = (float)I4src[i]; }
          break;
        case I8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F4dst[i] = (float)I8src[i]; }
          break;
        case F4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F4dst[i] = F4src[i]; }
          break;
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F4dst[i] = (float)F8src[i]; }
          break;
        default :
          go_BYE(-1);
          break;
      }
      break;
    case F8 : 
      switch ( src_qtype ) {
        case I1 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F8dst[i] = I1src[i]; }
          break;
        case I2 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F8dst[i] = I2src[i]; }
          break;
        case I4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F8dst[i] = I4src[i]; }
          break;
        case I8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F8dst[i] = (double)I8src[i]; }
          break;
        case F4 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F8dst[i] = F4src[i]; }
          break;
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { F8dst[i] = F8src[i]; }
          break;
        default :
          go_BYE(-1);
          break;
      }
      break;
    case UI8 : 
      switch ( src_qtype ) {
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) { UI8dst[i] = (uint64_t)F8src[i]; }
          break;
        default :
          go_BYE(-1);
          break;
      }
      break;
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}

