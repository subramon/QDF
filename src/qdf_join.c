/* Important to have the following mental picture in mind when reading 
 * this code and understanding this simple join algorithm.
 * We assume data is sorted in both source and destination
 * To use terminology below, 
 tkd[i] >= tkd[i-1] // not that keys in destination need not be unique 
 fkd[i] >= fkd[i-1] // not that keys in source      need not be unique 
 
 One might be tempted to consider that 
   (tkd[i], tvd[i]) are a key value pair. That is not true. Instead
   (tkd[i], tvd[tid[i]]) are a key value pair. 

   Note that we create a nn field for the destination value
   column that is created. So, if there is a key on the dest side 
   which is not on the source side, then the value is set to 0 
   and the nn column gets set to 0 
   */
#include "incs.h"
#include "qdf_struct.h"
#include "unique.h"
#include "qdf_external.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_join.h"
#ifdef DEBUG
#include "qdf_checkers.h"
#endif

#define SLOW_DEBUG // Use with great care. Slows things down a lot 

static int
qdf_access(
    QDF_REC_TYPE *ptr_qdf,
    void **ptr_data,
    qtype_t *ptr_qtype,
    uint32_t *ptr_n
    )
{
  int status = 0;
  //----------------------------------------------------------------
  char *x = ptr_qdf->data; if ( x == NULL ) { go_BYE(-1); }
  jtype_t jtype = get_jtype(x); 
  if ( jtype != j_array ) { go_BYE(-1); }
  *ptr_qtype = get_qtype(x); 
  *ptr_n     = get_arr_len(x); 
  *ptr_data  = get_arr_ptr(x);
BYE:
  return status;
}
  //----------------------------------------------------------------

