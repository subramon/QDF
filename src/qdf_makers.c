#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_setters.h"
#include "qdf_checkers.h"
#include "qtypes.h"
#include "multiple.h"
#include "qdf_makers.h"

int
make_nil(
    void *dummy, // for function signature consistency
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  mcr_chk_null(ptr_qdf, -1); 
  if ( dummy   != NULL ) { go_BYE(-1); }

  uint32_t qdf_size = sizeof(qdf_hdr_t);
  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size); 

  qdf_hdr_t *xp = (qdf_hdr_t *)x;
  xp->jtype = j_nil;

  ptr_qdf->size = qdf_size;
  ptr_qdf->data  = x;
BYE:
  return status;
}

int
make_boolean(
    bool bool_val,
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  mcr_chk_null(ptr_qdf, -1); 
  uint32_t qdf_size = sizeof(qdf_bool_hdr_t);

  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size); 

  qdf_bool_hdr_t *xp = (qdf_bool_hdr_t *)x;
  xp->jtype = j_bool;
  xp->qtype = BL;
  xp->bool_val = bool_val;

  ptr_qdf->size = qdf_size;
  ptr_qdf->data = x;
BYE:
  return status;
}

int
make_number(
    double num_val,
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  mcr_chk_null(ptr_qdf, -1); 
  uint32_t qdf_size = sizeof(qdf_number_hdr_t);

  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size); 

  qdf_number_hdr_t *xp = (qdf_number_hdr_t *)x;
  xp->jtype = j_number;
  xp->qtype = F8;
  xp->num_val = num_val;

  ptr_qdf->size = qdf_size;
  ptr_qdf->data = x;
BYE:
  return status;
}
//------------------------------------------------------
int
make_date(
    tm_t tm_val,
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  mcr_chk_null(ptr_qdf, -1); 
  uint32_t qdf_size = sizeof(qdf_date_hdr_t);

  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size);

  qdf_date_hdr_t *xp = (qdf_date_hdr_t *)x;
  xp->jtype = j_date; 
  xp->qtype = TM1;
  xp->tm_val = tm_val;

  ptr_qdf->size = qdf_size;
  ptr_qdf->data = x;
BYE:
  return status;
}
//--------------------------------------------------------------
int 
make_string(
    const char * const str_val,
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  mcr_chk_null(ptr_qdf, -1); 
  if ( str_val == NULL ) { go_BYE(-1); }
  uint32_t str_len = (uint32_t)strlen(str_val);
  uint32_t str_size = multiple_n(str_len + 1, 8); // +1 for nullc
  uint32_t qdf_size = sizeof(qdf_string_hdr_t) + str_size;

  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size); 

  qdf_string_hdr_t *xp = (qdf_string_hdr_t *)x;
  xp->jtype = j_string;
  xp->qtype = SC;
  xp->str_len = str_len;
  xp->str_size = str_size;
  char *cptr = (char *)(&xp[1]); 
  strcpy(cptr, str_val);

  ptr_qdf->size = qdf_size;
  ptr_qdf->data = x;
