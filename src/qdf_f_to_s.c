#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_f_to_s.h"

typedef enum { 
  f_to_s_undef,
  f_to_s_min,
  f_to_s_max,
  f_to_s_sum,
  f_to_s_sd,
} f_to_s_op_t;

static f_to_s_op_t
get_f_to_s_op(
    const char * const str_op
    )
{
  if ( strcmp(str_op, "min") == 0 ) { return f_to_s_min; } 
  if ( strcmp(str_op, "max") == 0 ) { return f_to_s_max; } 
  if ( strcmp(str_op, "sum") == 0 ) { return f_to_s_sum; } 
  if ( strcmp(str_op, "sd")  == 0 ) { return f_to_s_sd; } 
  return f_to_s_undef;
}
int
f_to_s(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const str_op,
    const SCLR_REC_TYPE *const ptr_in_sclr,
    SCLR_REC_TYPE * restrict ptr_out_sclr // output 
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  f_to_s_op_t op = get_f_to_s_op(str_op);
  if ( op == f_to_s_undef ) { go_BYE(-1); } 

  memset(ptr_out_sclr, 0, sizeof(SCLR_REC_TYPE)); // clean output 
  void * const x = ptr_qdf->data;
  void * y = get_arr_ptr(x); 

  jtype_t jtype = get_jtype(x);
  if ( jtype != j_array ) { go_BYE(-1); }

  qtype_t qtype = get_qtype(x); 
  if ( qtype == Q0 ) { go_BYE(-1); }

  uint32_t n = get_arr_len(x); 
  switch ( op )  {
    case f_to_s_sum : 
      status = sum(y, n, qtype, ptr_out_sclr); cBYE(status);
      break;
    case f_to_s_min : 
      status = min(y, n, qtype, ptr_out_sclr); cBYE(status);
      break;
    case f_to_s_max : 
      status = max(y, n, qtype, ptr_out_sclr); cBYE(status);
      break;
    case f_to_s_sd : 
      status = sd(y, n, qtype, ptr_in_sclr, ptr_out_sclr); cBYE(status);
      break;
    default :
      go_BYE(-1);
      break;
  }
  //--------------------------------------------------
BYE:
  return status;
}

int 
sum(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype, 
    SCLR_REC_TYPE * restrict ptr_out_sclr
    )
{
  int status = 0;
  register int64_t isum = 0;
  register double  fsum = 0;
  switch ( qtype ) {
    case I1 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        isum += ((const int8_t * const)X)[i]; 
      }
      break;
    case I2 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        isum += ((const int16_t * const)X)[i]; 
      }
      break;
    case I4 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        isum += ((const int32_t * const)X)[i]; 
      }
      break;
    case I8 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        isum += ((const int64_t * const)X)[i]; 
      }
      break;
    case F4 : 
#pragma omp simd reduction(+:fsum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        fsum += ((const float * const)X)[i]; 
      }
      break;
    case F8 : 
#pragma omp simd reduction(+:fsum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        fsum += ((const double * const)X)[i]; 
      }
      break;
    default :
      go_BYE(-1);
      break;
  }
  switch ( qtype ) {
    case I1 : case I2 : case I4 : case I8 : 
      ptr_out_sclr->val.i8 = isum;
      ptr_out_sclr->qtype = I8;
      break;
    case F4 : case F8 : 
      ptr_out_sclr->val.f8 = fsum;
      ptr_out_sclr->qtype = F8;
      break;
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}

