#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "rbc_checkers.h"
#include "num_lines.h"
#include "rbc_helpers.h"
#include "rbc_xhelpers.h"
#include "rbc_csv_to_df.h"

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
  RBC_REC_TYPE rbc;
  memset(&rbc, 0, sizeof(RBC_REC_TYPE));
  BUF_SPEC_TYPE buf_spec;
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));

  uint32_t n_rows = num_lines(infile); 
  bool is_hdr = true;

  status = rbc_csv_to_df(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &rbc); 
  status = chk_rbc(&rbc);
  if ( is_hdr ) { n_rows--; }
  if ( !x_get_is_df(&rbc) ) { go_BYE(-1); } 
  uint32_t chk_n_rows = x_get_obj_arr_len(&rbc); 
  if ( chk_n_rows != n_rows ) { go_BYE(-1); }
  status = chk_rbc(&rbc); cBYE(status);

  fprintf(stdout, "Test %s completed successfully\n", argv[0]);

BYE:
  free_rbc(&rbc);
  return status;
}
