#include "incs.h"
#include "free_2d_array.h"
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
  char **cols = NULL; uint32_t n_cols;
  QDF_REC_TYPE *qdfs = NULL; memset(&out_qdf, 0, sizeof(QDF_REC_TYPE));
  BUF_SPEC_TYPE buf_spec; memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  if ( argc != 5 ) { go_BYE(-1); } 
  const char * const infile = argv[1];
  char * const   concat_cols = argv[2];
  char * const concat_qtypes = argv[3];
  int itmp = atoi(argv[4]); if ( itmp < 0 ) { go_BYE(-1); } 
  uint32_t n_qdfs = (uint32_t)itmp; 


  bool is_hdr = true;
  status = split_str(concat_cols, ",", &cols, &n_cols); cBYE(status);
  qdfs = malloc(n_qdfs * sizeof(QDF_REC_TYPE));
  memset(qdfs, 0,  n_qdfs * sizeof(QDF_REC_TYPE));
  for ( uint32_t i = 0; i < n_qdfs; i++ ) { 
    status = qdf_csv_to_df(infile, NULL, 0, concat_cols, concat_qtypes,
        ",", "\"", "\n", is_hdr, &buf_spec, &(qdfs[i])); 
    cBYE(status);
    status = chk_qdf(&(qdfs[i])); cBYE(status);
    if ( !x_get_is_df(&(qdfs[i])) ){ go_BYE(-1); } 
  }
  status = qdf_concat(qdfs, n_qdfs, cols, n_cols, &out_qdf); 
  cBYE(status);

  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);

BYE:
  free_2d_array(&cols, n_cols);
  free_qdf(&out_qdf);
  free_qdfs(&qdfs, "free", n_qdfs);
  return status;
}
