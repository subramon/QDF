#include "incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "prep_args_for_read_csv.h"
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
  SCLR_REC_TYPE sclr;  memset(&sclr, 0, sizeof(SCLR_REC_TYPE));
  QDF_REC_TYPE qdf; memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE qdf_tm; memset(&qdf_tm, 0, sizeof(QDF_REC_TYPE));
  QDF_REC_TYPE qdf_fld; memset(&qdf_fld, 0, sizeof(QDF_REC_TYPE));
  qtype_t qtype; jtype_t jtype ;
  if ( argc != 6 ) { go_BYE(-1); } 
  const char * const infile = argv[1];
  char * const   in_cols = argv[2];
  char * const in_str_qtypes = argv[3];
  char * const tmcol  = argv[4];
  char * const opfile = argv[5];

  char **cols = NULL; uint32_t ncols = 0; // [ncols]
  qtype_t *qtypes = NULL;  // [ncols]
  uint32_t *widths = NULL;  // [ncols]
  char **formats = NULL;  // [ncols]
  bool *has_nulls = NULL; // [ncols]
  bool *is_load = NULL; // [ncols]
  bool is_hdr = true;

  status = prep_args_for_read_csv(in_cols, in_str_qtypes,
    &qtypes, &widths, &formats, &has_nulls, &is_load , &cols, &ncols);
  status = qdf_csv_to_df(infile, NULL, 0, cols, qtypes,
      widths, formats, has_nulls, is_load, ncols, 
      ",", "\"", "\n", is_hdr, &qdf); 
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
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_2d_array(&cols, ncols);
  // XX free_2d_array(&str_qtypes, ncols);
  if ( formats != NULL ) { 
    free_2d_array(&formats, ncols);
  }
  free_if_non_null(has_nulls);
  free_if_non_null(is_load);
  return status;
}
