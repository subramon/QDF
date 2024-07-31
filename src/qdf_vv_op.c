#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_vv_op.h"
//--------------------------------------
#include "vvadd.h"
#include "vvsub.h"
#include "vvmul.h"
#include "vvdiv.h"
#include "vvrem.h"
//--------------------------------------
#include "vveq.h"
#include "vvneq.h"
#include "vvlt.h"
#include "vvleq.h"
#include "vvgt.h"
#include "vvgeq.h"
//--------------------------------------
#include "vvbitwise_and.h"
#include "vvbitwise_or.h"
#include "vvbitwise_xor.h"
//--------------------------------------
#include "vvmin.h"
#include "vvmax.h"
//--------------------------------------
#include "vvlogical_and.h"
#include "vvlogical_or.h"

typedef enum {
  bop_undef,
  bop_vvadd,
  bop_vvsub,
  bop_vvmul,
  bop_vvdiv,
  bop_vvrem,

  bop_vvmin,
  bop_vvmax,

  bop_vvbitwise_and,
  bop_vvbitwise_or,
  bop_vvbitwise_xor,

  bop_vveq,
  bop_vvneq,
  bop_vvlt,
  bop_vvleq,
  bop_vvgt,
  bop_vvgeq,

  bop_vvlogical_and,
  bop_vvlogical_or,
} bop_t;

static bop_t
get_bop(
    const char * const str_bop
    )
{
  if ( strcmp(str_bop, "add") == 0 ) { return bop_vvadd; } 
  if ( strcmp(str_bop, "sub") == 0 ) { return bop_vvsub; } 
  if ( strcmp(str_bop, "mul") == 0 ) { return bop_vvmul; } 
  if ( strcmp(str_bop, "div") == 0 ) { return bop_vvdiv; } 
  if ( strcmp(str_bop, "rem") == 0 ) { return bop_vvrem; } 

  if ( strcmp(str_bop, "eq" ) == 0 ) { return bop_vveq; } 
  if ( strcmp(str_bop, "neq") == 0 ) { return bop_vvneq; } 
  if ( strcmp(str_bop, "lt" ) == 0 ) { return bop_vvlt; } 
  if ( strcmp(str_bop, "leq") == 0 ) { return bop_vvleq; } 
  if ( strcmp(str_bop, "gt" ) == 0 ) { return bop_vvgt; } 
  if ( strcmp(str_bop, "geq") == 0 ) { return bop_vvgeq; } 

  if ( strcmp(str_bop, "min" ) == 0 ) { return bop_vvmin; } 
  if ( strcmp(str_bop, "max"  ) == 0 ) { return bop_vvmax; } 

  if ( strcmp(str_bop, "bitwise_and" ) == 0 ) { return bop_vvbitwise_and; } 
  if ( strcmp(str_bop, "bitwise_or"  ) == 0 ) { return bop_vvbitwise_or; } 
  if ( strcmp(str_bop, "bitwise_xor" ) == 0 ) { return bop_vvbitwise_xor; } 

  if ( strcmp(str_bop, "logical_and" ) == 0 ) { return bop_vvlogical_and; } 
  if ( strcmp(str_bop, "logical_or"  ) == 0 ) { return bop_vvlogical_or; } 

  return bop_undef;
}

