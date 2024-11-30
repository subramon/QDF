#include "incs.h"
#include "aux_rcli.h"
#include "free_2d_array.h"
#include "qdf_struct.h"
#include "split_str.h"
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
  qtype_t *c_qtypes = NULL;
  char **str_qtypes = NULL; 
  int sock = -1;
  char **cols = NULL; uint32_t n_cols = 0; uint32_t uitmp;
  QDF_REC_TYPE qdf; memset(&qdf, 0, sizeof(QDF_REC_TYPE));

  if ( argc != 6 ) { go_BYE(-1); } 
  const char * const server  = argv[1];
  int portnum                = atoi(argv[2]);
  const char * const infile  = argv[3];
  char * const   concat_cols = argv[4];
  char * const concat_qtypes = argv[5];

  status = rconnect(server, portnum, 0, 0, &sock); cBYE(status);
  if ( sock <= 0 ) {  go_BYE(-1); }
  printf("Established connection\n");

  bool is_hdr = true;
  status = split_str(concat_cols, ",", &cols, &n_cols); cBYE(status);
  status = split_str(concat_qtypes, ",", &str_qtypes, &uitmp); cBYE(status);
  if ( uitmp != n_cols ) { go_BYE(-1); }

  c_qtypes = malloc(n_cols *sizeof(qtype_t));
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    c_qtypes[i] = get_c_qtype(str_qtypes[i]);
  }
  status = qdf_csv_to_df(infile, NULL, 0, concat_cols, concat_qtypes,
        ",", "\"", "\n", is_hdr, NULL, &qdf);
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
  free_if_non_null(c_qtypes);
  free_2d_array(&str_qtypes, n_cols);
  free_2d_array(&cols, n_cols);
  if ( sock > 0 ) { close(sock); }
  free_qdf(&qdf);
  return status;
}
