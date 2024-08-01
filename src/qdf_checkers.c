#include "incs.h"
#include "macros.h"
#include "multiple.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"

int
chk_qtype(
    qtype_t qtype
    )
{
  int status = 0;
  switch ( qtype ) { 
    case Q0 : case BL : 

    case I1 : case I2 : case I4 : case I8 : 

    case F2 : case F4 : case F8 : 

    case UI1 : case UI2 : case UI4 : case UI8 : 

    case SC : case SV : case TM1 : 
      break;
    default : 
      go_BYE(-1);
  }
BYE:
  return status;
}
int
chk_jtype(
    jtype_t jtype,
    qtype_t qtype
    )
{
  int status = 0;

  status = chk_qtype(qtype); cBYE(status);
  switch ( jtype ) { 
    case j_nil : 
      if ( qtype != Q0 ) { go_BYE(-1); } 
      break;
    case j_bool : 
      if ( qtype != BL ) { go_BYE(-1); } 
      break;
    case j_number :
        switch ( qtype ) { 
          case  I1 : 
          case  I2 : 
          case  I4 : 
          case  I8 : 
          case  F4 : 
          case  F8 : 
          case  UI1 : 
          case  UI2 : 
          case  UI4 : 
          case  UI8 : 
            /* all is well */
            break;
          default : 
            go_BYE(-1); 
            break;
        }
      break;
    case j_date : 
      if ( qtype != TM1 ) { go_BYE(-1); } 
      break;
    case j_string : 
      if ( qtype != SC ) { go_BYE(-1); } 
      break;
    case j_object : 
      // if ( qtype != Q0 ) {go_BYE(-1); }
      // Above commented deliberately. If all items in object are
      // of same type, we enter it here
      break;
    case j_array : 
      if ( qtype != Q0 ) {   // => array of uniform values
        switch ( qtype ) { 
          case BL : 
          case  I1 : case  I2 : case  I4 : case  I8 : 
          case  F2 : case  F4 : case  F8 : 
          case  UI1 : case  UI2 : case  UI4 : case  UI8 : 
          case  SC : 
          case  SV : 
          case  TM1 : 
            /* all is well */
            break;
          default : 
            go_BYE(-1);
            break;
        }
      }
      break;
    default : 
      go_BYE(-1); 
      break;
  }
BYE:
  return status;
}

int
chk_hdr(
    void *x
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); } 
  jtype_t  jtype = get_jtype(x);
  qtype_t  qtype = get_qtype(x); 
  // uint32_t poff  = get_poff(x);
  status = chk_jtype(jtype, qtype); cBYE(status);
BYE:
  return status;
}

int 
chk_mixed_array(
    char *x
    )
{
  int status = 0;

  status = chk_hdr(x); cBYE(status);
  jtype_t jtype  = get_jtype(x);     if ( jtype != j_array ) { go_BYE(-1); }
  uint32_t width = get_arr_width(x); if ( width != 0 ) { go_BYE(-1); } 
  qtype_t qtype  = get_qtype(x);     if ( qtype != Q0 ) { go_BYE(-1); } 

  uint32_t arr_len = get_arr_len(x);
  uint32_t arr_size = get_arr_size(x); 
  if ( arr_len != arr_size ) { go_BYE(-1); } 

  void * const ox = get_offsets_in_arr(x); 
  if ( ox == NULL ) { go_BYE(-1); }
  int32_t * const offsets = (int32_t * const)get_arr_ptr(ox); 

  //-----------------------------
  for ( uint32_t i = 0; i < arr_len; i++ ) { 
    // allowing == 0 is for case where we create empty and then populate
    if ( offsets[i] == 0 ) { continue; } 
    if ( offsets[i]  < 0 ) { go_BYE(-1); } 
    if ( !is_multiple_n((uint64_t)offsets[i], 8) ) { go_BYE(-1); }
  }
  //-- Array may not be fully created, but is created in order. 
  //-- So ith element not created => (i-1)th element not created
  for ( int i = (int)arr_len-2; i >= 0; i-- ) {
    if ( offsets[i] == 0 ) { if ( offsets[i+1] != 0 ) { go_BYE(-1); } } 
  }
  //-----------------------------
  for ( uint32_t i = 1; i < arr_len; i++ ) { 
    if ( offsets[i]   == 0 ) { break; }
    if ( offsets[i] <= offsets[i-1] ) { go_BYE(-1); }
  }
  //-----------------------------
  // Check each element of the array 
  for ( uint32_t i = 0; i < arr_len; i++ ) { 
    if ( offsets[i] == 0 ) { break; }
    char *lx = (char *)x + offsets[i];
    QDF_REC_TYPE child_qdf = 
    { .data = lx, .size = get_qdf_size(lx), .is_foreign = true };
    status = chk_qdf(&child_qdf); cBYE(status); // recursive checking
  }
BYE:
  return status;
}

