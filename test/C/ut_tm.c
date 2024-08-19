#include "incs.h"
#include "qdf_struct.h"
#include "qdf_checkers.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_tm.h"
#include "qdf_pr.h"
#include "qdf_f_to_s.h"
#include "qdf_csv_to_df.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  QDF_REC_TYPE qdf; memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE qdf_tm; memset(&qdf_tm, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE qdf_fld; memset(&qdf_fld, 0, sizeof(QDF_REC_TYPE));
  qtype_t qtype; jtype_t jtype ;
  if ( argc != 6 ) { go_BYE(-1); } 
  const char * const infile = argv[1];
  char * const   cols = argv[2];
  char * const qtypes = argv[3];
  char * const tmcol  = argv[4];
  char * const opfile = argv[5];

  bool is_hdr = true;
  BUF_SPEC_TYPE buf_spec;
  SCLR_REC_TYPE sclr; 
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));

  status = qdf_csv_to_df(infile, NULL, 0, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf); 
  cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  status = get_key_val(&qdf, -1, tmcol, &qdf_tm, NULL); cBYE(status);
  status = chk_qdf(&qdf_tm); cBYE(status);
  jtype = x_get_jtype(&qdf_tm);
  if ( jtype != j_array ) { go_BYE(-1); } 
  //----------------------
  status = tm_extract(&qdf_tm, "tm_mday", &qdf_fld); cBYE(status);
  status = chk_qdf(&qdf_fld); cBYE(status);
  qtype = x_get_qtype(&qdf_fld);
  if ( qtype != I1 ) { go_BYE(-1); } 
  jtype = x_get_jtype(&qdf_fld);
  if ( jtype != j_array ) { go_BYE(-1); } 
  free_qdf(&qdf_fld);
  //----------------------
  status = tm_extract(&qdf_tm, "tm_mon", &qdf_fld); cBYE(status);
  status = chk_qdf(&qdf_fld); cBYE(status);
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_fld, "min", NULL, &sclr); cBYE(status);
  if ( sclr.val.i1 != 0 ) { go_BYE(-1); } 
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_fld, "max", NULL, &sclr); cBYE(status);
  if ( sclr.val.i1 != 1 ) { go_BYE(-1); } 
  free_qdf(&qdf_fld);
  
  //----------------------
  status = tm_extract(&qdf_tm, "tm_yday", &qdf_fld); cBYE(status);
  status = chk_qdf(&qdf_tm); cBYE(status);
  qtype = x_get_qtype(&qdf_fld);
  if ( qtype != I2 ) { go_BYE(-1); } 
  jtype = x_get_jtype(&qdf_fld);
  if ( jtype != j_array ) { go_BYE(-1); } 
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_fld, "min", NULL, &sclr); cBYE(status);
  if ( sclr.val.i2 != 0 ) { go_BYE(-1); } 

  status = f_to_s(&qdf_fld, "max", NULL, &sclr); cBYE(status);
  if ( sclr.val.i2 != 34 ) { go_BYE(-1); } 
  free_qdf(&qdf_fld);
  //----------------------
  QDF_REC_TYPE qdf_time_band;
  memset(&qdf_time_band, 0, sizeof(QDF_REC_TYPE));
  status = x_time_band(&qdf_tm, 10, &qdf_time_band);
  status = chk_qdf(&qdf_time_band); cBYE(status);
  status = x_pr_array(&qdf_time_band, opfile); cBYE(status);
  free_qdf(&qdf_time_band);

  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);

BYE:
  free_qdf(&qdf);
  free_qdf(&qdf_tm);
  free_qdf(&qdf_fld);
  return status;
}
