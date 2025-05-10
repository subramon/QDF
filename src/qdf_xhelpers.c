#include "incs.h"
#include "qdf_struct.h"
#include "qdf_checkers.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
//-------------------------------------
bool 
x_get_is_df(
    const QDF_REC_TYPE * const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, false); 
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_object ) { WHEREAMI; return false; } 
  return get_is_df(ptr_qdf->data); 
}
//-------------------------------------
uint32_t
x_get_obj_arr_size(
    const QDF_REC_TYPE * const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, (uint32_t)0xFFFFFFFF); 
  if ( !get_is_df(ptr_qdf->data) ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  return get_obj_arr_size(ptr_qdf->data); 
}
//-------------------------------------
uint32_t
x_get_obj_arr_len(
    const QDF_REC_TYPE * const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, (uint32_t)0xFFFFFFFF); 
  if ( !get_is_df(ptr_qdf->data) ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  return get_obj_arr_len(ptr_qdf->data); 
}
//-------------------------------------
uint32_t
x_get_obj_len(
    const QDF_REC_TYPE *const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, (uint32_t)0xFFFFFFFF); 
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_object ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  return get_obj_len(ptr_qdf->data); 
}
//-------------------------------------
uint32_t
x_get_arr_len(
    const QDF_REC_TYPE *const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, (uint32_t)0xFFFFFFFF);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_array ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  return get_arr_len(ptr_qdf->data); 
}
//-------------------------------------
uint32_t
x_get_arr_width(
    const QDF_REC_TYPE *const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, (uint32_t)0xFFFFFFFF);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_array ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  qtype_t qtype = get_qtype(ptr_qdf->data);
  if ( qtype == Q0 ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  return get_arr_width(ptr_qdf->data); 
}
//-------------------------------------
uint32_t
x_get_arr_size(
    const QDF_REC_TYPE *const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, (uint32_t)0xFFFFFFFF);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_array ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  return get_arr_size(ptr_qdf->data); 
}
//-------------------------------------
jtype_t 
x_get_jtype( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, j_undef); 
  return get_jtype(ptr_qdf->data);
}
//--------------------------------------------------
uint32_t
x_get_poff( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, 0); 
  return get_poff(ptr_qdf->data); 
}
//--------------------------------------------------
qtype_t
x_get_qtype( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, Q0); 
  return get_qtype(ptr_qdf->data); 
}
//--------------------------------------------------
uint32_t
x_get_qdf_size(
    const QDF_REC_TYPE * const ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, 0);
  uint32_t n = ptr_qdf->size;
#ifdef DEBUG
  uint32_t chk_n = get_qdf_size(ptr_qdf->data);
  if ( chk_n != n ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; } 
#endif
  return ptr_qdf->size;
}

//--------------------------------------------------
double
x_get_num_val( // x_* functions are to be invoked from Lua 
    QDF_REC_TYPE *ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, 0);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_number ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  return get_num_val(ptr_qdf->data);
}
//-------------------------------------------------
int
x_get_date_val( // x_* functions are to be invoked from Lua 
    const QDF_REC_TYPE *const ptr_qdf,
    tm_t *ptr_tm
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_date ) { go_BYE(-1); } 
  status = get_date_val(ptr_qdf->data, ptr_tm); cBYE(status);
BYE:
  return status; 
}
//-------------------------------------------------
char * 
x_get_str_val( // x_* functions are to be invoked from Lua 
    QDF_REC_TYPE *ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, NULL);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_string ) { WHEREAMI; return NULL; }
  return get_str_val(ptr_qdf->data);
}
//-------------------------------------------------
bool
x_get_bool_val( // x_* functions are to be invoked from Lua 
    QDF_REC_TYPE *ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, false);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_bool ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; }
  bool bval = get_bool_val(ptr_qdf->data);
  return bval;
}
//-------------------------------------------------
uint32_t
x_get_str_len(
    QDF_REC_TYPE *ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, 0);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_string ) { WHEREAMI; return (uint32_t)0xFFFFFFFF; } 
  return get_str_len(ptr_qdf->data);
}
//-------------------------------------------------
// following only for uniform arrays 
int
x_get_arr_ptr(
    QDF_REC_TYPE *ptr_in,
    QDF_REC_TYPE *ptr_out
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_in, -1);

  if ( ptr_out == NULL ) { go_BYE(-1); }
  memset(ptr_out, 0, sizeof(QDF_REC_TYPE));

  qtype_t qtype = get_qtype(ptr_in->data); 
  if ( qtype == Q0 ) { go_BYE(-1); } 
  jtype_t jtype = get_jtype(ptr_in->data); 
  if ( jtype != j_array ) { go_BYE(-1); } 
  //---------------------------------------

  char * y = get_arr_ptr(ptr_in->data);
  if ( y == NULL ) { go_BYE(-1); } 
  ptr_out->data = y;
  ptr_out->is_foreign = true;
  ptr_out->size = ptr_in->size;