int 
chk_nil(
    void *x
    )
{
  int status = 0; 
  if ( x == NULL ) { go_BYE(-1); }
  qdf_hdr_t *xp = (qdf_hdr_t *)x;
  if ( xp->jtype != j_nil ) { go_BYE(-1); }
  if ( xp->qtype != Q0 )    { go_BYE(-1); }
  if ( xp->unused1 != 0 )   { go_BYE(-1); }
  if ( xp->unused2 != 0 )   { go_BYE(-1); }
  if ( xp->poff != 0 )      { go_BYE(-1); }
BYE:
  return status;
}

int 
chk_bool(
    void *x
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); }
  qdf_bool_hdr_t *xp = (qdf_bool_hdr_t *)x;
  if ( xp->jtype != j_bool ) { go_BYE(-1); }
  if ( xp->qtype != BL )     { go_BYE(-1); }
  if ( xp->unused1 != 0 )    { go_BYE(-1); }
  if ( xp->poff != 0 )       { go_BYE(-1); }
BYE:
  return status;
}

int 
chk_number(
    void *x
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); }
  qdf_number_hdr_t *xp = (qdf_number_hdr_t *)x;
  if ( xp->jtype != j_number ) { go_BYE(-1); }
  if ( xp->qtype != F8 )       { go_BYE(-1); }
  if ( xp->unused1 != 0 )      { go_BYE(-1); }
  if ( xp->unused2 != 0 )      { go_BYE(-1); }
  if ( xp->poff != 0 )         { go_BYE(-1); }
BYE:
  return status;
}

int 
chk_date(
    void *x
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); }
  qdf_date_hdr_t *xp = (qdf_date_hdr_t *)x;
  if ( xp->jtype != j_date ) { go_BYE(-1); }
  if ( xp->qtype != TM1 )     { go_BYE(-1); }
  if ( xp->unused1 != 0 )    { go_BYE(-1); }
  if ( xp->unused2 != 0 )    { go_BYE(-1); }
  if ( xp->poff != 0 )       { go_BYE(-1); }
  // IMPORTANT: Currently,we store time_since_epoch as I4
  // Hence, following constraint on tm_year
  // 2037-12-23-23 is 2145223398
  // 2^31 -1 is 2147483648 -1 
  if (( xp->tm_val.tm_year < 0)||( xp->tm_val.tm_year > 137 )){ go_BYE(-1);}
  if (( xp->tm_val.tm_mon < 0) ||( xp->tm_val.tm_mon > 12 )) { go_BYE(-1);}
  if (( xp->tm_val.tm_yday < 0)||( xp->tm_val.tm_yday > 365 )){ go_BYE(-1);}
  if (( xp->tm_val.tm_wday < 0)||( xp->tm_val.tm_wday > 6 )) { go_BYE(-1);}
  if (( xp->tm_val.tm_mday < 0)||( xp->tm_val.tm_mday > 31 )) { go_BYE(-1);}
  // if ( ( tmptr[0].tm_min < 0)||( tmptr[0].tm_min > 60 ) ) { go_BYE(-1);}
  // if ( ( tmptr[0].tm_sec < 0)||( tmptr[0].tm_sec > 60 ) ) { go_BYE(-1);}