BYE:
  return status;
}
//--------------------------------------------------------------
int
make_SC_array(
    char ** const svals, // [n_svals][..] 
    const char * const concat_svals, // null separated svals 
    uint32_t in_width, // space (including nullc) to be allocated per entry
    uint32_t arr_len,
    uint32_t arr_size,
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  // I used to have the following restriction but it did not work
  // when I as  creating an empty data frame
  // exactly one of svals or concat_svals must be provided
  // if ( svals  == NULL ) { if ( concat_svals == NULL ) { go_BYE(-1); }  }
  // if ( concat_svals == NULL ) { if ( svals == NULL ) { go_BYE(-1); }  }
  if ( ( concat_svals != NULL ) && ( svals != NULL ) ) { go_BYE(-1); } 
  if ( arr_len == 0 ) { go_BYE(-1); }
  mcr_chk_null(ptr_qdf, -1); 
  uint32_t out_width = 0; // how we will write out data 
  // in contrast to in_width which is how the data is given to us

  if ( arr_size == 0 ) { arr_size = arr_len; } 
  if ( arr_size < arr_len ) { go_BYE(-1); } 
  if ( arr_size == 0 ) { go_BYE(-1); } // IMPORTANT

  if ( concat_svals != NULL ) {
    // There are 2 options here, best explained with an example
    // Let 0 represent the null character
    // Let arr_len = 4
    // Let the strings be "a", "bc", def", "ghij"
    // If in_width == 0, then concat_svals = a0bc0def0ghij0
    // If in_width != 0, then in_width must be >= (4+1). Say it is 6
    // Then, concat_svals = a00000bc0000def000ghij00
    uint32_t maxlen = 0;
    uint32_t idx = 0;
    if ( in_width == 0 ) { 
      for ( uint32_t i = 0; i < arr_len; i++ ) {
        uint32_t len = (uint32_t)strlen(concat_svals+idx);
        if ( len > maxlen ) { maxlen = len; } 
        idx += (len+1); // +1 for nullc
      }
      out_width = multiple_n(maxlen+1, 8); 
       // +1 for nullc, multiple of 8 for QDF
    }
    else {
      out_width = in_width;
      // this is a check (see above example for explanation)
      for ( uint32_t i = 0; i < arr_len; i++ ) {
        if ( concat_svals[((i+1)*in_width)-1] != '\0' ) { go_BYE(-1); }
      }
    }
  }
  if ( svals != NULL ) { // => we are given an array of strings
    // Get max len of svals[i] since we store as SC
    uint32_t maxlen = 0;
    for ( uint32_t i = 0; i < arr_len; i++ ) {
      if ( svals[i] == NULL ) { go_BYE(-1); }
      uint32_t len = (uint32_t)strlen(svals[i]);
      if ( len > maxlen ) { maxlen = len; }
    }
    if ( in_width > 0 ) {  // user wants to specify width. Check its okay
      if ( in_width < maxlen+1 ) { go_BYE(-1); } // +1 for nullc
      in_width = multiple_n(in_width, 8); // QDF requirement
      out_width = in_width;
    }
    else {
      out_width = multiple_n(maxlen+1, 8); 
      // +1 for nullc, multiple of 8 is requirement of QDF
    }
  }
  if ( ( concat_svals == NULL ) && ( svals == NULL ) ) { 
    out_width = in_width;
  }
  if ( out_width <= 1 ) { go_BYE(-1); } 
  if ( out_width > 1024 ) { go_BYE(-1); } // TODO Document 

  uint32_t qdf_size = sizeof(qdf_array_hdr_t) + (arr_size * out_width);
  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size); 

  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  xp->jtype = j_array;
  xp->qtype = SC;
  if ( out_width >= USHRT_MAX ) { go_BYE(-1); } // bcos 16 bits for width 
  xp->width = (uint16_t)out_width; 
  xp->arr_len = arr_len; 
  xp->arr_size = arr_size; 
  xp->qdf_size = qdf_size;

  char *dstptr = (char *)(&xp[1]);
  if ( concat_svals != NULL ) { 
    uint32_t idx = 0;
    for ( uint32_t i = 0; i < arr_len; i++ ) {
      uint32_t len = (uint32_t)strlen(concat_svals+idx);
      strcpy(dstptr+(i*out_width), concat_svals+idx);
      if ( in_width == 0 ) { 
        idx += (len+1); // +1 for nullc
      }
      else {
        idx += in_width;
      }
    }
  }
  if ( svals != NULL ) { 
    for ( uint32_t i = 0; i < arr_len; i++ ) { 
      strcpy(dstptr, svals[i]);
      dstptr += out_width;
    }
  }
  ptr_qdf->size = qdf_size;
  ptr_qdf->data = x;
