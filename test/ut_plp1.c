#include "incs.h"
#include <pthread.h>
#include "configs.h"
#include "xconfigs.h"
#include "hive_struct.h"
#include "rbc_struct.h"
#include "row_prod_struct.h"
#include "data_set_prod_struct.h"

#include "aux_data_set_producer.h"
#include "holiday.h"
#include "rbc_makers.h"
#include "rbc_pr.h"
#include "num_lines.h"
#include "rbc_checkers.h"
#include "rbc_helpers.h"
#include "bridge_read_configs.h"
#include "free_configs.h"
#include "read_xconfigs.h"
#include "free_xconfigs.h"
#include "mk_lua_state.h"
#include "bridge_plp1.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  xconfig_t XC;
  hive_struct_t H;
  data_set_prod_output_t D;
  RBC_REC_TYPE in_rbc;
  lua_State *L = NULL;

  //------------------------
  memset(&C, 0, sizeof(config_t));
  memset(&XC, 0, sizeof(xconfig_t));
  memset(&H, 0, sizeof(hive_struct_t));
  memset(&in_rbc, 0, sizeof(RBC_REC_TYPE));
  memset(&D, 0, sizeof(data_set_prod_output_t));
  BUF_SPEC_TYPE buf_spec;
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  //------------------------

  if ( argc != 6 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  const char * const infile      = argv[2];
  char * const   cols            = argv[3];
  char * const qtypes            = argv[4];
  const char * const outfile     = argv[5];
  status = bridge_read_configs(config_file, &C); cBYE(status);
  status = read_xconfigs(&C, &XC); cBYE(status);
  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  // Load the configs into G
  char cmd[1024];
  sprintf(cmd, "G = require '%s'", C.cfg_dir); 
  status = luaL_dostring(L, cmd);
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  // create some sample data in in_rbc
  bool is_hdr = true;
  status = csv_to_data_frame(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &in_rbc); 
  cBYE(status);
  status = chk_rbc(&in_rbc); cBYE(status);

  int n_rows = num_lines(infile);
  if ( is_hdr ) { n_rows--; } 
  int chk_n_rows, sz;
  bool b_is_df = is_data_frame(&in_rbc, &chk_n_rows, &sz);
  if ( !b_is_df ) { go_BYE(-1); }
  // create space for output
  status = make_empty_data_frame(C.Rmeta.col_names, C.Rmeta.n_cols, 
      C.Rmeta.qtypes, n_rows, &(D.rbc)); cBYE(status);
  status = bridge_plp1(L, &XC, &in_rbc, &(D.rbc));
  cBYE(status);
  // do HL post processing
  for ( int i = 0; i < C.Rmeta.n_cols; i++ ) { 
    if ( strcmp(C.Rmeta.qtypes[i], "HL") != 0 ) { continue; }
    RBC_REC_TYPE col; memset(&col, 0, sizeof(RBC_REC_TYPE));
    status = get_key_val(&D.rbc, i, NULL, &col, NULL); cBYE(status);
#ifdef DEBUG
    jtype_t jtype = x_get_jtype(&col);
    if ( jtype != j_array ) { go_BYE(-1); }
    qtype_t qtype = x_get_qtype(&col);
    if ( qtype != HL ) { go_BYE(-1); }
#endif
    holiday_bmask_t *hptr = get_arr_ptr(col.data); 
    status = post_process_holiday_bmask(hptr, n_rows); 
    cBYE(status);
  }

  status = chk_rbc(&(D.rbc));  cBYE(status);
  status = pr_df_as_csv(&(D.rbc), NULL, 0, outfile); cBYE(status); 
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_configs(&C);
  free_xconfigs(&XC);
  free_rbc(&in_rbc);
  free_D(&D);
  if ( L != NULL ) { lua_close(L); L = NULL; }
  return status;
}
