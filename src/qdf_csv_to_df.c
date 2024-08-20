#include "incs.h"
#include "free_2d_array.h"
#include "multiple.h"
#include "qdf_struct.h"
#include "cat_to_buf.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "read_csv.h"
#include "split_str.h"
#include "num_lines.h"
#include "qdf_makers.h"
#include "qdf_csv_to_df.h"

// TODO P2 Add is_load to read_csv and to this function 
// input is a CSV file, output is a QDF dataframe
int
qdf_csv_to_df(
    const char * const infile, // INPUT 
    char *X, // INPUT (alternative to infile)
    size_t nX, // INPUT (alternative to infile)
    const char * const concat_cols, // INPUT 
    const char * const concat_qtypes, // INPUT 
    const char * const str_fld_sep, // INPUT 
    const char * const str_fld_delim, // INPUT 
    const char * const str_rec_sep, // INPUT 
    bool is_hdr, // INPUT 
    BUF_SPEC_TYPE *ptr_buf_spec, // INPUT 
    QDF_REC_TYPE *ptr_qdf // OUTPUT 
    )
{
  int status = 0;
  char ** cols = NULL; uint32_t n_cols = 0;
  char ** str_qtypes = NULL; uint32_t n_qtypes = 0;
  char **vals = NULL;
  uint32_t *widths = NULL;
  qtype_t *c_qtypes = NULL;

  char **alt_cols = NULL;
  char **alt_vals = NULL;
  uint32_t *alt_widths = NULL;
  qtype_t *alt_c_qtypes = NULL;

  if ( infile == NULL ) { 
    if ( ( X == NULL ) || ( nX == 0 ) ) { go_BYE(-1); }
  }
  else {
    if ( ( X != NULL ) || ( nX != 0 ) ) { go_BYE(-1); }
  }
  int64_t tempI8 = num_lines(infile, X, nX);  
  if ( tempI8 <= 0 ) { go_BYE(-1); }
  uint32_t n_rows = (uint32_t)tempI8;
  if ( is_hdr ) { n_rows--; }
  status = split_str(concat_cols, ",", &cols, &n_cols); cBYE(status);
  status = split_str(concat_qtypes, ",", &str_qtypes, &n_qtypes); cBYE(status);
  if ( n_cols != n_qtypes ) { go_BYE(-1); }
  if ( n_cols <= 0 ) { go_BYE(-1); }

  // Allocate space for columns
  c_qtypes = malloc(n_cols * sizeof(qtype_t));
  return_if_malloc_failed(c_qtypes);
  memset(c_qtypes, 0,  n_cols * sizeof(qtype_t));

  vals = malloc(n_cols * sizeof(char *));
  return_if_malloc_failed(vals);
  memset(vals, 0, n_cols * sizeof(char *));

  widths = malloc(n_cols * sizeof(uint32_t));
  return_if_malloc_failed(widths);
  memset(widths, 0, n_cols * sizeof(uint32_t));
  //----------------------------------------

  uint32_t alt_n_cols = 0;
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    c_qtypes[i] = get_c_qtype(str_qtypes[i]);
    widths[i]   = get_width_qtype(str_qtypes[i]);
    if ( c_qtypes[i] == SC ) { 
      widths[i]   = (uint32_t)multiple_n(widths[i], 8); 
    }
    // width needs to be multiple of 8 TODO: What about qtype != SC?
    //----------------------------------
    if ( c_qtypes[i] == Q0 ) { continue; } // don't load this column
    alt_n_cols++;
    uint32_t sz = widths[i] * n_rows;
    vals[i] = malloc(sz); return_if_malloc_failed(vals[i]);
  }
  if ( alt_n_cols == 0 ) { go_BYE(-1); }
  status = read_csv(infile, X, nX, str_qtypes, 
      vals, widths, n_rows, n_cols, str_fld_sep, 
      str_fld_delim, str_rec_sep, is_hdr);
  cBYE(status);
  uint32_t sz = 0;
  if ( ptr_buf_spec != NULL ) { 
    switch ( ptr_buf_spec->spec ) { 
      case buf_plus : 
        sz = (uint32_t)((int32_t)n_rows + ptr_buf_spec->buf.plus); break; 
      case buf_times : 
        sz = (uint32_t)((float)n_rows * ptr_buf_spec->buf.times); break; 
      case buf_absolute : 
        sz = (uint32_t)(ptr_buf_spec->buf.abs); break;
      case buf_undef : /* nothing to do */ break;
      default : go_BYE(-1); break;
    }
  }
  if ( alt_n_cols == n_cols ) { 
    status = make_data_frame(cols, n_cols, widths, 
        vals, n_rows, sz, c_qtypes, ptr_qdf);
    cBYE(status);
  }
  else {
    // Allocate space for columns that will actually be loaded
    alt_cols = malloc(alt_n_cols * sizeof(qtype_t));
    return_if_malloc_failed(alt_cols);
    memset(alt_cols, 0,  alt_n_cols * sizeof(qtype_t));

    alt_c_qtypes = malloc(alt_n_cols * sizeof(qtype_t));
    return_if_malloc_failed(alt_c_qtypes);
    memset(alt_c_qtypes, 0,  alt_n_cols * sizeof(qtype_t));

    alt_vals = malloc(alt_n_cols * sizeof(char *));
    return_if_malloc_failed(alt_vals);
    memset(alt_vals, 0, alt_n_cols * sizeof(char *));

    alt_widths = malloc(alt_n_cols * sizeof(uint32_t));
    return_if_malloc_failed(widths);
    memset(alt_widths, 0, alt_n_cols * sizeof(uint32_t));

    uint32_t alt_idx = 0;
    for ( uint32_t i = 0 ; i  < n_cols; i++ ) { 
      if ( c_qtypes[i] == Q0 ) { continue; }
      if ( alt_idx == alt_n_cols ) { go_BYE(-1); }
      alt_vals[alt_idx] = vals[i];
      alt_c_qtypes[alt_idx] = c_qtypes[i];
      alt_widths[alt_idx] = widths[i];
      alt_cols[alt_idx] = strdup(cols[i]);
      alt_idx++;
    }
    status = make_data_frame(alt_cols, alt_n_cols, alt_widths, 
        (const void **)alt_vals, n_rows, sz, alt_c_qtypes, ptr_qdf);
    cBYE(status);
    free_2d_array(&alt_cols, alt_n_cols);
    free_if_non_null(alt_c_qtypes);
    free_2d_array(&alt_vals, alt_n_cols);
    free_if_non_null(alt_widths);
    // following needed because we have transferred from vals to_alt_vals
    for ( uint32_t i = 0; i < n_cols; i++ ) { vals[i] = NULL; }
  }

BYE:
  free_if_non_null(c_qtypes);
  free_if_non_null(widths);
  free_2d_array(&str_qtypes, n_qtypes);
  free_2d_array(&cols, n_cols);
  if ( vals != NULL ) { 
    for ( uint32_t i = 0; i < n_cols; i++ ) { 
      free_if_non_null(vals[i]);
    }
    free_if_non_null(vals);
  }
  return status;
}
