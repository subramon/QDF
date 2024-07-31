#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "unique.h"
#include "sort1.h"
#include "fp32tobf16.h"
#include "tm2time.h"
#include "qdf_compress.h"
int
compress(
    const QDF_REC_TYPE *const src, // input 
    QDF_REC_TYPE *restrict dst1, // output 
    QDF_REC_TYPE *restrict dst2, // output 
    compress_mode_t *ptr_compress_mode // output 
    )
{
  int status = 0;
  uint32_t n_uq;
  mcr_chk_non_null(src, -1); 
  void * srcx = src->data;

  jtype_t jtype = get_jtype(srcx);
  if ( jtype != j_array ) { go_BYE(-1); }
  qtype_t src_qtype = get_qtype(srcx);

  uint32_t srcn = get_arr_len(srcx); if ( srcn == 0 ) { go_BYE(-1); } 
  const void * const srcptr = get_arr_ptr(srcx); 
  //------------------------------------------------------
  if ( src_qtype != TM ) { 
    status = num_unique(srcptr, NULL, srcn, src_qtype, &n_uq); cBYE(status);
  }
  //------------------------------------------------------
  *ptr_compress_mode = c_undef;
  switch ( src_qtype ) {
    case I1 : // no compression 
      *ptr_compress_mode = c_none;
      break;
    case I2 :
    case I4 :
      if ( ( n_uq < 256 ) && ( n_uq < 0.25 * srcn ) ) { 
        *ptr_compress_mode = c_distinct_vals;
      }
      else {
        *ptr_compress_mode = c_none; // TODO P3 Implement this
      }
      break;
    case F4 : 
      // Decide on which compression strategy to use
      if ( ( n_uq < 256 ) && ( n_uq < 0.25 * srcn ) ) { 
        *ptr_compress_mode = c_distinct_vals;
      }
      else {
        *ptr_compress_mode = c_F4_to_F2;
      }
      break;
    case TM : 
        *ptr_compress_mode = c_week_to_offset;
        break;
    default : 
      go_BYE(-1);
      break;
  }
  //----------------------------------
  switch ( *ptr_compress_mode ) {
    case c_none : 
      status = compress_none(src, dst1, dst2); cBYE(status);
      break;
    case c_distinct_vals : 
      status = compress_distinct_vals(src, src_qtype, n_uq, dst1, dst2); 
      cBYE(status);
      break;
    case c_F4_to_F2 : 
      status = compress_F4_to_F2(src, dst1, dst2); cBYE(status);
      break;
    case c_week_to_offset :
      status = compress_week_to_offset(src, dst1, dst2); cBYE(status);
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
int
compress_distinct_vals(
    const QDF_REC_TYPE *const src,
    qtype_t src_qtype,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    )
{ 
  int status = 0;
  switch ( src_qtype ) { 
    case I2 : 
      status = compress_distinct_vals_I2(src, n_uq, dst1, dst2); 
      break;
    case I4 : 
      status = compress_distinct_vals_I4(src, n_uq, dst1, dst2); 
      break;
    case F4 : 
      status = compress_distinct_vals_F4(src, n_uq, dst1, dst2); 
      break;
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}

int
compress_distinct_vals_F4(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    )
{ 
  int status = 0;
  float *invals = NULL;

  void *srcx = (void *)src->data;
  void *srcptr = get_arr_ptr(srcx); 
  float *F4src = (float *)srcptr;
  uint32_t srcn = get_arr_len(srcx); 

  status = make_num_array(NULL, srcn, 0, I1,  dst1);  cBYE(status);
  void *dst1ptr = get_arr_ptr(dst1->data); 
  uint8_t *UI1dst1 = (uint8_t *)dst1ptr; 

  status = make_num_array(NULL, n_uq, 0, F4,  dst2);  cBYE(status);
  void *dst2ptr = get_arr_ptr(dst2->data); 
  float *F4dst2 = (float *)dst2ptr; 

  // sort the input values 
  invals = malloc(srcn * sizeof(float));
  return_if_malloc_failed(invals);
  memcpy(invals, srcptr, srcn * sizeof(float));
  qsort(invals, srcn, sizeof(float), fn_sortF4_asc);

  // make the sorted values into unique values
  int outidx = 0;
  F4dst2[outidx++] = invals[0];
  for  ( uint32_t i = 1; i < srcn; i++ ) { 
    if ( F4dst2[outidx-1] != invals[i] ) {
      F4dst2[outidx++] = invals[i];
    }
  }
  if ( (uint32_t)outidx != n_uq ) { go_BYE(-1); }
  // Transform input values into indexes
  for ( uint32_t i = 0; i < srcn; i++ ) { 
    // simple sequential scan. Make faster later TODO P4
    int idx = -1;
    for ( uint32_t j = 0; j < n_uq; j++ ) {
      if ( F4src[i] == F4dst2[j] ) { 
        idx = (int)j; break;
      }
    }
    if ( idx < 0 ) { go_BYE(-1); }
    UI1dst1[i] = (uint8_t)idx;
  }
BYE:
  free_if_non_null(invals);
  return status;
}
int
compress_F4_to_F2(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE * restrict dst1,
    QDF_REC_TYPE * restrict dst2
    )
{
  int status = 0;

  void *srcx = (void *)src->data;
  void *srcptr = get_arr_ptr(srcx); 
  float *F4src = (float *)srcptr;
  uint32_t srcn = get_arr_len(srcx); 

  status = make_num_array(NULL, srcn, 0, F2, dst1);  cBYE(status);
  bfloat16 *F2dst1 = (bfloat16 *)get_arr_ptr(dst1->data); 
  for ( uint32_t i = 0; i < srcn; i++ )  {
    F2dst1[i] = fp32tobf16(F4src[i]); 
  }
  status = make_nil(NULL, dst2); cBYE(status);
BYE:
  return status;
}

int
compress_distinct_vals_I2(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE *dst1,
    QDF_REC_TYPE *dst2
    )
{ 
  int status = 0;
  int16_t *invals = NULL;

  void *srcx = (void *)src->data;
  void *srcptr = get_arr_ptr(srcx); 
  int16_t *I2src = (int16_t *)srcptr;
  uint32_t srcn = get_arr_len(srcx); 

  status = make_num_array(NULL, srcn, 0, I1,  dst1);  cBYE(status);
  void *dst1ptr = get_arr_ptr(dst1->data); 
  uint8_t *UI1dst1 = (uint8_t *)dst1ptr; 

  status = make_num_array(NULL, n_uq, 0, I2,  dst2);  cBYE(status);
  void *dst2ptr = get_arr_ptr(dst2->data); 
  int16_t *I2dst2 = (int16_t *)dst2ptr; 

  // sort the input values 
  invals = malloc(srcn * sizeof(int16_t));
  return_if_malloc_failed(invals);
  memcpy(invals, srcptr, srcn * sizeof(int16_t));
  qsort(invals, srcn, sizeof(int16_t), fn_sortI2_asc);

  // make the sorted values into unique values
  int outidx = 0;
  I2dst2[outidx++] = invals[0];
  for  ( uint32_t i = 1; i < srcn; i++ ) { 
    if ( I2dst2[outidx-1] != invals[i] ) {
      I2dst2[outidx++] = invals[i];
    }
  }
  if ( (uint32_t)outidx != n_uq ) { go_BYE(-1); }
  // Transform input values into indexes
  for ( uint32_t i = 0; i < srcn; i++ ) { 
    // simple sequential scan. Make faster later TODO P4
    int idx = -1;
    for ( uint32_t j = 0; j < n_uq; j++ ) {
      if ( I2src[i] == I2dst2[j] ) { 
        idx = (int)j; break;
      }
    }
    if ( idx < 0 ) { go_BYE(-1); }
    UI1dst1[i] = (uint8_t)idx;
  }
BYE:
  free_if_non_null(invals);
  return status;
}

int
compress_distinct_vals_I4(
    const QDF_REC_TYPE *const src,
    uint32_t n_uq,
    QDF_REC_TYPE *dst1,
    QDF_REC_TYPE *dst2
    )
{ 
  int status = 0;
  int32_t *invals = NULL;

  void *srcx = (void *)src->data;
  const int32_t * const I4src = (const int32_t * const)get_arr_ptr(srcx); 
  uint32_t srcn = get_arr_len(srcx); 

  status = make_num_array(NULL, srcn, 0, I1,  dst1);  cBYE(status);
  char * const UI1dst1 = (char * const)get_arr_ptr(dst1->data); 

  status = make_num_array(NULL, n_uq, 0, I4,  dst2);  cBYE(status);
  int32_t *I4dst2 = (int32_t *)get_arr_ptr(dst2->data); 

  // sort the input values 
  invals = malloc(srcn * sizeof(int32_t));
  return_if_malloc_failed(invals);
  memcpy(invals, I4src, srcn * sizeof(int32_t));
  qsort(invals, srcn, sizeof(int32_t), fn_sortI4_asc);

  // make the sorted values into unique values
  uint32_t outidx = 0;
  I4dst2[outidx++] = invals[0];
  for  ( uint32_t i = 1; i < srcn; i++ ) { 
    if ( I4dst2[outidx-1] != invals[i] ) {
      I4dst2[outidx++] = invals[i];
    }
  }
  if ( (uint32_t)outidx != n_uq ) { go_BYE(-1); }
  // Transform input values into indexes
  for ( uint32_t i = 0; i < srcn; i++ ) { 
    // simple sequential scan. Make faster later TODO P4
    int idx = -1;
    for ( uint32_t j = 0; j < n_uq; j++ ) {
      if ( I4src[i] == I4dst2[j] ) { 
        idx = (int)j; break;
      }
    }
    if ( idx < 0 ) { go_BYE(-1); }
    UI1dst1[i] = (uint8_t)outidx;
  }
BYE:
  free_if_non_null(invals);
  return status;
}
int
compress_none(
    const QDF_REC_TYPE * const src,
    QDF_REC_TYPE * dst1,
    QDF_REC_TYPE * dst2
    )
{
  int status = 0;
  dst1->data = malloc(src->size);
  return_if_malloc_failed(dst1->data);
  memcpy(dst1->data, src->data, src->size);
  dst1->size = src->size;
  status = make_nil(NULL, dst2); cBYE(status);
BYE:
  return status;
}

int
compress_week_to_offset(
    const QDF_REC_TYPE *const src,
    QDF_REC_TYPE *dst1,
    QDF_REC_TYPE *dst2
    )
{
  int status = 0;

  char *srcx = src->data;
  const void * const srcptr = get_arr_ptr(srcx); 
  const tm_t * const TMsrc = (const tm_t * const)srcptr;
  uint32_t srcn = get_arr_len(srcx); 
  tm_t min_tm;

  // find minimum and maximum value 
  time_t max_t = INT_MIN, min_t = INT_MAX, this_t; 
  time_t diff1 = (time_t)3072 * (time_t)7 * (time_t)86400;
  for ( uint32_t i = 0; i < srcn; i++ ) {
    struct tm t1;
    t_assign(&t1, TMsrc+i);
    this_t = tm2time(&t1);
    if ( this_t < min_t ) { min_t = this_t; min_tm = TMsrc[i]; }
    if ( this_t > max_t ) { max_t = this_t; }
  }
  //-------------------------------------------------
  if ( (max_t - min_t ) <= (255 * 7 * 86400 ) ) { 
    status = make_num_array(NULL, srcn, 0, UI1, dst1);  cBYE(status);
    char *dst1ptr = get_arr_ptr(dst1->data); 
    uint8_t  *UI1dst1 = (uint8_t *)dst1ptr;
    for ( uint32_t i = 0; i < srcn; i++ )  {
      struct tm t1;
      t_assign(&t1, TMsrc+i);
      this_t = tm2time(&t1);
      time_t tmp =  (this_t - min_t) / (7 * 86400);
      if ( tmp > (time_t)UINT_MAX ) { go_BYE(-1); } 
      UI1dst1[i] = (uint8_t)tmp;
    }
  }
  else if ( (max_t - min_t ) <= diff1 ) { 
    status = make_num_array(NULL, srcn, 0, UI2, dst1);  cBYE(status);
    void *dst1ptr = get_arr_ptr(dst1->data); 
    uint16_t  *UI2dst1 = (uint16_t *)dst1ptr;
    for ( uint32_t i = 0; i < srcn; i++ )  {
      struct tm t1;
      t_assign(&t1, TMsrc+i);
      this_t = tm2time(&t1);
      time_t tmp =  (this_t - min_t) / (7 * 86400);
      if ( tmp > (time_t)USHRT_MAX ) { go_BYE(-1); } 
      UI2dst1[i] = (uint16_t)tmp;
    }
  }
  else {
    go_BYE(-1);
  }
  status = make_date(min_tm, dst2); cBYE(status);
BYE:
  return status;
}
