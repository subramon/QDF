// sends a dataframe in QDF format to Rserve
#include <jansson.h>
#include "incs.h"
#include "qdf_struct.h"
#include "free_2d_array.h"
#include "multiple.h"
#include "cat_to_buf.h"
#include "tm2time.h"
#include "aux_rcli.h"
#include "get_bool.h"
#include "exists.h"
#include "set_vec.h"
#include "get_time_usec.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "chk_R_class.h"
#include "get_R_class.h"
#include "n_df.h"
#include "qdf_df_to_Rserve.h"

#undef TM1_AS_SC

int 
qdf_df_to_Rserve(
    int sock,
    QDF_REC_TYPE *ptr_qdf,
    const char * const df_name
)
{
  int status = 0;
  uint32_t bufsz = 1024; uint32_t buflen = 0; char *df_str = NULL;
  int32_t *I4buf = NULL; double *F8buf = NULL;
  char *exec_out = NULL;   size_t len_out = 0;
  char *cmd = NULL; bool brslt;
  json_t *root = NULL;
  json_error_t error;
  char **col_names = NULL; uint32_t n_cols;
  char *nn_col_name = NULL, *vec_class = NULL;

  uint32_t num_sent_cols = 0;
  uint32_t num_rows = x_get_obj_arr_len(ptr_qdf);
  I4buf = malloc(num_rows * sizeof(int32_t));
  F8buf = malloc(num_rows * sizeof(double));
  if ( num_rows == 0 ) { go_BYE(-1); } 
  //------------------------------------------

  if ( !x_get_is_df(ptr_qdf) ) { go_BYE(-1); }
  uint32_t len  = x_get_obj_arr_len(ptr_qdf);

  status = get_keys_as_array(ptr_qdf, &col_names, &n_cols); cBYE(status);
  if ( n_cols == 0 ) { go_BYE(-1); } 
  size_t len_col_name = 0;
  for ( uint32_t c = 0; c < n_cols; c++ ) {
    size_t l = strlen(col_names[c]);
    if ( l > len_col_name ) {  len_col_name = l; }
  }
  if ( len_col_name == 0 ) { go_BYE(-1); } 
  len_col_name += 32; // for things like nn_ and so on
  uint32_t len_cmd = (uint32_t)(4*multiple_n(len_col_name, 8) + 64);
  cmd = malloc(len_cmd); 
  //---------------------------------------
  df_str = malloc(bufsz); memset(df_str, 0, bufsz);
  status = cat_to_buf(&df_str, &bufsz, &buflen, df_name, 0);
  status = cat_to_buf(&df_str, &bufsz, &buflen, " <- data.frame( ", 0);
  cBYE(status);
  //---------------------------------------
  for ( uint32_t c = 0; c < n_cols; c++ ) {
    free_if_non_null(nn_col_name);
    const char * const col_name = col_names[c];
    // We use the following convention.
    // Assume that a key "foo" has null values
    // Then, we create another key "nn_foo" whose values are 0/1 and 
    // such that nn_foo[i] == 0 => foo[i] == NULL 
    // such that nn_foo[i] == 1 => foo[i] != NULL 
    if ( strncmp(col_name, "nn_", strlen("nn_")) == 0 ) { 
      continue;
    }
    num_sent_cols++;
    if ( c > 0 ) { 
      status = cat_to_buf(&df_str, &bufsz, &buflen, ", ", 0);
      cBYE(status);
    }
    status = cat_to_buf(&df_str, &bufsz, &buflen, col_name, 0);
    cBYE(status);
    //----------------------------------------------
    QDF_REC_TYPE col_qdf; memset(&col_qdf, 0, sizeof(col_qdf));
    status = get_key_val(ptr_qdf, -1, col_name, &col_qdf, NULL);
    cBYE(status);
    qtype_t qtype = x_get_qtype(&col_qdf);
    jtype_t chk_jtype = x_get_jtype(&col_qdf);
    if ( chk_jtype != j_array ) { go_BYE(-1); }
    char *dptr = get_arr_ptr(col_qdf.data);
    //--------------------------------------
    // START: Find out if there is an nn array associated
    nn_col_name = malloc(strlen(col_name) + 8);
    sprintf(nn_col_name, "nn_%s", col_name); 
    // STOP : Find out if there is an nn array associated
    //--------------------------------------
    switch ( qtype ) {
      case BL :
        for ( uint32_t i = 0; i < num_rows; i++ ) {
          I4buf[i] = (int32_t)((bool *)dptr)[i];
        }
        status = set_vec(sock, col_name, "I4", I4buf, NULL, num_rows, 0);
        cBYE(status);
        break;
      case I1 :
        for ( uint32_t i = 0; i < num_rows; i++ ) {
          I4buf[i] = (int32_t)((int8_t *)dptr)[i];
        }
        status = set_vec(sock, col_name, "I4", I4buf, NULL, num_rows, 0);
        cBYE(status);
        break;
      case I2 :
        for ( uint32_t i = 0; i < num_rows; i++ ) {
          I4buf[i] = (int32_t)((int16_t *)dptr)[i];
        }
        status = set_vec(sock, col_name, "I4", I4buf, NULL, num_rows, 0);
        cBYE(status);
        break;
      case I4 :
        status = set_vec(sock, col_name, "I4", dptr, NULL, num_rows, 0);
        cBYE(status);
        break;
      case F4 : 
        for ( uint32_t i = 0; i < num_rows; i++ ) {
          F8buf[i] = (double)((float *)dptr)[i];
        }
        status = set_vec(sock, col_name, "F8", F8buf, NULL, num_rows, 0);
        cBYE(status);
        break;
      case F8 :
        status = set_vec(sock, col_name, "F8", dptr, NULL, num_rows, 0);
        cBYE(status);
        break;
      case TM1 :
        // x is time in seconds since epoch / 86400 
        // > y = as.Date(x, origin='1970-01-01')
        {
          struct tm t1; memset(&t1, 0, sizeof(struct tm));
          struct tm zero; memset(&zero, 0, sizeof(struct tm));
          tm_t *tptr = (tm_t *)dptr;
#ifndef TM1_AS_SC
          // This was my initial plan for how to send TM1 to R
          for ( uint32_t i = 0; i < num_rows; i++ ) {
            t_assign(&t1, tptr+i);
            if ( memcmp(&t1, &zero, sizeof(struct tm)) == 0 ) { 
              I4buf[i] = 0;
            }
            else {
              time_t t = tm2time(&t1);
              I4buf[i] = (int32_t)(t/86400);
            }
          }
          status = set_vec(sock, col_name, "I4", I4buf, NULL, num_rows, 0);
          cBYE(status);
          snprintf(cmd, len_cmd-1, "%s[%s == 0] <- NA", col_name, col_name);
          status = exec_str(sock, cmd, NULL, NULL, -1); cBYE(status);

          snprintf(cmd, len_cmd-1, "%s = as.Date(%s, origin='1970-01-01')",
              col_name, col_name);
          status = exec_str(sock, cmd, NULL, NULL, -1); cBYE(status);
#else
          uint32_t width = strlen("YYYY-MM-DD")+1;
          size_t l_buflen = num_rows * width;
          char *buffer = malloc(l_buflen);
          return_if_malloc_failed(buffer);
          memset(buffer, 0, l_buflen);
          for ( uint32_t i = 0; i < num_rows; i++ ) { 
            char buf[width]; memset(buf, 0, width);
            if ( tptr[i].tm_year > 0 ) { 
            snprintf(buf, width-1, "%4d-%2d-%2d", 
                tptr[i].tm_year, tptr[i].tm_mon, tptr[i].tm_mday);
            }
            else {
              // entry has already been nulled out 
            }
            strcpy(buffer+(i*width), buf);
          }
          status = set_vec(sock, col_name, "SC", buffer, NULL, num_rows, width);
          free_if_non_null(buffer);
          cBYE(status);
#endif
        }
        break;
      case SC :
        {
          uint32_t width = x_get_arr_width(&col_qdf); 
          if ( width == 0 ) { go_BYE(-1); } 
          status = set_vec(sock, col_name, "SC", dptr, NULL, num_rows, width);
          cBYE(status);
          if ( strcmp(col_name, "within_cluster_embedding") == 0 ) {
#ifdef DEBUG
            // Check that C thinks it is valid JSON
            root = json_loads(dptr, 0, &error);
            if ( root == NULL ) { go_BYE(-1); } else { json_decref(root); }
            // Check that R thinks it is valid JSON
            bool bexists;
            status = exists1(sock, col_name, &bexists); cBYE(status);
            if ( !bexists ) { go_BYE(-1); } 
            for ( uint32_t k = 0; k < num_rows; k++ ) {
              // Print kth embedding 
              free_if_non_null(exec_out); len_out = 0;
              sprintf(cmd, "%s[%d]", col_name, k+1); 
              status = exec_str(sock, cmd, &exec_out, &len_out, XT_ARRAY_STR);
              // printf("kth embedding = %s \n", exec_out); 
              sprintf(cmd, "is_json <- jsonlite::validate(%s[%u])", 
                  col_name, k+1);
              status = exec_str(sock, cmd, NULL, NULL, -1); 
              cBYE(status);
              if ( !chk_R_class(sock, "is_json", "logical") ) { go_BYE(-1); } 
              bool is_json;
              status = get_bool(sock, "is_json", &is_json);
              if ( !is_json ) { go_BYE(-1); }
            }
#endif
          }
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
#ifdef DEBUG
    status = is_vector(sock, col_name, &brslt); cBYE(status);
    if ( !brslt ) { go_BYE(-1); }
    free_if_non_null(exec_out); len_out = 0;
    snprintf(cmd, len_cmd-1, "class(%s)", col_name); 
    status = exec_str(sock, cmd, &exec_out, &len_out,XT_ARRAY_STR); 
    cBYE(status);
    if ( ( exec_out == NULL ) || ( len_out == 0 ) ) { go_BYE(-1); }
    switch ( qtype ) { 
      case I1 : case I2 : case I4 : 
        if ( strcmp(exec_out, "integer") != 0 )  { go_BYE(-1); }
        break;
      case F4 : case F8 : 
        if ( strcmp(exec_out, "numeric") != 0 )  { go_BYE(-1); }
        break;
      case TM1 : 
#ifndef TM1_AS_SC
        if ( strcmp(exec_out, "Date") != 0 )  { go_BYE(-1); }
#else
        if ( strcmp(exec_out, "character") != 0 )  { go_BYE(-1); }
#endif
        break;
      case SC : 
        if ( strcmp(exec_out, "character") != 0 )  { go_BYE(-1); }
        break;
      default :
        // TODO More tests 
        break;
    }
    //--- Check length of vector
    free_if_non_null(exec_out); len_out = 0;
    snprintf(cmd, len_cmd, "length(%s)", col_name); 
    status = exec_str(sock, cmd, &exec_out, &len_out, XT_ARRAY_INT); 
    cBYE(status);
    if ( ( exec_out == NULL ) || ( len_out == 0 ) ) { go_BYE(-1); }
    if ( ((uint32_t *)exec_out)[0] != num_rows ) { go_BYE(-1); }
    free_if_non_null(exec_out);

#endif
    // Determine whether this column has a nn column
    len_col_name = strlen(col_name)+strlen("_`nn_") + 8;
    free_if_non_null(nn_col_name);
    nn_col_name = malloc(len_col_name);
    memset(nn_col_name, 0, len_col_name);
    sprintf(nn_col_name, "nn_%s", col_name);
    bool has_nulls = false;
    for ( uint32_t i = 0; i < n_cols; i++ ) {
      if ( strcmp(col_names[i], nn_col_name) == 0 ) {
        has_nulls = true;
        break;
      }
    }
    // send nn column if needed 
    if ( has_nulls ) {
      // get data for nn column 
      QDF_REC_TYPE nn_col_qdf; memset(&nn_col_qdf, 0, sizeof(nn_col_qdf));
      status = get_key_val(ptr_qdf, -1, nn_col_name, &nn_col_qdf, NULL);
      cBYE(status);
      qtype_t nn_qtype = x_get_qtype(&nn_col_qdf);
      if ( ( nn_qtype != I1 ) && ( nn_qtype != BL ) ) { go_BYE(-1); }
      jtype_t chk_nn_jtype = x_get_jtype(&nn_col_qdf);
      if ( chk_nn_jtype != j_array ) { go_BYE(-1); }
      char *nn_dptr = get_arr_ptr(nn_col_qdf.data);
      //--------------------------------------
      status = set_vec(sock, nn_col_name, "I1", nn_dptr, NULL, num_rows, 0);
      cBYE(status);
#ifdef DEBUG
      status = is_vector(sock, nn_col_name, &brslt); cBYE(status);
      if ( !brslt ) { go_BYE(-1); }
      memset(cmd, 0, len_cmd);
      sprintf(cmd, "%s <- %s != 0", nn_col_name, nn_col_name);
      status = exec_str(sock, cmd, NULL, NULL, -1); cBYE(status);
    vec_class = get_R_class(sock, nn_col_name); 
    if ( vec_class == NULL ) { go_BYE(-1); }
    free_if_non_null(vec_class);
      if ( !chk_R_class(sock, nn_col_name, "logical") ) { go_BYE(-1); } 
#endif
      memset(cmd, 0, len_cmd);
      switch ( qtype ) {
        case I1 : case I2 : case I4 : case I8 : 
          sprintf(cmd, "%s <- ifelse(%s, %s, NA_integer_); \n",
              col_name, nn_col_name, col_name); 
          break;
        case F4 : case F8 : 
          sprintf(cmd, "%s <- ifelse(%s, %s, NA_real_); \n",
              col_name, nn_col_name, col_name); 
          break;
        case SC : 
          sprintf(cmd, "%s <- ifelse(%s, %s, NA_character_); \n",
              col_name, nn_col_name, col_name); 
          break;
#ifndef TM1_AS_SC 
        case TM1 :
          sprintf(cmd, "%s <- as.Date(ifelse(%s, %s, NA_Date_)); \n",
              col_name, nn_col_name, col_name); 
#else
          sprintf(cmd, "%s <- ifelse(%s, %s, NA_character_); \n",
              col_name, nn_col_name, col_name); 
#endif
          break;
        default :
          go_BYE(-1);
          break;
      }
      if ( *cmd != '\0' ) { 
        status = exec_str(sock, cmd, NULL, NULL, -1); cBYE(status);
        sprintf(cmd, "rm(%s); \n", nn_col_name); 
        status = exec_str(sock, cmd, NULL, NULL, -1); cBYE(status);
        if ( qtype == TM1 ) {
#ifndef TM1_AS_SC 
          if ( !chk_R_class(sock, col_name, "Date") ) { go_BYE(-1); } 
#else
          if ( !chk_R_class(sock, col_name, "character") ) { go_BYE(-1); } 
#endif
        }
      }
    }
    free_if_non_null(nn_col_name);
  }
  status = cat_to_buf(&df_str, &bufsz, &buflen, ")", 0); 
  cBYE(status);
  // make data frame out of vectors
  status = exec_str(sock, df_str, NULL, NULL, -1); cBYE(status);
#ifdef DEBUG
  if ( !chk_R_class(sock, df_name, "data.frame") ) { go_BYE(-1); } 
  // Check number of columns in data frame 
  if ( ncols_df(sock, df_name) != num_sent_cols )  { go_BYE(-1); }
  if ( nrows_df(sock, df_name) != num_rows )  { go_BYE(-1); }
#endif
  sprintf(cmd, "saveRDS(%s, file  = \"/tmp/cfg.Rdata\")", df_name);
  status = exec_str(sock, df_str, NULL, NULL, -1); cBYE(status);
  
  // STOP: send stuff over to R
BYE:
  free_2d_array(&col_names, n_cols);
  free_if_non_null(df_str);
  free_if_non_null(I4buf);
  free_if_non_null(F8buf);
  free_if_non_null(nn_col_name);
  free_if_non_null(cmd);
  free_if_non_null(vec_class);
  return status;
}
/*
    // Check into nn column 
    QDF_REC_TYPE nn_col_qdf; memset(&nn_col_qdf, 0, sizeof(nn_col_qdf));
    char nn_col_name[len_col_name]; memset(nn_col_name, 0, len_col_name);
    char *nn_dptr = NULL;
    if ( ( meta->has_nulls != NULL ) && ( meta->has_nulls[c] ) ) { 
      sprintf(nn_col_name, "nn_%s", col_name); 
      // Note that has_nulls can be true and we may still not have
      // nn column in qdf because it was not needed
      bool b_is_key; status = is_key(ptr_qdf, nn_col_name, &b_is_key);
      cBYE(status);
      if ( b_is_key ) { 
        status = get_key_val(ptr_qdf, -1, nn_col_name, &nn_col_qdf, NULL);
        nn_dptr = get_arr_ptr(nn_col_qdf.data);
      }
*/
