#include "incs.h"
#include "qdf_struct.h"
#include "qdf_checkers.h"
#include "num_lines.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_csv_to_df.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  if ( argc != 4 ) { go_BYE(-1); } 
  const char * const infile = argv[1];
  char * const   cols = argv[2];
  char * const qtypes = argv[3];
  QDF_REC_TYPE qdf;
  memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  BUF_SPEC_TYPE buf_spec;
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));

  int itmp = num_lines(infile);  if ( itmp <= 0 ) { go_BYE(-1); }
  uint32_t n_rows = (uint32_t)itmp;
  bool is_hdr = true;

  status = qdf_csv_to_df(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf); 
  status = chk_qdf(&qdf);
  if ( is_hdr ) { n_rows--; }
  if ( !x_get_is_df(&qdf) ) { go_BYE(-1); } 
  uint32_t chk_n_rows = x_get_obj_arr_len(&qdf); 
  if ( chk_n_rows != n_rows ) { go_BYE(-1); }
  status = chk_qdf(&qdf); cBYE(status);

  fprintf(stdout, "SUCCESS; Test %s completed successfully\n", argv[0]);
BYE:
  free_qdf(&qdf);
  return status;
}
