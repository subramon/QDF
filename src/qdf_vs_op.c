#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_vs_op.h"
//--------------------------------------
#include "vsadd.h"
#include "vssub.h"
#include "vsmul.h"
#include "vsdiv.h"
#include "vsrem.h"
//--------------------------------------
#include "vseq.h"
#include "vsneq.h"
#include "vslt.h"
#include "vsleq.h"
#include "vsgt.h"
#include "vsgeq.h"
//--------------------------------------
#include "vsbitwise_and.h"
#include "vsbitwise_or.h"
#include "vsbitwise_xor.h"
//--------------------------------------
#include "vsmin.h"
#include "vsmax.h"
//--------------------------------------
#include "vslogical_and.h"
#include "vslogical_or.h"
//--------------------------------------
#include "vsshift_left.h"
#include "vsshift_right.h"
//--------------------------------------
#include "vsraised_to.h"

typedef enum {
  bop_undef,
  bop_vsadd,
  bop_vssub,
  bop_vsmul,
  bop_vsdiv,
  bop_vsrem,

  bop_vsbitwise_and,
  bop_vsbitwise_or,
  bop_vsbitwise_xor,

  bop_vsmin,
  bop_vsmax,

  bop_vseq,
  bop_vsneq,
  bop_vslt,
  bop_vsleq,
  bop_vsgt,
  bop_vsgeq,

  bop_vslogical_and,
  bop_vslogical_or,

  bop_vsshift_left,
  bop_vsshift_right,

  bop_vsraised_to,
} bop_t;

static bop_t
get_bop(
    const char * const str_bop
    )
{
  if ( strcmp(str_bop, "add") == 0 ) { return bop_vsadd; } 
  if ( strcmp(str_bop, "sub") == 0 ) { return bop_vssub; } 
  if ( strcmp(str_bop, "mul") == 0 ) { return bop_vsmul; } 
  if ( strcmp(str_bop, "div") == 0 ) { return bop_vsdiv; } 
  if ( strcmp(str_bop, "rem") == 0 ) { return bop_vsrem; } 

  if ( strcmp(str_bop, "eq" ) == 0 ) { return bop_vseq; } 
  if ( strcmp(str_bop, "neq") == 0 ) { return bop_vsneq; } 
  if ( strcmp(str_bop, "lt" ) == 0 ) { return bop_vslt; } 
  if ( strcmp(str_bop, "leq") == 0 ) { return bop_vsleq; } 
  if ( strcmp(str_bop, "gt" ) == 0 ) { return bop_vsgt; } 
  if ( strcmp(str_bop, "geq") == 0 ) { return bop_vsgeq; } 

  if ( strcmp(str_bop, "bitwise_and" ) == 0 ) { return bop_vsbitwise_and; } 
  if ( strcmp(str_bop, "bitwise_or"  ) == 0 ) { return bop_vsbitwise_or; } 
  if ( strcmp(str_bop, "bitwise_xor" ) == 0 ) { return bop_vsbitwise_xor; } 

  if ( strcmp(str_bop, "min" ) == 0 ) { return bop_vsmin; } 
  if ( strcmp(str_bop, "max" ) == 0 ) { return bop_vsmax; } 

  if ( strcmp(str_bop, "logical_and" ) == 0 ) { return bop_vslogical_and; } 
  if ( strcmp(str_bop, "logical_or"  ) == 0 ) { return bop_vslogical_or; } 

  if ( strcmp(str_bop, "shift_left" ) == 0 ) { return bop_vsshift_left; } 
  if ( strcmp(str_bop, "shift_right"  ) == 0 ) { return bop_vsshift_right; } 
  if ( strcmp(str_bop, "raised_to"  ) == 0 ) { return bop_vsraised_to; } 

  return bop_undef;
}