BYE:
  return status;
}
//--------------------------------------------------------------
int
make_bool_array(
    const bool *const vals,
    uint32_t arr_len,
    uint32_t arr_size,
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  // okay to have empty data if ( vals  == NULL ) { go_BYE(-1); }
  mcr_chk_null(ptr_qdf, -1); 

  uint32_t width = get_width_c_qtype(BL);
  if ( width ==  0 ) { go_BYE(-1); }
  if ( arr_size == 0 ) { arr_size = arr_len; } 
  if ( arr_size < arr_len ) { go_BYE(-1); } 
  if ( arr_size == 0 ) { go_BYE(-1); } // IMPORTANT

  uint32_t len_vals  = (arr_len * width); 
  uint32_t size_vals = (arr_size * width); 
  uint32_t qdf_size  = sizeof(qdf_array_hdr_t) + multiple_n(size_vals, 8);

  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size); 

  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  xp->jtype = j_array;
  xp->qtype = BL;
  xp->width = (uint16_t)width;
  xp->arr_len = arr_len;
  xp->arr_size = arr_size;
  xp->qdf_size = qdf_size;

  bool *dstptr = (bool *)(&xp[1]); 
  if ( vals != NULL ) { memcpy(dstptr, vals, len_vals); }

  ptr_qdf->size = qdf_size;
  ptr_qdf->data = x;
