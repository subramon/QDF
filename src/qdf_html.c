#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "qdf_html.h"

// This is applicable only for a "data frame". By that we mean
// a JSON object with n keys, the value of each of which is an array, 
// all of which must be of the same size
// This is quite inefficient but since it is mainly for debugging,
// I haven't found the need to optimize it
int
pr_df_as_html(
    const QDF_REC_TYPE * const ptr_qdf,
    char ** const disp_keys, // keys to be displayed 
    char ** const ui_keys, // UI name of key to be displayed 
    uint32_t n_disp,
    char ** const edit_keys, // keys to be editable 
    uint32_t n_edit,
    bool all_non_editable, 
    bool all_editable, 
    const char * const table_id, 
    const char * const caption,
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;

  mcr_chk_non_null(ptr_qdf, -1); 

  if ( n_disp == 0 ) { go_BYE(-1); }
  if ( n_edit > n_disp ) { go_BYE(-1); }
#ifdef HANDLE_NN
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
#endif 
  //-- Check whether disp_keys are in dataframe
  bool are_keys = false;
  status = are_keys_in_object(ptr_qdf, 
      (char ** const )disp_keys, n_keys, &are_keys);
  cBYE(status);
  if ( !are_keys ) { go_BYE(-1); }
  //-- Check whether edit_keys are in disp_keys
  //-- TODO 
  //------------------------------------------------------
  if ( ( file_name == NULL ) || ( *file_name == '\0' ) ) {
    fp = stdout;
  }
  else { 
    fp = fopen(file_name, "w");
    return_if_fopen_failed(fp, file_name, "w");
  }
  //-------------------------------------------------------
  fprintf(fp, "<table "
      "id=\"%s\" 
      "role=\"grid\" 
      "aria-describedby=\"table-help\">\n", table_id);
  fprintf(fp, "<caption>%s</caption>\n", caption):
  //-------------------------------------------------------
  fprintf(fp, " <thead> \n  <tr> \n");
  for ( uint32_t i = 0; i < n_disp; i++ ) { 
    fprintf(fp, " <th scope="col" data-key="%s">%s</th>\n",
        ui_keys[i], disp_keys[i]);
    // Do we need data-type?
  }
  fprintf(fp, "  </tr>\n</thead>\n><tbody>\n");
  //----- 
  if ( !x_get_is_df(ptr_qdf) ) { go_BYE(-1); }
  uint32_t n_rows = x_get_obj_arr_len(ptr_qdf); 
  //-------------------------------------------------------
  for ( uint32_t i = 0; i < n_rows; i++ ) { 
    fprintf(fp, "    <tr>\n");
    for ( uint32_t j = 0; j < n_disp; j++ ) { 
      fprintf(fp, " <td "
          "contenteditable = \"%s\"
          "data-key = \"%s\""
          "%s</td>\n",
          disp_keys[i], is_editable, value);
      // Do we need data-type?
    }
    fprintf(fp, "    </tr>\n");
  }
  fprintf(fp, "  </tbody>\n><table>>\n");
BYE:

  if ( as_html ) { 
    fprintf(fp, "  <table border =\"1\">\n    <thead>      <tr>\n"); 
  }
  for ( uint32_t i = 0; i < n_keys; i++ ) { 
    if ( as_html ) { 
      fprintf(fp, "<th> %s </th>", keys[i]);
    }
    else {
      if ( i > 0 ) { fprintf(fp, ","); }
      fprintf(fp, "%s", keys[i]);
    }
  }
  if ( as_html ) { 
    fprintf(fp, "      </tr>\n    </thead>\n    <tbody>\n"); 
  }
  fprintf(fp, "\n");
  //------------------------------------------
#ifdef TODO 
  for ( uint32_t i = 0; i < n_rows; i++ ) {
    if ( as_html ) { fprintf(fp, "      <tr>\n"); }
    for ( uint32_t j = 0; j < n_keys; j++ ) {
      if ( as_html ) { 
        fprintf(fp, "        <th>\n");
      }
      else {
        if ( j > 0 ) { fprintf(fp, ","); }
      }
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
        if ( ( qtype != BL ) && ( qtype != I1 ) ) { go_BYE(-1); }
        nn_ptr = get_arr_ptr(nn_col.data); 
      }
      free_if_non_null(nn_key);
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
      if ( as_html ) { 
        fprintf(fp, "        </th>\n");
      }
    }
    if ( as_html ) { fprintf(fp, "      </tr>"); }
    fprintf(fp, "\n");
  }
#endif

BYE:
  if ( file_name != NULL ) { 
    fclose_if_non_null(fp);
  }
  return status;
}