BYE:
  return status;
}

int 
chk_string(
    void *x
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); }
  qdf_string_hdr_t *xp = (qdf_string_hdr_t *)x;
  if ( xp->jtype != j_string ) { go_BYE(-1); }
  if ( xp->qtype != SC )       { go_BYE(-1); }
  if ( xp->unused1 != 0 )      { go_BYE(-1); }
  if ( xp->unused2 != 0 )      { go_BYE(-1); }
  if ( xp->poff != 0 )         { go_BYE(-1); }
  if ( xp->str_len >= xp->str_size ) { go_BYE(-1); }
  if ( xp->str_size <= 1 ) { go_BYE(-1); }
  if ( !is_multiple_n(xp->str_size, 8) ) { go_BYE(-1); }
BYE:
  return status;
}


int 
chk_object(
    void *x
    )
{
  int status = 0;
  if ( x == NULL ) { go_BYE(-1); }
  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  if ( xp->jtype != j_object ) { go_BYE(-1); }
  status = chk_qtype(xp->qtype); cBYE(status);
  if ( xp->unused1 != 0 )      { go_BYE(-1); }
  if ( xp->poff != 0 )         { go_BYE(-1); }

  jtype_t jtype = get_jtype(x);
  if ( jtype != j_object ) { go_BYE(-1); }
  // qtype can be set only if *ALL* elements of the object have the
  // same primitive type. Hence, following has been commented out
  // qtype_t qtype = get_qtype(x);
  // if ( qtype != Q0 ) { go_BYE(-1); }
  uint32_t obj_len = xp->obj_len;
  uint32_t obj_arr_len = xp->obj_arr_len;
  uint32_t obj_arr_size = xp->obj_arr_size;
  bool b_is_df = xp->is_df;
  if ( b_is_df ) { 
    if ( obj_arr_size == 0 ) { go_BYE(-1); }
    // can be 0 while nascent: if ( obj_arr_len  == 0 ) { go_BYE(-1); }
  }
  else {
    if ( obj_arr_size != 0 ) { go_BYE(-1); }
    if ( obj_arr_len  != 0 ) { go_BYE(-1); }
  }
  //----------------------------------------
  uint32_t qdf_size =  xp->qdf_size;
  if ( !is_multiple_n(qdf_size, 8) ) { go_BYE(-1); }
  //----------------------------------------
  void *kx = get_keys_in_obj(x); 
  status = chk_uniform_array(kx, SC);
  qdf_array_hdr_t *kxp = (qdf_array_hdr_t *)kx;
  if ( kxp->arr_len != obj_len ) { go_BYE(-1); }
  //----------------------------------------
  void *ox = get_offsets_in_obj(x); 
  if ( ox == NULL ) { go_BYE(-1); } 
  status = chk_uniform_array(ox, I4);
  qdf_array_hdr_t *oxp = (qdf_array_hdr_t *)ox;
  if ( oxp->arr_len != obj_len ) { go_BYE(-1); }
  //----------------------------------------
  uint32_t *offsets = (uint32_t *)get_arr_ptr(ox); 

  uint32_t base_off = sizeof(qdf_object_hdr_t) + kxp->qdf_size + oxp->qdf_size;
  if ( offsets[0] != 0 ) { // for incremental creation case 
   if ( offsets[0] != base_off ) { go_BYE(-1); }
  }
  // TODO P1 Code review needed
  // Check each element of the object
  uint32_t cols_size = 0;
  for ( uint32_t i = 0; i < obj_len; i++ ) { 
    if ( offsets[i] == 0 ) { continue; }
    char *lx = (char *)x + offsets[i];
    uint32_t sz = get_qdf_size(lx);  
    if ( i < (obj_len-1) ) { 
      if ( offsets[i+1] != 0 ) {  // for incremental creation case
        uint32_t chk_sz = offsets[i+1] - offsets[i];
       if ( sz != chk_sz ) { go_BYE(-1); }
      }
    }
    QDF_REC_TYPE child_qdf = 
    { .data = lx, .size = sz, .is_foreign = true };
    status = chk_qdf(&child_qdf); cBYE(status); // recursive checking
    cols_size += x_get_qdf_size(&child_qdf);
  }
  if ( ( base_off + cols_size ) != qdf_size ) { 
    go_BYE(-1);
  }


BYE:
  return status;
}

