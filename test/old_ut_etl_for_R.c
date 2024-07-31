#include "incs.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "macros.h"
#include "rs_mmap.h"
#include "cmem_struct.h"
#include "rbc_struct.h"
#include "configs.h"
#include "mt_struct.h"
#include "hive_struct.h"
#include "rbc.h"
#include "auxil.h"
#include "read_configs.h"
#include "free_configs.h"
#include "init_hive.h"
#include "free_hive.h"
#include "read_hive.h"
#include "bridge_etl_for_R.h"
#include "q_to_RBC.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  hive_run_t H;
  RBC_REC_TYPE out_rbc;
  RBC_REC_TYPE in_rbc;
  RBC_REC_TYPE pr_rbc;
  char *X = NULL; size_t nX = 0;

  memset(&out_rbc, 0, sizeof(RBC_REC_TYPE));
  memset(&in_rbc, 0, sizeof(RBC_REC_TYPE));
  memset(&pr_rbc, 0, sizeof(RBC_REC_TYPE));
  pr_rbc.size = 131072;
  pr_rbc.data = malloc(pr_rbc.size);
  memset(pr_rbc.data, 0,  pr_rbc.size);
  if ( argc != 2 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  memset(&C, 0, sizeof(config_t));
  memset(&H, 0, sizeof(hive_run_t));
  status = read_configs(config_file, &C); cBYE(status);
  status = init_hive(&C, &H); cBYE(status);
  bool is_eof = false;
  status = read_hive(C.file_names[0], &H.offset, 
      C.break_cols, C.n_break_cols, H.break_vals, 
      C.grp_cols, C.n_grp_cols, H.grp_vals, 
      C.qtypes, C.is_load, 
        H.num_nulls, &(H.vals), &(H.nn), H.out_size, H.out_len, 
        H.max_n_rows,&(H.n_rows), C.n_cols, C.fld_sep, C.rec_sep, &is_eof);

  status = q_to_RBC(C.col_names, C.widths, C.qtypes,
      C.is_load, H.vals, NULL, H.n_rows, C.n_cols, &in_rbc); 
  cBYE(status);
  status = check(&in_rbc); cBYE(status);
  int buflen = 0;
  status = pr(&in_rbc, &pr_rbc, &buflen); cBYE(status);
  // printf("%s\n", (char *)pr_rbc.data);
  

  status = q_to_RBC(C.for_R_col_names, C.for_R_widths, C.for_R_qtypes,
      NULL, NULL, NULL, H.n_rows, C.n_for_R_cols, &out_rbc); 
  cBYE(status);
  status = check(&out_rbc); cBYE(status);
  memset(pr_rbc.data, 0,  pr_rbc.size);
  buflen = 0;
  status = pr(&out_rbc, &pr_rbc, &buflen); cBYE(status);
  // printf("%s\n", (char *)pr_rbc.data);
 
  read_data_t D;
  memset(&D, 0, sizeof(read_data_t));
  status = bridge_etl_for_R(H.L, &in_rbc, C.cfg_dir, &D);
  cBYE(status);
  memset(pr_rbc.data, 0,  pr_rbc.size);
  buflen = 0;
  status = pr(&out_rbc, &pr_rbc, &buflen); cBYE(status);
  free_if_non_null(out_rbc.data);
  if ( buflen < 1024 ) { 
    printf("%s\n", (char *)pr_rbc.data);
  }
  else {
    printf("Nor printing output RBC. Too large\n");
  }


  printf("Successfully completed %s %s \n", argv[0], argv[1]);
BYE:
  free_if_non_null(D.rbc.data);
  free_2d_array(&(D.str_frmlas), D.n_frmlas);
  free_if_non_null(in_rbc.data);
  free_if_non_null(out_rbc.data);
  status = free_hive(&H, &C); cBYE(status);
  free_configs(&C);
  free_if_non_null(pr_rbc.data);
  if ( X != NULL ) { munmap(X, nX); }
  return status;
}