BYE:
  return status;
}
//--------------------------------------------------------------
int
make_num_array(
    const void * const vals,
    uint32_t arr_len, // how many elements in array
    uint32_t arr_size, // how big array should be 
    qtype_t qtype,
    QDF_REC_TYPE * restrict ptr_qdf
    )
{
  int status = 0;
  // okay to have empty data if ( vals  == NULL ) { go_BYE(-1); }
  // okay for arr_len to be 0
  mcr_chk_null(ptr_qdf, -1); 

  uint32_t width = get_width_c_qtype(qtype);
  if ( width ==  0 ) { go_BYE(-1); }
  if ( arr_size == 0 ) { arr_size = arr_len; } 
  if ( arr_size < arr_len ) { go_BYE(-1); } 
  if ( arr_size == 0 ) { go_BYE(-1); } // IMPORTANT

  uint32_t len_vals  = (arr_len  * width); 
  uint32_t size_vals = (arr_size * width); 
  uint32_t qdf_size  = sizeof(qdf_array_hdr_t) + multiple_n(size_vals, 8); 

  void *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  memset(x, 0, qdf_size); 

  qdf_array_hdr_t *xp = (qdf_array_hdr_t *)x;
  xp->jtype = j_array;
  xp->qtype = qtype;
  xp->width = (uint16_t)width;
  xp->arr_len = arr_len;
  xp->arr_size = arr_size;
  xp->qdf_size = qdf_size;

  char *dstptr = (char *)(&xp[1]); 
  if ( vals != NULL ) { memcpy(dstptr, vals, len_vals); }
  ptr_qdf->size = qdf_size;
  ptr_qdf->data = x;
BYE:
  return status;
}
//--------------------------------------------------------------
int
make_data_frame(
    char ** const cols, // names of columns 
    uint32_t n_cols, // number of columns 
    const uint32_t * const widths, // widths of columns 
    char **vals, // [n_cols][arr_size]
    uint32_t arr_len,
    uint32_t arr_size,  // 0 <= arr_len <= arr_size
    const qtype_t *const qtypes, // [n_cols]
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  QDF_REC_TYPE qdf_keys; memset(&qdf_keys, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE qdf_offs; memset(&qdf_offs, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE *qdf_cols = NULL;
  // the keys are converted into a uniform array of SC
  status = make_SC_array(cols, NULL, 0, n_cols, 0, &qdf_keys); cBYE(status);
#ifdef DEBUG
  status = chk_qdf(&qdf_keys); cBYE(status);
#endif
  // make an QDF for values of each column in the data frame
  qdf_cols = malloc(n_cols * sizeof(QDF_REC_TYPE));
  return_if_malloc_failed(qdf_cols);
  memset(qdf_cols, 0,  n_cols * sizeof(QDF_REC_TYPE));
  // check that keys are unique 
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    for ( uint32_t j = i+1; j < n_cols; j++ ) { 
      if ( strcmp(cols[i], cols[j]) == 0 ) { 
        fprintf(stderr, "Column [%s] is repeated \n", cols[i]); go_BYE(-1); 
      }
    }
  }
  //-------------------
  if ( arr_size == 0 ) { arr_size = arr_len; } 
  if ( arr_size < arr_len ) { 
    go_BYE(-1); 
  } 
  if ( arr_size == 0 ) { 
    go_BYE(-1); }
  for ( uint32_t i = 0; i < n_cols; i++ ) {
    if ( qtypes[i] == Q0 ) { go_BYE(-1); } 
    const void *vals_i = NULL;
    if ( vals != NULL ) { vals_i = vals[i]; }
    if ( qtypes[i] == SC ) { 
      if ( widths == NULL ) { go_BYE(-1); } 
      uint32_t width = widths[i];
      if ( width < 1 ) { go_BYE(-1); } 
      status = make_SC_array(NULL, vals_i, width, arr_len, arr_size,
          &(qdf_cols[i]));
      cBYE(status);
      // status = x_pr_array(qdf_cols+i, "_xxxx.csv");  
    }
    else {
      status = make_num_array(vals_i, arr_len, arr_size, qtypes[i], 
          &(qdf_cols[i]));
      cBYE(status);
    }
#ifdef DEBUG
    status = chk_qdf(&(qdf_cols[i])); cBYE(status);
#endif
  }
  // create offsets QDF for columns, placed after keys QDF 
  // Note the 0 for sz below 
  status = make_num_array(NULL, n_cols, 0, I4, &qdf_offs); 
  cBYE(status);
  QDF_REC_TYPE qdf_tmp; memset(&qdf_tmp, 0, sizeof(QDF_REC_TYPE));
  status = x_get_arr_ptr(&qdf_offs, &qdf_tmp); cBYE(status);

  // compute size of output QDF
  uint32_t l_hdr = sizeof(qdf_object_hdr_t);
  uint32_t l_key = x_get_qdf_size(&qdf_keys);  // keys QDF 
  uint32_t l_off = x_get_qdf_size(&qdf_offs);  // offsets QDF 
  uint32_t l_cols = 0; // adds up space for all the columns in the dataframe
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    l_cols += x_get_qdf_size(&(qdf_cols[i])); 
  }
  uint32_t qdf_size = l_hdr + l_key + l_off + l_cols; 
  //---------------------------------------------
  void *x = NULL;
  status = posix_memalign(&x, ALIGNMENT, qdf_size); cBYE(status);
  return_if_malloc_failed(x);
  memset(x, 0, qdf_size); // TODO P4 Delete later, this is for Valgrind 

  qdf_object_hdr_t *xp = (qdf_object_hdr_t *)x;
  xp->jtype = j_object;
  xp->obj_len = n_cols;
  xp->qdf_size = qdf_size;
  xp->obj_arr_len = arr_len;   // only when object is dataframe
  xp->obj_arr_size = arr_size; // only when object is dataframe
  xp->is_df = true;            // only when object is dataframe

  void *kx = get_keys_in_obj(x); if ( kx == NULL ) { go_BYE(-1); }
  memcpy(kx, qdf_keys.data, qdf_keys.size);
  free_qdf(&(qdf_keys)); // don't need keys any more 
  
  // Set the values of the offsets properly 
  uint32_t offset = l_hdr + l_key + l_off;
  // Now copy the QDF for offsets
  for ( uint32_t i = 0; i < n_cols; i++ ) {
    SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
    sclr.val.i4 = (int32_t)offset; sclr.qtype = I4; 
    status = x_set_arr_val(&qdf_offs, i, sclr); cBYE(status);
    offset += x_get_qdf_size(&(qdf_cols[i]));
  }
  // Copy offsets into place 
  void *ox = get_offsets_in_obj(x); if ( ox == NULL ) { go_BYE(-1); }
  memcpy(ox, qdf_offs.data, qdf_offs.size);
  free_qdf(&(qdf_offs)); // don't need offsets any more 
  // Now copy the QDF for the individual vectors
  char *bak_x = ((char *)x + (l_hdr+l_key+l_off));
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
#ifdef DEBUG
    status = chk_qdf(&(qdf_cols[i])); cBYE(status);
#endif
    uint32_t l = x_get_qdf_size(&(qdf_cols[i]));
    memcpy(bak_x, qdf_cols[i].data, l);
    bak_x += l;
    free_qdf(&(qdf_cols[i])); // not needed any more 
  }
  ptr_qdf->size = qdf_size;
  ptr_qdf->data  = x;