int 
max(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype,
    SCLR_REC_TYPE * restrict ptr_sclr
    )
{
  int status = 0;
  ptr_sclr->qtype = qtype;
  switch ( qtype ) {
    case UI1 :
      ptr_sclr->val.ui1 = ((const uint8_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        uint8_t val = ((const uint8_t *const)X)[i];
        if ( val > ptr_sclr->val.ui1 ) { ptr_sclr->val.ui1 = val; }
      }
      break;
    case UI2 :
      ptr_sclr->val.ui2 = ((const uint16_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        uint16_t val = ((const uint16_t *const)X)[i];
        if ( val > ptr_sclr->val.ui2 ) { ptr_sclr->val.ui2 = val; }
      }
      break;
    case I1 :
      ptr_sclr->val.i1 = ((const int8_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int8_t val = ((const int8_t *const)X)[i];
        if ( val > ptr_sclr->val.i1 ) { ptr_sclr->val.i1 = val; }
      }
      break;
    case I2 : 
      ptr_sclr->val.i2 = ((const int16_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int16_t val = ((const int16_t *const)X)[i];
        if ( val > ptr_sclr->val.i2 ) { ptr_sclr->val.i2 = val; }
      }
      break;
    case I4 : 
      ptr_sclr->val.i4 = ((const int32_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int32_t val = ((const int32_t *const)X)[i];
        if ( val > ptr_sclr->val.i4 ) { ptr_sclr->val.i4 = val; }
      }
      break;
    case I8 : 
      ptr_sclr->val.i8 = ((const int64_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int64_t val = ((const int64_t *const)X)[i];
        if ( val > ptr_sclr->val.i8 ) { ptr_sclr->val.i8 = val; }
      }
      break;
    case F4 : 
      ptr_sclr->val.f4 = ((const float *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        float val = ((const float *const)X)[i];
        if ( val > ptr_sclr->val.f4 ) { ptr_sclr->val.f4 = val; }
      }
      break;
    case F8 : 
      ptr_sclr->val.f8 = ((const double *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        double val = ((const double *const)X)[i];
        if ( val > ptr_sclr->val.f8 ) { ptr_sclr->val.f8 = val; }
      }
      break;
      break;
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
int 
min(
    const void * const X, 
    uint32_t n, 
    qtype_t qtype,
    SCLR_REC_TYPE * restrict ptr_sclr
    )
{
  int status = 0;
  ptr_sclr->qtype = qtype;
  switch ( qtype ) {
    case UI1 :
      ptr_sclr->val.ui1 = ((const uint8_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        uint8_t val = ((const uint8_t *const)X)[i];
        if ( val < ptr_sclr->val.ui1 ) { ptr_sclr->val.ui1 = val; }
      }
      break;
    case UI2 :
      ptr_sclr->val.ui2 = ((const uint16_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        uint16_t val = ((const uint16_t *const)X)[i];
        if ( val < ptr_sclr->val.ui2 ) { ptr_sclr->val.ui2 = val; }
      }
      break;
    case I1 :
      ptr_sclr->val.i1 = ((const int8_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int8_t val = ((const int8_t *const)X)[i];
        if ( val < ptr_sclr->val.i1 ) { ptr_sclr->val.i1 = val; }
      }
      break;
    case I2 : 
      ptr_sclr->val.i2 = ((const int16_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int16_t val = ((const int16_t *const)X)[i];
        if ( val < ptr_sclr->val.i2 ) { ptr_sclr->val.i2 = val; }
      }
      break;
    case I4 : 
      ptr_sclr->val.i4 = ((const int32_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int32_t val = ((const int32_t *const)X)[i];
        if ( val < ptr_sclr->val.i4 ) { ptr_sclr->val.i4 = val; }
      }
      break;
    case I8 : 
      ptr_sclr->val.i8 = ((const int64_t *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        int64_t val = ((const int64_t *const)X)[i];
        if ( val < ptr_sclr->val.i8 ) { ptr_sclr->val.i8 = val; }
      }
      break;
    case F4 : 
      ptr_sclr->val.f4 = ((const float *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        float val = ((const float *const)X)[i];
        if ( val < ptr_sclr->val.f4 ) { ptr_sclr->val.f4 = val; }
      }
      break;
    case F8 : 
      ptr_sclr->val.f8 = ((const double *const)X)[0]; 
      for ( uint32_t i = 1; i < n; i++ ) { 
        double val = ((const double *const)X)[i];
        if ( val < ptr_sclr->val.f8 ) { ptr_sclr->val.f8 = val; }
      }
      break;
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}

int 
sd(
    const void  * const X, 
    uint32_t n, 
    qtype_t qtype, 
    const SCLR_REC_TYPE * const ptr_in_sclr,
    SCLR_REC_TYPE *ptr_out_sclr
    )
{
  int status = 0;
  register double sum = 0;
  double mu;
  status = get_sclr_as_F8(ptr_in_sclr, &mu);  cBYE(status);
  switch ( qtype ) {
    case I1 : 
#pragma omp simd reduction(+:sum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        double val = (double)((const int8_t * const )X)[i];
        double val_minus_mu = val - mu;
        sum += sqr(val_minus_mu); 
      }
      break;
    case I2 : 
#pragma omp simd reduction(+:sum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        double val = (double)((const int16_t * const )X)[i];
        double val_minus_mu = val - mu;
        sum += sqr(val_minus_mu); 
      }
      break;
    case I4 : 
#pragma omp simd reduction(+:sum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        double val = (double)((const int32_t * const )X)[i];
        double val_minus_mu = val - mu;
        sum += sqr(val_minus_mu); 
      }
      break;
    case I8 : 
#pragma omp simd reduction(+:sum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        double val = (double)((const int64_t * const )X)[i];
        double val_minus_mu = val - mu;
        sum += sqr(val_minus_mu); 
      }
      break;
    case F4 : 
#pragma omp simd reduction(+:sum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        double val = (double)((const float * const )X)[i];
        double val_minus_mu = val - mu;
        sum += sqr(val_minus_mu); 
      }
      break;
    case F8 : 
#pragma omp simd reduction(+:sum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        double val = (double)((const double * const )X)[i];
        double val_minus_mu = val - mu;
        sum += sqr(val_minus_mu); 
      }
      break;
    default :
      go_BYE(-1);
      break;
  }
  ptr_out_sclr->val.f8 = sqrt(sum/n);
  ptr_out_sclr->qtype = F8;
BYE:
  return status;
}
