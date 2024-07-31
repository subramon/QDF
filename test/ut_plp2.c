#include "incs.h"
#include <pthread.h>
#include "configs.h"
#include "hive_struct.h"
#include "rbc_struct.h"
#include "row_prod_struct.h"
#include "data_set_prod_struct.h"

#include "auxil.h"
#include "rbc_pr.h"
#include "rbc_checkers.h"
#include "rbc_helpers.h"
#include "bridge_read_configs.h"
#include "free_configs.h"
#include "read_xconfigs.h"
#include "free_xconfigs.h"
#include "mk_lua_state.h"
#include "aux_mgcv_worker.h"
#include "bridge_plp2.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  xconfig_t XC;
  RBC_REC_TYPE in_rbc;
  lua_State *L = NULL;
  mgcv_worker_output_t *M = NULL; // [nF] 
  int nF = 0;
  //------------------------
  memset(&C, 0, sizeof(config_t));
  memset(&XC, 0, sizeof(xconfig_t));
  memset(&in_rbc, 0, sizeof(RBC_REC_TYPE));
  BUF_SPEC_TYPE buf_spec;
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  //------------------------

  if ( argc != 6 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  const char * const infile      = argv[2];
  char * const   cols            = argv[3];
  char * const qtypes            = argv[4];
  const char * const out_prefix  = argv[5];
  status = bridge_read_configs(config_file, &C); cBYE(status);
  status = read_xconfigs(&C, &XC); cBYE(status);
  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  status = load_configs(L, C.cfg_dir); cBYE(status);
  // g_T_n_uq is shared across formulas. We reset it here
  status = luaL_dostring(L, "g_T_n_uq = {} "); cBYE(status);
  // create some sample data in in_rbc
  bool is_hdr = true;
  status = csv_to_data_frame(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &in_rbc); 
  cBYE(status);
  status = chk_rbc(&in_rbc); cBYE(status);

  int n_rows, sz;
  bool b_is_df = is_data_frame(&in_rbc, &n_rows, &sz);
  if ( !b_is_df ) { go_BYE(-1); }

  nF = C.n_frmlas; 
  int nC = C.n_components;
  int nB = C.ddl_in.n_break_cols;
  M = malloc(nF * sizeof(mgcv_worker_output_t));
  memset(M, 0, nF * sizeof(mgcv_worker_output_t));
  status = mgcv_wrk_out_init(M, nF, nB, nC); cBYE(status); 
  //------------------------
  for ( int f = 0; f < nF; f++ ) { 
    char outfile[1024];
    char *fkey = C.frmla_keys[f];
    sprintf(outfile, "%s_%s.csv", out_prefix, fkey); 
    RBC_REC_TYPE lrbc; memset(&lrbc, 0, sizeof(RBC_REC_TYPE));
    status = copy_rbc(&lrbc, &in_rbc); cBYE(status);
    status = chk_rbc(&lrbc); cBYE(status);
    bool bval;  int ival;
    bval = is_data_frame(&lrbc, &ival, &sz);
    if ( !bval ) { go_BYE(-1); } 
    status = bridge_plp2(L, &XC, &lrbc, fkey, &(M[f])); cBYE(status);
    printf("%s:%s\n", fkey, M[f].str_frmla);
    if ( M[f].n_rows > n_rows ) { go_BYE(-1); }
    status = chk_rbc(&lrbc); cBYE(status);
    status = pr_df_as_csv(&lrbc, NULL, 0, outfile); cBYE(status); 
    free_rbc(&lrbc); 
  }
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  status = mgcv_wrk_out_mfree(M, nF);
  if ( status < 0 ) { WHEREAMI; }
  free_if_non_null(M);
  free_configs(&C);
  free_configs(&XC);
  free_rbc(&in_rbc);
  if ( L != NULL ) { lua_close(L); L = NULL; }
  return status;
}