#ifdef DEBUG
  status = chk_qdf(ptr_qdf); cBYE(status);
#endif
BYE:
  if ( qdf_cols != NULL ) { 
    for ( uint32_t i = 0; i < n_cols; i++ ) { 
      free_qdf(&(qdf_cols[i]));
    }
  }
  free_if_non_null(qdf_cols);
  free_qdf(&(qdf_keys));
  free_qdf(&(qdf_offs));
  return status;
}

int
make_mixed_array_or_object(
    const QDF_REC_TYPE * const qdf_vals, // [n_qdfs]
    char ** const keys, // [n_qdfs]
    uint32_t n_qdfs,
    QDF_REC_TYPE *ptr_out_qdf
    )
{
  int status = 0;
  QDF_REC_TYPE qdf_offs; memset(&qdf_offs, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE qdf_keys; memset(&qdf_keys, 0, sizeof(QDF_REC_TYPE));
  mcr_chk_null(ptr_out_qdf, -1); 

  if ( n_qdfs == 0 ) { 
    // TODO P1 We do not handle this case properly. We have "hacked"
    // around it by returning nil
    status = make_nil(NULL, ptr_out_qdf); cBYE(status);
    return status;
  }
  bool is_keys = false;
  jtype_t jtype;

  // create keys for columns
  if ( keys != NULL ) { 
    status = make_SC_array(keys, NULL, 0, n_qdfs, 0, &qdf_keys); 
    cBYE(status);
    is_keys  = true;
    jtype = j_object;
  }
  else {
    jtype = j_array;
  }
  // create offsets for columns
  status = make_num_array(NULL, n_qdfs, 0, I4, &qdf_offs); cBYE(status);

  // compute len of output QDF
  uint32_t l_hdr = 0, l_keys = 0, l_offs = 0, l_qdfs = 0;
  if ( is_keys ) { 
    l_hdr = sizeof(qdf_object_hdr_t);
    l_keys = x_get_qdf_size(&qdf_keys); 
  }
  else {
    l_hdr = sizeof(qdf_array_hdr_t);
  }
  l_offs = x_get_qdf_size(&qdf_offs); 

  if ( qdf_vals != NULL ) { 
    for ( uint32_t i = 0; i < n_qdfs; i++ ) { 
      l_qdfs += x_get_qdf_size(&(qdf_vals[i])); 
    }
  }
  //---------------------------------------------
#ifdef DEBUG
  if ( qdf_vals != NULL ) { 
    // This condition happens when we use append_mixed_array
    // to add the individual components of the object/array later
    for ( uint32_t i = 0; i < n_qdfs; i++ ) { 
      status = chk_qdf(&(qdf_vals[i])); cBYE(status);
    }
  }
#endif

  uint32_t qdf_size = l_hdr + l_keys + l_offs + l_qdfs;
  char *x = NULL;
  status = posix_memalign((void **)&x, ALIGNMENT, qdf_size); cBYE(status);
  return_if_malloc_failed(x);
  memset(x, 0, qdf_size); // TODO P4 Delete later, this is for Valgrind 

  set_jtype(x, jtype); 
  set_qtype(x, Q0); 
  set_qdf_size(x, qdf_size);
  if ( jtype == j_object ) { 
    set_obj_len(x, n_qdfs); 
  }
  else { 
    set_arr_len(x, n_qdfs); 
    set_arr_size(x, n_qdfs); 
  }
  ptr_out_qdf->size = qdf_size;
  ptr_out_qdf->data  = x;

  // place keys if provided
  if ( is_keys ) { 
    memcpy(x+l_hdr, qdf_keys.data, qdf_keys.size);
  }
  // IMPORTANT: Note the early return
  // place (UN-initialized) offsets in place 
  int32_t *offptr = (int32_t *)get_arr_ptr(qdf_offs.data); 
  for ( uint32_t i = 0; i < n_qdfs; i++ ) { offptr[i] = 0; }
  memcpy(x+(l_hdr+l_keys), qdf_offs.data, qdf_offs.size);
  if ( qdf_vals == NULL ) { return status; } 

  // Now set the offsets properly
  uint32_t offset = l_hdr + l_keys + l_offs;
  // Now copy the QDF for offsets
  for ( uint32_t i = 0; i < n_qdfs; i++ ) {
    SCLR_REC_TYPE sclr;
    sclr.val.i4 = (int32_t)offset; sclr.qtype = I4;
    status = x_set_arr_val(&qdf_offs, i, sclr); cBYE(status);
    offset += x_get_qdf_size(&(qdf_vals[i]));
  }
#ifdef DEBUG
  status = chk_qdf(&qdf_offs); cBYE(status);
#endif
  // place (initialized) offsets in place 
  memcpy(x+(l_hdr+l_keys), qdf_offs.data, qdf_offs.size);
  free_qdf(&(qdf_offs));
  // Now copy the QDF for the individual vectors
  offset = l_hdr + l_keys + l_offs;
  for ( uint32_t i = 0; i < n_qdfs; i++ ) { 
    memcpy(x+offset, qdf_vals[i].data, qdf_vals[i].size);
    offset += qdf_vals[i].size;
  }
BYE:
  free_qdf(&qdf_offs);
  free_qdf(&qdf_keys);
  return status;
}

// this function is used when we create an empty array/object of
// fixed size and then keep adding to it. Assumptio is that we add in
// order i.e., add first element, then second element and so on
int
append_mixed_array_or_object(
    QDF_REC_TYPE *ptr_dst,
    QDF_REC_TYPE *ptr_src
    )
{
  int status = 0;
  char *x = NULL;
  mcr_chk_non_null(ptr_dst, -1 );
  mcr_chk_non_null(ptr_src, -1 );
  if ( ptr_dst->is_read_only ) { go_BYE(-1); } 
  if ( ptr_dst->is_mmap  ) { go_BYE(-1); } 

  char *dx = ptr_dst->data; 
  char *sx = ptr_src->data; 

  jtype_t djtype = get_jtype(dx); 
  // destination must be object or array
  if ( ( djtype != j_object )  && ( djtype != j_array ) ) { go_BYE(-1); }

  uint32_t dn  = get_obj_len(dx);

  uint32_t key_qdf_size = 0; 
  uint32_t hdr_len;
  if ( djtype == j_object ) { // jump over keys if object 
    hdr_len = sizeof(qdf_object_hdr_t);
    char *kx = dx+hdr_len;
    key_qdf_size = get_qdf_size(kx); 
#ifdef DEBUG
    status = chk_uniform_array(kx, SC); cBYE(status); 
#endif
  }
  else {
    hdr_len = sizeof(qdf_array_hdr_t);
  }
  char *ox = dx+(hdr_len+key_qdf_size);
#ifdef DEBUG
  status = chk_uniform_array(ox, I4); cBYE(status); 
#endif

  // see if there is a spot to place the new guy 
  int32_t *offptr = (int32_t *)get_arr_ptr(ox); 
  uint32_t dst_idx = 0; bool found_spot = false; 
  for ( uint32_t i = 0; i < dn; i++ ) { 
    if ( offptr[i] == 0 ) { 
      dst_idx = i; found_spot = true; break; 
    }
  }
  if ( !found_spot ) { go_BYE(-1); } // no space
  // debugging check
  for ( uint32_t i = dst_idx; i < dn; i++ ) { 
    if ( offptr[i] != 0 ) { 
      go_BYE(-1); 
    } 
  }
  // now that we know we have space, new length of qdf
  // is length of current qdf (dst_qdf) + what we are appending (src_qdf)
  uint32_t sqdf_size = x_get_qdf_size(ptr_src);
  uint32_t dqdf_size = x_get_qdf_size(ptr_dst);
  uint32_t new_dqdf_size = sqdf_size + dqdf_size ;
  // update offset for new guy
  offptr[dst_idx] = (int32_t)dqdf_size; // TODO P3 fix sign of offptr
  //---------------------------------------------

  status = posix_memalign((void *)&x, ALIGNMENT, new_dqdf_size); cBYE(status);
  return_if_malloc_failed(x);
  memset(x, 0, new_dqdf_size); // TODO P4 Delete later, this is for Valgrind 
  memcpy(x,          dx, dqdf_size);
  memcpy(x+dqdf_size, sx, sqdf_size);
  set_qdf_size(x, new_dqdf_size); 

  // clean out old stuff
  free_qdf(ptr_dst);
  memset(ptr_dst, 0, sizeof(QDF_REC_TYPE));
  // put in new stuff
  ptr_dst->data = x;
  ptr_dst->size = new_dqdf_size;
BYE:
  return status;
}

int
make_empty_data_frame(
    char ** const cols, // [n_cols]
    uint32_t n_cols,
    const char ** const str_qtypes, // [n_cols]
    uint32_t sz_rows,
    QDF_REC_TYPE *ptr_qdf
    )
{
  int status = 0;
  qtype_t *qtypes = NULL;
  uint32_t *widths = NULL;
  // As a prelim, convert qtypes from  strings to enums
  widths = malloc(n_cols * sizeof(uint32_t));
  return_if_malloc_failed(widths);
  qtypes = malloc(n_cols * sizeof(qtype_t));
  return_if_malloc_failed(qtypes);
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    qtypes[i] = get_c_qtype(str_qtypes[i]); 
    widths[i] = get_width_qtype(str_qtypes[i]); 
  }
  //--------------------------
  status = make_data_frame(cols, n_cols, widths, NULL, sz_rows, 0, 
      qtypes, ptr_qdf);
  cBYE(status);
BYE:
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  return status;
}

