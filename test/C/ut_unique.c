#include "incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "prep_args_for_read_csv.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "num_lines.h"
#include "qdf_uq.h"
#include "unique.h"
#include "qdf_csv_to_df.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  double *dvals = NULL; uint32_t n, n_uq, max_n_uq = 10; 
  QDF_REC_TYPE qdf, col; 
  memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  memset(&col, 0, sizeof(QDF_REC_TYPE));
  const char * const infile = "../data/test_unique_1.csv";
  char * in_cols = strdup("x");
  char * in_str_qtypes = strdup("I2");
  bool is_hdr = true;
  char **cols = NULL; uint32_t ncols = 0;
  qtype_t *qtypes = NULL;  // [ncols]
  uint32_t *widths = NULL;  // [ncols]
  char **formats = NULL;  // [ncols]
  bool *has_nulls = NULL; // [ncols]
  bool *is_load = NULL; // [ncols]

  if ( argc != 1 ) { go_BYE(-1); } 

  status = prep_args_for_read_csv(in_cols, in_str_qtypes,
    &qtypes, &widths, &formats, &has_nulls, &is_load , &cols, &ncols);
  status = qdf_csv_to_df(infile, NULL, 0, cols, qtypes,
      widths, formats, has_nulls, is_load, ncols, 
      ",", "\"", "\n", is_hdr, &qdf); 

  status = chk_qdf(&qdf); cBYE(status);
  status = get_key_val(&qdf, -1, "x", &col, NULL); cBYE(status);
  status = x_num_unique(&col, NULL, &n); cBYE(status);
  if ( n != 5 ) { go_BYE(-1); } 
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
  // TODO P2 Repeat test with null values 
  // TODO P2 write  atest for is_unique
  // A test for num_unique_saturated
  n = 20;
  dvals = malloc(n * sizeof(double));
  double d = 1;
  for ( uint32_t i = 0; i < n; i++ ) { 
    dvals[i] = d++;
  }
  status = num_unique_saturated((const char * const)dvals, 
         n, F8, max_n_uq, &n_uq);
  cBYE(status);
  if ( n_uq != max_n_uq ) { go_BYE(-1); }
  //---------------------------------------
  d = 1; 
  for ( uint32_t i = 0; i < n; i++ ) { 
    dvals[i] = d++; if ( ( i % 3 ) == 0 ) { d = 1; }
  }
  status = num_unique_saturated((const char * const)dvals, 
      n, F8, max_n_uq, &n_uq);
  cBYE(status);
  if ( n_uq != 3 ) { go_BYE(-1); }
  fprintf(stdout, "SUCCESS. Completed %s \n", argv[0]);

BYE:
  free_qdf(&qdf);
  free_qdf(&col);
  free_if_non_null(in_cols);
  free_if_non_null(in_str_qtypes);
  free_if_non_null(dvals);
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_2d_array(&cols, ncols);
  free_2d_array(&formats, ncols);
  free_if_non_null(has_nulls);
  free_if_non_null(is_load);
  return status;
}
