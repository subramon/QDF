// Given a vector, return 
// 1) unique values of the vector
// 1) number of times each unique value occurs
#include "incs.h"
#include "qdf_struct.h"
#include "sort1.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_vals_counts.h"
//START_FUNC_DECL
int
vals_counts(
    const QDF_REC_TYPE * const ptr_qdf_src,  // input
    QDF_REC_TYPE * restrict ptr_qdf_val, // output 
    QDF_REC_TYPE * restrict ptr_qdf_cnt // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  void *src_vals = NULL;
  void *dst_vals = NULL;
  int32_t *dst_cnts = NULL;
  mcr_chk_null(ptr_qdf_val, -1); 
  mcr_chk_null(ptr_qdf_cnt, -1); 
  //==============================
  // access input values, make a copy and sort ascending 
  qtype_t src_qtype = x_get_qtype(ptr_qdf_src); 
  uint32_t src_width     = x_get_arr_width(ptr_qdf_src); 
  if ( src_width == 0 ) { go_BYE(-1); } 
  uint32_t src_n         = get_arr_len(ptr_qdf_src->data); 
  if ( src_n == 0 ) { go_BYE(-1); } 
  status = posix_memalign((void **)&src_vals, 16, (src_n * src_width));
  cBYE(status); 
  memcpy(src_vals, get_arr_ptr(ptr_qdf_src->data), src_n*src_width);
  switch ( src_qtype ) { 
   case I4 : qsort(src_vals, src_n, src_width, fn_sortI4_asc); break;
   case I8 : qsort(src_vals, src_n, src_width, fn_sortI8_asc); break;
   default : go_BYE(-1); break;
  }
  //--- Count number of  unique values
  uint32_t dst_n = 0; 
  switch ( src_qtype ) { 
   case I4 : 
     {
       int32_t *l_src_vals = (int32_t *)src_vals; 
       int32_t l_src_val = l_src_vals[0]; dst_n = 1;
       for ( uint32_t i = 0; i < src_n; i++ ) { 
         if ( l_src_val == l_src_vals[i] ) { continue; }
         l_src_val = l_src_vals[i]; dst_n++;
       }
     }
     break;
   case I8 : 
     {
       int64_t *l_src_vals = (int64_t *)src_vals; 
       int64_t l_src_val = l_src_vals[0]; dst_n = 1;
       for ( uint32_t i = 0; i < src_n; i++ ) { 
         if ( l_src_val == l_src_vals[i] ) { continue; }
         l_src_val = l_src_vals[i]; dst_n++;
       }
     }
     break;
   default : 
     go_BYE(-1);
     break;
  }
  //---------------------------------
  // Make space for output
  uint32_t dst_width = src_width;
  status = posix_memalign((void **)&dst_vals, 16, 
      (dst_n * dst_width));
  cBYE(status); 
  status = posix_memalign((void **)&dst_cnts, 16, 
      (dst_n * sizeof(int32_t)));
  cBYE(status); 

  //--- Set unique values and counts
  uint32_t dst_idx = 0; 
  switch ( src_qtype ) { 
   case I4 : 
     {
       int32_t *l_src_vals = (int32_t *)src_vals; 
       int32_t *l_dst_vals = (int32_t *)dst_vals; 
       int32_t *l_dst_cnts = (int32_t *)dst_cnts; 
       l_dst_vals[dst_idx] = l_src_vals[0]; 
       l_dst_cnts[dst_idx] = 1;
       for ( uint32_t i = 0; i < src_n; i++ ) { 
         if ( l_dst_vals[dst_idx] == l_src_vals[i] ) { 
           l_dst_cnts[dst_idx]++;
         }
         else {
           dst_idx++;
           l_dst_vals[dst_idx] = l_src_vals[i]; 
           l_dst_cnts[dst_idx] = 1;
         }
       }
     }
     break;
   case I8 : 
     {
       int64_t *l_src_vals = (int64_t *)src_vals; 
       int64_t *l_dst_vals = (int64_t *)dst_vals; 
       int32_t *l_dst_cnts = (int32_t *)dst_cnts; 
       l_dst_vals[dst_idx] = l_src_vals[0]; 
       l_dst_cnts[dst_idx] = 1;
       for ( uint32_t i = 1; i < src_n; i++ ) { 
         if ( l_dst_vals[dst_idx] == l_src_vals[i] ) { 
           l_dst_cnts[dst_idx]++;
         }
         else {
           dst_idx++;
           l_dst_vals[dst_idx] = l_src_vals[i]; 
           l_dst_cnts[dst_idx] = 1;
         }
       }
     }
     break;
   default : 
     go_BYE(-1);
     break;
  }
  //---------------------------------
  // Create space for output 
  status = make_num_array(dst_vals, dst_n, 0, src_qtype, ptr_qdf_val); 
  cBYE(status);
  status = make_num_array(dst_cnts, dst_n, 0, I4,    ptr_qdf_cnt); 
  cBYE(status);
BYE:
  free_if_non_null(src_vals);
  free_if_non_null(dst_vals);
  free_if_non_null(dst_cnts);
  return status;
}