// We are currently vrey restrictive on what types that join understands
int 
qdf_join(
    QDF_REC_TYPE *ptr_from_key,
    QDF_REC_TYPE *ptr_from_idx,
    QDF_REC_TYPE *ptr_from_val,

    QDF_REC_TYPE *ptr_to_key,
    QDF_REC_TYPE *ptr_to_idx,
    QDF_REC_TYPE *ptr_to_val,
    QDF_REC_TYPE *ptr_nn_to_val
    ) 
{
  int status = 0;
  int32_t * copy_fidI4 = NULL;

  if ( ptr_from_key  == NULL ) { go_BYE(-1); }
  if ( ptr_from_idx == NULL ) { go_BYE(-1); }
  if ( ptr_from_val == NULL ) { go_BYE(-1); }

  if ( ptr_to_key  == NULL ) { go_BYE(-1); }
  if ( ptr_to_idx == NULL ) { go_BYE(-1); }
  if ( ptr_to_val == NULL ) { go_BYE(-1); }
  if ( ptr_nn_to_val == NULL ) { go_BYE(-1); }
  
  // fkd = from  key data pointer
  void *fkd; qtype_t fkqtype; uint32_t fkn; // ptr to data in "from" key 
  // fid = from index data pointer
  void *fid; qtype_t fiqtype; uint32_t fin; // ptr to data in "from" index
  // fvd = from value data pointer
  void *fvd; qtype_t fvqtype; uint32_t fvn; // ptr to data in "from" value
  status = qdf_access(ptr_from_key, &fkd, &fkqtype, &fkn); cBYE(status); 
  status = qdf_access(ptr_from_idx, &fid, &fiqtype, &fin); cBYE(status); 
  status = qdf_access(ptr_from_val, &fvd, &fvqtype, &fvn); cBYE(status); 
  if ( fkn != fin ) { go_BYE(-1); } 
  if ( fkn != fvn ) { go_BYE(-1); } 
  if ( fkqtype != I4 ) { go_BYE(-1); } // TODO P4 current limitation
  if ( fiqtype != I4 ) { go_BYE(-1); } // TODO P4 current limitation
  // TODO P4 current limitation
  if ( ! (( fvqtype == I4 ) || ( fvqtype == F4 ) )) { go_BYE(-1); }
#ifdef DEBUG
#endif

  //----------------------------------------------------------------
  void *tkd; qtype_t tkqtype; uint32_t tkn; // ptr to data in "to" key 
  void *tid; qtype_t tiqtype; uint32_t tin; // ptr to data in "to" index
  void *tvd; qtype_t tvqtype; uint32_t tvn; // ptr to data in "to" value
  void *nn_tvd; qtype_t nn_tvqtype; uint32_t nn_tvn; // ptr to data in "to" nn value
  status = qdf_access(ptr_to_key, &tkd, &tkqtype, &tkn); cBYE(status); 
  status = qdf_access(ptr_to_idx, &tid, &tiqtype, &tin); cBYE(status); 
  status = qdf_access(ptr_to_val, &tvd, &tvqtype, &tvn); cBYE(status); 
  status = qdf_access(ptr_nn_to_val, &nn_tvd, &nn_tvqtype, &nn_tvn); cBYE(status); 
  if ( tkn != tin ) { go_BYE(-1); } 
  if ( tkn != tvn ) { go_BYE(-1); } 
  if ( tkn != nn_tvn ) { go_BYE(-1); } 
  if ( tkqtype != I4 ) { go_BYE(-1); } // TODO P4 current limitation
  if ( tiqtype != I4 ) { go_BYE(-1); } // TODO P4 current limitation
  // TODO P4 current limitation
  if ( !(( tvqtype == I4 ) || ( tvqtype == F4 ))) { go_BYE(-1); }
  if ( fvqtype != tvqtype ) { go_BYE(-1); }
  if ( nn_tvqtype != I1 ) { go_BYE(-1); } 
  uint32_t tvwidth = x_get_arr_width(ptr_to_val);
  if ( tvwidth == 0 ) { go_BYE(-1); } 

  //----------------------------------------------------------------
  uint32_t fromidx = 0; uint32_t toidx = 0;
  int8_t *nn_tvdI4 = (int8_t *)nn_tvd;
  //----------------------------------------------------------------
  int32_t *tkdI4 = (int32_t *)tkd; 
  int32_t *tidI4 = (int32_t *)tid; 
  // int32_t *tvdI4 = (int *)tvd; 

  const int32_t * const fkdI4 = (const int32_t * const)fkd; 
  const int32_t * const fidI4 = (const int32_t * const)fid; 
  // int32_t *fvdI4 = (int *)fvd; 
  //----------------------------------------------------------------
#ifdef DEBUG
  // keys must be sorted in ascending order
  // NOTE that it is not strictly ascending 
  for ( uint32_t i = 1; i < tkn; i++ ) {
    if ( tkdI4[i] < tkdI4[i-1] ) { go_BYE(-1); }
  }
  for ( uint32_t i = 1; i < fkn; i++ ) {
    if ( fkdI4[i] < fkdI4[i-1] ) { go_BYE(-1); }
  }
  if ( !is_unique((const char * const)tidI4, NULL, tin, I4) ) { go_BYE(-1); } 
  if ( !is_unique((const char * const)fidI4, NULL, fin, I4) ) { go_BYE(-1); } 
#endif
  // Set all destination values to undefined
  memset(tvd,    0, tkn * sizeof(tvwidth));
  memset(nn_tvd, 0, tkn * sizeof(bool));

  for ( ; ; ) { 
    if ( fromidx >= fkn ) { break; } // nothing more in "from" data 
    if ( toidx   >= tkn ) { break; } // nothing more in "to" data 
    int tkval = tkdI4[toidx];
    int fkval = fkdI4[fromidx];
    if ( tkval > fkval ) { fromidx++; continue; }
    if ( tkval < fkval ) { toidx++; continue; }
    // tkval == fkval ) 
    int tival = tidI4[toidx];
    int fival = fidI4[fromidx];

    if ( ( tival < 0 ) || ( tival >= (int)tin ) ) { go_BYE(-1); }
    if ( ( fival < 0 ) || ( fival >= (int)fin ) ) { go_BYE(-1); }
    // if you want to disallow multiple assignment, enable below
    // if ( nn_tvdI4[tival] == 1 ) { go_BYE(-1); }

    switch ( fvqtype ) { 
      case I4 : ((int32_t *)tvd)[tival] = ((int32_t *)fvd)[fival]; break; 
      case F4 : ((float *)tvd)[tival] = ((float *)fvd)[fival]; break; 
      default : go_BYE(-1); break; 
    }
    // tvdI4[tival] = fvdI4[fival];
    nn_tvdI4[tival] = 1;
    toidx++; 
  }
#ifdef DEBUG
  toidx = fromidx = 0;
  bool any_missing_values = false;
  for ( ; toidx < tkn; ) { 
    int32_t tkval = tkdI4[toidx];
    int32_t fkval = fkdI4[fromidx];
    if ( tkval > fkval ) { fromidx++; continue; }
    if ( tkval == fkval ) { toidx++; continue; }
    // if ( tkval < fkval ) 
    any_missing_values = true;
    break;
  }
  int32_t sum = 0; 
  for ( uint32_t i = 0; i < tkn; i++ ) { 
    if ( ( nn_tvdI4[i] < 0 ) ||  ( nn_tvdI4[i] > 1 ) ) { go_BYE(-1); }
    sum += nn_tvdI4[i];
  }
  if ( any_missing_values ) {
    if ( sum == 0 ) { 
      go_BYE(-1); }
  }
  else {
    if ( sum != (int)tkn ) { go_BYE(-1); }
  }
#endif

BYE:
  free_if_non_null(copy_fidI4);
  return status;
}
