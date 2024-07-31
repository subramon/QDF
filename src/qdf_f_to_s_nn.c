#include "incs.h"
#include "qdf_struct.h"
#include "auxil.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_f_to_s_nn.h"

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
// num_sclr returns number of values used to create out_sclr
int
f_to_s_nn(
    const QDF_REC_TYPE *const ptr_qdf,
    const QDF_REC_TYPE *const ptr_nn_qdf,
    const char * const str_op,
    const QDF_SCLR_TYPE *const ptr_in_sclr,
    QDF_SCLR_TYPE * restrict ptr_out_sclr, // output 
    QDF_SCLR_TYPE * restrict ptr_num_sclr // output 
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  f_to_s_op_t op = get_f_to_s_op(str_op);
  if ( op == f_to_s_undef ) { go_BYE(-1); } 

  memset(ptr_out_sclr, 0, sizeof(QDF_SCLR_TYPE)); // clean output 
  memset(ptr_num_sclr, 0, sizeof(QDF_SCLR_TYPE)); // clean output 
  const char * const x = ptr_qdf->data;
  const char * const y = get_arr_ptr(x); 

  const char * const nn_x = ptr_nn_qdf->data;
  const int8_t * const nn_y = (const int8_t * const)get_arr_ptr(nn_x); 

  jtype_t jtype = get_jtype(x);
  if ( jtype != j_array ) { go_BYE(-1); }

  qtype_t qtype = get_qtype(x); 
  if ( qtype == Q0 ) { go_BYE(-1); }

  jtype_t nn_jtype = get_jtype(nn_x);
  if ( nn_jtype != j_array ) { go_BYE(-1); }

  qtype_t nn_qtype = get_qtype(nn_x); 
  if ( nn_qtype != I1 ) { go_BYE(-1); }

  uint32_t n = get_arr_len(x); 
  uint32_t nn_n = get_arr_len(nn_x); 
  if ( n != nn_n ) { go_BYE(-1); }
  // calculate number of non null values 
  register int64_t isum = 0;
#pragma omp simd reduction(+:isum)
  for ( uint32_t i = 0; i < n; i++ ) { 
    isum += nn_x[i];
#ifdef DEBUG
    if ( ( nn_x[i] != 0 ) && ( nn_x[i] != 1 ) ) { go_BYE(-1); } 
#endif
  }
  ptr_num_sclr.qtype = I4;
  ptr_num_sclr.val.i4 = isum;
  //---------------------------------------------------

  uint32_t n = get_arr_len(x); 
  switch ( op )  {
    case f_to_s_sum : 
      status = sum_nn(y, nn_y, n, qtype, ptr_out_sclr); 
      cBYE(status);
      break;
    case f_to_s_min : 
      status = min_nn(y, nn_y, n, qtype, ptr_out_sclr); 
      cBYE(status);
      break;
    case f_to_s_max : 
      status = max_nn(y, nn_y, n, qtype, ptr_out_sclr); 
      cBYE(status);
      break;
    case f_to_s_sd : 
      status = sd_nn(y, nn_y, n, qtype, ptr_in_sclr, ptr_out_sclr); 
      cBYE(status);
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
sum_nn(
    const char * const X, 
    const int8_t * const nn_X, 
    uint32_t n, 
    qtype_t qtype, 
    QDF_SCLR_TYPE * restrict ptr_out_sclr
    )
{
  int status = 0;

  register int64_t isum = 0;
  register double  fsum = 0;

  switch ( qtype ) {
    case I1 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        isum += ((const int8_t * const)X)[i]; 
      }
      break;
    case I2 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        isum += ((const int16_t * const)X)[i]; 
      }
      break;
    case I4 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        isum += ((const int32_t * const)X)[i]; 
      }
      break;
    case I8 : 
#pragma omp simd reduction(+:isum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        isum += ((const int64_t * const)X)[i]; 
      }
      break;
    case F4 : 
#pragma omp simd reduction(+:fsum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        fsum += ((const float * const)X)[i]; 
      }
      break;
    case F8 : 
