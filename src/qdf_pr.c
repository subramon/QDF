#include "incs.h"
#include "cat_to_buf.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_pr.h"
#include "split_str.h"

#define mcr_pr_comma() { \
                if ( i > 0 ) {  \
                  status = cat_to_buf(&buf, &bufsz, ptr_len, ", ", 2);  \
                  cBYE(status); \
                } \
}
#define mcr_pr_dquote() { \
                  status = cat_to_buf(&buf, &bufsz, ptr_len, "\"", 1);  \
                  cBYE(status); \
}
// This is applicable only for a "data frame". By that we mean
// a JSON object with n keys, the value of each of which is an array, 
// all of which must be of the same size
// This is quite inefficient but since it is mainly for debugging,
// I haven't found the need to optimize it
int
pr_df_as_csv(
    const QDF_REC_TYPE * const ptr_qdf,
    char ** const keys_to_pr,
    uint32_t n_keys_to_pr,
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;
  char ** keys = NULL; uint32_t n_keys = 0;

  mcr_chk_non_null(ptr_qdf, -1); 
  if ( file_name == NULL ) { go_BYE(-1); }
  // There are 2 kinds of invocations. We will figure them out below
  if ( keys_to_pr == NULL ) {
    // print all keys 
    if ( n_keys_to_pr > 0 ) { go_BYE(-1); }
    // TODO Get keys to be all keys in dataframe
    QDF_REC_TYPE keys_qdf; memset(&keys_qdf, 0, sizeof(QDF_REC_TYPE));
    status = get_keys(ptr_qdf, &keys_qdf); cBYE(status);
    status = chk_qdf(&keys_qdf); cBYE(status);
    //-- we need to discount nn_ keys 
    uint32_t tmp_n_keys = get_arr_len(keys_qdf.data);  
    if ( tmp_n_keys == 0 ) { go_BYE(-1); }
    n_keys = 0;
    for ( uint32_t i = 0; i < tmp_n_keys; i++ ) { 
      SCLR_REC_TYPE sclr;
      status = get_arr_val(keys_qdf.data, i, &sclr, NULL); 
      if ( strncmp(sclr.val.str, "nn_", strlen("nn_")) != 0 ) {
        n_keys++;
      }
    }
    //----------------------------------------
    keys = malloc(n_keys * sizeof(char *));
    return_if_malloc_failed(keys);
    memset(keys, 0,  n_keys * sizeof(char *));
    uint32_t kidx = 0;
    for ( uint32_t i = 0; i < tmp_n_keys; i++ ) { 
      SCLR_REC_TYPE sclr;
      status = get_arr_val(keys_qdf.data, i, &sclr, NULL); 
      if ( strncmp(sclr.val.str, "nn_", strlen("nn_")) != 0 ) {
        if ( kidx == n_keys ) { go_BYE(-1); }
        keys[kidx++] = strdup(sclr.val.str);
      }
    }
  }
  else {
    if ( n_keys_to_pr == 0 ) { go_BYE(-1); }
    // discount nn_ keys 
    n_keys = 0;
    uint32_t tmp_n_keys = n_keys_to_pr;
    for ( uint32_t i = 0; i < tmp_n_keys; i++ ) { 
      if ( strncmp(keys_to_pr[i], "nn_", strlen("nn_")) != 0 ) { 
        n_keys++;
      }
    }
    if ( n_keys == 0 ) { go_BYE(-1); }
    keys = malloc(n_keys * sizeof(char *));
    return_if_malloc_failed(keys);
    memset(keys, 0,  n_keys * sizeof(char *));
    uint32_t kidx = 0;
    for ( uint32_t i = 0; i < tmp_n_keys; i++ ) { 
      if ( strncmp(keys_to_pr[i], "nn_", strlen("nn_")) != 0 ) { 
        if ( kidx == n_keys ) { go_BYE(-1); }
        keys[kidx++] = strdup(keys_to_pr[i]);
      }
    }
  }
  //------------------------------------------------------
  fp = fopen(file_name, "w");
  return_if_fopen_failed(fp, file_name, "w");
  // print header line 
  for ( uint32_t i = 0; i < n_keys; i++ ) { 
    if ( i > 0 ) { fprintf(fp, ","); }
    fprintf(fp, "%s", keys[i]);
  }
  fprintf(fp, "\n");
  //------------------------------------------

  uint32_t n_rows = x_get_obj_arr_len(ptr_qdf); 
  if ( !x_get_is_df(ptr_qdf) ) { go_BYE(-1); }
  bool are_keys = false;
  status = are_keys_in_object(ptr_qdf, 
      (char ** const )keys, n_keys, &are_keys);
  cBYE(status);
  if ( !are_keys ) { go_BYE(-1); }
  for ( uint32_t i = 0; i < n_rows; i++ ) { 
    for ( uint32_t j = 0; j < n_keys; j++ ) {
      if ( j > 0 ) { fprintf(fp, ","); }
      // START: for potential nn column 
      const char * nn_ptr = NULL;
      QDF_REC_TYPE nn_col; memset(&nn_col, 0, sizeof(QDF_REC_TYPE));
      bool is_nn = false; 
      char *nn_key = NULL;
      size_t len = strlen(keys[j]) + 8; 
      nn_key = malloc(len); memset(nn_key, 0, len);
      sprintf(nn_key, "nn_%s", keys[j]);
      status = is_key(ptr_qdf, nn_key, &is_nn); 
      if ( is_nn ) { 
        status = get_key_val(ptr_qdf, -1, nn_key, &nn_col, NULL); 
        cBYE(status);
        qtype_t qtype = get_qtype(nn_col.data); 
        if ( qtype != BL ) { go_BYE(-1); }
        nn_ptr = get_arr_ptr(nn_col.data); 
      }
      // STOP : for potential nn column 
      QDF_REC_TYPE col; memset(&col, 0, sizeof(QDF_REC_TYPE));
      status = get_key_val(ptr_qdf, -1, keys[j], &col, NULL); cBYE(status);
      qtype_t qtype = get_qtype(col.data); 
      uint32_t width = 0;
      if ( qtype == SC ) { 
        width = get_arr_width(col.data); if ( width < 1 ) { go_BYE(-1); } 
      }
      const char * const valptr = get_arr_ptr(col.data); 
      status = pr_1(valptr, nn_ptr, qtype, width, i, fp); cBYE(status);
    }
    fprintf(fp, "\n");
  }
BYE:
  if ( keys != NULL ) { 
    for ( uint32_t i = 0; i < n_keys; i++ ) { 
      free_if_non_null(keys[i]);
    }
  }
  free_if_non_null(keys);
  fclose_if_non_null(fp);
  return status;
}

