#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_coalesce.h"
// Aims to implement coalesce as defined in R 
int
coalesce(
    const QDF_REC_TYPE * const src1, // input numeric array F4
    const QDF_REC_TYPE * const nn_src1, // input numeric array I1
    const QDF_REC_TYPE * const src2, // input numeric array F4
    const QDF_REC_TYPE * const nn_src2, // input numeric array I1
    QDF_REC_TYPE * restrict dst, // output numeric array F4
    QDF_REC_TYPE * restrict nn_dst, // output numeric array I1
    int *ptr_num_nulls
   )
{
  int status = 0;
  const char *nn_s1ptr = NULL; const char *nn_s2ptr = NULL;
  int num_nulls = 0;
  //==============================
  mcr_chk_null(dst, -1); 
  mcr_chk_null(nn_dst, -1); 
  //==============================
  mcr_chk_non_null(src1, -1); 
  char *s1 = src1->data; 
  jtype_t s1jtype = get_jtype(s1); 
  if ( s1jtype != j_array ) {  go_BYE(-1); }
  uint32_t src1_n = get_arr_len(s1); 
  qtype_t  src1_qtype = get_qtype(s1); 
  if ( src1_qtype != F4 ) { go_BYE(-1); } 
  const float * const s1ptr = (const float * const) get_arr_ptr(s1);
  //==============================
  if ( nn_src1 != NULL ) { 
    mcr_chk_non_null(nn_src1, -1); 
    char *nn_s1 = nn_src1->data; 
    jtype_t nn_s1jtype = get_jtype(nn_s1); 
    if ( nn_s1jtype != j_array ) {  go_BYE(-1); }
    uint32_t nn_src1_n = get_arr_len(nn_s1); 
    qtype_t  nn_src1_qtype = get_qtype(nn_s1); 
    if ( nn_src1_qtype != I1 ) { go_BYE(-1); } 
    nn_s1ptr = get_arr_ptr(nn_s1);
    if ( src1_n != nn_src1_n ) { go_BYE(-1); }
  }
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2 = src2->data; 
  jtype_t s2jtype = get_jtype(s2); 
  if ( s2jtype != j_array ) {  go_BYE(-1); }
  uint32_t src2_n = get_arr_len(s2); 
  qtype_t  src2_qtype = get_qtype(s2); 
  if ( src2_qtype != F4 ) { go_BYE(-1); } 
  const float * const s2ptr = (const float * const) get_arr_ptr(s2);
  //==============================
  if ( nn_src2 != NULL ) { 
    mcr_chk_non_null(nn_src2, -1); 
    char *nn_s2 = nn_src2->data; 
    jtype_t nn_s2jtype = get_jtype(nn_s2); 
    if ( nn_s2jtype != j_array ) {  go_BYE(-1); }
    uint32_t nn_src2_n = get_arr_len(nn_s2); 
    qtype_t  nn_src2_qtype = get_qtype(nn_s2); 
    if ( nn_src2_qtype != I1 ) { go_BYE(-1); } 
    nn_s2ptr = get_arr_ptr(nn_s2);
    if ( src2_n != nn_src2_n ) { go_BYE(-1); }
  }
  //==============================
  if ( src1_n != src2_n ) { go_BYE(-1); }
  //==============================
  status = make_num_array(NULL, src1_n, 0, F4, dst); cBYE(status);
  status = make_num_array(NULL, src1_n, 0, I1, nn_dst); cBYE(status);
  void *s3 = dst->data; 
  float *s3ptr = get_arr_ptr(s3);

  void *nn_s3    = nn_dst->data; 
  char *nn_s3ptr = get_arr_ptr(nn_s3);

  for ( uint32_t i = 0; i < src1_n; i++ ) { 
    if ( ( nn_s1ptr == NULL ) || ( nn_s1ptr[i] == 1 ) ) {
      s3ptr[i] = s1ptr[i];
      nn_s3ptr[i] = 1; 
    }
    else if ( ( nn_s2ptr == NULL ) || ( nn_s2ptr[i] == 1 ) ) {
      s3ptr[i] = s2ptr[i];
      nn_s3ptr[i] = 1; 
    }
    else {
      s3ptr[i] = 0;
      nn_s3ptr[i] = 0; 
      num_nulls++;
    }
  }
BYE:
  *ptr_num_nulls = num_nulls;
  return status;
}
