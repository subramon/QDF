// TODO P2: This is very custom to mgcv. Should not be part of lQDF
#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_fill.h"
int
fill(
    const QDF_REC_TYPE * const src, 
    const QDF_REC_TYPE * const nn, 
    const QDF_REC_TYPE * const grp, 
    const QDF_REC_TYPE * const tim, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  int8_t *copy_nn = NULL;

  mcr_chk_null(dst, -1); 
  mcr_chk_non_null(src, -1); 
  char *sx = src->data; 
  uint32_t n_src     = get_arr_len(sx);   
  if ( n_src == 0 ) { go_BYE(-1); } 
  uint32_t sz_src     = get_arr_size(sx);   
  if ( sz_src < n_src ) { go_BYE(-1); } 
  jtype_t sjtype = get_jtype(sx);     
  if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx);     
  if ( sqtype != F4 ) { go_BYE(-1); }
  const float * const F4src = (const float * const) get_arr_ptr(sx); 
  //==============================
  mcr_chk_non_null(nn, -1); 
  char *nnx = nn->data; 
  uint32_t n_nn     = get_arr_len(nnx); 
  if ( n_src != n_nn ) { go_BYE(-1); }
  jtype_t nn_jtype = get_jtype(nnx);  
  if ( nn_jtype != j_array ) { go_BYE(-1); }
  qtype_t nn_qtype = get_qtype(nnx);   
  if ( nn_qtype != I1 ) { go_BYE(-1); }
  const char * const nn_ptr = get_arr_ptr(nnx); 
  //==============================
  mcr_chk_non_null(grp, -1); 
  char *grpx = grp->data; 
  uint32_t n_grp     = get_arr_len(grpx); 
  if ( n_grp == 0 ) { go_BYE(-1); } 
  jtype_t grp_jtype = get_jtype(grpx); 
  if ( grp_jtype != j_array ) {go_BYE(-1);}
  qtype_t grp_qtype = get_qtype(grpx); 
  if ( grp_qtype != I4 ) { go_BYE(-1); }
  const int32_t * const I4grp = (const int32_t * const)get_arr_ptr(grpx); 
  //==============================
  mcr_chk_non_null(tim, -1); 
  char *timx = tim->data; 
  uint32_t n_tim     = get_arr_len(timx); 
  if ( n_tim != n_src ) { go_BYE(-1); }
  jtype_t tim_jtype = get_jtype(timx); 
  if (tim_jtype != j_array ) { go_BYE(-1); }
  qtype_t tim_qtype = get_qtype(timx); 
  if (tim_qtype != I4 ) { go_BYE(-1); }
  const int32_t * const I4tim = (const int32_t * const)get_arr_ptr(timx); 
  //==============================
  // make a copy of nn since we will modify it as we go along
  // technically, we do not need to modify it but it is useful
  // for debugging to not mess with the input 
  copy_nn = malloc(n_nn * sizeof(int8_t));
  return_if_malloc_failed(copy_nn);
  memcpy(copy_nn, nn_ptr, n_nn * sizeof(int8_t));
  // Count number of null values 
  uint32_t n_null = 0;
  for ( uint32_t i = 0; i < n_nn; i++ ) { 
    if ( copy_nn[i] == 0 ) { n_null++; } 
  }
  //==============================
  qtype_t dqtype = sqtype;
  uint32_t n_dst  = n_src; 
  uint32_t sz_dst = sz_src; 
  status = make_num_array(NULL, n_dst, sz_dst, dqtype,  dst); cBYE(status);
  void *dstptr = get_arr_ptr(dst->data); 
  float *F4dst = (float *)dstptr; 
  // copy from source to destination.  Will over-write
  // selected parts of destination later, depending on nn
  memcpy(F4dst, F4src, n_dst * sizeof(float)); 
  // early return case, then nothing more to do since
  // we have copied src to dst
  if ( n_null == 0 ) { goto BYE; }
