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
#include "qdf_vals_sums.h"

typedef struct _key_I4_t {
  int32_t key;
  int32_t num;
} key_I4_t;

static int
sort_key_I4(const void *p1, const void *p2)
{
  const key_I4_t  *u1 = (const key_I4_t *)p1;
  const key_I4_t  *u2 = (const key_I4_t *)p2;
  if ( u1->key < u2->key ) {
    return -1;
  }
  else {
    return 1;
  }
}

typedef struct _key_I8_t {
  int64_t key;
  int32_t num;
} key_I8_t;

static int
sort_key_I8(const void *p1, const void *p2)
{
  const key_I8_t  *u1 = (const key_I8_t *)p1;
  const key_I8_t  *u2 = (const key_I8_t *)p2;
  if ( u1->key < u2->key ) {
    return -1;
  }
  else {
    return 1;
  }
}

// ASSUMPTION THAT SUMS CAN BE CONTAINED in int32_t TODO P3
//START_FUNC_DECL
int
vals_sums(
    const QDF_REC_TYPE * const ptr_qdf_key,  // input
    const QDF_REC_TYPE * const ptr_qdf_num,  // input
    QDF_REC_TYPE * restrict ptr_qdf_val, // output 
    QDF_REC_TYPE * restrict ptr_qdf_sum // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  void *dst_keys = NULL;
  int32_t *dst_sums = NULL;
  void *key_num_vals = NULL;
  mcr_chk_null(ptr_qdf_val, -1); 
  mcr_chk_null(ptr_qdf_sum, -1); 
  //==============================
  // access input values, make a copy and sort ascending 
  qtype_t key_qtype = x_get_qtype(ptr_qdf_key); 
  uint32_t key_width     = x_get_arr_width(ptr_qdf_key); 
  if ( key_width == 0 ) { go_BYE(-1); } 
  uint32_t key_n         = get_arr_len(ptr_qdf_key->data); 
  if ( key_n     == 0 ) { go_BYE(-1); } 

  qtype_t num_qtype = x_get_qtype(ptr_qdf_num); 
  if ( num_qtype != I4 ) { go_BYE(-1); } // TODO P4 to be relaxed
  switch ( key_qtype ) { 
    case I4 : 
      {
        key_num_vals = malloc(key_n * sizeof(key_I4_t));
        int32_t *key_ptr = (int32_t *)get_arr_ptr(ptr_qdf_key->data); 
        int32_t *num_ptr = (int32_t *)get_arr_ptr(ptr_qdf_num->data); 
        for ( uint32_t i = 0; i < key_n; i++ ) { 
          ((key_I4_t *)key_num_vals)[i].key = key_ptr[i];
          ((key_I4_t *)key_num_vals)[i].num = num_ptr[i];
        }
        qsort(key_num_vals, key_n, sizeof(key_I4_t), sort_key_I4); 
      }
      break;
    case I8 : 
      {
        key_num_vals = malloc(key_n * sizeof(key_I8_t));
        int64_t *key_ptr = (int64_t *)get_arr_ptr(ptr_qdf_key->data); 
        int32_t *num_ptr = (int32_t *)get_arr_ptr(ptr_qdf_num->data); 
        for ( uint32_t i = 0; i < key_n; i++ ) { 
          ((key_I8_t *)key_num_vals)[i].key = key_ptr[i];
          ((key_I8_t *)key_num_vals)[i].num = num_ptr[i];
        }
        qsort(key_num_vals, key_n, sizeof(key_I8_t), sort_key_I8); 
      }
      break;
    default : go_BYE(-1); break;
  }
  //--- Count number of  unique values
  uint32_t dst_n = 0; 
  switch ( key_qtype ) { 
   case I4 : 
     {
       key_I4_t *l_key_num_vals = (key_I4_t *)key_num_vals; 
       int32_t l_key_val = l_key_num_vals[0].key; dst_n = 1;
       for ( uint32_t i = 1; i < key_n; i++ ) { 
         if ( l_key_val == l_key_num_vals[i].key ) { continue; }
         l_key_val = l_key_num_vals[i].key; dst_n++;
       }
     }
     break;
   case I8 : 
     {
       key_I8_t *l_key_num_vals = (key_I8_t *)key_num_vals; 
       int64_t l_key_val = l_key_num_vals[0].key; dst_n = 1;
       for ( uint32_t i = 1; i < key_n; i++ ) { 
         if ( l_key_val == l_key_num_vals[i].key ) { continue; }
         l_key_val = l_key_num_vals[i].key; dst_n++;
       }
     }
     break;
   default : 
     go_BYE(-1);
     break;
  }
  //---------------------------------
  // Make space for output
  uint32_t dst_width = key_width;
  if ( dst_width == 0 ) { go_BYE(-1); } 
  dst_keys = malloc(dst_n * dst_width);
  dst_sums = malloc(dst_n * sizeof(int32_t)); 

  //--- Set unique values and counts
  uint32_t dst_idx = 0; 
  switch ( key_qtype ) { 
   case I4 : 
     {
       key_I4_t *l_key_num_vals = (key_I4_t *)key_num_vals; 
       int32_t *l_dst_keys = (int32_t *)dst_keys; 
       int32_t *l_dst_sums = (int32_t *)dst_sums; 
       l_dst_keys[dst_idx] = l_key_num_vals[0].key;
       l_dst_sums[dst_idx] = l_key_num_vals[0].num;
       for ( uint32_t i = 1; i < key_n; i++ ) { 
         if ( l_dst_keys[dst_idx] == l_key_num_vals[i].key ) { 
           l_dst_sums[dst_idx] += l_key_num_vals[i].num;
         }
         else {
           dst_idx++;
           l_dst_keys[dst_idx] = l_key_num_vals[i].key;
           l_dst_sums[dst_idx] = l_key_num_vals[i].num;
         }
       }
     }
     break;
   case I8 : 
     {
       key_I8_t *l_key_num_vals = (key_I8_t *)key_num_vals; 
       int64_t *l_dst_keys = (int64_t *)dst_keys; 
       int32_t *l_dst_sums = (int32_t *)dst_sums; 
       l_dst_keys[dst_idx] = l_key_num_vals[0].key;
       l_dst_sums[dst_idx] = l_key_num_vals[0].num;
       for ( uint32_t i = 1; i < key_n; i++ ) { 
         if ( l_dst_keys[dst_idx] == l_key_num_vals[i].key ) { 
           l_dst_sums[dst_idx] += l_key_num_vals[i].num;
         }
         else {
           dst_idx++;
           l_dst_keys[dst_idx] = l_key_num_vals[i].key;
           l_dst_sums[dst_idx] = l_key_num_vals[i].num;
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
  status = make_num_array(dst_keys, dst_n, 0, key_qtype, ptr_qdf_val); 
  cBYE(status);
  status = make_num_array(dst_sums, dst_n, 0, I4,        ptr_qdf_sum); 
  cBYE(status);
BYE:
  free_if_non_null(key_num_vals);
  free_if_non_null(dst_keys);
  free_if_non_null(dst_sums);
  return status;
}
