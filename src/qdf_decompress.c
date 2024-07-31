#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "bf16tofp32.h"
#include "tm2time.h"
#include "qdf_decompress.h"
int
decompress(
    const QDF_REC_TYPE * const src1,
    const QDF_REC_TYPE * const src2,
    compress_mode_t compress_mode,
    QDF_REC_TYPE * restrict dst
    )
{
  int status = 0;
  mcr_chk_non_null(src1, -1); 
  mcr_chk_non_null(src2, -1); 

  switch ( compress_mode ) {
    case c_distinct_vals :
      {
        qtype_t src2_qtype = x_get_qtype(src2); 
        status = decompress_distinct_vals(src1, src2, src2_qtype, dst); 
        cBYE(status);
      }
    break;
    case c_F4_to_F2 :
    status = decompress_F4_to_F2(src1, dst); cBYE(status);
    break;
    case c_week_to_offset :
    // HANDLED AS A SPECIAL CASE 
    go_BYE(-1); 
    break;
    default :
    go_BYE(-1);
    break;
  }
BYE:
  return status;
}

int
decompress_distinct_vals(
    const QDF_REC_TYPE * const src1,
    const QDF_REC_TYPE * const src2,
    qtype_t qtype,
    QDF_REC_TYPE * restrict dst
    )
{
  int status = 0;
  //-------------------------------------------------
  mcr_chk_non_null(src1, -1);
  mcr_chk_non_null(src2, -1);
  void *src1x = src1->data;
  jtype_t src1_jtype = get_jtype(src1x);
  if ( src1_jtype != j_array ) { go_BYE(-1); }

  qtype_t src1_qtype = get_qtype(src1x);
  if ( src1_qtype != I1 ) { go_BYE(-1); }

  uint32_t src1n = get_arr_len(src1x); 

  const uint8_t * const UI1src1 = (const uint8_t * const)get_arr_ptr(src1x);
  //-------------------------------------------------
  void *src2x = src2->data;
  jtype_t src2_jtype = get_jtype(src2x);
  if ( src2_jtype != j_array ) { go_BYE(-1); }

  // int src2n = get_arr_len(src2x); 
  // TODO P1 Review this code. Do we care about src2n? 
  const int16_t * const I2src2 = (const int16_t * const)get_arr_ptr(src2x);
  const int32_t * const I4src2 = (const int32_t * const)get_arr_ptr(src2x);
  const float   * const F4src2 = (const float   * const)get_arr_ptr(src2x);
  //-------------------------------------------------
  void * dstptr = get_arr_ptr(dst->data); 
  int16_t *I2dst = (int16_t *)dstptr;
  int32_t *I4dst = (int32_t *)dstptr;
  float   *F4dst = (float   *)dstptr;
  switch ( qtype ) { 
    case I2 : 
        status = make_num_array(NULL, src1n, 0, I2,  dst); cBYE(status);

        for ( uint32_t i = 0; i < src1n; i++ ) { 
          I2dst[i] = I2src2[UI1src1[i]];
        }
      break;
    case I4 : 
        status = make_num_array(NULL, src1n, 0, I4,  dst); cBYE(status);
        for ( uint32_t i = 0; i < src1n; i++ ) { 
          I4dst[i] = I4src2[UI1src1[i]];
        }
      break;
    case F4 : 
        status = make_num_array(NULL, src1n, 0, F4,  dst); cBYE(status);
        for ( uint32_t i = 0; i < src1n; i++ ) { 
          F4dst[i] = F4src2[UI1src1[i]];
        }
      break;
    default :
      go_BYE(-1);
      break;
  }
  //-------------------------------------------------
BYE:
  return status;
}
int
decompress_F4_to_F2(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE * restrict dst
    )
{
  int status = 0;
  mcr_chk_non_null(src, -1); 
  void *srcx = (void *)src->data;

  jtype_t src_jtype = get_jtype(srcx);
  if ( src_jtype != j_array ) { go_BYE(-1); }

  qtype_t src_qtype = get_qtype(srcx);
  if ( src_qtype != F2 ) { go_BYE(-1); }

  uint32_t srcn = get_arr_len(srcx); 

  const bfloat16 * const F2src = (const bfloat16 * const)get_arr_ptr(srcx);

  status = make_num_array(NULL, srcn, 0, F4,  dst); cBYE(status);
  float *F4dst = (float *)get_arr_ptr(dst->data); 

  for ( uint32_t i = 0; i < srcn; i++ ) { 
    F4dst[i] = bf16tofp32(F2src[i]);
  }
BYE:
  return status;
}
