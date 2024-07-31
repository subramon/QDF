#include "incs.h"
#include "rbc.h"
#include "auxil.h"
#include "split_str.h"
#include "rs_mmap.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char ** infiles = NULL; int n_in = 0; 
  char **cols = NULL; int n_cols = 0; 
  RBC_REC_TYPE *in_rbcs = NULL; 
  RBC_REC_TYPE out_rbc;
  memset(&out_rbc, 0, sizeof(RBC_REC_TYPE));
  if ( argc != 4 ) { go_BYE(-1); }
  // arg[1] is colon separated list of files names 
  // arg[2] is colon separated list of column names
  // arg[3] is output RBC as a CSV fille 

  // Get number of input files and their number n_in 
  status = split_str(argv[1], ":", &infiles, &n_in);
  if ( n_in <= 1 ) { go_BYE(-1); }
  // Get number of columns in each file and their number n_cols
  status = split_str(argv[2], ",", &cols, &n_cols);
  if ( n_cols < 1 ) { go_BYE(-1); }
  int sum_length = 0;
  status = load_rbcs_from_files(infiles, n_in, &in_rbcs, &sum_length);
  cBYE(status);
  //--------------------------
  status = merge_df(in_rbcs, n_in, cols, n_cols, &out_rbc);
  cBYE(status);
  printf("sum_length = %d \n", sum_length );
  // check output
  status = check(&out_rbc); cBYE(status);
  // check length 
  int length = length_df(&out_rbc);
  if ( sum_length != length ) { go_BYE(-1); }
  // check all keys in output 
  for ( int i = 0; i < n_cols; i++ ) { 
    uint32_t idx;
    bool b_is_key = is_key(&out_rbc, cols[i], &idx);
    if ( !b_is_key ) { go_BYE(-1); }
  }
  // print output 
  status = pr_df_as_csv(&out_rbc, cols, n_cols, argv[3]); cBYE(status);

BYE:
  free_if_non_null(out_rbc.data);
  free_rbcs(&in_rbcs, "free", n_in);
  free_2d_array(&infiles, n_in);
  free_2d_array(&cols, n_cols);
  return status;
}