// This is for calling from Lua 
int
x_pr_json(
    const QDF_REC_TYPE * const ptr_qdf,
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;
  QDF_REC_TYPE out; memset(&out, 0, sizeof(QDF_REC_TYPE));
  uint32_t len = 0;
  status = pr_json(ptr_qdf, &out, &len, NULL); cBYE(status);
  if ( ( out.data == NULL ) || ( out.size == 0 ) ) { go_BYE(-1); }

  if ( file_name == NULL ) { 
    fp = stdout;
  }
  else {
    fp = fopen(file_name, "w");
    return_if_fopen_failed(fp, file_name, "w");
  }
  fwrite(out.data, len, 1, fp);
  free_qdf(&out); 
BYE:
  if ( file_name != NULL ) { 
    fclose_if_non_null(fp); 
  }
  return status;
}
int
pr_json(
    const QDF_REC_TYPE * const ptr_qdf,
    QDF_REC_TYPE * restrict ptr_out_str, // where output string is created
    uint32_t *ptr_len, // how much of allocated space has been used 
    FILE *fp // note that either fp or out_str is provided, not both
    )
{
  int status = 0;
  char valbuf[1024]; // make big enough to fit all types, incl. TM1, 
  uint32_t vblen = sizeof(valbuf); 
  double dval; int64_t valI8; bool bval;
  QDF_REC_TYPE qdf_keys, qdf_val; 

  mcr_chk_non_null(ptr_qdf, -1); 
  // decide where to write 
  bool to_file; // TODO Need to implement P3
  if ( ptr_out_str == NULL ) { 
    if ( fp == NULL ) { go_BYE(-1); }
    if ( ptr_len != NULL ) { go_BYE(-1); }
    to_file = true;
  }
  else {
    if ( fp != NULL ) { go_BYE(-1); }
    if ( ptr_len == NULL ) { go_BYE(-1); }
    to_file = false;
  }
  //-----------------------------------------
  char * buf = (char *)ptr_out_str->data;
  uint32_t bufsz  = ptr_out_str->size;
  if ( *ptr_len > bufsz ) { go_BYE(-1); }
  //-----------------------------------------
  void * x = ptr_qdf->data;
  jtype_t jtype = get_jtype(x);
  qtype_t qtype = get_qtype(x); 
  switch ( jtype ) { 
    case j_nil : 
      status = cat_to_buf(&buf, &bufsz, ptr_len, "null", 0); 
      cBYE(status);
      break;
    case j_bool : 
      bval = get_bool_val(x);
      if (  bval == 0 )  { 
        status = cat_to_buf(&buf, &bufsz, ptr_len, "false", 0); 
      }
      else if (  bval == 1 )  { 
        status = cat_to_buf(&buf, &bufsz, ptr_len, "true", 0); 
      }
      else {
        go_BYE(-1);
      }
      cBYE(status);
      break;
    case j_number : 
      dval = get_num_val(x); 
      memset(valbuf, 0, vblen);
      if ( trunc(dval) == dval ) {
        valI8 = (int64_t)dval;
        snprintf(valbuf, vblen-1, "%" PRIi64, valI8);
      }
      else {
        snprintf(valbuf, vblen-1, "%lf", dval);
      }
      status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);  
      cBYE(status);
      break;
    case j_date : 
      {
        tm_t tmval; status = get_date_val(x, &tmval); cBYE(status);
        memset(valbuf, 0, vblen);
        snprintf(valbuf, vblen-1, "\"%d:%02d:%02d:%d:%d:%d\"", 
            tmval.tm_year + 1900,
            tmval.tm_mon + 1,
            tmval.tm_mday,
            tmval.tm_hour,
            // tmval.tm_min,
            // tmval.tm_sec,
            tmval.tm_wday,
            tmval.tm_yday);
        status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);  
        cBYE(status);
      }
      break;
    case j_string : 
      {
      char * cptr = get_str_val(x); 
      mcr_pr_dquote();
      status = cat_to_buf(&buf, &bufsz, ptr_len, cptr, 0);
      cBYE(status);
      mcr_pr_dquote();
      }
      break;
    case j_array : 
      {
      uint32_t n_elem = get_arr_len(x); 
      uint32_t width  = get_arr_width(x); 
      status = cat_to_buf(&buf, &bufsz, ptr_len, "[", 1); // start of array
      cBYE(status);
      switch ( qtype ) { //
        case BL : 

        case I1 : 
        case I2 : 
        case I4 : 
        case I8 : 

        case UI1 : 
        case UI2 : 

        case F4 : 
        case F8 : 
        case TM1 : 
          {
          const char * const valptr = get_arr_ptr(x); 
          switch ( qtype ) {
            case BL : 
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "%s", 
                    ((const int8_t * const )valptr)[i] ? "true" : "false");
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              break;
            case I1 : 
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "%d", ((const int8_t *)valptr)[i]);
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              break;
            case UI1 : 
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "%d", ((const uint8_t *)valptr)[i]);
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              break;
            case I2 : 
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "%d", ((const int16_t *)valptr)[i]);
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              break;
            case UI2 : 
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "%u", ((const uint16_t *)valptr)[i]);
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              break;
            case I4 : 
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "%d", ((const int32_t *)valptr)[i]);
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              break;
            case I8 : 
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "%" PRIi64 "", ((const int64_t *)valptr)[i]);
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              break;
            case F4 : 
              {
              const float * const F4ptr = (const float * const )valptr;
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                if ( ceil(F4ptr[i]) == floor(F4ptr[i]) ) { 
                  valI8 = (int64_t)F4ptr[i];
                  snprintf(valbuf, vblen-1, "%" PRIi64 "", valI8);
                }
                else {
                  snprintf(valbuf, vblen-1, "%lf", F4ptr[i]);
                }
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              }
              break;
            case F8 : 
              {
              const double * const F8ptr = (const double * const)valptr;
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                if ( trunc(F8ptr[i]) == F8ptr[i] ) { 
                  valI8 = (int64_t)F8ptr[i];
                  snprintf(valbuf, vblen-1, "%" PRIi64 "", valI8);
                }
                else {
                  snprintf(valbuf, vblen-1, "%lf", F8ptr[i]);
                }
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              }
              break;
            case TM1 : 
              {
              const tm_t * const tptr = (const tm_t * const )valptr;
              for ( uint32_t i = 0; i < n_elem; i++ ) { 
                mcr_pr_comma();
                snprintf(valbuf, vblen-1, "\"%d:%02d:%02d:%d:%d:%d\"", 
                    tptr[i].tm_year + 1900,
                    tptr[i].tm_mon + 1,
                    tptr[i].tm_mday,
                    tptr[i].tm_hour,
                    tptr[i].tm_wday,
                    // tptr[i].tm_min,
                    // tptr[i].tm_sec,
                    tptr[i].tm_yday);
                status = cat_to_buf(&buf, &bufsz, ptr_len, valbuf, 0);
                cBYE(status);
              }
              }
              break;
            default : 
              go_BYE(-1);
              break;
          }
          break;
          }
        case SC :
          {
            const char * cptr = get_arr_ptr(x); 
            for ( uint32_t i = 0; i < n_elem; i++ ) { 
              mcr_pr_comma();
              mcr_pr_dquote();
              status = cat_to_buf(&buf, &bufsz, ptr_len, cptr, 0);
              cBYE(status);
              mcr_pr_dquote();
              cptr += width;
            }
          }
          break;
        case Q0 : 
          {
            void * ox = get_offsets_in_arr(x); 
            if ( ox == NULL ) { go_BYE(-1); }
            qdf_array_hdr_t *oxp = (qdf_array_hdr_t *)ox;
            if ( oxp->jtype != j_array ) { go_BYE(-1); }
            if ( oxp->qtype != I4      ) { go_BYE(-1); }
            for ( uint32_t i = 0; i < n_elem; i++ ) {
              SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
              //            mcr_pr_comma();
              if ( i > 0 ) {  
                status = cat_to_buf(&buf, &bufsz, ptr_len, ", ", 2);  
                cBYE(status); 
              } 
              status = get_arr_val(ox, i, &sclr, NULL); cBYE(status);
              int offset = sclr.val.i4;
              void * child_loc = (void *)((char *)x + offset);
              uint32_t child_qdf_size = get_qdf_size(child_loc); 
              QDF_REC_TYPE child = { .data = child_loc, .size = child_qdf_size };
#ifdef DEBUG
              status = chk_qdf(&child); cBYE(status);
#endif
              ptr_out_str->data = buf; ptr_out_str->size = bufsz; 
              status = pr_json(&child, ptr_out_str, ptr_len, fp); 
              cBYE(status);
              buf = ptr_out_str->data; bufsz = ptr_out_str->size;
            }
          }
          break;
        default : 
          go_BYE(-1);
          break;
      }
      status = cat_to_buf(&buf, &bufsz, ptr_len, "]", 1); // end of array
      cBYE(status);
      break;
      }

    case j_object : 
      memset(&qdf_keys, 0, sizeof(QDF_REC_TYPE));
      status = get_keys(ptr_qdf, &qdf_keys); cBYE(status);
      uint32_t n_keys = get_arr_len(qdf_keys.data); 
      status = cat_to_buf(&buf, &bufsz, ptr_len, "{", 1); cBYE(status);
      for ( uint32_t i = 0; i < n_keys; i++ ) {
        mcr_pr_comma();
        SCLR_REC_TYPE sclr;
        status = get_arr_val(qdf_keys.data, i, &sclr, NULL); cBYE(status);
        status = cat_to_buf(&buf, &bufsz, ptr_len, "\"", 1); cBYE(status);
        status = cat_to_buf(&buf, &bufsz, ptr_len, sclr.val.str, 0); 
        cBYE(status);
        status = cat_to_buf(&buf, &bufsz, ptr_len, "\" : ", 4); cBYE(status);
        // Set up for recursion 
        memset(&qdf_val, 0, sizeof(QDF_REC_TYPE));
        status = get_key_val(ptr_qdf, (int)i, NULL, &qdf_val, NULL); cBYE(status);
        ptr_out_str->data = buf;
        ptr_out_str->size = bufsz;
        status = pr_json(&qdf_val, ptr_out_str, ptr_len, fp); cBYE(status); 
        buf = ptr_out_str->data;
        bufsz = ptr_out_str->size;
      }
      status = cat_to_buf(&buf, &bufsz, ptr_len, "}", 1); cBYE(status);
      break;

    default : 
      go_BYE(-1);
      break;
  }
