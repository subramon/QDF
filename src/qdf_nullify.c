#include "q_incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_pr.h"
#include "qdf_nullify.h"
// Currently, the nn vector for a vector is stored in a somewhat
// janky manner. We will need to revisit this code when that is fixed
int
qdf_nullify(
    QDF_REC_TYPE *ptr_qdf, // data frame containing col to be null'ed
    const char * const col_name
    )
{
  int status = 0;
  char *nn_col_name = NULL; 
  if ( ( col_name == NULL ) || ( *col_name == '\0' ) ) { go_BYE(-1); }
  if ( !get_is_df(ptr_qdf->data) ) { go_BYE(-1); }
  // Determine whether this column has a nn column
  uint32_t len_col_name = strlen(col_name)+strlen("_`nn_") + 8;
  nn_col_name = malloc(len_col_name);
  memset(nn_col_name, 0, len_col_name);
  sprintf(nn_col_name, "nn_%s", col_name);
  // get data for base column and  nullify it 
  QDF_REC_TYPE col_qdf; memset(&col_qdf, 0, sizeof(col_qdf));
  status = get_key_val(ptr_qdf, -1, col_name, &col_qdf, NULL);
  cBYE(status);
  // printf("PRE "); pr_csv(&col_qdf, NULL); // TODO 
  jtype_t chk_jtype = get_jtype(col_qdf.data);
  if ( chk_jtype != j_array ) { go_BYE(-1); }
  uint32_t width = get_arr_width(col_qdf.data);
  if ( width == 0 ) { go_BYE(-1); } 
  uint32_t n = get_arr_len(col_qdf.data); 
  if ( n == 0 ) { go_BYE(-1); }
  char *dptr = get_arr_ptr(col_qdf.data);
  memset(dptr, 0, n*width);

  // get data for nn column and  nullify it 
  QDF_REC_TYPE nn_col_qdf; memset(&nn_col_qdf, 0, sizeof(nn_col_qdf));
  status = get_key_val(ptr_qdf, -1, nn_col_name, &nn_col_qdf, NULL);
  cBYE(status);
  if ( ( nn_col_qdf.data == NULL ) || ( nn_col_qdf.size == 0 ) ) {
    go_BYE(-1);
  }
  qtype_t nn_qtype = get_qtype(nn_col_qdf.data);
  if ( ( nn_qtype != I1 ) && ( nn_qtype != BL ) ) { go_BYE(-1); }
  jtype_t chk_nn_jtype = get_jtype(nn_col_qdf.data);
  if ( chk_nn_jtype != j_array ) { go_BYE(-1); }
  uint32_t nn_n = get_arr_len(nn_col_qdf.data); 
  if ( nn_n == 0 ) { go_BYE(-1); }
  if ( nn_n != n ) { go_BYE(-1); }

  char *nn_dptr = get_arr_ptr(nn_col_qdf.data);
  switch ( nn_qtype ) { 
    case I1 : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        ((int8_t *)nn_dptr)[i] = 0;
      }
      break;
    case BL : 
      for ( uint32_t i = 0; i < n; i++ ) { 
        ((bool *)nn_dptr)[i] = false;
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
  // printf("POST "); pr_csv(&col_qdf, NULL); // TODO 
  // printf("POST "); pr_csv(&nn_col_qdf, NULL); // TODO 
BYE:
  free_qdf(&col_qdf);
  free_qdf(&nn_col_qdf);
  free_if_non_null(nn_col_name);
  return status;
}