int
place_in_data_frame(
    QDF_REC_TYPE *ptr_dst, // dataframe to be modified in place
    const  char * const key, // name  of  column
    QDF_REC_TYPE *ptr_src // column to be placed into dst
    )
{
  int status = 0;
  mcr_chk_non_null(ptr_dst, -1);
  mcr_chk_non_null(ptr_src, -1);
  if ( key     == NULL ) { go_BYE(-1); }

  void *dx = ptr_dst->data; 
  qdf_object_hdr_t *dxp = (qdf_object_hdr_t *)dx;
  if ( dxp->is_df == false ) { go_BYE(-1); }
  uint32_t dst_arr_len  = dxp->obj_arr_len;

  void *sx = ptr_src->data; 
  qdf_array_hdr_t *sxp = (qdf_array_hdr_t *)sx;
  if ( sxp->jtype != j_array ) { go_BYE(-1); }
  uint32_t src_arr_len  = sxp->arr_len;
  if ( src_arr_len == 0 ) { go_BYE(-1); }  // src is empty, nothing to do

  // tmp_qdf is what will be over-written by src_qdf
  QDF_REC_TYPE tmp_qdf; memset(&tmp_qdf, 0, sizeof(QDF_REC_TYPE));
  uint32_t offset = 0; // where src should be placed in dst 
  status = get_key_val(ptr_dst, -1, key, &tmp_qdf, &offset); cBYE(status);
  // src_qdf must conform to tmp_qdf
  qdf_array_hdr_t *txp = (qdf_array_hdr_t *)(tmp_qdf.data);
  if ( txp->jtype != j_array ) { go_BYE(-1); }
  if ( sxp->jtype != txp->jtype ) { go_BYE(-1); }
  if ( sxp->qdf_size != txp->qdf_size ) { go_BYE(-1); }

  
  if ( dst_arr_len > 0 )  {
    if ( src_arr_len != dst_arr_len ) { go_BYE(-1); }
  }
  else {
    dxp->obj_arr_len = src_arr_len; 
  }
  // following may be too strong. Consider relaxing it
  // if ( src_arr_size != obj_arr_size ) { go_BYE(-1); }

  // place src_qdf in correct spot in dst 
  memcpy((char *)dx+offset, sx, sxp->qdf_size);
BYE:
  return status;
}
