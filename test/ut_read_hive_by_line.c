#include "incs.h"
#include "cmem_struct.h"
#include "rbc_struct.h"
#include "configs.h"
#include "hive_struct.h"

#include "rs_mmap.h"
#include "read_hive_by_line.h"
#include "prnt_hive.h"
#include "read_configs.h"
#include "free_configs.h"
#include "init_hive.h"
#include "free_hive.h"

#define BUFSZ 128
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  hive_run_t H;
  const char * out_suffix = NULL;
  char outfilename[64]; 
  char *buf = NULL;
  int64_t *l_break_vals = NULL;
  int64_t *l_grp_vals = NULL;
  char *X = NULL; size_t nX = 0; // for whole file 
  char *Y = NULL; size_t sz_Y = 1024, nY = 0; // for single line 

  memset(&H, 0, sizeof(hive_run_t));
  memset(&C, 0, sizeof(config_t));
  FILE *ofp = NULL;

  if ( argc != 3 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  out_suffix     = argv[2]; // write hive file as CSV file 

  status = read_configs(config_file, &C); cBYE(status);
  status = init_hive(&C, &H); cBYE(status);

  bool is_eov = false;
  int num_data_sets = 0;
  l_break_vals = malloc(C.n_break_cols * sizeof(int64_t));
  return_if_malloc_failed(l_break_vals);
  l_grp_vals = malloc(C.n_grp_cols * sizeof(int64_t));
  return_if_malloc_failed(l_grp_vals);
  buf = malloc(BUFSZ);
  return_if_malloc_failed(buf);
  for ( int f = 0; f < C.n_files; f++ ) { // iterate over files 
    status = rs_mmap(C.file_names[f], &X, &nX, 0); cBYE(status);
    Y = malloc(sz_Y);
    return_if_malloc_failed(Y);
    // gather data into a line 
    size_t xidx = 0;
    int lno = 0; // for debugging 
    for ( ; xidx < nX ; lno++ ) { // for each line in the file 
      size_t start_xidx = xidx, yidx;
      // read in a line 
      for ( yidx = 0; X[xidx] != C.rec_sep; ) { 
        if ( yidx >= sz_Y ) { 
          sz_Y *= 2;
          Y = realloc(Y, sz_Y);
        }
        Y[yidx++] = X[xidx++];
      }
      Y[yidx++] = X[xidx++]; // put end-of-rec into Y buffer
      nY = yidx;
      status = read_hive_by_line( C.qtypes, C.is_load, 
          C.break_cols, C.n_break_cols, 
          C.grp_cols, C.n_grp_cols, 
          C.n_cols, C.fld_sep, C.rec_sep, 
          Y, nY, buf, BUFSZ, 
          l_break_vals, H.break_vals, 
          l_grp_vals, H.grp_vals, 
          H.num_nulls, &(H.vals), &(H.nn), H.out_size, H.out_len, 
          H.max_n_rows, &(H.n_rows), &is_eov, true);
      cBYE(status);
      if ( is_eov ) { 
        // dump what you have read to the next guy 
        // reset for next data set 
        // we read one line too much. back up
        xidx = start_xidx;
        lno--;
        printf("Data set %d has %d  rows \n", num_data_sets, H.n_rows);
        sprintf(outfilename, "%s_%d.csv", out_suffix, num_data_sets);
        ofp = fopen(outfilename, "w");
        return_if_fopen_failed(ofp, outfilename, "w");
        status = prnt_hive_by_row(C.qtypes, H.vals, H.nn, H.n_rows, 
            C.n_cols, C.holiday_str, ofp);
        cBYE(status);
        fclose_if_non_null(ofp);
        H.n_rows = 0; 
        num_data_sets++;
      }
    }
    printf("Read %d lines from file %s \n", lno, C.file_names[f]);
    rs_munmap(X, nX);
  }
  // this is to handle last data set 
  num_data_sets++;
  printf("Data set %d has %d  rows \n", num_data_sets, H.n_rows);
  sprintf(outfilename, "%s_%d.csv", out_suffix, num_data_sets);
  ofp = fopen(outfilename, "w");
  return_if_fopen_failed(ofp, outfilename, "w");
  status = prnt_hive_by_row(C.qtypes, H.vals, H.nn, H.n_rows, 
      C.n_cols, C.holiday_str, ofp);
  cBYE(status);
  fclose_if_non_null(ofp);
  //---------------------

  printf("Completed %s successfully\n", argv[0]);
BYE:
  fclose_if_non_null(ofp);
  status = free_hive(&H, &C);
  status = free_configs(&C);
  free_if_non_null(buf);
  free_if_non_null(l_break_vals);
  free_if_non_null(l_grp_vals);
  free_if_non_null(Y);
  rs_munmap(X, nX);
  return status;
}
