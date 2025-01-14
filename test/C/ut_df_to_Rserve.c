#include "incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "prep_args_for_read_csv.h"
#include "aux_rcli.h"
#include "qdf_struct.h"
#include "qdf_checkers.h"
#include "qdf_df_to_Rserve.h"
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
  int sock = -1;
  QDF_REC_TYPE qdf; memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  char **cols = NULL; uint32_t ncols = 0;
  qtype_t *qtypes = NULL;  // [ncols]
  uint32_t *widths = NULL;  // [ncols]
  char **formats = NULL;  // [ncols]
  bool *has_nulls = NULL; // [ncols]
  bool *is_load = NULL; // [ncols]

  if ( argc != 6 ) { go_BYE(-1); } 
  const char * const server  = argv[1];
  int portnum                = atoi(argv[2]);
  const char * const infile  = argv[3];
  char * const   in_cols = argv[4];
  char * const in_str_qtypes = argv[5];

  status = rconnect(server, portnum, 0, 0, &sock); cBYE(status);
  if ( sock <= 0 ) {  go_BYE(-1); }
  printf("Established connection\n");

  bool is_hdr = true;
  status = prep_args_for_read_csv(in_cols, in_str_qtypes,
    &qtypes, &widths, &formats, &has_nulls, &is_load , &cols, &ncols);
  status = qdf_csv_to_df(infile, NULL, 0, cols, qtypes,
      widths, formats, has_nulls, is_load, ncols, 
      ",", "\"", "\n", is_hdr, &qdf); 
  cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  if ( !x_get_is_df(&qdf) ) { go_BYE(-1); } 

  status = qdf_df_to_Rserve(sock, &qdf, "df_name");
  cBYE(status);

  char Rcmd[1024];
  sprintf(Rcmd, "saveRDS(df_name, file  = \"/tmp/cfg.Rdata\")");
  status = exec_str(sock, Rcmd, NULL, NULL, -1); cBYE(status);
  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);

BYE:
  if ( sock > 0 ) { close(sock); }
  free_qdf(&qdf);
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_2d_array(&cols, ncols);
  free_2d_array(&formats, ncols);
  free_if_non_null(has_nulls);
  free_if_non_null(is_load);
  return status;
}
