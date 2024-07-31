// Start at 0, every time you see the "on" value, increment by "incr" value
#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_grp_sawtooth.h"
// Operates like sawtooth but instead of operating on all n 
// elements of src in one shot, it operates on it in nG groups
int
qdf_grp_sawtooth(
    const QDF_REC_TYPE *const ptr_src, // input
    const QDF_REC_TYPE *const ptr_n_grp, // input
    const char * const str_dqtype, // input 
    QDF_REC_TYPE * restrict ptr_dst // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  mcr_chk_null(ptr_dst, -1); 
  //==============================
  mcr_chk_non_null(ptr_src, -1); 
  char *sdata = ptr_src->data; 
  jtype_t sjtype = get_jtype(sdata); 
  if ( sjtype != j_array ) {  go_BYE(-1); }
  uint32_t sn  = get_arr_len(sdata); 
  uint32_t ssz = get_arr_size(sdata); 
  qtype_t sqtype = get_qtype(sdata); 
  const char * const sx = get_arr_ptr(sdata);
  //==============================
  mcr_chk_non_null(ptr_n_grp, -1); 
  char *gdata = ptr_n_grp->data; 
  jtype_t gjtype = get_jtype(gdata); 
  if ( gjtype != j_array ) {  go_BYE(-1); }
  qtype_t gqtype = get_qtype(gdata); 
  if ( gqtype != I4 ) {  go_BYE(-1); }
  uint32_t gn  = get_arr_len(gdata);  
  if ( gn == 0 ) { go_BYE(-1); } 
  const int32_t * const n_grp = (const int32_t * const) get_arr_ptr(gdata);
#ifdef DEBUG
  uint32_t sum = 0;
  for ( uint32_t i = 0; i < gn; i++ ) { 
    if ( n_grp[i] <= 0 ) { go_BYE(-1); } 
    sum += (uint32_t)n_grp[i];
  }
  if ( sum != sn ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < sn; i++ ) { 
    if ( ((const int8_t * const)sx)[i] < 0 ) { go_BYE(-1); } 
  }
#endif
  //==============================

  qtype_t dqtype = sqtype; 
  if ( str_dqtype != NULL ) { 
    dqtype = get_c_qtype(str_dqtype); 
    if ( dqtype == Q0 ) { go_BYE(-1); }
  }
  status = make_num_array(NULL, sn, ssz, sqtype, ptr_dst); cBYE(status);
  char *dx  = get_arr_ptr(ptr_dst->data);
  switch ( sqtype ) { 
    case I1 : 
      switch ( dqtype ) { 
        case I1 : 
          status = grp_sawtooth_I1_I1((const int8_t * const)sx, sn,
              n_grp, gn, (int8_t *) dx); 
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
  cBYE(status);
BYE:
  return status;
}
int
grp_sawtooth_I1_I1(
    const int8_t *const s, // [sn] 
    uint32_t sn,
    const int32_t * const n_grp, // [gn]
    uint32_t gn,
    int8_t *d // [sn]
    )
{
  int status = 0;
  if ( s == NULL ) { go_BYE(-1); } 
  if ( d == NULL ) { go_BYE(-1); } 
  if ( sn == 0 ) { go_BYE(-1); }
  if ( gn == 0 ) { go_BYE(-1); }

  uint32_t lb = 0; uint32_t ub = 0;
  for ( uint32_t gidx = 0; gidx < gn; gidx++ ) {
    if ( n_grp[gidx] <= 0 ) { go_BYE(-1); }
    ub = lb + (uint32_t)n_grp[gidx];
    int8_t prev_d = 0;
    if ( ub > sn ) { go_BYE(-1); } 
    for ( uint32_t i = lb; i < ub; i++ ) { 
      if ( s[i] == 0 ) { 
        d[i] = 0;
      }
      else {
        d[i] = (int8_t)(prev_d + s[i]);
      }
      prev_d = d[i];
    }
    lb = ub;
  }
BYE:
  return status;
}
