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
qdf_add_col_to_df(
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
  qtype_t *qtypes = NULL; uint32_t chk_n;
  uint32_t *widths = NULL; 

  uint32_t sz_rows = get_obj_arr_size(src->data); 
  uint32_t n_rows  = get_obj_arr_len(src->data); 
  status = get_keys_as_array(src, &cols, &n_cols); cBYE(status);
  // new column cannot have same name as existing columns 
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    if ( strcmp(cols[i], col) == 0 ) { go_BYE(-1); }
  }
  status = get_qtypes_as_array(src, &qtypes, &chk_n); cBYE(status);
  if ( chk_n != n_cols ) { go_BYE(-1); }
  status = get_widths_as_array(src, &qtypes, &chk_n); cBYE(status);
  if ( chk_n != n_cols ) { go_BYE(-1); }

  status = make_empty_data_frame(cols, n_cols, qtypes, widths, 
      sz_rows, dst); // TODO P3 Should this be n_rows ?
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
  return status;
}
