#include "incs.h"
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
  char * cols = strdup("x");
  char * qtypes = strdup("I2");
  bool is_hdr = true;
  BUF_SPEC_TYPE buf_spec;
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  if ( argc != 1 ) { go_BYE(-1); } 

  status = qdf_csv_to_df(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf); 
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
  free_if_non_null(cols);
  free_if_non_null(dvals);
  free_if_non_null(qtypes);
  return status;
}
