// The purpose of this is to isolate the code that starts at a
// data set from Hive and produces the input to R 
// ut_do_R can then take over and call mgcv, do formula specific stuff...

#include "incs.h"
#include "rbc_struct.h"

#include "auxil.h"
#include "rbc_checkers.h"
#include "num_lines.h"
#include "rbc_pr.h"
#include "rbc_helpers.h"
#include "line_to_cells.h"
#include "cells_to_vals.h"
#include "holiday.h"
#include "get_time_usec.h"
#include "rbc_makers.h"

// For DFE data set hash map
#include "DFEDS_HMAP/inc/dfeds_rs_hmap_struct.h"
rs_hmap_config_t g_dfeds_hmap_config; // for DFE data set hashmap
dfeds_rs_hmap_t  g_dfeds_hmap;        // for DFE data set hashmap

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *ifp = NULL;

  char fld_sep = ',';
  char rec_sep = '\n';
  uint32_t cellsz = 1024; // TODO P4 hard coded
  uint32_t n_cols = 4; // TODO P4 hard coded
  char **cells = NULL;
  char **str_qtypes = NULL;
  void **vals = NULL; 
  bool **nn = NULL;
  bool *is_load = NULL;
  bool *has_nulls = NULL;
  qtype_t *qtypes = NULL;
  char **col_names = NULL;
  uint32_t *widths = NULL;
  bool is_hdr = true; // TODO P4 hard coded
  RBC_REC_TYPE rbc; memset(&rbc, 0, sizeof(RBC_REC_TYPE));

  if ( argc != 3 ) { go_BYE(-1); }
  char *infile     = argv[1];
  char *outfile    = argv[2]; 
  cells = malloc(n_cols * sizeof(char *));
  memset(cells, 0, n_cols * sizeof(char *));
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    cells[i] = malloc(cellsz);
    memset(cells[i], 0, cellsz);
  }

  col_names = malloc(n_cols * sizeof(char *));
  col_names[0] = strdup("co_loc_i");
  col_names[1] = strdup("holiday_array");
  col_names[2] = strdup("slq_unit_q");
  col_names[3] = strdup("week_start_date");

  str_qtypes = malloc(n_cols * sizeof(char *));
  str_qtypes[0] = strdup("I4");
  str_qtypes[1] = strdup("HL");
  str_qtypes[2] = strdup("F4");
  str_qtypes[3] = strdup("TM:%Y-%m-%d");
  is_load = malloc(n_cols * sizeof(bool));
  for ( uint32_t i = 0; i < n_cols; i++ ) { is_load[i] = true; }

  has_nulls = malloc(n_cols * sizeof(bool));
  for ( uint32_t i = 0; i < n_cols; i++ ) { has_nulls[i] = false; }

  qtypes = malloc(n_cols * sizeof(qtype_t));
  qtypes[0] = I4;
  qtypes[1] = HL;
  qtypes[2] = F4;
  qtypes[3] = TM;

  widths = malloc(n_cols * sizeof(uint32_t));
  widths[0] = 4; 
  widths[1] = 8; 
  widths[2] = 4; 
  widths[3] = sizeof(tm_t);

  if ( strcmp(infile, outfile) == 0 ) { go_BYE(-1); } 
  
  ifp = fopen(infile, "r");
  return_if_fopen_failed(ifp, infile,"r");
  uint32_t rowidx = 0;
  int itmp = num_lines(infile); if ( itmp <= 0 ) { go_BYE(-1); }
  uint32_t nL = (uint32_t)itmp;


  vals = malloc(n_cols * sizeof(void *));
  memset(vals, 0, n_cols * sizeof(void *));
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    vals[i] = malloc(8 * nL); // TODO P4 hard coded
  }
  nn = malloc(n_cols * sizeof(bool *));
  memset(nn, 0, n_cols * sizeof(bool *));

  for ( uint32_t i = 0; i < nL; i++ ) { 
    // read a line at a time from Hive data set into X[nX]
    if ( feof(ifp) ) { go_BYE(-1); }
    char *X = NULL; size_t nX = 0;
    ssize_t nr = getline(&X, &nX, ifp); 
    if ( nr <= 0 ) { go_BYE(-1); }
    // skip first line if header
    if ( ( i == 0 ) && ( is_hdr ) ) { continue; }
    // break down the line into individual cells
    status = line_to_cells(X, nX, n_cols, fld_sep, rec_sep, cells, cellsz); 
    cBYE(status);
    // convert strings in cells into values
    bool is_null, is_err;
    status = cells_to_vals(cells, n_cols, cellsz, str_qtypes, widths, 
        is_load, has_nulls, rowidx, &nL, &vals, &nn, &is_null, &is_err); 
    cBYE(status); 
    if ( !is_null && !is_err ) { 
      rowidx++;
    }
    free_if_non_null(X);
  }
  uint32_t n_rows = rowidx;
  fprintf(stderr, "Read %d good rows out of %d \n", n_rows, nL);
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    if ( strcmp(str_qtypes[i], "HL") != 0 ) { continue; }
    status = post_process_holiday_bmask((holiday_bmask_t *)vals[i], n_rows);
    cBYE(status);
  }
  status = make_data_frame((const char ** const)col_names, n_cols, 
      NULL, (const void ** )vals, n_rows, 0, qtypes, &rbc);
  cBYE(status);
  status = chk_rbc(&rbc); cBYE(status);
  status = pr_df_as_csv(&rbc, NULL, 0, outfile); cBYE(status);
BYE:
  free_rbc(&rbc);
  free_if_non_null(is_load);
  free_if_non_null(has_nulls);
  free_if_non_null(nn);
  free_if_non_null(widths);
  free_if_non_null(qtypes);
  free_2d_array((char ***)&vals, n_cols);
  free_2d_array((char ***)&cells, n_cols);
  free_2d_array((char ***)&str_qtypes, n_cols);
  free_2d_array((char ***)&col_names, n_cols);
  fclose_if_non_null(ifp);
  return status;
}
