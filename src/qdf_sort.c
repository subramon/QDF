// candidate for code generation
#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_sort.h"
#include "sort1.h"
//START_FUNC_DECL
int
qdf_sort(
    QDF_REC_TYPE *src, // input and output (modified in place)
    const char * const mode
   )
//STOP_FUNC_DECL
{
  int status = 0;
  //==============================
  mcr_chk_non_null(src,-1);
  if ( src->is_read_only ) { go_BYE(-1); }
  if ( mode == NULL ) { go_BYE(-1); } 
  if ( !( ( strcmp(mode, "ascending") == 0 ) || 
        ( strcmp(mode, "descending") == 0 ) ) ) { go_BYE(-1); }
  //==============================
  char *sx = src->data; 
  jtype_t s1jtype = get_jtype(sx); 
  if ( s1jtype != j_array ) {  go_BYE(-1); }
  uint32_t src_n     = get_arr_len(sx); 
  qtype_t  src_qtype = get_qtype(sx); 
  char * s1ptr = get_arr_ptr(sx);
  //==============================
  switch ( src_qtype ) { 
    case F2 :
    if ( strcmp(mode, "ascending") == 0 ) {
      qsort(s1ptr, src_n, sizeof(bfloat16), fn_sortF2_asc);
    }
    else {
      qsort(s1ptr, src_n, sizeof(bfloat16), fn_sortF2_dsc);
    }
    break;
    case F4 :
    if ( strcmp(mode, "ascending") == 0 ) {
      qsort(s1ptr, src_n, sizeof(float), fn_sortF4_asc);
    }
    else {
      qsort(s1ptr, src_n, sizeof(float), fn_sortF4_dsc);
    }
    break;
    case I1 :
    if ( strcmp(mode, "ascending") == 0 ) {
      qsort(s1ptr, src_n, sizeof(int8_t), fn_sortI1_asc);
    }
    else {
      go_BYE(-1); // TODO 
    }
    break;
    case I4 :
    if ( strcmp(mode, "ascending") == 0 ) {
      qsort(s1ptr, src_n, sizeof(int32_t), fn_sortI4_asc);
    }
    else {
      go_BYE(-1); // TODO 
    }
    break;
    case I8 :
    if ( strcmp(mode, "ascending") == 0 ) {
      qsort(s1ptr, src_n, sizeof(int64_t), fn_sortI8_asc);
    }
    else {
      qsort(s1ptr, src_n, sizeof(int64_t), fn_sortI8_dsc);
    }
    break;
    default : 
    go_BYE(-1);
    break;
  }
BYE:
  return status;
}
