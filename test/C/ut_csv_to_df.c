#include <jansson.h>
#include "incs.h"
#include "qtypes.h"
#include "qdf_struct.h"
#include "free_2d_array.h"
#include "split_str.h"
#include "num_lines.h"
#include "qdf_checkers.h"
#include "qdf_external.h"
#include "qdf_helpers.h"
#include "qdf_pr.h"
#include "qdf_xhelpers.h"
#include "qdf_csv_to_df.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  json_t *jroot = NULL; json_error_t error; // for debugging 

  char **cols = NULL; // [ncols]
  uint32_t ncols = 0; 
  char **str_qtypes = NULL; // [nqtypes]
  uint32_t nqtypes = 0; 
  QDF_REC_TYPE qdf; memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE jqdf; memset(&jqdf, 0, sizeof(QDF_REC_TYPE));
  qtype_t *qtypes = NULL;  // [ncols]
  uint32_t *widths = NULL;  // [ncols]
  char **formats = NULL;  // [ncols]
  bool *has_nulls = NULL; // [ncols]
  bool *is_load = NULL; // [ncols]


  if ( argc != 5 ) { go_BYE(-1); } 
  const char * const infile = argv[1];
  char * const in_cols = argv[2];
  char * const in_str_qtypes = argv[3];
  const char * const opfile = argv[4];
  bool is_hdr = true;

  status = split_str(in_cols, ",", &cols, &ncols);
  cBYE(status);
  if ( ncols == 0 ) { go_BYE(-1); }

  status = split_str(in_str_qtypes, ",", &str_qtypes, &nqtypes);
  cBYE(status);
  if ( nqtypes != ncols ) { go_BYE(-1); }

  qtypes    = malloc(ncols * sizeof(qtype_t));
  widths    = malloc(ncols * sizeof(uint32_t));
  formats   = malloc(ncols * sizeof(char *));
  has_nulls = malloc(ncols * sizeof(bool));
  is_load   = malloc(ncols * sizeof(bool));

  for ( uint32_t i = 0; i < ncols; i++ ) { 
    has_nulls[i] = false;
    is_load[i] = true;
    qtypes[i] = get_c_qtype(str_qtypes[i]);
    formats[i] = get_format(str_qtypes[i]);
    widths[i] = get_width_c_qtype(qtypes[i]);
  }

  status = qdf_csv_to_df(infile, NULL, 0, cols, qtypes,
      widths, formats, has_nulls, is_load, ncols, 
      ",", "\"", "\n", is_hdr, &qdf); 
  status = chk_qdf(&qdf);
  if ( !x_get_is_df(&qdf) ) { go_BYE(-1); } 
  int itmp = num_lines(infile, NULL, 0);
  uint32_t chk_nrows = 0;
  if ( is_hdr ) { 
    if ( itmp <= 1 ) { go_BYE(-1); }
    chk_nrows = (uint32_t)itmp - 1;
  }
  else {
    chk_nrows = (uint32_t)itmp;
  }
  uint32_t nrows = x_get_obj_arr_len(&qdf); 
  if ( chk_nrows != nrows ) { go_BYE(-1); }
  status = chk_qdf(&qdf); cBYE(status);
  status = pr_df_as_csv(&qdf, NULL, 0, opfile); cBYE(status);
  jqdf.size = 65536;
  jqdf.data = malloc(65536);
  memset(jqdf.data,0,  65536);
  uint32_t jlen = 0;
  status = pr_json(&qdf, &jqdf, &jlen, NULL); cBYE(status);
  printf("%s\n", (char *)jqdf.data);
  jroot = json_loads((char *)jqdf.data, 0, &error);
  if ( jroot == NULL ) { go_BYE(-1); } else { json_decref(jroot); }

  fprintf(stdout, "SUCCESS; Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_2d_array(&cols, ncols);
  free_2d_array(&str_qtypes, ncols);
  if ( formats != NULL ) { 
    free_2d_array(&formats, ncols);
  }
  free_if_non_null(has_nulls);
  free_if_non_null(is_load);
  free_qdf(&qdf);
  free_qdf(&jqdf);
  return status;
}
