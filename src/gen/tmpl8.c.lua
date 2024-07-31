return require 'Q/UTILS/lua/code_gen' {
  declaration = [[
#include "incs.h"
#include "macros.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"

typedef struct _sort_struct_${qtype1}_${qtype2}_t {
  uint32_t idx; // idx is always I4
  ${ctype1} val1_${qtype1};
  ${ctype2} val2_${qtype2};
} sort_struct_${qtype1}_${qtype2}_t;

extern int
${fn}(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   );
 ]],

 definition = [[

#include "${fn}.h"
static int
sortcompare_${qtype1}_${qtype2}(const void *p1, const void *p2)
{
  const sort_struct_${qtype1}_${qtype2}_t  *u1 = 
    (const sort_struct_${qtype1}_${qtype2}_t *)p1;
  const sort_struct_${qtype1}_${qtype2}_t  *u2 = 
    (const sort_struct_${qtype1}_${qtype2}_t *)p2;
  ${ctype1} v1 = u1->val1_${qtype1};
  ${ctype1} v2 = u2->val1_${qtype1};
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    ${ctype2} t1 = u1->val2_${qtype2};
    ${ctype2} t2 = u2->val2_${qtype2};
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_${qtype1}_${qtype2}(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_${qtype1}_${qtype2}_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != ${qtype1} ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != ${qtype2} ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_${qtype1}_${qtype2}_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_${qtype1} = ((const ${ctype1} * const)s1ptr)[i];
    X[i].val2_${qtype2} = ((const ${ctype2} * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_${qtype1}_${qtype2}_t), sortcompare_${qtype1}_${qtype2});
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}
]],
}