//START_FUNC_DECL
// it is possible that ptr_x == ptr_dst. This is "in situ" use case
int
vv_op(
    QDF_REC_TYPE * ptr_x, // input numeric array 
    const QDF_REC_TYPE * const ptr_y, // input numeric array 
    const char * const str_bop,
    QDF_REC_TYPE *ptr_dst // output numeric array 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  bool in_situ = false;
  //==============================
  mcr_chk_non_null(ptr_x, -1); 
  char *xdata = ptr_x->data; 
  jtype_t sjtype1 = get_jtype(xdata); 
  if ( sjtype1 != j_array ) {  go_BYE(-1); }
  uint32_t sn1  = get_arr_len(xdata); 
  uint32_t ssz1 = get_arr_size(xdata); 
  qtype_t xqtype = get_qtype(xdata); 
  char *xptr = get_arr_ptr(xdata); 
  //==============================
  mcr_chk_non_null(ptr_y, -2); 
  char *ydata = ptr_y->data; 
  jtype_t sjtype2 = get_jtype(ydata); 
  if ( sjtype2 != j_array ) {  go_BYE(-1); }
  uint32_t sn2  = get_arr_len(ydata); 
  qtype_t yqtype = get_qtype(ydata); 
  char *yptr = get_arr_ptr(ydata); 
  //==============================
  if ( sn1 != sn2 ) { go_BYE(-1); }
  // NOTE: We are not set up to handle type promotion
  if ( xqtype != yqtype ) { go_BYE(-1); } 
  //==============================
  bop_t bop = get_bop(str_bop); if ( bop == bop_undef ) { go_BYE(-1); } 
  //==============================
  if ( ptr_dst == ptr_x ) { in_situ = true; } 
  // allocate space for output for z:= x op y 
  if ( !in_situ ) {
    mcr_chk_null(ptr_dst, -1); 
    switch ( bop ) {
      case bop_vvadd : case bop_vvsub : case bop_vvmul : 
      case bop_vvdiv : case bop_vvrem : 
      case bop_vvbitwise_and : case bop_vvbitwise_or : 
      case bop_vvbitwise_xor : 
      case bop_vvmin : case bop_vvmax : 
        status = make_num_array(NULL, sn1, ssz1, xqtype, ptr_dst);  
        break;
      case bop_vveq : case bop_vvneq : 
      case bop_vvlt : case bop_vvleq : 
      case bop_vvgt : case bop_vvgeq :
      case bop_vvlogical_and : case bop_vvlogical_or : 
        status = make_num_array(NULL, sn1, ssz1, I1, ptr_dst);  
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  else { // NO allocation for for x:= x op y 
    // Check if in situ is possible
    switch( bop ) { 
      case bop_vveq : case bop_vvneq : 
      case bop_vvlt : case bop_vvleq : 
      case bop_vvgt : case bop_vvgeq :
      case bop_vvlogical_and : case bop_vvlogical_or : 
        if ( xqtype != I1 ) { go_BYE(-1); } 
      default : 
        // nothing to check 
        break;
    }
  }
  char *zptr = get_arr_ptr(ptr_dst->data); 
  if ( zptr == NULL ) { go_BYE(-1); }

  switch ( bop ) {
    case bop_vvadd : 
      status = vvadd(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
    case bop_vvsub : 
      status = vvsub(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
    case bop_vvmul : 
      status = vvmul(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
    case bop_vvdiv : 
      status = vvdiv(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
    case bop_vvrem : 
      status = vvrem(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
      //-----------------------------------------------------
    case bop_vvbitwise_and : 
      status = vvbitwise_and(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
    case bop_vvbitwise_or : 
      status = vvbitwise_or(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
    case bop_vvbitwise_xor : 
      status = vvbitwise_xor(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
      //-----------------------------------------------------
    case bop_vvmin : 
      status = vvmin(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
    case bop_vvmax : 
      status = vvmax(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
      //-----------------------------------------------------
    case bop_vvlogical_and : 
      status = vvlogical_and(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
    case bop_vvlogical_or : 
      status = vvlogical_or(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
      //-----------------------------------------------------
    case bop_vveq : 
      status = vveq(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
    case bop_vvneq : 
      status = vvneq(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
    case bop_vvlt : 
      status = vvlt(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
    case bop_vvleq : 
      status = vvleq(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
    case bop_vvgt : 
      status = vvgt(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break;
    case bop_vvgeq : 
      status = vvgeq(xptr, yptr, sn1, xqtype, zptr); cBYE(status);
      break; 
      //-----------------------------------------------------
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
