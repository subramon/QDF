#include "incs.h"
#include "qtypes.h"
#include "qdf_struct.h"
#include "free_2d_array.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_pr.h"
#include "qdf_html.h"

// TODO P2: Deal with id_keys 
// TODO P2: Make sure that text is escaped when needed 
//
// This is applicable only for a "data frame". By that we mean
// a JSON object with n keys, the value of each of which is an array, 
// all of which must be of the same size
// This is quite inefficient but since it is mainly for debugging,
// I haven't found the need to optimize it
int
pr_df_as_html(
    const QDF_REC_TYPE * const ptr_qdf,
    char ** const disp_keys, // [n_disp] Logical name: for display 
    char ** const in_viz_keys, // [n_disp] Display name
    char ** const in_id_keys, // [n_disp] id for JS/HTML purposes
    bool *is_editable, // [n_disp]
    uint32_t n_disp,
    char ** const in_edit_keys, // keys to be editable 
    uint32_t n_edit,
    bool is_all_non_editable, 
    bool is_all_editable, 
    const char * const table_id, 
    const char * const caption,
    const char * const file_name,
    char **ptr_outbuf,
    size_t *ptr_outlen,
    bool as_str // disregard file_name in this case
    )
{
  int status = 0;
  FILE *fp = NULL;
  char **viz_keys = NULL, **id_keys = NULL, **edit_keys = NULL;
  char **all_keys = NULL; uint32_t n_all = 0; 
  uint32_t *width = NULL;
  qtype_t *qtype = NULL;
  char **nnptr = NULL;
  char **valptr = NULL;


  // START: Basic checks on inputs 
  mcr_chk_non_null(ptr_qdf, -1); 
  if ( n_disp == 0 ) { go_BYE(-1); }
  if ( n_edit > n_disp ) { go_BYE(-1); }
  status = get_keys_as_array(ptr_qdf, &all_keys,  &n_all); cBYE(status);
  //-- Check whether disp_keys are alnum or underscore 
  for ( uint32_t i = 0; i < n_disp; i++ ) { 
    for ( char *cptr = disp_keys[i]; *cptr != '\0'; cptr++ )  {
      if ( !( ( isalnum(*cptr) ) || ( *cptr == '_') ) ) { go_BYE(-1); }
    }
  }
  //-- Check whether disp_keys are in dataframe
  for ( uint32_t i = 0; i < n_disp; i++ ) { 
    bool found = false;
    for ( uint32_t j = 0; j < n_all; j++ ) { 
      if ( strcmp(disp_keys[i], all_keys[j]) == 0 ) { 
        found = true; break;
      }
    }
    if ( !found ) { go_BYE(-1); }
  }
  //-- Check whether edit_keys are in disp_keys
  for ( uint32_t i = 0; i < n_edit; i++ ) { 
    bool found = false;
    for ( uint32_t j = 0; j < n_disp; j++ ) { 
      if ( strcmp(edit_keys[i], disp_keys[j]) == 0 ) { 
        found = true; break;
      }
    }
    if ( !found ) { go_BYE(-1); }
  }
  // STOP: Basic checks on inputs 
  // START: Create id, viz if needed and other auxiliary data structures
  if ( in_viz_keys != NULL ) { 
    viz_keys = in_viz_keys;
  }
  else {
    viz_keys = malloc(n_disp * sizeof(char *));
    memset(viz_keys, 0,  n_disp * sizeof(char *));
    for ( uint32_t i = 0; i < n_disp; i++ ) { 
      viz_keys[i] = strdup(disp_keys[i]);
    }
  }
  if ( in_id_keys != NULL ) { 
    id_keys = in_id_keys;
  }
  else {
    id_keys = malloc(n_disp * sizeof(char *));
    memset(id_keys, 0,  n_disp * sizeof(char *));
    for ( uint32_t i = 0; i < n_disp; i++ ) { 
      id_keys[i] = strdup(disp_keys[i]);
    }
  }
  if ( in_edit_keys != NULL ) { 
    edit_keys = in_edit_keys;
    if ( n_edit == 0 ) { go_BYE(-1); } 
  }
  else {
    if ( n_edit != 0 ) { go_BYE(-1); } 
    edit_keys = malloc(n_disp * sizeof(char *));
    memset(edit_keys, 0,  n_disp * sizeof(char *));
    for ( uint32_t i = 0; i < n_disp; i++ ) { 
      edit_keys[i] = strdup(disp_keys[i]);
    }
    n_edit = n_disp;
  }
  //-------------------------------------------------------------
  width = malloc(n_disp * sizeof(uint32_t));
  memset(width, 0,  n_disp * sizeof(uint32_t));

  qtype = malloc(n_disp * sizeof(qtype_t));
  memset(qtype, 0,  n_disp * sizeof(qtype_t));

  nnptr = malloc(n_disp * sizeof(char *));
  memset(nnptr, 0,  n_disp * sizeof(char *));

  valptr = malloc(n_disp * sizeof(char *));
  memset(valptr, 0,  n_disp * sizeof(char *));

  // get pointers to values
  for  ( uint32_t i = 0; i < n_disp; i++ ) {
    QDF_REC_TYPE col; memset(&col, 0, sizeof(QDF_REC_TYPE));
    status = get_key_val(ptr_qdf, -1, disp_keys[i], &col, NULL); 
    cBYE(status);
    qtype[i] = get_qtype(col.data); 
    if ( qtype[i] == SC ) {
      width[i] = get_arr_width(col.data); if ( width[i] < 1 ) {go_BYE(-1);}
    }
    valptr[i] = get_arr_ptr(col.data);
  }
  // get pointers to nn
  for  ( uint32_t i = 0; i < n_disp; i++ ) {
    QDF_REC_TYPE nn_col; memset(&nn_col, 0, sizeof(QDF_REC_TYPE));
    bool is_nn = false; 
    char *nn_key = NULL;
    size_t len = strlen(disp_keys[i]) + 8; 
    nn_key = malloc(len); memset(nn_key, 0, len);
    sprintf(nn_key, "nn_%s", disp_keys[i]);
    status = is_key(ptr_qdf, nn_key, &is_nn); 
    if ( is_nn ) { 
      status = get_key_val(ptr_qdf, -1, nn_key, &nn_col, NULL); 
      cBYE(status);
      qtype_t l_qtype = get_qtype(nn_col.data); 
      if ( ( l_qtype != BL ) && ( l_qtype != I1 ) ) { go_BYE(-1); }
      nnptr[i] = (char *)get_arr_ptr(nn_col.data); 
    }
    free_if_non_null(nn_key);
  }
  // STOP: Create id, viz if needed and other auxiliary data structures
  //-------------------------------------------------------------
  if ( as_str ) { 
    fp = open_memstream(ptr_outbuf, ptr_outlen);
  }
  else {
    if ( ( file_name == NULL ) || ( *file_name == '\0' ) ) {
      fp = stdout;
    }
    else { 
      fp = fopen(file_name, "w");
      return_if_fopen_failed(fp, file_name, "w");
    }
  }
  //-------------------------------------------------------
  fprintf(fp, "<table "
      "id=\"%s\" "
      "role=\"grid\"  "
      "aria-describedby=\"table-help\">\n", 
      table_id != NULL  ? table_id : "tbl_id");
  fprintf(fp, "<caption>%s</caption>\n", 
      caption != NULL  ? caption : "Caption of Table");
  //-------------------------------------------------------
  fprintf(fp, " <thead> \n  <tr> \n");
  for ( uint32_t i = 0; i < n_disp; i++ ) { 
    fprintf(fp, " <th scope=\"col\" data-key=\"%s\">%s</th>\n",
        id_keys[i], disp_keys[i]);
    // Do we need data-type?
  }
  fprintf(fp, "  </tr>\n</thead>\n<tbody>\n");
  //----- 
  if ( !x_get_is_df(ptr_qdf) ) { go_BYE(-1); }
  uint32_t n_rows = x_get_obj_arr_len(ptr_qdf); 
  //-------------------------------------------------------
  for ( uint32_t i = 0; i < n_rows; i++ ) { 
    fprintf(fp, "    <tr>\n");
    for ( uint32_t j = 0; j < n_disp; j++ ) { 
      //--------------------------------------------
      bool is_editable_j = false;
      if ( is_all_editable ) { 
        is_editable_j = true;
      }
      else if ( is_all_non_editable ) { 
        is_editable_j = false;
      }
      else {
        if ( is_editable != NULL ) {
          is_editable_j =  is_editable[j];
        }
      }
      //--------------------------------------------
      fprintf(fp, " <td contenteditable = \"%s\" data-key = \"%s\" > ",
          is_editable_j ? "true" : "false", id_keys[j]);
      status = pr_1(valptr[j], nnptr[j], qtype[j], width[j], i, fp); 
      if ( status != 0 ) {
        printf("hhello world \n");
      }
      cBYE(status);
      fprintf(fp, " </td>\n");
      // Do we need data-type?
    }
    fprintf(fp, "    </tr>\n");
  }
  fprintf(fp, "  </tbody>\n</table>\n");
  //------------------------------------------

  if ( as_str ) { 
    fclose_if_non_null(fp);
  }
  else {
    if ( file_name != NULL ) { 
      fclose_if_non_null(fp);
    }
  }
BYE:
  if ( in_viz_keys == NULL ) { free_2d_array(&viz_keys, n_disp); } 
  if ( in_id_keys == NULL ) { free_2d_array(&id_keys, n_disp); } 
  if ( in_edit_keys == NULL ) { free_2d_array(&edit_keys, n_disp); } 
  free_if_non_null(width);
  free_if_non_null(qtype);
  free_if_non_null(valptr);
  free_if_non_null(nnptr);
  return status;
}