//START_FUNC_DECL
// it is possible that ptr_x == ptr_dst. This is "in situ" use case
int
vs_op(
    QDF_REC_TYPE * ptr_x, // input numeric array 
    const SCLR_REC_TYPE * const ptr_sclr, // input scalar
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
  // NOTE: We are not set up to handle type promotion
  if ( xqtype != ptr_sclr->qtype ) { go_BYE(-1); } 
  //==============================
  bop_t bop = get_bop(str_bop); if ( bop == bop_undef ) { go_BYE(-1); } 
  //==============================
  if ( ptr_dst == ptr_x ) { in_situ = true; } 
  // allocate space for output for z:= x op y 
  if ( !in_situ ) {
    mcr_chk_null(ptr_dst, -1); 
    switch ( bop ) {
      case bop_vsadd : case bop_vssub : case bop_vsmul : 
      case bop_vsdiv : case bop_vsrem : 
      case bop_vsbitwise_and : case bop_vsbitwise_or : 
      case bop_vsbitwise_xor : 
      case bop_vsmin : case bop_vsmax : 
      case bop_vsraised_to : 
      case bop_vsshift_left : case bop_vsshift_right : 
        status = make_num_array(NULL, sn1, ssz1, xqtype, ptr_dst);  
        break;
      case bop_vseq : case bop_vsneq : 
      case bop_vslt : case bop_vsleq : 
      case bop_vsgt : case bop_vsgeq :
      case bop_vslogical_and : case bop_vslogical_or : 
        status = make_num_array(NULL, sn1, ssz1, I1, ptr_dst);  
        break;
      default : 
        fprintf(stderr, "Unknown operator %s \n", str_bop);
        go_BYE(-1);
        break;
    }
  }
  else { // NO allocation for for x:= x op y 
    // Check if in situ is possible
    switch( bop ) { 
      case bop_vseq : case bop_vsneq : 
      case bop_vslt : case bop_vsleq : 
      case bop_vsgt : case bop_vsgeq :
      case bop_vslogical_and : case bop_vslogical_or : 
        if ( xqtype != I1 ) { go_BYE(-1); } 
      default : 
        // nothing to check 
        break;
    }
  }
  char *zptr = get_arr_ptr(ptr_dst->data); 
  if ( zptr == NULL ) { go_BYE(-1); }

  switch ( bop ) {
    case bop_vsadd : 
      status = vsadd(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vssub : 
      zptr[0] = 0;
      status = vssub(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vsmul : 
      status = vsmul(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vsdiv : 
      status = vsdiv(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vsrem : 
      status = vsrem(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
      //-----------------------------------------------------
    case bop_vsmax : 
      status = vsmax(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vsmin : 
      status = vsmin(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
      //-----------------------------------------------------
    case bop_vsbitwise_and : 
      status = vsbitwise_and(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vsbitwise_or : 
      status = vsbitwise_or(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break; 
    case bop_vsbitwise_xor : 
      status = vsbitwise_xor(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break; 
      //-----------------------------------------------------
    case bop_vslogical_and : 
      status = vslogical_and(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vslogical_or : 
      status = vslogical_or(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break; 
      //-----------------------------------------------------
    case bop_vseq : 
      status = vseq(xptr, ptr_sclr, sn1, (int8_t *)zptr); cBYE(status);
      break; 
    case bop_vsneq : 
      status = vsneq(xptr, ptr_sclr, sn1, (int8_t *)zptr); cBYE(status);
      break; 
    case bop_vslt : 
      status = vslt(xptr, ptr_sclr, sn1, (int8_t *)zptr); cBYE(status);
      break; 
    case bop_vsleq : 
      status = vsleq(xptr, ptr_sclr, sn1, (int8_t *)zptr); cBYE(status);
      break; 
    case bop_vsgt : 
      status = vsgt(xptr, ptr_sclr, sn1, (int8_t *)zptr); cBYE(status);
      break;
    case bop_vsgeq : 
      status = vsgeq(xptr, ptr_sclr, sn1, (int8_t *)zptr); cBYE(status);
      break; 
      //-----------------------------------------------------
    case bop_vsshift_left : 
      status = vsshift_left(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vsshift_right : 
      status = vsshift_right(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    case bop_vsraised_to : 
      status = vsraised_to(xptr, ptr_sclr, sn1, zptr); cBYE(status);
      break;
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