#ifdef DEBUG
  uint32_t sum = 0;
  for ( uint32_t g = 0; g < n_grp; g++ ) {
    int n_in_grp = I4grp[g];
    if ( n_in_grp <= 0 ) { go_BYE(-1); }
    sum += (uint32_t)n_in_grp;
  }
  if ( sum != n_src ) { go_BYE(-1); }
#endif

  uint32_t n_replace = 0;
  uint32_t lb = 0, ub = 0;
  // We will process the input values in src a group at a time 
  for ( uint32_t g = 0; g < n_grp; g++ ) {
    int n_in_grp = I4grp[g];
    lb = ub;
    ub = lb + (uint32_t)n_in_grp;
    if ( ub > n_src ) { go_BYE(-1); }
    //  Determine if any back-fill is needed 
    for ( uint32_t j = lb+1; j < ub; j++ ) {  // Note the +1 
      if ( copy_nn[j] == 1 ) { continue; }
      if ( copy_nn[j] == 0 ) { 
        if ( copy_nn[j-1] == 1 ) { 
          // previous value defined and current value undefined
          F4dst[j] = F4src[j-1];
          copy_nn[j] = 1;
          n_replace++;
        }
      }
    }
    //  Determine if any forward-fill is needed 
    if ( copy_nn[lb] == 0 ) { // find first spot where nn == 1
      bool found_defined = false; uint32_t good_pos = 0;
      for ( uint32_t j = lb+1; j < ub; j++ ) {  // Note the +1 
        if ( copy_nn[j] == 1 ) { 
          found_defined = true; good_pos = j; break; 
        }
      }
      if ( found_defined ) { 
        for ( uint32_t j = lb; j < good_pos; j++ ) { 
          if ( copy_nn[j] == 1 ) { go_BYE(-1); } 
          F4dst[j] = F4src[good_pos];
          copy_nn[j] = 1;
          n_replace++;
          /*
          printf("FWD  F4dst[%d] = %f := F4src[%d] = %f \n",
            j, F4dst[j], good_pos, F4src[good_pos]);
            */
        }
      }
    }
  }
  if ( ub != n_src ) { go_BYE(-1); }
  // If there are still null entries, average over time for them
  if ( n_replace >  n_nn ) { go_BYE(-1); }
  if ( n_replace != n_nn ) { 
    for ( uint32_t i = 0; i < n_src; i++ ) { 
      if ( copy_nn[i] == 1 ) { continue; }
      // TODO P3 This is very inefficient but we will optimize later
      // Average over all defined values with the same time as i'th entry 
      float numer = 0; float denom = 0;
      for ( uint32_t k = 0; k < n_src; k++ ) { 
        if ( copy_nn[k] == 0 ) { continue; }
        if ( I4tim[i] != I4tim[k] ) { continue; }
        numer += F4src[k]; 
        denom++;
      }
      if ( denom != 0 ) { 
        F4dst[i] = numer / denom;
        copy_nn[i] = 1;
        n_replace++;
      }
    }
  }
  //-- There may *still* be some null values 
  if ( n_replace != n_nn ) {
    for ( uint32_t i = 0; i < n_src; i++ ) { 
      if ( copy_nn[i] == 1 ) { continue; }
      // Average over *all* defined values 
      // TODO P3 This is very inefficient but we will optimize later
      float numer = 0; float denom = 0;
      for ( uint32_t k = 0; k < n_src; k++ ) { 
        if ( copy_nn[k] == 0 ) { continue; }
        numer += F4src[k]; 
        denom++;
      }
      if ( denom == 0 ) { go_BYE(-1); }
      F4dst[i] = numer / denom;
      copy_nn[i] = 1;
      n_replace++;
    }
  }
  // Now there had better *NOT* be any null values 
  if ( n_null != n_replace ) { go_BYE(-1); }

BYE:
  free_if_non_null(copy_nn);
  return status;
}
