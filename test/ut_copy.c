#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "rbc_checkers.h"
#include "rbc_helpers.h"
#include "rbc_xhelpers.h"
#include "rbc_copy.h"
#include "rbc_pr.h"
#include "rbc_csv_to_df.h"
#include "rbc_f_to_s.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  RBC_REC_TYPE rbc, rbc_x, rbc_y;

  memset(&rbc,   0, sizeof(RBC_REC_TYPE));
  memset(&rbc_x, 0, sizeof(RBC_REC_TYPE));
  memset(&rbc_y, 0, sizeof(RBC_REC_TYPE));

  RBC_SCLR_TYPE sclr_sum_x, sclr_sum_y;
  RBC_SCLR_TYPE sclr_min_x, sclr_max_x, sclr_min_y, sclr_max_y;

  memset(&sclr_sum_x, 0, sizeof(sclr_sum_x));
  memset(&sclr_sum_y, 0, sizeof(sclr_sum_y));
  memset(&sclr_min_x, 0, sizeof(sclr_min_x));
  memset(&sclr_min_y, 0, sizeof(sclr_min_y));
  memset(&sclr_max_x, 0, sizeof(sclr_max_x));
  memset(&sclr_max_y, 0, sizeof(sclr_max_y));

  if ( argc != 6 ) { go_BYE(-1); } 

  const char * const infile = argv[1];
  char * const   cols = argv[2];
  char * const qtypes = argv[3];
  char * const infld  = argv[4];
  char * const outfld = argv[5];
  jtype_t jtype ;

  bool is_hdr = false;
  BUF_SPEC_TYPE buf_spec; 
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));

  status = rbc_csv_to_df(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &rbc); 
  cBYE(status);
  status = get_key_val(&rbc, -1, infld, &rbc_x, NULL); cBYE(status);
  status = chk_rbc(&rbc_x); cBYE(status);
  jtype = x_get_jtype(&rbc_x);
  if ( jtype != j_array ) { go_BYE(-1); } 
  qtype_t xqtype = x_get_qtype(&rbc_x);
  if ( xqtype != F8 ) { go_BYE(-1); }  // IMPORTANT
  status = f_to_s(&rbc_x, "sum", NULL, &sclr_sum_x); cBYE(status);
  status = f_to_s(&rbc_x, "min", NULL, &sclr_min_x); cBYE(status);
  status = f_to_s(&rbc_x, "max", NULL, &sclr_max_x); cBYE(status);
  //----------------------
  status = get_key_val(&rbc, -1, outfld, &rbc_y, NULL); cBYE(status);
  status = chk_rbc(&rbc_y); cBYE(status);

  status = rbc_copy_array(&rbc_y, &rbc_x); cBYE(status);
  status = chk_rbc(&rbc_y); cBYE(status);
  jtype = x_get_jtype(&rbc_y);
  if ( jtype != j_array ) { go_BYE(-1); } 
  qtype_t yqtype = x_get_qtype(&rbc_y);
  if ( yqtype != I4 ) { go_BYE(-1); }  // IMPORTANT

  status = f_to_s(&rbc_y, "sum", NULL, &sclr_sum_y); cBYE(status);
  status = f_to_s(&rbc_y, "min", NULL, &sclr_min_y); cBYE(status);
  status = f_to_s(&rbc_y, "max", NULL, &sclr_max_y); cBYE(status);
  //----------------------
  if ( sclr_sum_x.val.f8 != sclr_sum_y.val.i4 ) { go_BYE(-1); }
  if ( sclr_min_x.val.f8 != sclr_min_y.val.i4 ) { go_BYE(-1); }
  if ( sclr_max_x.val.f8 != sclr_max_y.val.i4 ) { go_BYE(-1); }
  //----------------------
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);

BYE:
  free_rbc(&rbc);
  return status;
}
