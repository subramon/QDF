#include "incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "prep_args_for_read_csv.h"
#include "qdf_struct.h"
#include "split_str.h"
#include "qdf_checkers.h"
#include "qdf_concat.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_external.h"
#include "qdf_csv_to_df.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  QDF_REC_TYPE out_qdf; memset(&out_qdf, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE *qdfs = NULL; memset(&out_qdf, 0, sizeof(QDF_REC_TYPE));
  if ( argc != 5 ) { go_BYE(-1); } 
  const char * const infile = argv[1];
  char * const   in_cols = argv[2];
  char * const in_str_qtypes = argv[3];

  char **cols = NULL; uint32_t ncols = 0;
  qtype_t *qtypes = NULL;  // [ncols]
  uint32_t *widths = NULL;  // [ncols]
  char **formats = NULL;  // [ncols]
  bool *has_nulls = NULL; // [ncols]
  bool *is_load = NULL; // [ncols]

  int itmp = atoi(argv[4]); if ( itmp < 0 ) { go_BYE(-1); } 
  uint32_t n_qdfs = (uint32_t)itmp; 
  qdfs = malloc(n_qdfs * sizeof(QDF_REC_TYPE));
  memset(qdfs, 0,  n_qdfs * sizeof(QDF_REC_TYPE));

  bool is_hdr = true;
  for ( uint32_t i = 0; i < n_qdfs; i++ ) {
    status = prep_args_for_read_csv(in_cols, in_str_qtypes,
        &qtypes, &widths, &formats, &has_nulls, &is_load , &cols, &ncols);
    status = qdf_csv_to_df(infile, NULL, 0, cols, qtypes,
        widths, formats, has_nulls, is_load, ncols, 
        ",", "\"", "\n", is_hdr, &(qdfs[i])); 
    status = chk_qdf(&(qdfs[i])); cBYE(status);
    if ( !x_get_is_df(&(qdfs[i])) ){ go_BYE(-1); } 
    //----------------------------------------
    free_if_non_null(qtypes);
    free_if_non_null(widths);
    free_2d_array(&formats, ncols);
    free_if_non_null(has_nulls);
    free_if_non_null(is_load);
    free_2d_array(&cols, ncols);
    //----------------------------------------
  }
    status = prep_args_for_read_csv(in_cols, in_str_qtypes,
        &qtypes, &widths, &formats, &has_nulls, &is_load , &cols, &ncols);
  status = qdf_concat(qdfs, n_qdfs, cols, ncols, &out_qdf); 
  cBYE(status);

  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);

BYE:
  free_qdf(&out_qdf);
  free_qdfs(&qdfs, "free", n_qdfs);
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_2d_array(&formats, ncols);
  free_if_non_null(has_nulls);
  free_if_non_null(is_load);
  free_2d_array(&cols, ncols);
  return status;
}
