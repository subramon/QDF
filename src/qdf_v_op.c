#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "sawtooth.h"
#include "flatten_sawtooth.h"
#include "qdf_v_op.h"

#define mcr_conv_error(s, d) { \
  fprintf(stderr, "Cannot convert from %s to %s \n", \
      get_str_qtype(s), get_str_qtype(d)); \
}

typedef enum _optype_t { 
  uop_undef, // always first 
  uop_abs,
  uop_bitwise_not,
  uop_convert,
  uop_exp,
  uop_log,
  uop_logical_not,
  uop_reciprocal,
  uop_round,
  uop_sawtooth,
  uop_flatten_sawtooth,
} uop_t;

static uop_t
get_uop(
    const char * const str_uop
    )
{
  if ( strcmp(str_uop, "abs") == 0 ) { return uop_abs; } 
  if ( strcmp(str_uop, "bitwise_not") == 0 ) { return uop_bitwise_not; } 
  if ( strncmp(str_uop, "convert:", strlen("convert:")) == 0 ) { 
    return uop_convert; 
  }
  if ( strcmp(str_uop, "exp") == 0 ) { return uop_exp; } 
  if ( strcmp(str_uop, "log") == 0 ) { return uop_log; } 
  if ( strcmp(str_uop, "logical_not") == 0 ) { return uop_logical_not; } 
  if ( strcmp(str_uop, "reciprocal") == 0 ) { return uop_reciprocal; } 
  if ( strcmp(str_uop, "round") == 0 ) { return uop_round; } 
  if ( strcmp(str_uop, "sawtooth") == 0 ) { return uop_sawtooth; } 
  if ( strcmp(str_uop, "flatten_sawtooth") == 0 ) { return uop_flatten_sawtooth; } 

  return uop_undef;
}
// dst:= op(src) src can be == dst for in_situ == true 
//START_FUNC_DECL
int
v_op(
    QDF_REC_TYPE * ptr_src, // input numeric array 
    const char * const str_uop,
    QDF_REC_TYPE *ptr_dst // output numeric array 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  //==============================
  mcr_chk_non_null(ptr_src, -1); 
  char *sx = ptr_src->data; 
  jtype_t sjtype = get_jtype(sx); 
  if ( sjtype != j_array ) {  go_BYE(-1); }
  uint32_t sn  = get_arr_len(sx); 
  uint32_t ssz = get_arr_size(sx); 
  qtype_t sqtype = get_qtype(sx); 
  const void * const srcptr = get_arr_ptr(sx);
  //==============================
  bool in_situ = false;
  if ( ptr_src == ptr_dst ) { 
    in_situ = true;
  }
  else {
    mcr_chk_null(ptr_dst, -1);
  }
  //==============================
  uop_t uop = get_uop(str_uop); if ( uop == uop_undef ) { go_BYE(-1); } 
  // destination type same as source type except for a few operators
  qtype_t dqtype = sqtype; // type conversion not handled

  if ( uop  == uop_convert ) { 
    const char *cptr = str_uop + strlen("convert:");
    dqtype = get_c_qtype(cptr); 
    // Can convert in place only if widths match 
    uint32_t swidth = get_width_c_qtype(sqtype);
    uint32_t dwidth = get_width_c_qtype(dqtype);
    if ( swidth != dwidth ) { 
      if ( ptr_src == ptr_dst ) { go_BYE(-1);
      }
    }
  }
  if ( uop == uop_logical_not ) { 
    dqtype = I1;
  }
  if ( dqtype == Q0 ) { go_BYE(-1); } 
  //--------------------------------------------------
  if ( !in_situ ) { 
    switch ( uop ) { 
      case uop_abs : case uop_bitwise_not : case uop_convert : 
      case uop_exp : case uop_log : case uop_reciprocal : case uop_round : 
      case uop_sawtooth : case uop_flatten_sawtooth : 
        status = make_num_array(NULL, sn, ssz, dqtype, ptr_dst); 
        cBYE(status);
        break;
      case uop_logical_not  :
        status = make_num_array(NULL, sn, ssz, I1, ptr_dst); cBYE(status);
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  else { // check if in situ is possible
    switch ( uop ) { 
      case uop_logical_not :
      if ( dqtype != I1 ) { go_BYE(-1); }
      default : 
      // nothing to check
      break;
    }
  }

  const float  * const src_F4ptr = (const float  * const)get_arr_ptr(sx);
  const double * const src_F8ptr = (const double * const)get_arr_ptr(sx); 

  void *dstptr = get_arr_ptr(ptr_dst->data); 
  float  *dst_F4ptr = (float  *)dstptr;
  double *dst_F8ptr = (double *)dstptr;
  //
  // TODO P4 Following code should be auto-generated 
  switch ( uop ) { 
    case uop_sawtooth : 
      status = sawtooth(srcptr, sn, sqtype, dstptr); cBYE(status);
      break;
    //------------------------------------------------------------
    case uop_flatten_sawtooth : 
      status = flatten_sawtooth(srcptr, sn, sqtype, dstptr); cBYE(status);
      break;
    //------------------------------------------------------------
    case uop_abs : 
      switch ( sqtype ) { 
        case I1 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((int8_t *)dstptr)[i] = 
              (int8_t)abs(((const int8_t * const )srcptr)[i]);
          }
          break;
        case I2 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((int16_t *)dstptr)[i] = 
              (int16_t)abs(((const int16_t * const )srcptr)[i]);
          }
          break;
        case I4 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((int32_t *)dstptr)[i] = 
              (int32_t)abs(((const int32_t * const )srcptr)[i]);
          }
          break;
        case I8 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((int64_t *)dstptr)[i] = 
              (int64_t)llabs(((const int64_t * const )srcptr)[i]);
          }
          break;
        case F4 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((float *)dstptr)[i] = 
              (float)fabsf(((const float * const )srcptr)[i]);
          }
          break;
        case F8 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((double *)dstptr)[i] = 
              (double)fabs(((const double * const )srcptr)[i]);
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      break;
    //------------------------------------------------------------
    case uop_bitwise_not : 
      switch ( sqtype ) { 
        case I1 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint8_t *)dstptr)[i] = 
              (uint8_t)~(((const uint8_t * const )srcptr)[i]);
          }
          break;
        case I2 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint16_t *)dstptr)[i] = 
              (uint16_t)~(((const uint16_t * const )srcptr)[i]);
          }
          break;
        case I4 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint32_t *)dstptr)[i] = 
              (uint32_t)~(((const uint32_t * const )srcptr)[i]);
          }
          break;
        case I8 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint64_t *)dstptr)[i] = 
              (uint64_t)~(((const uint64_t * const )srcptr)[i]);
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      break;
    //------------------------------------------------------------
    case uop_logical_not : 
      switch ( sqtype ) { 
        case I1 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint8_t *)dstptr)[i] = 
              !(((const uint8_t * const )srcptr)[i]);
          }
          break;
        case I2 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint8_t *)dstptr)[i] = 
              !(((const uint16_t * const )srcptr)[i]);
          }
          break;
        case I4 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint8_t *)dstptr)[i] = 
              !(((const uint32_t * const )srcptr)[i]);
          }
          break;
        case I8 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((uint8_t *)dstptr)[i] = 
              !(((const uint64_t * const )srcptr)[i]);
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      break;
    //------------------------------------------------------------
    case uop_log : 
      switch ( sqtype ) { 
        case F4 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            ((float *)dstptr)[i] = logf(((const float * const)srcptr)[i]);
          }
          break;
        case F8 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            dst_F8ptr[i] = log(((const double * const)srcptr)[i]);
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      break;
      //-------------------------------------
    case uop_reciprocal : 
      switch ( sqtype ) {
        case F4 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            dst_F4ptr[i] = (float)1.0 / src_F4ptr[i];
          }
          break;
        case F8 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            dst_F8ptr[i] = 1.0 / src_F8ptr[i];
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      break;
      //-------------------------------------
    case uop_exp : 
      switch ( sqtype ) {
        case F4 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            dst_F4ptr[i] = expf(src_F4ptr[i]);
          }
          break;
        case F8 :
          for ( uint32_t i = 0; i < sn; i++ ) {
            dst_F8ptr[i] = exp(src_F8ptr[i]);
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      break;
      //-------------------------------------
    case uop_round : 
      switch ( dqtype ) { 
        case F4 : 
          for ( uint32_t i = 0; i < sn; i++ ) {
            dst_F4ptr[i] = roundf(src_F4ptr[i]);
          }
          break;
        case F8 : 
          for ( uint32_t i = 0; i < sn; i++ ) {
            dst_F8ptr[i] = round(src_F8ptr[i]);
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      break; 
      //-------------------------------------
    case uop_convert : 
      switch ( sqtype ) { 
        case F8 :
          switch ( dqtype ) { 
            case I1 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int8_t *)dstptr)[i] = (int8_t)((const double * const)srcptr)[i];
              }
              break;
            case I2 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int16_t *)dstptr)[i] = (int16_t)((const double * const)srcptr)[i];
              }
              break;
            case I4 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int32_t *)dstptr)[i] = (int32_t)((const double * const)srcptr)[i];
              }
              break;
            case I8 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int64_t *)dstptr)[i] = (int64_t)((const double * const)srcptr)[i];
              }
              break;
            case F4 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((float *)dstptr)[i] = (float)((const double * const)srcptr)[i];
              }
              break;
            case F8 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((double *)dstptr)[i] = ((const double * const)srcptr)[i];
              }
              break;
            default : 
              mcr_conv_error(sqtype, dqtype); go_BYE(-1);
              break;
          }
          break; 
        case F4 :
          switch ( dqtype ) { 
            case I4 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int32_t *)dstptr)[i] = (int32_t)((const  float * const)srcptr)[i];
              }
              break;
            default : 
              go_BYE(-1);
              break;
          }
          break;
        case I4 :
          switch ( dqtype ) { 
            case F4 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((float *)dstptr)[i] = (float)((const int32_t * const)srcptr)[i];
              } 
              break;
            case I8 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int64_t *)dstptr)[i] = (int64_t)((const int32_t * const)srcptr)[i];
              } 
              break;
            default : 
              go_BYE(-1);
              break;
          }
          break;
        case I1 :
          switch ( dqtype ) { 
            case I8 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int64_t *)dstptr)[i] = ((const int8_t * const)srcptr)[i];
              } 
              break;
            default : 
              go_BYE(-1);
              break;
          }
          break;
        case I2 :
          switch ( dqtype ) { 
            case I8 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int64_t *)dstptr)[i] = ((const int16_t * const)srcptr)[i];
              } 
              break;
            default : 
              go_BYE(-1);
              break;
          }
          break;
        case I8 :
          switch ( dqtype ) { 
            case I4 : 
              for ( uint32_t i = 0; i < sn; i++ ) {
                ((int32_t *)dstptr)[i] = (int32_t)((const int64_t * const)srcptr)[i];
              } 
              break;
            default : 
              mcr_conv_error(sqtype, dqtype); go_BYE(-1);
              break;
          }
          break;
        default : 
              mcr_conv_error(sqtype, dqtype); go_BYE(-1);
          break;
      }
      break;
      //-------------------------------------
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
