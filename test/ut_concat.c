#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "split_str.h"
#include "rbc_checkers.h"
#include "rbc_concat.h"
#include "rbc_helpers.h"
#include "rbc_xhelpers.h"
#include "rbc_external.h"
#include "rbc_csv_to_df.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  RBC_REC_TYPE out_rbc; memset(&out_rbc, 0, sizeof(RBC_REC_TYPE));
  char **cols = NULL; uint32_t n_cols;
  RBC_REC_TYPE *rbcs = NULL; memset(&out_rbc, 0, sizeof(RBC_REC_TYPE));
  BUF_SPEC_TYPE buf_spec; memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  if ( argc != 5 ) { go_BYE(-1); } 
  const char * const infile = argv[1];
  char * const   concat_cols = argv[2];
  char * const concat_qtypes = argv[3];
  int itmp = atoi(argv[4]); if ( itmp < 0 ) { go_BYE(-1); } 
  uint32_t n_rbcs = (uint32_t)itmp; 


  bool is_hdr = true;
  status = split_str(concat_cols, ",", &cols, &n_cols); cBYE(status);
  rbcs = malloc(n_rbcs * sizeof(RBC_REC_TYPE));
  memset(rbcs, 0,  n_rbcs * sizeof(RBC_REC_TYPE));
  for ( uint32_t i = 0; i < n_rbcs; i++ ) { 
    status = rbc_csv_to_df(infile, concat_cols, concat_qtypes,
        ",", "\"", "\n", is_hdr, &buf_spec, &(rbcs[i])); 
    cBYE(status);
    status = chk_rbc(&(rbcs[i])); cBYE(status);
    if ( !x_get_is_df(&(rbcs[i])) ){ go_BYE(-1); } 
  }
  status = rbc_concat(rbcs, n_rbcs, (const char ** const)cols, n_cols, 
      &out_rbc); 
  cBYE(status);

  fprintf(stdout, "Test %s completed successfully\n", argv[0]);

BYE:
  free_2d_array(&cols, n_cols);
  free_rbc(&out_rbc);
  free_rbcs(&rbcs, "free", n_rbcs);
  return status;
}
