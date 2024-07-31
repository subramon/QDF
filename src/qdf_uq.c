#include "incs.h"
#include "qdf_struct.h"
#include "lookup8.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "sort1.h"
#include "unique.h"
#include "qdf_uq.h"

int
x_num_unique(
    const QDF_REC_TYPE * const ptr_qdf,
    const QDF_REC_TYPE * const ptr_nn_qdf,
    uint32_t *ptr_n_uq
    )
{
  int status = 0;
  char *vals = NULL; char *nn_vals = NULL;
  uint32_t num;
  //---------------------------
  mcr_chk_non_null(ptr_qdf, -1); 
  char *x = ptr_qdf->data;
  jtype_t jtype = get_jtype(x); if ( jtype != j_array ) { go_BYE(-1); }
  qtype_t qtype = get_qtype(x); if ( qtype == Q0 ) { go_BYE(-1); }
  const char * const valptr = get_arr_ptr(x); 
  uint32_t w = get_arr_width(x);       if ( w == 0 ) { go_BYE(-1); }
  uint32_t n = get_arr_len(x); 

  // copy values of qdf into "vals" since we modify (sort) to calc unique
  vals = malloc(w * n);
  return_if_malloc_failed(vals);
  memcpy(vals, valptr, n*w);
  //---------------------------
  if ( ptr_nn_qdf != NULL ) {
    mcr_chk_non_null(ptr_nn_qdf, -1); 
    char *nn_x = ptr_nn_qdf->data;
    jtype_t nn_jtype = get_jtype(nn_x); if ( nn_jtype != j_array ) { go_BYE(-1); }
    qtype_t nn_qtype = get_qtype(nn_x); if ( nn_qtype == I1 ) { go_BYE(-1); }
    const char * const nn_valptr = get_arr_ptr(nn_x);
    uint32_t nn_w = get_arr_width(nn_x);  if ( nn_w != 1 ) { go_BYE(-1); } 
    uint32_t nn_n = get_arr_len(nn_x);    if ( nn_n != n ) { go_BYE(-1); }
    nn_vals = malloc(nn_w * nn_n);
    return_if_malloc_failed(nn_vals);
    memcpy(nn_vals, nn_valptr, nn_n*nn_w);
  }
  //---------------------------
  status = num_unique(vals, nn_vals, n, qtype, &num); cBYE(status);
  *ptr_n_uq = num;
BYE:
  free_if_non_null(vals);
  free_if_non_null(nn_vals);
  return status;
}
//---------------------------------------------
int
x_is_unique(
    const QDF_REC_TYPE *const ptr_qdf,
    const QDF_REC_TYPE *const ptr_nn_qdf,
    bool *ptr_is_uq
    )
{
  int status = 0;
  const char *nnptr = NULL;
  //---------------------------
  mcr_chk_non_null(ptr_qdf, -1); 
  char *x = ptr_qdf->data;
  jtype_t jtype = get_jtype(x); if ( jtype != j_array ) { go_BYE(-1); }
  qtype_t qtype = get_qtype(x); if ( qtype == Q0 ) { go_BYE(-1); }
  const char * const valptr = get_arr_ptr(x);
  uint32_t n = get_arr_len(x);

  //---------------------------
  if ( ptr_nn_qdf != NULL ) {
    mcr_chk_non_null(ptr_nn_qdf, -1); 
    char *nn_x = ptr_nn_qdf->data;
    jtype_t nn_jtype = get_jtype(nn_x); if ( nn_jtype != j_array ) { go_BYE(-1); }
    qtype_t nn_qtype = get_qtype(nn_x); if ( nn_qtype == I1 ) { go_BYE(-1); }
    nnptr = get_arr_ptr(nn_x);
    uint32_t nn_n = get_arr_len(nn_x); if ( nn_n != n ) { go_BYE(-1); }
  }
  //---------------------------
  *ptr_is_uq = is_unique(valptr, nnptr, n, qtype); 
BYE:
  return status; 
}

int
x_num_unique_saturated(
    const QDF_REC_TYPE *const ptr_qdf,
    uint32_t max_n_uq,
    uint32_t * ptr_n_uq
    )
{
  int status = 0;
  //---------------------------
  mcr_chk_non_null(ptr_qdf, -1); 
  if ( max_n_uq == 0 ) { go_BYE(-1); }
  char *x = ptr_qdf->data;
  jtype_t jtype = get_jtype(x); if ( jtype != j_array ) { go_BYE(-1); }
  qtype_t qtype = get_qtype(x); if ( qtype == Q0 ) { go_BYE(-1); }
  const char * const valptr = get_arr_ptr(x);
  uint32_t n = get_arr_len(x);

  status = num_unique_saturated(valptr, n, qtype, max_n_uq, ptr_n_uq);
BYE:
  return status;
}
