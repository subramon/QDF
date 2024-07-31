
#include "mk_sort_idx_I1_I1.h"
static int
sortcompare_I1_I1(const void *p1, const void *p2)
{
  const sort_struct_I1_I1_t  *u1 = 
    (const sort_struct_I1_I1_t *)p1;
  const sort_struct_I1_I1_t  *u2 = 
    (const sort_struct_I1_I1_t *)p2;
  int8_t v1 = u1->val1_I1;
  int8_t v2 = u2->val1_I1;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int8_t t1 = u1->val2_I1;
    int8_t t2 = u2->val2_I1;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I1_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I1_I1_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I1 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I1 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I1_I1_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I1 = ((const int8_t * const)s1ptr)[i];
    X[i].val2_I1 = ((const int8_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I1_I1_t), sortcompare_I1_I1);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I1_I2.h"
static int
sortcompare_I1_I2(const void *p1, const void *p2)
{
  const sort_struct_I1_I2_t  *u1 = 
    (const sort_struct_I1_I2_t *)p1;
  const sort_struct_I1_I2_t  *u2 = 
    (const sort_struct_I1_I2_t *)p2;
  int8_t v1 = u1->val1_I1;
  int8_t v2 = u2->val1_I1;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int16_t t1 = u1->val2_I2;
    int16_t t2 = u2->val2_I2;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I1_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I1_I2_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I1 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I2 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I1_I2_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I1 = ((const int8_t * const)s1ptr)[i];
    X[i].val2_I2 = ((const int16_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I1_I2_t), sortcompare_I1_I2);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I1_I4.h"
static int
sortcompare_I1_I4(const void *p1, const void *p2)
{
  const sort_struct_I1_I4_t  *u1 = 
    (const sort_struct_I1_I4_t *)p1;
  const sort_struct_I1_I4_t  *u2 = 
    (const sort_struct_I1_I4_t *)p2;
  int8_t v1 = u1->val1_I1;
  int8_t v2 = u2->val1_I1;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int32_t t1 = u1->val2_I4;
    int32_t t2 = u2->val2_I4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I1_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I1_I4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I1 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I1_I4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I1 = ((const int8_t * const)s1ptr)[i];
    X[i].val2_I4 = ((const int32_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I1_I4_t), sortcompare_I1_I4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I1_I8.h"
static int
sortcompare_I1_I8(const void *p1, const void *p2)
{
  const sort_struct_I1_I8_t  *u1 = 
    (const sort_struct_I1_I8_t *)p1;
  const sort_struct_I1_I8_t  *u2 = 
    (const sort_struct_I1_I8_t *)p2;
  int8_t v1 = u1->val1_I1;
  int8_t v2 = u2->val1_I1;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int64_t t1 = u1->val2_I8;
    int64_t t2 = u2->val2_I8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I1_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I1_I8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I1 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I1_I8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I1 = ((const int8_t * const)s1ptr)[i];
    X[i].val2_I8 = ((const int64_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I1_I8_t), sortcompare_I1_I8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I1_F4.h"
static int
sortcompare_I1_F4(const void *p1, const void *p2)
{
  const sort_struct_I1_F4_t  *u1 = 
    (const sort_struct_I1_F4_t *)p1;
  const sort_struct_I1_F4_t  *u2 = 
    (const sort_struct_I1_F4_t *)p2;
  int8_t v1 = u1->val1_I1;
  int8_t v2 = u2->val1_I1;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    float t1 = u1->val2_F4;
    float t2 = u2->val2_F4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I1_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I1_F4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I1 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I1_F4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I1 = ((const int8_t * const)s1ptr)[i];
    X[i].val2_F4 = ((const float * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I1_F4_t), sortcompare_I1_F4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I1_F8.h"
static int
sortcompare_I1_F8(const void *p1, const void *p2)
{
  const sort_struct_I1_F8_t  *u1 = 
    (const sort_struct_I1_F8_t *)p1;
  const sort_struct_I1_F8_t  *u2 = 
    (const sort_struct_I1_F8_t *)p2;
  int8_t v1 = u1->val1_I1;
  int8_t v2 = u2->val1_I1;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    double t1 = u1->val2_F8;
    double t2 = u2->val2_F8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I1_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I1_F8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I1 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I1_F8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I1 = ((const int8_t * const)s1ptr)[i];
    X[i].val2_F8 = ((const double * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I1_F8_t), sortcompare_I1_F8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I2_I1.h"
static int
sortcompare_I2_I1(const void *p1, const void *p2)
{
  const sort_struct_I2_I1_t  *u1 = 
    (const sort_struct_I2_I1_t *)p1;
  const sort_struct_I2_I1_t  *u2 = 
    (const sort_struct_I2_I1_t *)p2;
  int16_t v1 = u1->val1_I2;
  int16_t v2 = u2->val1_I2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int8_t t1 = u1->val2_I1;
    int8_t t2 = u2->val2_I1;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I2_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I2_I1_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I2 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I1 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I2_I1_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I2 = ((const int16_t * const)s1ptr)[i];
    X[i].val2_I1 = ((const int8_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I2_I1_t), sortcompare_I2_I1);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I2_I2.h"
static int
sortcompare_I2_I2(const void *p1, const void *p2)
{
  const sort_struct_I2_I2_t  *u1 = 
    (const sort_struct_I2_I2_t *)p1;
  const sort_struct_I2_I2_t  *u2 = 
    (const sort_struct_I2_I2_t *)p2;
  int16_t v1 = u1->val1_I2;
  int16_t v2 = u2->val1_I2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int16_t t1 = u1->val2_I2;
    int16_t t2 = u2->val2_I2;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I2_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I2_I2_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I2 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I2 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I2_I2_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I2 = ((const int16_t * const)s1ptr)[i];
    X[i].val2_I2 = ((const int16_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I2_I2_t), sortcompare_I2_I2);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I2_I4.h"
static int
sortcompare_I2_I4(const void *p1, const void *p2)
{
  const sort_struct_I2_I4_t  *u1 = 
    (const sort_struct_I2_I4_t *)p1;
  const sort_struct_I2_I4_t  *u2 = 
    (const sort_struct_I2_I4_t *)p2;
  int16_t v1 = u1->val1_I2;
  int16_t v2 = u2->val1_I2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int32_t t1 = u1->val2_I4;
    int32_t t2 = u2->val2_I4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I2_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I2_I4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I2 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I2_I4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I2 = ((const int16_t * const)s1ptr)[i];
    X[i].val2_I4 = ((const int32_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I2_I4_t), sortcompare_I2_I4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I2_I8.h"
static int
sortcompare_I2_I8(const void *p1, const void *p2)
{
  const sort_struct_I2_I8_t  *u1 = 
    (const sort_struct_I2_I8_t *)p1;
  const sort_struct_I2_I8_t  *u2 = 
    (const sort_struct_I2_I8_t *)p2;
  int16_t v1 = u1->val1_I2;
  int16_t v2 = u2->val1_I2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int64_t t1 = u1->val2_I8;
    int64_t t2 = u2->val2_I8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I2_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I2_I8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I2 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I2_I8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I2 = ((const int16_t * const)s1ptr)[i];
    X[i].val2_I8 = ((const int64_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I2_I8_t), sortcompare_I2_I8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I2_F4.h"
static int
sortcompare_I2_F4(const void *p1, const void *p2)
{
  const sort_struct_I2_F4_t  *u1 = 
    (const sort_struct_I2_F4_t *)p1;
  const sort_struct_I2_F4_t  *u2 = 
    (const sort_struct_I2_F4_t *)p2;
  int16_t v1 = u1->val1_I2;
  int16_t v2 = u2->val1_I2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    float t1 = u1->val2_F4;
    float t2 = u2->val2_F4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I2_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I2_F4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I2 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I2_F4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I2 = ((const int16_t * const)s1ptr)[i];
    X[i].val2_F4 = ((const float * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I2_F4_t), sortcompare_I2_F4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I2_F8.h"
static int
sortcompare_I2_F8(const void *p1, const void *p2)
{
  const sort_struct_I2_F8_t  *u1 = 
    (const sort_struct_I2_F8_t *)p1;
  const sort_struct_I2_F8_t  *u2 = 
    (const sort_struct_I2_F8_t *)p2;
  int16_t v1 = u1->val1_I2;
  int16_t v2 = u2->val1_I2;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    double t1 = u1->val2_F8;
    double t2 = u2->val2_F8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I2_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I2_F8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I2 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I2_F8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I2 = ((const int16_t * const)s1ptr)[i];
    X[i].val2_F8 = ((const double * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I2_F8_t), sortcompare_I2_F8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I4_I1.h"
static int
sortcompare_I4_I1(const void *p1, const void *p2)
{
  const sort_struct_I4_I1_t  *u1 = 
    (const sort_struct_I4_I1_t *)p1;
  const sort_struct_I4_I1_t  *u2 = 
    (const sort_struct_I4_I1_t *)p2;
  int32_t v1 = u1->val1_I4;
  int32_t v2 = u2->val1_I4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int8_t t1 = u1->val2_I1;
    int8_t t2 = u2->val2_I1;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I4_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I4_I1_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I1 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I4_I1_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I4 = ((const int32_t * const)s1ptr)[i];
    X[i].val2_I1 = ((const int8_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I4_I1_t), sortcompare_I4_I1);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I4_I2.h"
static int
sortcompare_I4_I2(const void *p1, const void *p2)
{
  const sort_struct_I4_I2_t  *u1 = 
    (const sort_struct_I4_I2_t *)p1;
  const sort_struct_I4_I2_t  *u2 = 
    (const sort_struct_I4_I2_t *)p2;
  int32_t v1 = u1->val1_I4;
  int32_t v2 = u2->val1_I4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int16_t t1 = u1->val2_I2;
    int16_t t2 = u2->val2_I2;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I4_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I4_I2_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I2 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I4_I2_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I4 = ((const int32_t * const)s1ptr)[i];
    X[i].val2_I2 = ((const int16_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I4_I2_t), sortcompare_I4_I2);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I4_I4.h"
static int
sortcompare_I4_I4(const void *p1, const void *p2)
{
  const sort_struct_I4_I4_t  *u1 = 
    (const sort_struct_I4_I4_t *)p1;
  const sort_struct_I4_I4_t  *u2 = 
    (const sort_struct_I4_I4_t *)p2;
  int32_t v1 = u1->val1_I4;
  int32_t v2 = u2->val1_I4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int32_t t1 = u1->val2_I4;
    int32_t t2 = u2->val2_I4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I4_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I4_I4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I4_I4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I4 = ((const int32_t * const)s1ptr)[i];
    X[i].val2_I4 = ((const int32_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I4_I4_t), sortcompare_I4_I4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I4_I8.h"
static int
sortcompare_I4_I8(const void *p1, const void *p2)
{
  const sort_struct_I4_I8_t  *u1 = 
    (const sort_struct_I4_I8_t *)p1;
  const sort_struct_I4_I8_t  *u2 = 
    (const sort_struct_I4_I8_t *)p2;
  int32_t v1 = u1->val1_I4;
  int32_t v2 = u2->val1_I4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int64_t t1 = u1->val2_I8;
    int64_t t2 = u2->val2_I8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I4_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I4_I8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I4_I8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I4 = ((const int32_t * const)s1ptr)[i];
    X[i].val2_I8 = ((const int64_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I4_I8_t), sortcompare_I4_I8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I4_F4.h"
static int
sortcompare_I4_F4(const void *p1, const void *p2)
{
  const sort_struct_I4_F4_t  *u1 = 
    (const sort_struct_I4_F4_t *)p1;
  const sort_struct_I4_F4_t  *u2 = 
    (const sort_struct_I4_F4_t *)p2;
  int32_t v1 = u1->val1_I4;
  int32_t v2 = u2->val1_I4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    float t1 = u1->val2_F4;
    float t2 = u2->val2_F4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I4_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I4_F4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I4_F4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I4 = ((const int32_t * const)s1ptr)[i];
    X[i].val2_F4 = ((const float * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I4_F4_t), sortcompare_I4_F4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I4_F8.h"
static int
sortcompare_I4_F8(const void *p1, const void *p2)
{
  const sort_struct_I4_F8_t  *u1 = 
    (const sort_struct_I4_F8_t *)p1;
  const sort_struct_I4_F8_t  *u2 = 
    (const sort_struct_I4_F8_t *)p2;
  int32_t v1 = u1->val1_I4;
  int32_t v2 = u2->val1_I4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    double t1 = u1->val2_F8;
    double t2 = u2->val2_F8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I4_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I4_F8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I4_F8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I4 = ((const int32_t * const)s1ptr)[i];
    X[i].val2_F8 = ((const double * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I4_F8_t), sortcompare_I4_F8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I8_I1.h"
static int
sortcompare_I8_I1(const void *p1, const void *p2)
{
  const sort_struct_I8_I1_t  *u1 = 
    (const sort_struct_I8_I1_t *)p1;
  const sort_struct_I8_I1_t  *u2 = 
    (const sort_struct_I8_I1_t *)p2;
  int64_t v1 = u1->val1_I8;
  int64_t v2 = u2->val1_I8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int8_t t1 = u1->val2_I1;
    int8_t t2 = u2->val2_I1;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I8_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I8_I1_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I1 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I8_I1_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I8 = ((const int64_t * const)s1ptr)[i];
    X[i].val2_I1 = ((const int8_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I8_I1_t), sortcompare_I8_I1);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I8_I2.h"
static int
sortcompare_I8_I2(const void *p1, const void *p2)
{
  const sort_struct_I8_I2_t  *u1 = 
    (const sort_struct_I8_I2_t *)p1;
  const sort_struct_I8_I2_t  *u2 = 
    (const sort_struct_I8_I2_t *)p2;
  int64_t v1 = u1->val1_I8;
  int64_t v2 = u2->val1_I8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int16_t t1 = u1->val2_I2;
    int16_t t2 = u2->val2_I2;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I8_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I8_I2_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I2 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I8_I2_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I8 = ((const int64_t * const)s1ptr)[i];
    X[i].val2_I2 = ((const int16_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I8_I2_t), sortcompare_I8_I2);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I8_I4.h"
static int
sortcompare_I8_I4(const void *p1, const void *p2)
{
  const sort_struct_I8_I4_t  *u1 = 
    (const sort_struct_I8_I4_t *)p1;
  const sort_struct_I8_I4_t  *u2 = 
    (const sort_struct_I8_I4_t *)p2;
  int64_t v1 = u1->val1_I8;
  int64_t v2 = u2->val1_I8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int32_t t1 = u1->val2_I4;
    int32_t t2 = u2->val2_I4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I8_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I8_I4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I8_I4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I8 = ((const int64_t * const)s1ptr)[i];
    X[i].val2_I4 = ((const int32_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I8_I4_t), sortcompare_I8_I4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I8_I8.h"
static int
sortcompare_I8_I8(const void *p1, const void *p2)
{
  const sort_struct_I8_I8_t  *u1 = 
    (const sort_struct_I8_I8_t *)p1;
  const sort_struct_I8_I8_t  *u2 = 
    (const sort_struct_I8_I8_t *)p2;
  int64_t v1 = u1->val1_I8;
  int64_t v2 = u2->val1_I8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int64_t t1 = u1->val2_I8;
    int64_t t2 = u2->val2_I8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I8_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I8_I8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I8_I8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I8 = ((const int64_t * const)s1ptr)[i];
    X[i].val2_I8 = ((const int64_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I8_I8_t), sortcompare_I8_I8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I8_F4.h"
static int
sortcompare_I8_F4(const void *p1, const void *p2)
{
  const sort_struct_I8_F4_t  *u1 = 
    (const sort_struct_I8_F4_t *)p1;
  const sort_struct_I8_F4_t  *u2 = 
    (const sort_struct_I8_F4_t *)p2;
  int64_t v1 = u1->val1_I8;
  int64_t v2 = u2->val1_I8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    float t1 = u1->val2_F4;
    float t2 = u2->val2_F4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I8_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I8_F4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I8_F4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I8 = ((const int64_t * const)s1ptr)[i];
    X[i].val2_F4 = ((const float * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I8_F4_t), sortcompare_I8_F4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_I8_F8.h"
static int
sortcompare_I8_F8(const void *p1, const void *p2)
{
  const sort_struct_I8_F8_t  *u1 = 
    (const sort_struct_I8_F8_t *)p1;
  const sort_struct_I8_F8_t  *u2 = 
    (const sort_struct_I8_F8_t *)p2;
  int64_t v1 = u1->val1_I8;
  int64_t v2 = u2->val1_I8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    double t1 = u1->val2_F8;
    double t2 = u2->val2_F8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_I8_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_I8_F8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != I8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_I8_F8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_I8 = ((const int64_t * const)s1ptr)[i];
    X[i].val2_F8 = ((const double * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_I8_F8_t), sortcompare_I8_F8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F4_I1.h"
static int
sortcompare_F4_I1(const void *p1, const void *p2)
{
  const sort_struct_F4_I1_t  *u1 = 
    (const sort_struct_F4_I1_t *)p1;
  const sort_struct_F4_I1_t  *u2 = 
    (const sort_struct_F4_I1_t *)p2;
  float v1 = u1->val1_F4;
  float v2 = u2->val1_F4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int8_t t1 = u1->val2_I1;
    int8_t t2 = u2->val2_I1;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F4_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F4_I1_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I1 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F4_I1_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F4 = ((const float * const)s1ptr)[i];
    X[i].val2_I1 = ((const int8_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F4_I1_t), sortcompare_F4_I1);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F4_I2.h"
static int
sortcompare_F4_I2(const void *p1, const void *p2)
{
  const sort_struct_F4_I2_t  *u1 = 
    (const sort_struct_F4_I2_t *)p1;
  const sort_struct_F4_I2_t  *u2 = 
    (const sort_struct_F4_I2_t *)p2;
  float v1 = u1->val1_F4;
  float v2 = u2->val1_F4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int16_t t1 = u1->val2_I2;
    int16_t t2 = u2->val2_I2;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F4_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F4_I2_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I2 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F4_I2_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F4 = ((const float * const)s1ptr)[i];
    X[i].val2_I2 = ((const int16_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F4_I2_t), sortcompare_F4_I2);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F4_I4.h"
static int
sortcompare_F4_I4(const void *p1, const void *p2)
{
  const sort_struct_F4_I4_t  *u1 = 
    (const sort_struct_F4_I4_t *)p1;
  const sort_struct_F4_I4_t  *u2 = 
    (const sort_struct_F4_I4_t *)p2;
  float v1 = u1->val1_F4;
  float v2 = u2->val1_F4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int32_t t1 = u1->val2_I4;
    int32_t t2 = u2->val2_I4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F4_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F4_I4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F4_I4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F4 = ((const float * const)s1ptr)[i];
    X[i].val2_I4 = ((const int32_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F4_I4_t), sortcompare_F4_I4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F4_I8.h"
static int
sortcompare_F4_I8(const void *p1, const void *p2)
{
  const sort_struct_F4_I8_t  *u1 = 
    (const sort_struct_F4_I8_t *)p1;
  const sort_struct_F4_I8_t  *u2 = 
    (const sort_struct_F4_I8_t *)p2;
  float v1 = u1->val1_F4;
  float v2 = u2->val1_F4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int64_t t1 = u1->val2_I8;
    int64_t t2 = u2->val2_I8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F4_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F4_I8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F4_I8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F4 = ((const float * const)s1ptr)[i];
    X[i].val2_I8 = ((const int64_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F4_I8_t), sortcompare_F4_I8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F4_F4.h"
static int
sortcompare_F4_F4(const void *p1, const void *p2)
{
  const sort_struct_F4_F4_t  *u1 = 
    (const sort_struct_F4_F4_t *)p1;
  const sort_struct_F4_F4_t  *u2 = 
    (const sort_struct_F4_F4_t *)p2;
  float v1 = u1->val1_F4;
  float v2 = u2->val1_F4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    float t1 = u1->val2_F4;
    float t2 = u2->val2_F4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F4_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F4_F4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F4_F4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F4 = ((const float * const)s1ptr)[i];
    X[i].val2_F4 = ((const float * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F4_F4_t), sortcompare_F4_F4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F4_F8.h"
static int
sortcompare_F4_F8(const void *p1, const void *p2)
{
  const sort_struct_F4_F8_t  *u1 = 
    (const sort_struct_F4_F8_t *)p1;
  const sort_struct_F4_F8_t  *u2 = 
    (const sort_struct_F4_F8_t *)p2;
  float v1 = u1->val1_F4;
  float v2 = u2->val1_F4;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    double t1 = u1->val2_F8;
    double t2 = u2->val2_F8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F4_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F4_F8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F4 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F4_F8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F4 = ((const float * const)s1ptr)[i];
    X[i].val2_F8 = ((const double * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F4_F8_t), sortcompare_F4_F8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F8_I1.h"
static int
sortcompare_F8_I1(const void *p1, const void *p2)
{
  const sort_struct_F8_I1_t  *u1 = 
    (const sort_struct_F8_I1_t *)p1;
  const sort_struct_F8_I1_t  *u2 = 
    (const sort_struct_F8_I1_t *)p2;
  double v1 = u1->val1_F8;
  double v2 = u2->val1_F8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int8_t t1 = u1->val2_I1;
    int8_t t2 = u2->val2_I1;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F8_I1(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F8_I1_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I1 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F8_I1_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F8 = ((const double * const)s1ptr)[i];
    X[i].val2_I1 = ((const int8_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F8_I1_t), sortcompare_F8_I1);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F8_I2.h"
static int
sortcompare_F8_I2(const void *p1, const void *p2)
{
  const sort_struct_F8_I2_t  *u1 = 
    (const sort_struct_F8_I2_t *)p1;
  const sort_struct_F8_I2_t  *u2 = 
    (const sort_struct_F8_I2_t *)p2;
  double v1 = u1->val1_F8;
  double v2 = u2->val1_F8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int16_t t1 = u1->val2_I2;
    int16_t t2 = u2->val2_I2;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F8_I2(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F8_I2_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I2 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F8_I2_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F8 = ((const double * const)s1ptr)[i];
    X[i].val2_I2 = ((const int16_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F8_I2_t), sortcompare_F8_I2);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F8_I4.h"
static int
sortcompare_F8_I4(const void *p1, const void *p2)
{
  const sort_struct_F8_I4_t  *u1 = 
    (const sort_struct_F8_I4_t *)p1;
  const sort_struct_F8_I4_t  *u2 = 
    (const sort_struct_F8_I4_t *)p2;
  double v1 = u1->val1_F8;
  double v2 = u2->val1_F8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int32_t t1 = u1->val2_I4;
    int32_t t2 = u2->val2_I4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F8_I4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F8_I4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F8_I4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F8 = ((const double * const)s1ptr)[i];
    X[i].val2_I4 = ((const int32_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F8_I4_t), sortcompare_F8_I4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F8_I8.h"
static int
sortcompare_F8_I8(const void *p1, const void *p2)
{
  const sort_struct_F8_I8_t  *u1 = 
    (const sort_struct_F8_I8_t *)p1;
  const sort_struct_F8_I8_t  *u2 = 
    (const sort_struct_F8_I8_t *)p2;
  double v1 = u1->val1_F8;
  double v2 = u2->val1_F8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    int64_t t1 = u1->val2_I8;
    int64_t t2 = u2->val2_I8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F8_I8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F8_I8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != I8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F8_I8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F8 = ((const double * const)s1ptr)[i];
    X[i].val2_I8 = ((const int64_t * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F8_I8_t), sortcompare_F8_I8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F8_F4.h"
static int
sortcompare_F8_F4(const void *p1, const void *p2)
{
  const sort_struct_F8_F4_t  *u1 = 
    (const sort_struct_F8_F4_t *)p1;
  const sort_struct_F8_F4_t  *u2 = 
    (const sort_struct_F8_F4_t *)p2;
  double v1 = u1->val1_F8;
  double v2 = u2->val1_F8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    float t1 = u1->val2_F4;
    float t2 = u2->val2_F4;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F8_F4(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F8_F4_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F4 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F8_F4_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F8 = ((const double * const)s1ptr)[i];
    X[i].val2_F4 = ((const float * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F8_F4_t), sortcompare_F8_F4);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}

#include "mk_sort_idx_F8_F8.h"
static int
sortcompare_F8_F8(const void *p1, const void *p2)
{
  const sort_struct_F8_F8_t  *u1 = 
    (const sort_struct_F8_F8_t *)p1;
  const sort_struct_F8_F8_t  *u2 = 
    (const sort_struct_F8_F8_t *)p2;
  double v1 = u1->val1_F8;
  double v2 = u2->val1_F8;
  if ( v1 < v2 ) { 
    return -1;
  }
  else if ( v1 > v2 ) {
    return 1; 
  }
  else { 
    double t1 = u1->val2_F8;
    double t2 = u2->val2_F8;
    if ( t1 < t2 ) { 
      return -1;
    }
    else {
      return 1;
    }
  }
}

int
mk_sort_idx_F8_F8(
    const QDF_REC_TYPE * const src1, 
    const QDF_REC_TYPE * const src2, 
    QDF_REC_TYPE *dst // output 
   )
{
  int status = 0;
  sort_struct_F8_F8_t *X = NULL;

  mcr_chk_non_null(src1, -1); 
  char *s1x = src1->data; 
  uint32_t s1n = get_arr_len(s1x); 
  if ( s1n < 1 ) { go_BYE(-1); }
  jtype_t s1jtype = get_jtype(s1x); 
  if ( s1jtype != j_array ) { go_BYE(-1); }
  qtype_t s1qtype = get_qtype(s1x); 
  if ( s1qtype != F8 ) { go_BYE(-1); }
  const char * const *s1ptr = get_arr_ptr(s1x); 
  //==============================
  mcr_chk_non_null(src2, -1); 
  char *s2x = src2->data; 
  uint32_t s2n = get_arr_len(s2x); 
  if ( s2n < 1 ) { go_BYE(-1); }
  jtype_t s2jtype = get_jtype(s2x); 
  if ( s2jtype != j_array ) { go_BYE(-1); }
  qtype_t s2qtype = get_qtype(s2x); 
  if ( s2qtype != F8 ) { go_BYE(-1); }
  const char * const *s2ptr = get_arr_ptr(s2x); 
  //==============================
  if ( s1n != s2n ) { go_BYE(-1); }
  //==============================
  uint32_t n = s1n;
  X = malloc(n * sizeof(sort_struct_F8_F8_t));
  return_if_malloc_failed(X); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    X[i].idx = i;
    X[i].val1_F8 = ((const double * const)s1ptr)[i];
    X[i].val2_F8 = ((const double * const)s2ptr)[i];
  }
  qsort(X, n, sizeof(sort_struct_F8_F8_t), sortcompare_F8_F8);
  status = make_num_array(NULL, n, 0, I4,  dst); cBYE(status);
  char *dstptr = get_arr_ptr(dst->data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    ((int32_t * )dstptr)[i] = (int32_t)X[i].idx;
  }
BYE:
  free_if_non_null(X);
  return status;
}