//--------------------------------------------------
int
chk_uniform_array(
    void *x,
    qtype_t qtype
    )
{
  int status = 0;
  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  if ( xp->jtype != j_array ) { go_BYE(-1); } 
  if ( xp->qtype != qtype   ) { go_BYE(-1); } 
  if ( xp->poff != 0   ) { go_BYE(-1); } 
  if ( xp->unused1 != 0   ) { go_BYE(-1); } 

  uint32_t arr_width = xp->width;
  if ( arr_width == 0 ) { go_BYE(-1); } 

  uint32_t arr_size = xp->arr_size;
  if ( arr_size == 0 ) { go_BYE(-1); } 

  uint32_t arr_len  = xp->arr_len;
  if ( arr_size <  arr_len ) { go_BYE(-1); }

  uint32_t qdf_size = xp->qdf_size;
  if ( qdf_size == 0 ) { go_BYE(-1); } 
  if ( !is_multiple_n(qdf_size, 8) ) { go_BYE(-1); }

    // 24 for header, rest for data 
  uint32_t chk_qdf_size = 
    multiple_n((arr_size * arr_width), 8) + sizeof(qdf_array_hdr_t);
  if ( chk_qdf_size != qdf_size ) { go_BYE(-1); } 
  switch ( qtype ) {
    case BL : 
    case I1 : 
    case UI1 : 
      if ( arr_width != 1 ) { go_BYE(-1); } 
      break;
    case I2 : 
    case UI2 : 
      if ( arr_width != 2 ) { go_BYE(-1); } 
      break;
    case I4 : 
    case UI4 : 
      if ( arr_width != 4 ) { go_BYE(-1); } 
      break;
    case I8 : 
    case UI8 : 
      if ( arr_width != 8 ) { go_BYE(-1); } 
      break;
    case F2 : 
      if ( arr_width != 2 ) { go_BYE(-1); } 
      break;
    case F4 : 
      if ( arr_width != 4 ) { go_BYE(-1); } 
      break;
    case F8 : 
      if ( arr_width != 8 ) { go_BYE(-1); } 
      break;
    case SC : 
      break;
    case SV : 
      go_BYE(-1); // TODO 
      break;
    case TM1 : 
      if ( arr_width != sizeof(tm_t) ) { go_BYE(-1); } 
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}

int
chk_qdf(
    const QDF_REC_TYPE * const ptr_qdf
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, -1); 
  void *x = ptr_qdf->data;

  // cross check size of QDF 
  size_t qdf_size  = get_qdf_size(x); 
  if ( qdf_size != ptr_qdf->size ) { go_BYE(-1); }
  if ( !is_multiple_n(qdf_size, 8) ) { go_BYE(-1); } 
  //-----------------------
  jtype_t jtype = get_jtype(x); 
  qtype_t qtype = get_qtype(x); 
  status = chk_jtype(jtype, qtype); cBYE(status);
  //------------------------------------
  switch ( jtype ) {
    case j_nil : 
      status = chk_nil(x); cBYE(status);
      if ( ptr_qdf->size != 8 ) { go_BYE(-1); }
      break;
    case j_bool  : 
      status = chk_bool(x); cBYE(status);
      if ( ptr_qdf->size != 8 ) { go_BYE(-1); }
      break;
    case j_number : 
      status = chk_number(x); cBYE(status);
      if ( ptr_qdf->size != 16 ) { go_BYE(-1); }
      break;
    case j_date : 
      status = chk_date(x); cBYE(status);
      if ( ptr_qdf->size != 16 ) { go_BYE(-1); }
      break;
    case j_string : 
      status = chk_string(x); cBYE(status);
      break;
    case j_array : 
      {
        if ( qtype == Q0 ) { // uniform array
          status = chk_mixed_array(x); cBYE(status);
        }
        else {
          status = chk_uniform_array(x, qtype); cBYE(status);
        }
      }
      break; 
    case j_object : 
      status = chk_object(x); cBYE(status);
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}

int 
chk_is_data_frame(
      const QDF_REC_TYPE * const ptr_qdf,
      bool *ptr_is_df
    )
{
  int status = 0;
  if ( ptr_qdf == NULL ) { go_BYE(-1); }
  void *x = (void *)ptr_qdf->data;

  *ptr_is_df = false;
  jtype_t jtype = get_jtype(x); 
  if ( jtype != j_object ) { return 0; } 
#ifdef DEBUG
  status = chk_object(x); cBYE(status);
#endif

  uint32_t obj_arr_len   = x_get_arr_len(x); 
  uint32_t obj_arr_size  = x_get_arr_size(x); 
  if ( obj_arr_size == 0 ) { go_BYE(-1); } 

  // get number of rows and do some basic checks
  QDF_REC_TYPE keys; memset(&keys, 0, sizeof(QDF_REC_TYPE));
  status = get_keys(ptr_qdf, &keys); cBYE(status);
  uint32_t n_keys = x_get_arr_len(&keys); 
  if ( n_keys != get_obj_len(x) ) { go_BYE(-1); } 


  for ( uint32_t j = 0; j < n_keys; j++ ) {
    SCLR_REC_TYPE key; memset(&key, 0, sizeof(SCLR_REC_TYPE));
    QDF_REC_TYPE col;  memset(&col, 0, sizeof(QDF_REC_TYPE));
    status = get_arr_val(keys.data, j, &key, NULL); cBYE(status);
    status = get_key_val(ptr_qdf, -1, key.val.str, &col, NULL);cBYE(status);
    jtype = x_get_jtype(&col);
    // all elements must be an array
    if ( jtype != j_array ) { return status; } 
    qtype_t qtype = x_get_qtype(&col);
    // array must be of primitive type 
    if ( qtype == Q0 ) { return status; } 
    uint32_t arr_len = x_get_arr_len(&col); 
    if ( obj_arr_len != 0 ) { 
      if ( arr_len != 0 ) { 
        if ( obj_arr_len != arr_len ) { go_BYE(-1); } 
      }
      else {
        // okay to have 0 elements
      }
    }
    //-----------------------------
    uint32_t arr_size = x_get_arr_size(&col); 
    if ( obj_arr_size != arr_size ) { go_BYE(-1); } 
    //-----------------------------
  }
  *ptr_is_df = true;
BYE:
  return status; 
}

int
are_keys_in_object(
      const QDF_REC_TYPE *const ptr_qdf,
      const char ** const keys, // [n_keys]
      uint32_t n_keys,
      bool *ptr_are_keys
    )

{
  int status = 0;
  mcr_chk_non_null(ptr_qdf, false); 
  if ( keys == NULL ) { go_BYE(-1); }
  if ( n_keys <= 0 ) { go_BYE(-1); }

  void *x = (void *)ptr_qdf->data;
  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  if ( xp->jtype != j_object ) { go_BYE(-1); }

  *ptr_are_keys = true;
  for ( uint32_t j = 0; j < n_keys; j++ ) {
    if ( keys[j] == NULL ) { go_BYE(-1); }
    QDF_REC_TYPE val; memset(&val, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_qdf, -1, keys[j], &val, NULL); cBYE(status);
    if ( val.data == NULL ) { *ptr_are_keys = false; break; }
#ifdef DEBUG
    status = chk_qdf(&val); cBYE(status);
#endif
  }
BYE:
  return status;
}