BYE:
  return status;
}
//-------------------------------------------------
char *
x_get_offsets(
    QDF_REC_TYPE *ptr_qdf
    )
{
  mcr_chk_non_null(ptr_qdf, NULL);
  char *x = ptr_qdf->data;
  jtype_t jtype = get_jtype(x); 
  if ( jtype != j_array ) { WHEREAMI; return NULL; } 
  qtype_t qtype = get_qtype(x); 
  if ( qtype != Q0 ) { WHEREAMI; return NULL; } 
  x = ptr_qdf->data; x+= 24;
#ifdef DEBUG
  int status = chk_uniform_array(x, I4); 
  if ( status != 0 ) { WHEREAMI; return NULL; }
#endif
  return x;
}
//-------------------------------------------------
// DANGEROUS --- USE WITH CAUTION
int
x_set_qtype(
    const QDF_REC_TYPE * const ptr_qdf,
    qtype_t qtype
    )
{
  int status = 0; 
  mcr_chk_non_null(ptr_qdf, -1); 
  status = chk_qtype(qtype); cBYE(status);
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( ( jtype == j_number ) || ( jtype == j_array ) ) {
    set_qtype(ptr_qdf->data, qtype);
  }
  else {
    go_BYE(-1);
  }
BYE:
  return status;
}
//-------------------------------------
int
x_set_foreign(
    QDF_REC_TYPE * ptr_qdf
    )
{
  int status = 0; 
  mcr_chk_non_null(ptr_qdf, -1); 
  ptr_qdf->is_foreign = true;
BYE:
  return status;
}
//-------------------------------------
int
x_set_obj_arr_size(
    const QDF_REC_TYPE * const ptr_qdf,
    uint32_t arr_size
    )
{
  int status = 0; 
  mcr_chk_non_null(ptr_qdf, -1); 
  if ( !get_is_df(ptr_qdf->data) ) { go_BYE(-1); } 
  set_obj_arr_size(ptr_qdf->data, arr_size); 
BYE:
  return status;
}
//-------------------------------------
int
x_set_obj_arr_len(
    const QDF_REC_TYPE * const ptr_qdf,
    uint32_t arr_len
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  if ( !get_is_df(ptr_qdf->data) ) { go_BYE(-1); }
  set_obj_arr_len(ptr_qdf->data, arr_len); 
BYE:
  return status; 
}
//-------------------------------------
int
x_set_arr_len(
    const QDF_REC_TYPE * const ptr_qdf,
    uint32_t arr_len
    )
{
  int status = 0;
  if ( ptr_qdf == NULL ) { go_BYE(-1); } 
  mcr_chk_non_null(ptr_qdf, -1); 
  set_arr_len(ptr_qdf->data, arr_len); 
BYE:
  return status; 
}
//-------------------------------------
int
x_set_is_df(
    const QDF_REC_TYPE * const ptr_qdf,
    bool is_df
    )
{
  int status = 0; 
  mcr_chk_non_null(ptr_qdf, -1); 
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_object ) { go_BYE(-1); }
  set_is_df(ptr_qdf->data, is_df);
BYE:
  return status; 
}
//-------------------------------------
int
x_set_num_val(
    const QDF_REC_TYPE * const ptr_qdf,
    double num_val
    )
{
  int status = 0; 
  mcr_chk_non_null(ptr_qdf, -1); 
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_number ) { go_BYE(-1); }
  set_num_val(ptr_qdf->data, num_val); 
BYE:
  return status; 
}
//-------------------------------------
