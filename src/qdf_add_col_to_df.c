#include "incs.h"
#include "free_2d_array.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_makers.h"
#include "qdf_setters.h"
#include "qdf_checkers.h"
#include "qtypes.h"
#include "multiple.h"
#include "qdf_add_col_to_df.h"

int
<<<<<<< HEAD
qdf_add_col_to_df(
=======
add_col_to_df(
>>>>>>> 8b19c14aaaedc7bfae3693856510625aada06477
    QDF_REC_TYPE *src,
    const char * const col, // name of new column
    QDF_REC_TYPE *add, // column to be added 
    QDF_REC_TYPE *dst
    )
{
  int status = 0;
  mcr_chk_non_null(src, -1); 
  mcr_chk_null(dst, -1); 
  char **cols = NULL; uint32_t n_cols = 0;
<<<<<<< HEAD
  qtype_t *qtypes = NULL; uint32_t chk_n;
  uint32_t *widths = NULL; 
=======
  qtype_t *qtypes = NULL; 
  uint32_t *widths = NULL; 
  char **new_cols = NULL; uint32_t new_n_cols = 0;
  qtype_t *new_qtypes = NULL; 
  uint32_t *new_widths = NULL; 
  uint32_t chk_n;
>>>>>>> 8b19c14aaaedc7bfae3693856510625aada06477

  uint32_t sz_rows = get_obj_arr_size(src->data); 
  uint32_t n_rows  = get_obj_arr_len(src->data); 
  status = get_keys_as_array(src, &cols, &n_cols); cBYE(status);
  // new column cannot have same name as existing columns 
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    if ( strcmp(cols[i], col) == 0 ) { go_BYE(-1); }
  }
  status = get_qtypes_as_array(src, &qtypes, &chk_n); cBYE(status);
  if ( chk_n != n_cols ) { go_BYE(-1); }
<<<<<<< HEAD
  status = get_widths_as_array(src, &qtypes, &chk_n); cBYE(status);
  if ( chk_n != n_cols ) { go_BYE(-1); }

  status = make_empty_data_frame(cols, n_cols, qtypes, widths, 
      sz_rows, dst); // TODO P3 Should this be n_rows ?
=======
  status = get_widths_as_array(src, &widths, &chk_n); cBYE(status);
  if ( chk_n != n_cols ) { go_BYE(-1); }
  // add to create new meta data 
  new_n_cols  = n_cols + 1; 
  new_qtypes = malloc(new_n_cols * sizeof(qtype_t));
  new_widths = malloc(new_n_cols * sizeof(uint32_t));
  new_cols   = malloc(new_n_cols * sizeof(char *));
  memset(new_cols, 0,  new_n_cols * sizeof(char *));
  // add existing columns
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    new_qtypes[i] = qtypes[i];
    new_widths[i] = widths[i];
    new_cols[i]   = strdup(cols[i]); 
  }
  // Check
  if ( get_jtype(add->data) != j_array ) { go_BYE(-1); }
  chk_n = get_arr_len(add->data); 
  if ( chk_n != n_rows ) { go_BYE(-1); }
  // Add new column
  new_qtypes[n_cols] = get_qtype(add->data);
  new_widths[n_cols] = get_arr_width(add->data);
  new_cols[n_cols]   = strdup(col);

  // Check length of new_col versus length of existing 
  status = make_empty_data_frame(new_cols, new_n_cols, new_qtypes, 
      new_widths, sz_rows, dst); // TODO P3 Should this be n_rows ?
>>>>>>> 8b19c14aaaedc7bfae3693856510625aada06477
  cBYE(status);
  // add old columns
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    QDF_REC_TYPE col_qdf; memset(&col_qdf, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(src, -1, cols[i], &col_qdf, NULL); cBYE(status);
    status = place_in_data_frame(dst, cols[i], &col_qdf);
  }
  // add new column 
  status = place_in_data_frame(dst, col, add); cBYE(status);
BYE:
  free_2d_array(&cols, n_cols);
  free_if_non_null(qtypes);
  free_if_non_null(widths);
<<<<<<< HEAD
=======
  free_2d_array(&new_cols, new_n_cols);
  free_if_non_null(new_qtypes);
  free_if_non_null(new_widths);
>>>>>>> 8b19c14aaaedc7bfae3693856510625aada06477
  return status;
}
