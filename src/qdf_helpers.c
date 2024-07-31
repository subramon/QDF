#include "incs.h"
#include "macros.h"
#include "qdf_struct.h"
#include "qtypes.h"
#include "lookup8.h"
#include "qdf_checkers.h"
#include "qdf_helpers.h"

//--------------------------------------------------
int
set_is_df(
    void *x,
    bool  is_df
    )
{
  int status = 0;
  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  if ( xp->jtype != j_object ) { go_BYE(-1); }
  xp->is_df = is_df; 
BYE:
  return status; 
}
//--------------------------------------------------

bool
get_is_df(
    const void * const x
    )
{
  const qdf_object_hdr_t *const xp = (const qdf_object_hdr_t * const)x;
  if ( xp->jtype != j_object ) { WHEREAMI; }
  return xp->is_df;
}
//--------------------------------------------------
int
set_arr_width(
    void *x,
    uint32_t  width
    )
{
  int status = 0;
  // 16 bits for width => max is 65535
  if ( width >= 65535 ) { go_BYE(-1); } 
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  xp->width = (uint16_t)width;
BYE:
  return status;
}
//--------------------------------------------------
uint32_t 
get_arr_width(
    const void * const x
    )
{
  const qdf_array_hdr_t *const xp = (const qdf_array_hdr_t * const)x;
  if ( xp->jtype != j_array ) { WHEREAMI; return UINT_MAX; }
  return (uint32_t)xp->width;
}
//--------------------------------------------------
void
set_jtype(
    void *x,
    jtype_t  jtype
    )
{
  qdf_hdr_t *xp = (qdf_hdr_t *)x;
  xp->jtype = jtype; 
}
//--------------------------------------------------
jtype_t 
get_jtype(
    const void * const x
    )
{
  const qdf_hdr_t *const xp = (const qdf_hdr_t * const)x;
  return (jtype_t)xp->jtype;
}
//--------------------------------------------------
void
set_qtype(
    void *x,
    qtype_t qtype
    )
{
  qdf_hdr_t *xp = (qdf_hdr_t * )x;
  xp->qtype = qtype;
}
//--------------------------------------------------
qtype_t 
get_qtype(
    const void * const x
    )
{
  const qdf_hdr_t *const xp = (const qdf_hdr_t * const)x;
  return (qtype_t)xp->qtype;
}
//--------------------------------------------------
uint32_t
get_poff(
    const void * const x
    )
{
  const qdf_hdr_t *const xp = (const qdf_hdr_t * const)x;
  return xp->poff;
}
//--------------------------------------------------
int
set_str_len(
    void *x,
    uint32_t str_len
    )
{
  int status = 0;
  qdf_string_hdr_t *xp = (qdf_string_hdr_t *)x;
  if ( xp->jtype != j_string ) { go_BYE(-1); }
  xp->str_len = str_len;
BYE:
  return status;
}
//--------------------------------------------------
uint32_t
get_str_len(
    const void * const x
    )
{
  const qdf_string_hdr_t *const xp = (const qdf_string_hdr_t * const)x;
  if ( xp->jtype != j_string ) { 
    WHEREAMI; return UINT_MAX; }
  return xp->str_len;
}
//--------------------------------------------------
int
set_str_size(
    void *x,
    uint32_t str_size
    )
{
  int status = 0;
  qdf_string_hdr_t *xp = (qdf_string_hdr_t *)x;
  if ( xp->jtype != j_string ) { go_BYE(-1); }
  xp->str_size = str_size;
BYE:
  return status; 
}
//--------------------------------------------------
uint32_t
get_str_size(
    const void * const x
    )
{
  const qdf_string_hdr_t *const xp = (const qdf_string_hdr_t * const)x;
  if ( xp->jtype != j_string ) { WHEREAMI; return UINT_MAX; }
  return xp->str_size;
}
//--------------------------------------------------
// df_arr_size -> non-zero if objct is a dataframe
int
set_obj_arr_size(
    void *x,
    uint32_t arr_size
    )
{
  int status = 0;
  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  if ( xp->jtype != j_object ) { go_BYE(-1); } 
  if ( !xp->is_df ) { go_BYE(-1); } 
  xp->obj_arr_size = arr_size;
BYE:
  return status;
}
//--------------------------------------------------
uint32_t
get_obj_arr_size(
    const void * const x
    )
{
  const qdf_object_hdr_t *const xp = (const qdf_object_hdr_t * const)x;
  if ( xp->jtype != j_object ) { WHEREAMI; return UINT_MAX; }
  return xp->obj_arr_size;
}
//-------------------------------------------------
// obj_arr_len -> non-zero if objct is a dataframe
int
set_obj_arr_len(
    void *x,
    uint32_t arr_len
    )
{
  int status = 0;
  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  if ( !xp->is_df ) { go_BYE(-1); } 
  if ( xp->jtype != j_object ) { go_BYE(-1); }
  xp->obj_arr_len = arr_len;
BYE:
  return status;
}
//-------------------------------------------------
uint32_t
get_obj_arr_len(
    const void * const x
    )
{
  const qdf_object_hdr_t *const xp = (const qdf_object_hdr_t * const)x;
  if ( xp->jtype != j_object ) { WHEREAMI; return UINT_MAX; }
  return xp->obj_arr_len;
}
//-------------------------------------------------
// obj_len is number of elements in object
int
set_obj_len(
    void *x,
    uint32_t obj_len
    )
{
  int status = 0;
  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  if ( xp->jtype != j_object ) { go_BYE(-1); } 
  xp->obj_len = obj_len;
BYE:
  return status;
}
//-------------------------------------------------
uint32_t
get_obj_len(
    const void * const x
    )
{
  const qdf_object_hdr_t *const xp = (const qdf_object_hdr_t *const )x;
  return xp->obj_len;
}
//-------------------------------------------------
int
set_arr_len(
    void *x,
    uint32_t arr_len
    )
{
  int status = 0;
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  if ( xp->jtype != j_array ) { go_BYE(-1); } 
  xp->arr_len = arr_len;
BYE:
  return status;
}
//-------------------------------------------------
uint32_t
get_arr_len(
    const void * const x
    )
{
  const qdf_array_hdr_t *const xp = (const qdf_array_hdr_t *const )x;
  if ( xp->jtype != j_array ) { WHEREAMI; return UINT_MAX; }
  return xp->arr_len;
}
//-------------------------------------------------
int
set_arr_size(
    void *x,
    uint32_t arr_size
    )
{
  int status = 0;
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  if ( xp->jtype != j_array ) { go_BYE(-1); } 
  xp->arr_size = arr_size;
BYE:
  return status;
}
//-------------------------------------------------
uint32_t
get_arr_size(
    const void * const x
    )
{
  const qdf_array_hdr_t *const xp = (const qdf_array_hdr_t *const )x;
  if ( xp->jtype != j_array ) { WHEREAMI; return UINT_MAX; }
  return xp->arr_size;
}
//-------------------------------------------------
int
set_num_val(
    void *x,
    double num_val
    )
{
  int status = 0;
  qdf_number_hdr_t *xp = (qdf_number_hdr_t *)x;
  if ( xp->jtype != j_number ) { go_BYE(-1); }
  xp->num_val = num_val;
BYE:
  return status;
}
//-------------------------------------------------
double
get_num_val(
    const void * const x
    )
{
  const qdf_number_hdr_t *const xp = (const qdf_number_hdr_t *const )x;
  if ( xp->jtype != j_number ) { WHEREAMI; return DBL_MAX; }
  return xp->num_val;
}
//-------------------------------------------------
int
set_date_val(
    void *x,
    tm_t tm_val
    )
{
  int status = 0;
  qdf_date_hdr_t *xp = (qdf_date_hdr_t *)x;
  if ( xp->jtype != j_date ) { go_BYE(-1); }
  xp->tm_val = tm_val;
BYE:
  return status; 
}
//-------------------------------------------------
int
get_date_val(
    const void * const x,
    tm_t *ptr_tm 
    )
{
  int status = 0;
  const qdf_date_hdr_t *const xp = (const qdf_date_hdr_t *const )x;
  if ( xp->jtype != j_date ) { go_BYE(-1); }
  *ptr_tm =  xp->tm_val;
BYE:
  return status;
}
//-------------------------------------------------
// TODO P1 Isn't this dangerous? if new str val is too long?
int
set_str_val(
    void *x,
    const char * const str_val 
    )
{
  int status = 0;
  qdf_string_hdr_t *xp = (qdf_string_hdr_t *)x;
  if ( xp->jtype != j_string ) { go_BYE(-1); }
  if ( strlen(str_val) >= xp->str_size ) { go_BYE(-1); }
  char *dstptr = (char *)(xp+1); 
  // TODO P1 delete following check once verified 
  if (((char *)dstptr - (char *)x ) != 16 ) { go_BYE(-1); }
  strcpy(dstptr, str_val); 
BYE:
  return status;
}
//-------------------------------------------------
char * 
get_str_val(
    void * x
    )
{
  qdf_string_hdr_t *xp = (qdf_string_hdr_t *)x;
  if ( xp->jtype != j_string ) { WHEREAMI; return NULL; }
  return (char *)(xp+1); 
}
//-------------------------------------------------
int
set_bool_val(
    void *x,
    bool bool_val
    )
{
  int status = 0;
  qdf_bool_hdr_t *xp = (qdf_bool_hdr_t *)x;
  if ( xp->jtype != j_bool ) { go_BYE(-1); }
  xp->bool_val  = bool_val; 
BYE:
  return status;
}
bool
get_bool_val(
    const void * const x
    )
{
  const qdf_bool_hdr_t *const xp = (const qdf_bool_hdr_t *const )x;
  if ( xp->jtype != j_bool ) { WHEREAMI; return false; }
  return xp->bool_val;
}
//-------------------------------------------------
int
free_qdf(
    QDF_REC_TYPE *ptr_qdf
    ) 
{
  int status = 0;
  if ( ptr_qdf == NULL ) { go_BYE(-1); }
  if ( ptr_qdf->is_foreign == false ) { 
    if ( ptr_qdf->is_mmap ) { 
      rs_munmap(ptr_qdf->data, ptr_qdf->size);
    }
    else {
      free_if_non_null(ptr_qdf->data);;
    }
  }
  memset(ptr_qdf, 0, sizeof(QDF_REC_TYPE));
BYE:
  return status;
}
//-------------------------------------------------
// This makes a new qdf TODO P1 Compare this with clone and
// reconcile the 2 implementations if overlap
int
copy_qdf(
    QDF_REC_TYPE *ptr_dst,
    const QDF_REC_TYPE * const ptr_src
    )
{
  int status = 0; 

  mcr_chk_null(ptr_dst, -1);
  mcr_chk_non_null(ptr_src, -1);

  size_t sz = ptr_src->size;
  status = posix_memalign(&(ptr_dst->data), 16, sz); cBYE(status);
  memcpy(ptr_dst->data, ptr_src->data, sz);
  ptr_dst->size = sz;
BYE:
  return status;
}
//-------------------------------------------------
// only for non-uniform arrays
uint32_t * 
get_offsets_in_arr(
    void * x
    )
{
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  if ( xp->qtype != Q0 ) { WHEREAMI; return NULL; }
  return (uint32_t *)(xp+1); 
}
//-------------------------------------------------
// following only for uniform arrays 
int
get_arr_val(
    void * x,
    uint32_t idx,
    SCLR_REC_TYPE *out_sclr,
    QDF_REC_TYPE *out_qdf
    )
{
  int status = 0;
  jtype_t jtype = mcr_get_jtype(x); 
  qtype_t qtype = mcr_get_qtype(x); 
  uint32_t width = get_arr_width(x); 
  uint32_t arr_len = get_arr_len(x); 

  if ( jtype != j_array ) { go_BYE(-1); }
  if ( idx >= arr_len ) { go_BYE(-1); }

  if ( qtype == Q0 ) { // handle mixed arrays separately from uniform
    mcr_chk_null(out_qdf, -1); 
    if ( width != 0 ) { go_BYE(-1);  }
    // jump to start of offsets
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    void * const ox = get_offsets_in_arr(x);  
    if ( ox == NULL ) { go_BYE(-1); } 
    status = get_arr_val(ox, idx, &sclr, NULL); 
    if ( sclr.qtype != I4 ) { go_BYE(-1); } 
    if ( sclr.val.i4 <= 0 ) { go_BYE(-1); } 
    uint32_t offset = (uint32_t)sclr.val.i4; 
    char * vx = (char *)x + offset;
    uint32_t size = get_qdf_size((void *)vx);
    out_qdf->data = vx; out_qdf->size = size; out_qdf->is_foreign = true;
#ifdef DEBUG
    status = chk_qdf(out_qdf); cBYE(status);
#endif
    return status;
  }
  // now we are in the uniform array case
  if ( out_sclr == NULL ) { go_BYE(-1); } 
  memset(out_sclr, 0, sizeof(SCLR_REC_TYPE));

  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  void *w = (void *)(xp+1);

  out_sclr->qtype = qtype;
  switch ( qtype ) {
    case BL : out_sclr->val.i1 = ((const bool * const)w)[idx]; break; 

    case I1 : out_sclr->val.i1 = ((const int8_t * const)w)[idx]; break; 
    case I2 : out_sclr->val.i2 = ((const int16_t * const)w)[idx]; break; 
    case I4 : out_sclr->val.i4 = ((const int32_t * const)w)[idx]; break; 
    case I8 : out_sclr->val.i8 = ((const int64_t * const)w)[idx]; break; 

    case UI1 : out_sclr->val.ui1 = ((const uint8_t * const)w)[idx]; break; 
    case UI2 : out_sclr->val.ui2 = ((const uint16_t * const)w)[idx]; break; 
    case UI4 : out_sclr->val.ui4 = ((const uint32_t * const)w)[idx]; break; 
    case UI8 : out_sclr->val.ui8 = ((const uint64_t * const)w)[idx]; break; 

    case F4 : out_sclr->val.f4 = ((const float * const)w)[idx]; break; 
    case F8 : out_sclr->val.f8 = ((const double * const)w)[idx]; break; 

    case SC : 
              if ( width == 0 ) { go_BYE(-1); }
              out_sclr->val.str = (char *)w + (idx*width);
              break;
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}

/*
uint32_t
get_num_keys(
    void *x
    )
{
  uint8_t *ux8 = (uint8_t *)x;
  ux8 += 24;
  // we are now positioned at start of SC array that stores keys
  int n_keys = get_arr_len(ux8);
  // TODO P1 Move this to checkers 
  if ( n_keys != ((uint32_t *)x)[2] ) { WHEREAMI; return 0xFFFFFFFF; } 
  return n_keys;
}
*/

// get pointer to keys QDF within object QDF 
void *
get_keys_in_obj(
    void *x
    )
{
  if ( x == NULL ) { return NULL; }
  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  if ( xp->jtype != j_object ) { WHEREAMI; return NULL; }
  // after header, next thing is QDF for keys. This is kx
  void *kx  = (void *)(&(xp[1]));
  return kx;
}
// get pointer to offsets QDF within object QDF 
void *
get_offsets_in_obj(
    void *x
    )
{
  void *kx = get_keys_in_obj(x); 
  if ( kx == NULL ) { WHEREAMI; return NULL; }
  qdf_array_hdr_t *kxp = (qdf_array_hdr_t *)kx;
  uint32_t key_size = kxp->qdf_size;
  void *ox = (void *)((char *)kx + key_size);
  return ox; 
}

int
get_keys(
    const QDF_REC_TYPE * const ptr_qdf,
    QDF_REC_TYPE *ptr_qdf_keys
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  mcr_chk_null(ptr_qdf_keys, -1); 

  void *kx = get_keys_in_obj(ptr_qdf->data); 
  qdf_array_hdr_t *kxp = (qdf_array_hdr_t *)kx; 
  if ( kxp->jtype != j_array ) { go_BYE(-1); } // keys is an array of SC
  if ( kxp->qtype != SC      ) { go_BYE(-1); } // keys is an array of SC
  ptr_qdf_keys->size = kxp->qdf_size; 
  ptr_qdf_keys->data = kx; 
  ptr_qdf_keys->is_foreign = true;

BYE:
  return status;
}
// following only for uniform arrays 
void * 
get_arr_ptr(
    void * x
    )
{
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  if ( xp->jtype != j_array ) { WHEREAMI; return NULL; }
  if ( xp->qtype == Q0 ) { WHEREAMI; return NULL; }
  void *kx  = (void *)(&(xp[1]));
  return kx;
}
// Note that the input is a uniform array of SC
int
is_val_in_SC_array(
    const QDF_REC_TYPE * const ptr_qdf, // input 
    const char * const key, // input 
    int32_t *ptr_idx  // output 
    )
{
  int status = 0;

  void *x = ptr_qdf->data;
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x; 
  if ( xp->jtype != j_array ) { go_BYE(-1); } // input is an array of SC
  if ( xp->qtype != SC      ) { go_BYE(-1); } // input is an array of SC
  uint32_t width = xp->width; if ( width == 0 ) { go_BYE(-1); } 
  uint32_t arr_len = xp->arr_len;

  char *cx = get_arr_ptr(x); 
  *ptr_idx = -1; // assume element not in array 
  for ( uint32_t i = 0; i < arr_len; i++ ) { 
    if ( strcmp(cx, key) == 0 ) {
      *ptr_idx = (int32_t)i; break;
    }
    cx += width;
  }
BYE:
  return status;
}
// TODO P2 Uniform treatment of array and object 
// Currently, this only works for objects
int
get_key_val(
    const QDF_REC_TYPE *const ptr_in,
    int nidx, // key described as index 
    const char * const sidx, // key described as string
    QDF_REC_TYPE *ptr_out,
    uint32_t *ptr_offset // how far the output QDF is from start of input QDF
   )
{
  int status = 0;
  uint32_t uidx; 

  mcr_chk_non_null(ptr_in, -1);
  mcr_chk_null(ptr_out, -1);
  // describe key either by EXACTLY ONE OF index or name 
  if ( ( nidx >= 0 ) && ( sidx != NULL ) ) { go_BYE(-1); }
  if ( ( nidx < 0  ) && ( sidx == NULL ) ) { go_BYE(-1); }
  //--------------------------------------------------------
  void *x = ptr_in->data;
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x; 
  if ( xp->jtype != j_object ) { go_BYE(-1); } 
  if ( nidx >= 0 ) { 
    uidx = (uint32_t)nidx;
  }
  else {
    int32_t kidx;
    QDF_REC_TYPE keys_qdf; memset(&keys_qdf, 0, sizeof(QDF_REC_TYPE));
    status = get_keys(ptr_in, &keys_qdf); cBYE(status);
#ifdef DEBUG
    status = chk_uniform_array(keys_qdf.data, SC); cBYE(status);
#endif
    status = is_val_in_SC_array(&keys_qdf, sidx, &kidx); cBYE(status);
    if ( kidx < 0 ) { 
      printf("Missing key %d, %s \n", nidx, sidx);  go_BYE(-1); 
    }
    uidx = (uint32_t)kidx;
  }
  //  position at start of offsets
  void * ox = get_offsets_in_obj(x); 
  if ( ox == NULL ) { go_BYE(-1); }
  SCLR_REC_TYPE off_sclr; memset(&off_sclr, 0, sizeof(QDF_REC_TYPE));
  status = get_arr_val(ox, uidx, &off_sclr, NULL); cBYE(status);
#ifdef DEBUG
  if ( off_sclr.qtype != I4 ) { go_BYE(-1); }
  if ( off_sclr.val.i4 <= 0 ) { go_BYE(-1); }
  if ( (uint32_t)off_sclr.val.i4 >= xp->qdf_size ) { go_BYE(-1); }
#endif
  void *vx = (void *)(((char *)x) + off_sclr.val.i4);
  // we are now positioned at start of value of this key 
  ptr_out->data = vx;
  ptr_out->size = get_qdf_size(vx);
  ptr_out->is_foreign = true;
  if ( ptr_offset != NULL ) { 
    if ( (char *)vx < (char *)x ) { go_BYE(-1); } 
    *ptr_offset = (uint32_t)((char *)vx - (char *)x);
  }
BYE:
  return status;
}

//
// following only for uniform arrays 
// This is the converse of get_arr_val() 
// In get_arr_val() where input is idx, output is val
// In get_arr_idx() where input is val, output is idx
// Note that if the value exists more that once, first idx returned
// If value does not exist, -1 is returned
int
get_arr_idx(
    void * x, // input 
    SCLR_REC_TYPE *ptr_sclr, // input 
    int32_t *ptr_idx // output 
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); }
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  if ( xp->jtype != j_array ) { go_BYE(-1); }
  if ( xp->qtype == Q0 ) { go_BYE(-1); }

  *ptr_idx = -1; 
  int32_t arr_len   = (int)xp->arr_len;
  uint32_t  str_width = xp->width;
  void * y = get_arr_ptr(x); 
  int idx = -1;

  switch ( xp->qtype ) {
    //------------------------------------------------
    case I1 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.i1 == ((int8_t *)y)[i] ) { idx = i; break; } 
      }
      break;
    case I2 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.i2 == ((int16_t * )y)[i] ) { idx = i; break; } 
      }
      break;
    case I4 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.i4 == ((int32_t * )y)[i] ) { idx = i; break; } 
      }
      break;
    case I8 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.i8 == ((int64_t * )y)[i] ) { idx = i; break; } 
      }
      break;
      //------------------------------------------------
    case UI1 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.ui1 == ((uint8_t * )y)[i] ) { idx = i; break; } 
      }
      break;
    case UI2 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.ui2 == ((uint16_t * )y)[i] ) { idx = i; break; } 
      }
      break;
    case UI4 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.ui4 == ((uint32_t * )y)[i] ) { idx = i; break; } 
      }
      break;
    case UI8 : 
      for ( int i = 0; i < arr_len; i++ ) { 
        if ( ptr_sclr->val.ui8 == ((uint64_t * )y)[i] ) { idx = i; break; } 
      }
      break;
      //------------------------------------------------
    case SC : 
      {
        char *cptr = (char *)y;
        if ( ptr_sclr->val.str == NULL ) { go_BYE(-1); }
        for ( int i = 0; i < arr_len; i++ ) { 
          if ( strcmp(ptr_sclr->val.str, cptr) == 0 ) { 
            idx = i; break; 
          } 
          cptr += str_width;
        }
      }
      //------------------------------------------------
      break;
    default : go_BYE(-1); break;
  }
  *ptr_idx = idx;