BYE:
  ptr_out_str->data = buf;
  ptr_out_str->size = bufsz;
  return status;
}

int
pr_1(
    const void * const valptr, 
    const char * const nn_ptr,
    qtype_t qtype, 
    uint32_t width,
    uint32_t idx,
    FILE *fp
    )
{
  int status = 0;
  if ( fp == NULL ) { go_BYE(-1); }
  if ( ( nn_ptr != NULL ) && ( nn_ptr[idx] == 0 ) ) { 
    fprintf(fp, "\"\"");
    return status;
  }

  switch ( qtype ) {
    //-------------------
    case BL :
      {
        bool bval = ((const bool * const )valptr)[idx];
        fprintf(fp, "%s", bval ? "true" : "false");
      }
      break;
    case I1 :
      fprintf(fp, "%d", ((const int8_t * const )valptr)[idx]);
      break;
    case I2 :
      fprintf(fp, "%d", ((const int16_t * const )valptr)[idx]);
      break;
    case I4 :
      fprintf(fp, "%d", ((const int32_t * const )valptr)[idx]);
      break;
    case I8 :
      fprintf(fp, "%" PRIi64 "", ((const int64_t * const )valptr)[idx]);
      break;
      //-------------------
    case UI1 :
      fprintf(fp, "%u", ((const uint8_t * const )valptr)[idx]);
      break;
    case UI2 :
      fprintf(fp, "%u", ((const uint16_t * const )valptr)[idx]);
      break;
    case UI4 :
      fprintf(fp, "%u", ((const uint32_t * const )valptr)[idx]);
      break;
    case UI8 :
      fprintf(fp, "%" PRIu64 "", ((const uint64_t * const )valptr)[idx]);
      break;
      //-------------------
    case F4 :
      fprintf(fp, "%f", ((const float * const )valptr)[idx]);
      break;
    case F8 :
      fprintf(fp, "%lf", ((const double * const )valptr)[idx]);
      break;
      //-------------------
    case SC :
      {
        const char *cptr = (const char *)valptr;
        if ( width <= 1 ) { go_BYE(-1); } 
        cptr += (idx*width);
        fprintf(fp, "\""); 
        for ( ; *cptr != '\0'; cptr++ ) { 
          if ( ( *cptr == '\\' ) || ( *cptr == '\"' ) ) {
            fprintf(fp, "\\");
          }
          fprintf(fp, "%c", *cptr); 
        }
        fprintf(fp, "\""); 
      }
      break;
      //-------------------
    case TM1 :
      {
        const tm_t * const tmptr = (const tm_t * const )valptr;
        fprintf(fp, "%d-%02d-%02d", 
            1900+tmptr[idx].tm_year, 
            1+tmptr[idx].tm_mon, 
            tmptr[idx].tm_mday);
      }
      break;
      //-------------------
    default :
      printf("qtype = %d \n", qtype );
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}

int
x_pr_array(
    const QDF_REC_TYPE *const ptr_qdf,
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;

  mcr_chk_non_null(ptr_qdf, -1); 
  char *x = ptr_qdf->data;
  uint32_t n_elem = get_arr_len(x); if ( n_elem <= 0 ) { go_BYE(-1); }
  const char * const valptr = get_arr_ptr(x); 
  qtype_t qtype = get_qtype(x); if ( qtype == Q0 ) { go_BYE(-1); }
  jtype_t jtype = get_jtype(x); if ( jtype != j_array ) { go_BYE(-1); }
  uint32_t width = get_arr_width(x); if ( width == 0 ) { go_BYE(-1); }

  if ( ( file_name == NULL ) ||  ( *file_name == '\0' ) ) {
    fp = stdout;
  }
  else {
    fp = fopen(file_name, "w");
    return_if_fopen_failed(fp, file_name, "w");
  }
  for ( uint32_t i = 0; i < n_elem; i++ ) { 
    status = pr_1(valptr, NULL, qtype, width, i, fp);  cBYE(status);
    fprintf(fp, "\n");
  }
BYE:
  if ( ( file_name == NULL ) ||  ( *file_name == '\0' ) ) {
    // nothing to do
  }
  else {
    fclose_if_non_null(fp);
  }
  return status;
}

// This is only valid if jtype of input is j_array
int
pr_csv(
    const QDF_REC_TYPE * const src,
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;
  mcr_chk_non_null(src, -1); 
  //------------------------------------------------------
  if ( file_name == NULL ) { 
    fp = stdout; 
  }
  else { 
    fp = fopen(file_name, "w");
    return_if_fopen_failed(fp, file_name, "w");
  }
  //-------------------------
  char *sx = src->data; 
  jtype_t sjtype = get_jtype(sx); if ( sjtype != j_array ) {  go_BYE(-1); }
  uint32_t n     = get_arr_len(sx); 
  qtype_t sqtype = get_qtype(sx); 
  const char * const srcptr    = get_arr_ptr(src->data); 
  uint32_t width = 0;
  if ( sqtype == SC ) { 
    width = get_arr_width(sx); if ( width <= 1 )  { go_BYE(-1); } 
  }
  //-----------------------------------
  for ( uint32_t i = 0; i < n; i++ ) { 
    status = pr_1(srcptr, NULL, sqtype, width, i, fp); cBYE(status);
    fprintf(fp, "\n");
  }
BYE:
  if ( file_name != NULL ) { 
    fclose_if_non_null(fp);
  }
  return status;
}
int 
qdf_to_json_file(
    QDF_REC_TYPE *ptr_qdf, 
    const char * const template,
    char **ptr_json_file
    )
{
  int status = 0;
#define N 128
  char out_file[N]; memset(out_file, 0, N);
  QDF_REC_TYPE str_qdf; memset(&str_qdf, 0, sizeof(str_qdf));
  FILE *fp = NULL;
  // make a tempfile 
  strncpy(out_file, template, N-1);
  int fd =  mkstemp(out_file); if ( fd < 0 ) { go_BYE(-1);}
  close(fd); 
  strcat(out_file, "."); strcat(out_file, "json"); 
  // convert qdf to json 
  uint32_t len = 0; 
  status = pr_json(ptr_qdf, &str_qdf, &len, NULL); cBYE(status);
  // dump string to file 
  fp = fopen(out_file, "w");
  return_if_fopen_failed(fp, out_file, "w");
  fwrite(str_qdf.data, str_qdf.size, 1, fp); 
  *ptr_json_file = strdup(out_file);
BYE:
  fclose_if_non_null(fp);
  free_qdf(&str_qdf); // No longer needed, we have out_file
  return status;
}
