#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_srt_grp_cnt.h"
// Assumption is that input is sorted ascending 
// If the input is  1 1 2 2 2 3 3 3 3 4 6 6 6 6 6 
// The output is 2 3 4 1 5, the number of items in each group
// Groups are (1 1) (2 2 2) (3 3 3 3) (4) (6 6 6 6 6)
static uint32_t
count_uq(
      const void * const sptr, 
      uint32_t sn,
      qtype_t sqtype
      ) 
{
  int status = 0; 

  uint32_t n_uq = 1;
  switch ( sqtype ) {
    case I1 : 
      {
        const int8_t * const svals = (const int8_t * const ) sptr;
        int8_t sval = svals[0];
        for ( uint32_t i = 1; i < sn; i++ ) { 
          if ( sval != svals[i] ) { 
            sval = svals[i];
            n_uq++;
          }
        }
      }
      break;
    case I2 : 
      {
        const int16_t * const svals = (const int16_t * const ) sptr;
        int16_t sval = svals[0];
        for ( uint32_t i = 1; i < sn; i++ ) { 
          if ( sval != svals[i] ) { 
            sval = svals[i];
            n_uq++;
          }
        }
      }
      break;
    case I4 : 
      {
        const int32_t * const svals = (const int32_t * const )sptr; 
        int32_t sval = svals[0];
        for ( uint32_t i = 1; i < sn; i++ ) { 
          if ( sval != svals[i] ) { 
            sval = svals[i];
            n_uq++;
          }
        }
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  if ( status == 0 ) { return n_uq; } else { return 0; }
}
static int
num_in_grp(
    const void * const sptr,
    uint32_t sn,
    qtype_t sqtype,
    void *restrict dptr
    )
{
  int status = 0; 
  int32_t *dvals = (int32_t *)dptr;

  int didx = 0;
  dvals[didx] = 1;
  switch ( sqtype ) { 
    case I1 : 
      {
        const int8_t * const svals = (const int8_t * const )sptr;
        int8_t sval = svals[0];
        for ( uint32_t i = 1; i < sn; i++ ) { 
          if ( sval == svals[i] ) { 
            dvals[didx]++;
          }
          else {
            didx++;
            dvals[didx] = 1;
            sval = svals[i];
          }
        }
      }
      break;
    case I2 : 
      {
        const int16_t * const svals = (const int16_t * const )sptr;
        int16_t sval = svals[0];
        for ( uint32_t i = 1; i < sn; i++ ) { 
          if ( sval == svals[i] ) { 
            dvals[didx]++;
          }
          else {
            didx++;
            dvals[didx] = 1;
            sval = svals[i];
          }
        }
      }
      break;
    case I4 : 
      {
        const int32_t * const svals = (const int32_t * const )sptr;
        int32_t sval = svals[0];
        for ( uint32_t i = 1; i < sn; i++ ) { 
          if ( sval == svals[i] ) { 
            dvals[didx]++;
          }
          else {
            didx++;
            dvals[didx] = 1;
            sval = svals[i];
          }
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

int
srt_grp_cnt(
    const QDF_REC_TYPE * const src, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;

  mcr_chk_non_null(src, -1); 
  void *sx = src->data; 
  uint32_t sn         = get_arr_len(sx); if ( sn == 0 ) { go_BYE(-1); }
  jtype_t sjtype = get_jtype(sx);   if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx); 
  const void *const sptr     = get_arr_ptr(sx); 
#ifdef DEBUG
  switch ( sqtype ) { 
    case I1 : 
      for ( uint32_t i = 1;  i < sn; i++ ) { 
        const int8_t * const I1ptr = (const int8_t * const)sptr; 
        if ( I1ptr[i] < I1ptr[i-1] ) { go_BYE(-1); } 
      }
      break;
    case I2 : 
      for ( uint32_t i = 1;  i < sn; i++ ) { 
        const int16_t * const I2ptr = (const int16_t * const)sptr; 
        if ( I2ptr[i] < I2ptr[i-1] ) { go_BYE(-1); } 
      }
      break;
    case I4 : 
      for ( uint32_t i = 1;  i < sn; i++ ) { 
        const int32_t * const I4ptr = (const int32_t * const)sptr; 
        if ( I4ptr[i] < I4ptr[i-1] ) { go_BYE(-1); } 
      }
      break;
    default :
      go_BYE(-1);
      break;
  }
#endif

  //==============================
  // Calculate number of unique values
  uint32_t n_uq = count_uq(sptr, sn, sqtype); if ( n_uq == 0 ) { go_BYE(-1); }
  status = make_num_array(NULL, n_uq, 0, I4,  dst); cBYE(status);
  char *dptr = get_arr_ptr(dst->data); 
  status = num_in_grp(sptr, sn, sqtype, dptr); cBYE(status);
BYE:
  return status;
}
