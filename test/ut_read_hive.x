//  ut_read_hive 44 ../data/hive1.txt ../data/qtypes1.csv _out1
#include "incs.h"
#include "cmem_struct.h"
#include "rbc_struct.h"
#include "configs.h"
#include "hive_struct.h"

#include "rbc.h"
#include "read_hive.h"
#include "prnt_hive.h"
#include "q_to_RBC.h"
#include "read_configs.h"
#include "free_configs.h"
#include "init_hive.h"
#include "free_hive.h"
#include "prune.h"
#include "mark_rows_to_prune.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  hive_run_t H;
  RBC_REC_TYPE rbc, orbc;
  const char * outfile     = NULL;
  const char * binfile     = NULL;

  memset(&rbc, 0, sizeof(RBC_REC_TYPE));
  memset(&orbc, 0, sizeof(RBC_REC_TYPE));
  memset(&H, 0, sizeof(hive_run_t));
  memset(&C, 0, sizeof(config_t));
  FILE *ofp = NULL;
  FILE *bfp = NULL;

  if ( argc != 4 ) { go_BYE(-1); }
  const char * const cfg_dir = argv[1];
  outfile     = argv[2]; // write hive file as CSV file 
  binfile     = argv[3]; // write RBC as binary file 

  status = read_configs(cfg_dir, &C); cBYE(status);
  status = init_hive(&C, &H); cBYE(status);
  int iter = 0;
  if ( C.n_files != 1 ) { go_BYE(-1); } // TODO > 1 needs to be implemented
  void **chk_vals = NULL; int chk_n_rows; // for what to print
  bool is_eof = false;
  int cum_n_rows = 0;
  for ( int lno = 0; !is_eof ; iter++, lno++ ) { 
    status = read_hive(C.file_names[0], &H.offset, 
        C.break_cols, C.n_break_cols, H.break_vals, 
        C.grp_cols, C.n_grp_cols, H.grp_vals, 
        C.qtypes, C.is_load, 
        H.num_nulls, &(H.vals), &(H.nn), H.out_size, H.out_len, 
        H.max_n_rows, &H.n_rows, C.n_cols, C.fld_sep, C.rec_sep, &is_eof);
    printf("Set %d, lines = %d, total lines = %d \n", lno, H.n_rows, cum_n_rows);
    cum_n_rows += H.n_rows;
    cBYE(status);
#ifdef DEBUG
    for ( int j = 0; j < C.n_cols; j++ ) { 
      if ( C.is_load[j] == false ) { continue; }
      if ( C.has_nulls[j] == false ) { 
        if ( H.num_nulls[j] != 0 ) { go_BYE(-1);
        }
      }
    }
#endif
    bool is_prune = true; // Just for testing 
    bool is_any_null = false;
    if ( is_prune ) { 
      // prune null values
      for ( int j = 0; j < C.n_cols; j++ ) { 
        if ( H.num_nulls[j] != 0 ) { is_any_null = true; break; }
      }
      if ( is_any_null ) { 
        // create bool is_prune[..] marking rows to prune
        status = mark_rows_to_prune(H.nn, C.n_cols, H.n_rows, 
            C.is_load, H.is_prune);
        cBYE(status);
        // count number of rows that have been marked as is_prune
        int n_null;
        status = sum_bool(H.is_prune, H.n_rows, &n_null);
        if ( n_null == H.n_rows ) { 
          fprintf(stderr, "Nothing after pruning nulls. TODO\n");
          go_BYE(-1);
        }
        if ( n_null > 0 ) { 
          H.good_n_rows = H.n_rows - n_null;
          // now we have to prune stuff
          status = prune(H.vals, H.good_vals, C.n_cols,
              C.is_load, C.widths, H.n_rows, H.good_n_rows, H.is_prune);
          cBYE(status);
          chk_vals = H.good_vals;
          chk_n_rows = H.good_n_rows;
        }
        else {
          chk_vals = H.vals;
          chk_n_rows = H.n_rows;
        }
      }
      else {
        chk_vals = H.vals;
        chk_n_rows = H.n_rows;
      }
    }
  }
  if ( *outfile == '\0' ) { 
    ofp = stdout;
  }
  else {
    ofp = fopen(outfile, "w");
    return_if_fopen_failed(ofp, outfile, "w");
  }
  status = prnt_hive_by_row(C.qtypes, H.vals, H.nn, H.n_rows, 
      C.n_cols, C.holiday_str, ofp);
  cBYE(status);
  status = q_to_RBC(C.col_names, C.widths, C.qtypes, C.is_load,
      chk_vals, NULL, chk_n_rows, C.n_cols, &rbc);
  cBYE(status);
  if ( rbc.data == NULL ) { go_BYE(-1); }
  status = check(&rbc); cBYE(status);
  orbc.size = 120000; // TODO FIX 
  orbc.data = malloc(orbc.size);
  uint32_t buflen = 0;
  status = pr(&rbc, &orbc, &buflen); cBYE(status);
  if ( *binfile != '\0' ) { 
    bfp = fopen(binfile, "wb");
    return_if_fopen_failed(bfp, binfile, "wb");
    int nw = fwrite(rbc.data, rbc.size, 1, bfp);
    if ( nw != 1 ) { go_BYE(-1); }
    fclose_if_non_null(bfp);
  }
  printf("Completed %s successfully\n", argv[0]);
BYE:
  free_if_non_null(orbc.data);
  free_if_non_null(rbc.data);
  if ( ( outfile != NULL ) && ( *outfile != '\0' ) ) { 
    fclose_if_non_null(ofp);
  }
  fclose_if_non_null(bfp);
  status = free_hive(&H, &C);
  status = free_configs(&C);
  return status;
}