BYE:
  return status;
}

int
is_key(
    const QDF_REC_TYPE *const ptr_in,
    const char * const sidx, // key described as string
    bool *ptr_is_key
   )
{
  int status = 0;
  mcr_chk_non_null(ptr_in, -1); 
  if ( sidx     == NULL ) { go_BYE(-1); }

  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)ptr_in->data;
  if ( xp->jtype != j_object ) { go_BYE(-1); }

  int kidx;
  QDF_REC_TYPE kqdf; memset(&kqdf, 0, sizeof(QDF_REC_TYPE));
  status = get_keys(ptr_in, &kqdf); cBYE(status);
  status = is_val_in_SC_array(&kqdf, sidx, &kidx); cBYE(status);
  if ( kidx < 0 ) { *ptr_is_key = false; } else { *ptr_is_key = true; }
BYE:
  return status;
}
//------------------------------------------------------
bool
is_nil(
    const QDF_REC_TYPE *const ptr_in
    )
{
  if ( ptr_in == NULL ) { return false; }
  void *x = ptr_in->data;
  if ( x == NULL ) { return false; }
  jtype_t jtype = get_jtype(x);
  if ( jtype == j_nil ) { return true; } else { return false; }
}

//--------------------------------------------------------
void
zero_qdf(
    QDF_REC_TYPE *ptr_qdf
    )
{
  ptr_qdf->data = NULL;
  ptr_qdf->size = 0;
  ptr_qdf->is_mmap = false;
  ptr_qdf->is_foreign = false;
  ptr_qdf->is_read_only = false;
  ptr_qdf->junk = false;
  //-- following for debugging 
  ptr_qdf->id = 0;
  ptr_qdf->name = NULL; 
}
//--------------------------------------------------------
int
set_qdf_size(
    void *x,
    uint32_t qdf_size
    )
{
  int status = 0;
  jtype_t jtype = mcr_get_jtype(x);
  switch ( jtype ) {
    case j_nil     : case j_bool    : case j_number  : 
    case j_date    : case j_string  : 
      go_BYE(-1); 
    case j_array   : 
      {
      qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
      xp->qdf_size = qdf_size;
      }
      break;
    case j_object  : 
      {
      qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
      xp->qdf_size = qdf_size;
      }
      break;
    default : WHEREAMI; break;
  }
BYE:
  return status; 
}
uint32_t
get_qdf_size(
    const void * x
    )
{
  if ( x == NULL ) { return 0; } 
  jtype_t jtype = get_jtype(x);
  switch ( jtype ) {
    case j_nil     : case j_bool    : 
      {
        const qdf_bool_hdr_t *xp = (const qdf_bool_hdr_t *)x;
        return sizeof(qdf_bool_hdr_t); 
      }
    case j_number  : 
      {
        const qdf_number_hdr_t *xp = (const qdf_number_hdr_t *)x;
        return sizeof(qdf_number_hdr_t); 
      }
    case j_date  : 
      {
        const qdf_date_hdr_t *xp = (const qdf_date_hdr_t *)x;
        return sizeof(qdf_date_hdr_t); 
      }
    case j_string  : 
      {
        const qdf_string_hdr_t *xp = (const qdf_string_hdr_t *)x;
        return sizeof(qdf_string_hdr_t) + xp->str_size;
      }
    case j_array : 
      {
        const qdf_array_hdr_t *xp = (const qdf_array_hdr_t *)x;
        return xp->qdf_size;
      }
    case j_object : 
      {
        const qdf_object_hdr_t *xp = (const qdf_object_hdr_t *)x;
        return xp->qdf_size;
      }
    default : WHEREAMI; return 0; 
  }
}
int 
get_sclr_as_F8(
    const SCLR_REC_TYPE *const ptr_sclr,
    double *ptr_x
    )
{
  int status = 0;
  if ( ptr_sclr == NULL ) { go_BYE(-1); } 
  switch ( ptr_sclr->qtype ) { 

    case I1 : *ptr_x = ptr_sclr->val.i1; break;
    case I2 : *ptr_x = ptr_sclr->val.i2; break;
    case I4 : *ptr_x = ptr_sclr->val.i4; break;
    case I8 : *ptr_x = (double)ptr_sclr->val.i8; break;
    case F4 : *ptr_x = ptr_sclr->val.f4; break;
    case F8 : *ptr_x = ptr_sclr->val.f8; break;
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
int 
get_keys_as_array(
    QDF_REC_TYPE *ptr_qdf,
    char ***ptr_keys, 
    uint32_t *ptr_n_keys
    )
{
  int status = 0;
  uint32_t n_keys = 0;
  char **keys = NULL; // [n_keys] 
  //--------------------------------------------------
  if ( ptr_qdf == NULL ) { go_BYE(-1); } 
#ifdef DEBUG
  status = chk_qdf(ptr_qdf); cBYE(status);
#endif
  jtype_t jtype = get_jtype(ptr_qdf->data);
  if ( jtype != j_object ) { go_BYE(-1); }
  n_keys = get_obj_len(ptr_qdf->data);  if ( n_keys == 0 ) { go_BYE(-1); }
  //--------------------------------------------------
  // find out keys in old qdf 
  QDF_REC_TYPE keys_qdf; memset(&keys_qdf, 0, sizeof(QDF_REC_TYPE));
  status = get_keys(ptr_qdf, &keys_qdf);
  n_keys = get_obj_len(keys_qdf.data);
  if ( n_keys == 0 ) { go_BYE(-1); }
  keys = malloc(n_keys * sizeof(char *));
  memset(keys, 0, (n_keys * sizeof(char *)));
  //--------------------------------------------------
  for ( uint32_t i = 0; i < n_keys; i++ ) { // for each key in src 
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    status = get_arr_val(keys_qdf.data, i, &sclr, NULL);  cBYE(status);
    const char * const key = sclr.val.str;
    if ( key == NULL ) { go_BYE(-1); }
    keys[i] = strdup(key);
  }
  *ptr_keys = keys;
  *ptr_n_keys = n_keys;
BYE:
  return status;
}
int 
get_str_qtypes_as_array(
    QDF_REC_TYPE *ptr_qdf,
    char ***ptr_str_qtypes, 
    uint32_t *ptr_n_str_qtypes
    )
{
  int status = 0;
  uint32_t n_str_qtypes = 0;
  char **str_qtypes = NULL; // [n_keys] 
  //--------------------------------------------------
  if ( ptr_qdf == NULL ) { go_BYE(-1); } 
#ifdef DEBUG
  status = chk_qdf(ptr_qdf); cBYE(status);
#endif
  if ( !get_is_df(ptr_qdf->data) ) { go_BYE(-1); }
  n_str_qtypes = get_obj_len(ptr_qdf->data);  
  if ( n_str_qtypes == 0 ) { go_BYE(-1); }
  str_qtypes = malloc(n_str_qtypes * sizeof(char *));
  //--------------------------------------------------
  for ( int i = 0; i < (int)n_str_qtypes; i++ ) { // for each key in src 
    QDF_REC_TYPE col_qdf; memset(&col_qdf, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_qdf, i, NULL, &col_qdf, NULL); cBYE(status);
    const char *str_qtype = get_str_qtype(get_qtype(col_qdf.data));
    if ( str_qtype == NULL ) { go_BYE(-1); }
    str_qtypes[i] = strdup(str_qtype);
  }
  *ptr_str_qtypes = str_qtypes;
  *ptr_n_str_qtypes = n_str_qtypes;
BYE:
  return status;
}
