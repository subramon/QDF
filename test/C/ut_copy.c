#include "incs.h"
#include "qdf_struct.h"
#include "qdf_checkers.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_copy.h"
#include "qdf_pr.h"
#include "qdf_csv_to_df.h"
#include "qdf_f_to_s.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  QDF_REC_TYPE qdf, qdf_x, qdf_y;

  memset(&qdf,   0, sizeof(QDF_REC_TYPE));
  memset(&qdf_x, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_y, 0, sizeof(QDF_REC_TYPE));

  SCLR_REC_TYPE sclr_sum_x, sclr_sum_y;
  SCLR_REC_TYPE sclr_min_x, sclr_max_x, sclr_min_y, sclr_max_y;

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

  status = qdf_csv_to_df(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf); 
  cBYE(status);
  status = get_key_val(&qdf, -1, infld, &qdf_x, NULL); cBYE(status);
  status = chk_qdf(&qdf_x); cBYE(status);
  jtype = x_get_jtype(&qdf_x);
  if ( jtype != j_array ) { go_BYE(-1); } 
  qtype_t xqtype = x_get_qtype(&qdf_x);
  if ( xqtype != F8 ) { go_BYE(-1); }  // IMPORTANT
  status = f_to_s(&qdf_x, "sum", NULL, &sclr_sum_x); cBYE(status);
  status = f_to_s(&qdf_x, "min", NULL, &sclr_min_x); cBYE(status);
  status = f_to_s(&qdf_x, "max", NULL, &sclr_max_x); cBYE(status);
  //----------------------
  status = get_key_val(&qdf, -1, outfld, &qdf_y, NULL); cBYE(status);
  status = chk_qdf(&qdf_y); cBYE(status);

  status = qdf_copy_array(&qdf_y, &qdf_x); cBYE(status);
  status = chk_qdf(&qdf_y); cBYE(status);
  jtype = x_get_jtype(&qdf_y);
  if ( jtype != j_array ) { go_BYE(-1); } 
  qtype_t yqtype = x_get_qtype(&qdf_y);
  if ( yqtype != I4 ) { go_BYE(-1); }  // IMPORTANT

  status = f_to_s(&qdf_y, "sum", NULL, &sclr_sum_y); cBYE(status);
  status = f_to_s(&qdf_y, "min", NULL, &sclr_min_y); cBYE(status);
  status = f_to_s(&qdf_y, "max", NULL, &sclr_max_y); cBYE(status);
  //----------------------
  if ( sclr_sum_x.val.f8 != sclr_sum_y.val.i4 ) { go_BYE(-1); }
  if ( sclr_min_x.val.f8 != sclr_min_y.val.i4 ) { go_BYE(-1); }
  if ( sclr_max_x.val.f8 != sclr_max_y.val.i4 ) { go_BYE(-1); }
  //----------------------
  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);

BYE:
  free_qdf(&qdf);
  return status;
}