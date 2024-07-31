#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_ifxthenyelsez.h"
//START_FUNC_DECL
int
ifxthenyelsez(
    const QDF_REC_TYPE * const x, // "x" input numeric array 
    const QDF_REC_TYPE * const y, // "y" input numeric array 
    const QDF_REC_TYPE * const z, // "z" input numeric array 
    QDF_REC_TYPE *ptr_dst // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  //==============================
  mcr_chk_non_null(x, -1); 
  void *xdata = x->data; 
  jtype_t xjtype = get_jtype(xdata); 
  if ( xjtype != j_array ) {  go_BYE(-1); }
  qtype_t xqtype = get_qtype(xdata); 
  if ( xqtype != I1 ) { go_BYE(-1); }
  uint32_t nx    = get_arr_len(xdata); 
  //==============================
  mcr_chk_non_null(y, -1); 
  void *ydata = y->data; 
  jtype_t yjtype = get_jtype(ydata); 
  if ( yjtype != j_array ) {  go_BYE(-1); }
  qtype_t yqtype = get_qtype(ydata); 
  if ( yqtype == Q0 ) {  go_BYE(-1); }
  uint32_t ny    = get_arr_len(ydata); 
  uint32_t ywidth = get_arr_width(ydata);
  uint32_t szy    = get_arr_size(ydata);
  if ( nx != ny ) { go_BYE(-1); }
  //==============================
  mcr_chk_non_null(z, -1); 
  void *zdata = z->data; 
  jtype_t zjtype = get_jtype(zdata); 
  if ( zjtype != j_array ) {  go_BYE(-1); }
  qtype_t zqtype = get_qtype(zdata); 
  uint32_t nz    = get_arr_len(zdata); 
  uint32_t zwidth = get_arr_width(zdata);
  if ( ny != nz ) { go_BYE(-1); }
  if ( ywidth != zwidth ) { go_BYE(-1); }
  if ( yqtype != zqtype ) { go_BYE(-1); } 
  //==============================
  // TODO P3 We are not yet set up to handle type promotion
  mcr_chk_null(ptr_dst, -1); 
  status = make_num_array(NULL, ny, szy, yqtype, ptr_dst);  cBYE(status);
  char *dstptr = get_arr_ptr(ptr_dst->data); 
  //==============================
  const uint8_t * const xptr = (const uint8_t * const)get_arr_ptr(xdata); 
  const char    * yptr = (const char    * )get_arr_ptr(ydata); 
  const char    * zptr = (const char    * )get_arr_ptr(zdata); 

  for ( uint32_t i = 0; i < ny; i++ ) {
    if ( xptr[i] == 1 ) { 
      // leave dst unchanged 
      memcpy(dstptr, yptr, ywidth);
    }
    else if ( xptr[i] == 0 ) { 
      memcpy(dstptr, zptr, zwidth);
    }
    else {
      go_BYE(-1);
    }
    yptr += ywidth;
    zptr += zwidth;
    dstptr += zwidth;
  }
BYE:
  return status;
}
//START_FUNC_DECL
int
ifxthenyelsez_sclr(
    const QDF_REC_TYPE * const x, // "x" input numeric array 
    const QDF_REC_TYPE * const y, // "y" input numeric array 
    const SCLR_REC_TYPE * const z, // "z" input scalar
    QDF_REC_TYPE *ptr_dst // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  //==============================
  mcr_chk_non_null(x, -1); 
  void *xdata = x->data; 
  jtype_t xjtype = get_jtype(xdata); 
  if ( xjtype != j_array ) {  go_BYE(-1); }
  uint32_t nx    = get_arr_len(xdata); 
  qtype_t xqtype = get_qtype(xdata); 
  if ( xqtype != I1 ) { go_BYE(-1); }
  //==============================
  mcr_chk_non_null(y, -1); 
  void *ydata = y->data; 
  jtype_t yjtype = get_jtype(ydata); 
  if ( yjtype != j_array ) {  go_BYE(-1); }
  uint32_t ny     = get_arr_len(ydata); 
  qtype_t yqtype  = get_qtype(ydata); 
  uint32_t ywidth = get_arr_width(ydata);
  uint32_t szy    = get_arr_size(ydata);
  if ( nx != ny ) { go_BYE(-1); }
  //==============================
  // TODO P3 We are not yet set up to handle type promotion
  status = make_num_array(NULL, ny, szy, yqtype, ptr_dst);  cBYE(status);
  char *dstptr = get_arr_ptr(ptr_dst->data); 
  //==============================
  const uint8_t * const xptr = (const uint8_t * const)get_arr_ptr(xdata); 
  const char * yptr = (const char * )get_arr_ptr(ydata); 

  for ( uint32_t i = 0; i < ny; i++ ) {
    if ( xptr[i] == 1 ) { 
      // leave dst unchanged 
      memcpy(dstptr, yptr, ywidth);
    }
    else if ( xptr[i] == 0 ) { 
      memcpy(dstptr, &(z->val), ywidth);
    }
    else {
      go_BYE(-1);
    }
    yptr   += ywidth;
    dstptr += ywidth;
  }
BYE:
  return status;
}
