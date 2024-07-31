#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "rbc_checkers.h"
#include "rbc_helpers.h"
#include "rbc_xhelpers.h"
#include "rbc_tm.h"
#include "rbc_pr.h"
#include "rbc_f_to_s.h"
#include "rbc_csv_to_df.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  const char * const infile = argv[1];
  char * const   cols = argv[2];
  char * const qtypes = argv[3];
  char * const tmcol  = argv[4];
  RBC_REC_TYPE rbc; memset(&rbc, 0, sizeof(RBC_REC_TYPE));
  RBC_REC_TYPE rbc_tm; memset(&rbc_tm, 0, sizeof(RBC_REC_TYPE));
  RBC_REC_TYPE rbc_fld; memset(&rbc_fld, 0, sizeof(RBC_REC_TYPE));
  qtype_t qtype; jtype_t jtype ;
  if ( argc != 5 ) { go_BYE(-1); } 

  bool is_hdr = true;
  BUF_SPEC_TYPE buf_spec;
  RBC_SCLR_TYPE sclr; 
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));

  status = rbc_csv_to_df(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &rbc); 
  cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  status = get_key_val(&rbc, -1, tmcol, &rbc_tm, NULL); cBYE(status);
  status = chk_rbc(&rbc_tm); cBYE(status);
  jtype = x_get_jtype(&rbc_tm);
  if ( jtype != j_array ) { go_BYE(-1); } 
  //----------------------
  status = tm_extract(&rbc_tm, "tm_mday", &rbc_fld); cBYE(status);
  status = chk_rbc(&rbc_fld); cBYE(status);
  qtype = x_get_qtype(&rbc_fld);
  if ( qtype != I1 ) { go_BYE(-1); } 
  jtype = x_get_jtype(&rbc_fld);
  if ( jtype != j_array ) { go_BYE(-1); } 
  free_rbc(&rbc_fld);
  //----------------------
  status = tm_extract(&rbc_tm, "tm_mon", &rbc_fld); cBYE(status);
  status = chk_rbc(&rbc_fld); cBYE(status);
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&rbc_fld, "min", NULL, &sclr); cBYE(status);
  if ( sclr.val.i1 != 0 ) { go_BYE(-1); } 
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&rbc_fld, "max", NULL, &sclr); cBYE(status);
  if ( sclr.val.i1 != 1 ) { go_BYE(-1); } 
  free_rbc(&rbc_fld);
  
  //----------------------
  status = tm_extract(&rbc_tm, "tm_yday", &rbc_fld); cBYE(status);
  status = chk_rbc(&rbc_tm); cBYE(status);
  qtype = x_get_qtype(&rbc_fld);
  if ( qtype != I2 ) { go_BYE(-1); } 
  jtype = x_get_jtype(&rbc_fld);
  if ( jtype != j_array ) { go_BYE(-1); } 
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&rbc_fld, "min", NULL, &sclr); cBYE(status);
  if ( sclr.val.i2 != 0 ) { go_BYE(-1); } 

  status = f_to_s(&rbc_fld, "max", NULL, &sclr); cBYE(status);
  if ( sclr.val.i2 != 34 ) { go_BYE(-1); } 
  free_rbc(&rbc_fld);
  //----------------------
  RBC_REC_TYPE rbc_time_band;
  memset(&rbc_time_band, 0, sizeof(RBC_REC_TYPE));
  status = x_time_band(&rbc_tm, 10, &rbc_time_band);
  status = chk_rbc(&rbc_time_band); cBYE(status);
  status = x_pr_array(&rbc_time_band, "../test/_time_band.csv"); cBYE(status);
  free_rbc(&rbc_time_band);

  fprintf(stdout, "Test %s completed successfully\n", argv[0]);

BYE:
  free_rbc(&rbc);
  free_rbc(&rbc_tm);
  free_rbc(&rbc_fld);
  return status;
}
