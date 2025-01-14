#include "incs.h"
#include "free_2d_array.h"
#include "multiple.h"
#include "num_lines_safe.h"
#include "qdf_struct.h"
#include "cat_to_buf.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "read_csv.h"
#include "split_str.h"
#include "num_lines.h"
#include "qdf_makers.h"
#include "add_nn_keys.h"
#include "qdf_csv_to_df.h"

// TODO P2 Add is_load to read_csv and to this function 
// input is a CSV file, output is a QDF dataframe
int
qdf_csv_to_df(
    const char * const infile, // INPUT 
    char *X, // INPUT (alternative to infile)
    size_t nX, // INPUT (alternative to infile)
    char ** const in_cols, // INPUT  [in_ncols]
    const qtype_t * const in_qtypes, // INPUT  [in_ncols]
    const uint32_t * const in_widths, // INPUT  [in_ncols]
    char ** const in_formats, // INPUT [in_ncols]
    const bool * const in_has_nulls, // INPUT  [in_ncols]
    const bool * const in_is_load, // INPUT  [in_ncols]
    uint32_t in_ncols,
    const char * const str_fld_sep, // INPUT 
    const char * const str_fld_delim, // INPUT 
    const char * const str_rec_sep, // INPUT 
    bool is_hdr, // INPUT 
    QDF_REC_TYPE *ptr_qdf // OUTPUT 
    )
{
  int status = 0;
  char **in_vals = NULL; bool **in_nn_vals = NULL; 
  // following is input to dataframe 
  char **vals = NULL; 
  char **cols = NULL;  // [ncols]
  qtype_t *qtypes = NULL;  // [ncols]
  uint32_t *widths = NULL;  // [ncols]
  uint32_t ncols; 

  //----------------------------------------
  uint32_t nrows; 
  uint32_t max_cell_width = 1024-1;
  status = num_lines_safe(infile, X, nX, max_cell_width, in_ncols,
      is_hdr, str_fld_sep, str_rec_sep, &nrows);
  cBYE(status);
  if ( nrows == 0 ) { go_BYE(-1); }
  // check no dupes 
  for ( uint32_t i = 0; i < in_ncols; i++ ) { 
    for ( uint32_t j = i+1; j < in_ncols; j++ ) { 
      if ( strcmp(in_cols[i], in_cols[j]) == 0 ) { 
        fprintf(stderr, "Duplicate names %u, %u %s \n", i, j, in_cols[i]);
        go_BYE(-1); }
    }
  }
  // START: Allocate space for data  being read in 
  in_vals = malloc(in_ncols * sizeof(char *));
  memset(in_vals, 0, in_ncols * sizeof(char *));
  in_nn_vals = malloc(in_ncols * sizeof(bool *));
  memset(in_nn_vals, 0,  in_ncols * sizeof(bool *));

  for ( uint32_t i = 0; i < in_ncols; i++ ) { 
    if ( !in_is_load[i] ) { continue; }
    uint32_t in_width_i = in_widths[i];
    if ( in_width_i == 0 ) { go_BYE(-1); }
    in_vals[i] = malloc(nrows * in_width_i);
    return_if_malloc_failed(in_vals[i]);
    memset(in_vals[i], 0,  nrows * in_width_i);
    if ( in_has_nulls[i] ) { 
      in_nn_vals[i] = malloc(nrows * sizeof(bool));
      return_if_malloc_failed(in_nn_vals[i]);
      memset(in_nn_vals[i], 0,  nrows * sizeof(bool));
    }
  }
  //  STOP: Allocate space for data  being read in 

  // read CSV file 
  status = read_csv(infile, X, nX, in_qtypes, in_widths, in_formats, 
      in_is_load, in_has_nulls, in_vals, in_nn_vals, nrows, in_ncols, 
      str_fld_sep, str_fld_delim, str_rec_sep, is_hdr);
  cBYE(status);
  // convert from in_* to *  This is necessitated by the fact
  // that in_cols does not fully represent all the columns in the
  // dataframe that we will create
  // 1) If is_load[i] == false, then in_col[i] will not show up in df
  // 2) If has_nulls[i] == false, then in_col[i] AND "nn_" .. in_col[i] 
  //   will show up in df
  // Figure out what goes into dataframe 
  status = add_nn_keys(in_cols, in_qtypes, in_widths, in_has_nulls,
      in_is_load, in_ncols, 
      &cols, &qtypes, &widths, &ncols);
  cBYE(status);
  // Move data into format needed for make_data_frame
  vals = malloc(ncols * sizeof(char *));
  memset(vals, 0,  ncols * sizeof(char *));
  uint32_t inidx = 0, outidx = 0;
  for ( ; inidx < in_ncols; inidx++ ) { 
    if ( !in_is_load[inidx] ) { continue; }
    vals[outidx++] = in_vals[inidx];
    in_vals[inidx] = NULL;
    if ( in_has_nulls[inidx] ) {
      vals[outidx++] = (char *)in_nn_vals[inidx];
      in_nn_vals[inidx] = NULL;
    }
  }
  // Now we can throw away in_vals and in_nn_vals
  free_if_non_null(in_vals); 
  free_if_non_null(in_nn_vals); 

  // convert from in_vals to vals, from in_nn_vals to nn_vals
  status = make_data_frame(cols, ncols, widths, vals, nrows, 0, 
      qtypes, ptr_qdf);
  cBYE(status);
BYE:
  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_2d_array(&cols, ncols);
  // Note that we do not free in_vals[i] because if it was not NULL,
  // then it was handed off to vals[i]. Same for in_nn_vals[i]
  free_if_non_null(in_vals); 
  free_if_non_null(in_nn_vals); 
  //--------------------
  if ( vals != NULL ) { 
    for ( uint32_t i = 0; i < ncols; i++ ) { 
      free_if_non_null(vals[i]);
    }
    free_if_non_null(vals);
  }
  //--------------------
  return status;
}