#pragma omp simd reduction(+:fsum)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
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
min_nn(
    const char * const X, 
    const int8_t * const nn_X, 
    uint32_t n, 
    qtype_t qtype,
    QDF_SCLR_TYPE * restrict ptr_out_sclr
    )
{
  int status = 0;
  bool first = true; 
  ptr_out_sclr->qtype = qtype;
  switch ( qtype ) {
    case UI1 :
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        uint8_t val =  ((const uint8_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val < ptr_sclr->val.ui1 ) { ptr_sclr->val.ui1 = val; }
      }
      break;
    case UI2 :
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        uint16_t val = ((const uint16_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val < ptr_sclr->val.ui2 ) { ptr_sclr->val.ui2 = val; }
      }
      break;
    case I1 :
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int8_t val = ((const int8_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val < ptr_sclr->val.i1 ) { ptr_sclr->val.i1 = val; }
      }
      break;
    case I2 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int16_t val = ((const int16_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val < ptr_sclr->val.i2 ) { ptr_sclr->val.i2 = val; }
      }
      break;
    case I4 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int32_t val = ((const int32_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val < ptr_sclr->val.i4 ) { ptr_sclr->val.i4 = val; }
      }
      break;
    case I8 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int64_t val = ((const int64_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val < ptr_sclr->val.i8 ) { ptr_sclr->val.i8 = val; }
      }
      break;
    case F4 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        float val = ((const float *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val < ptr_sclr->val.f4 ) { ptr_sclr->val.f4 = val; }
      }
      break;
    case F8 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        double val = ((const double *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
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
max_nn(
    const char * const X, 
    const int8_t * const nn_X, 
    uint32_t n, 
    qtype_t qtype,
    QDF_SCLR_TYPE * restrict ptr_out_sclr
    )
{
  int status = 0;
  bool first = true; 
  ptr_out_sclr->qtype = qtype;
  switch ( qtype ) {
    case UI1 :
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        uint8_t val =  ((const uint8_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.ui1 ) { ptr_sclr->val.ui1 = val; }
      }
      break;
    case UI2 :
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        uint16_t val = ((const uint16_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.ui2 ) { ptr_sclr->val.ui2 = val; }
      }
      break;
    case I1 :
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int8_t val = ((const int8_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.i1 ) { ptr_sclr->val.i1 = val; }
      }
      break;
    case I2 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int16_t val = ((const int16_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.i2 ) { ptr_sclr->val.i2 = val; }
      }
      break;
    case I4 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int32_t val = ((const int32_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.i4 ) { ptr_sclr->val.i4 = val; }
      }
      break;
    case I8 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        int64_t val = ((const int64_t *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.i8 ) { ptr_sclr->val.i8 = val; }
      }
      break;
    case F4 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        float val = ((const float *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.f4 ) { ptr_sclr->val.f4 = val; }
      }
      break;
    case F8 : 
      for ( uint32_t i = 1; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        double val = ((const double *const)X)[i];
        if ( first ) { ptr_sclr->val.ui1 = val; first = false; continue; }
        if ( val > ptr_sclr->val.f8 ) { ptr_sclr->val.f8 = val; }
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
sd_nn(
    const char  * const X, 
    const int8_t  * const nn_X, 
    uint32_t n, 
    qtype_t qtype, 
    const QDF_SCLR_TYPE * const ptr_in_sclr,
    QDF_SCLR_TYPE *ptr_out_sclr
    )
{
  int status = 0;
  register double numer = 0;
  register int32_t denom = 0;
  double mu;
  status = get_sclr_as_F8(ptr_in_sclr, &mu);  cBYE(status);
  switch ( qtype ) {
    case I1 : 
#pragma omp simd reduction(+:numer)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        double val = (double)((const int8_t * const )X)[i];
        double val_minus_mu = val - mu;
        numer += sqr(val_minus_mu); 
        denom++; 
      }
      break;
    case I2 : 
#pragma omp simd reduction(+:numer)
      for ( uint32_t i = 0; i < n; i++ ) { 
        double val = (double)((const int16_t * const )X)[i];
        double val_minus_mu = val - mu;
        numer += sqr(val_minus_mu); 
        denom++; 
      }
      break;
    case I4 : 
#pragma omp simd reduction(+:numer)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        double val = (double)((const int32_t * const )X)[i];
        double val_minus_mu = val - mu;
        numer += sqr(val_minus_mu); 
        denom++; 
      }
      break;
    case I8 : 
#pragma omp simd reduction(+:numer)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        double val = (double)((const int64_t * const )X)[i];
        double val_minus_mu = val - mu;
        numer += sqr(val_minus_mu); 
        denom++; 
      }
      break;
    case F4 : 
#pragma omp simd reduction(+:numer)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        double val = (double)((const float * const )X)[i];
        double val_minus_mu = val - mu;
        numer += sqr(val_minus_mu); 
        denom++; 
      }
      break;
    case F8 : 
#pragma omp simd reduction(+:numer)
      for ( uint32_t i = 0; i < n; i++ ) { 
        if ( nn_X[i] == 0 ) { continue; }
        double val = (double)((const double * const )X)[i];
        double val_minus_mu = val - mu;
        numer += sqr(val_minus_mu); 
        denom++; 
      }
      break;
    default :
      go_BYE(-1);
      break;
  }
  if ( denom != 0 ) { 
    ptr_out_sclr->val.f8 = sqrt(numer/denom);
    ptr_out_sclr->qtype = F8;
  }
BYE:
  return status;
}
