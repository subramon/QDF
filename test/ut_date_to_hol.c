#include "incs.h"
#include "macros.h"
#include "num_lines.h"
#include "rbc_struct.h"
#include "rbc_makers.h"
#include "rbc_t_epoch_to_hol.h"
#include "rbc_helpers.h"
#include "rbc_xhelpers.h"
#include "rbc_csv_to_df.h"
#include "holiday.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  date_to_hol_t H; memset(&H, 0, sizeof(date_to_hol_t));
  if ( argc != 3 ) { go_BYE(-1); }
  const char * const infile = argv[1];
  const char * const outfile = argv[2];
  const char * const format = "%Y-%m-%d";
  BUF_SPEC_TYPE buf_spec; memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  RBC_REC_TYPE rbc; memset(&rbc, 0, sizeof(RBC_REC_TYPE));
  RBC_REC_TYPE bmask; memset(&bmask, 0, sizeof(RBC_REC_TYPE));
  RBC_REC_TYPE chk_bmask; memset(&chk_bmask, 0, sizeof(RBC_REC_TYPE));
  RBC_REC_TYPE t_epoch; memset(&t_epoch, 0, sizeof(RBC_REC_TYPE));


  uint32_t nl = num_lines(infile); 
  status = read_date_to_hol(infile, format, &H); cBYE(status);
  status = prnt_date_to_hol(&H, format, outfile); cBYE(status);
  //--------------
  const char * const cols = "bmask,t_epoch,week_start_date";
  const char * const qtypes = "HL,I4,TM:%Y-%m-%d";
  bool is_hdr = false;

  status = rbc_csv_to_df(outfile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &rbc); 
  // extract bmask from rbc 
  status = get_key_val(&rbc, -1, "bmask", &bmask, NULL); cBYE(status); status = get_key_val(&rbc, -1, "t_epoch", &t_epoch, NULL); cBYE(status);
  uint32_t n = x_get_arr_len(&bmask); if ( n != nl ) { go_BYE(-1); }
  // make check bmask
  status = make_num_array(NULL, n, 0, HL, &chk_bmask); cBYE(status);
  status = t_epoch_to_hol(&t_epoch, &H, &chk_bmask); cBYE(status);
  holiday_bmask_t *h1ptr = get_arr_ptr(bmask.data); 
  holiday_bmask_t *h2ptr = get_arr_ptr(chk_bmask.data); 
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( h1ptr[i] != h2ptr[i] ) { go_BYE(-1); }
  }
BYE:
  free_rbc(&rbc);
  free_rbc(&chk_bmask);
  status = free_date_to_hol(&H); 
  return status;
}
