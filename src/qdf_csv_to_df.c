#include "incs.h"
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

static uint32_t
extract_width_SC(
    char * const str_qtype
    )
{
  int status = 0;
  if ( str_qtype == NULL ) { go_BYE(-1); }
  if ( strlen(str_qtype) < 4 ) { go_BYE(-1); } 
  char *cptr = str_qtype + strlen("SC:");
  if ( *cptr == '\0' ) { go_BYE(-1); } 
  for ( char *xptr = cptr; *xptr != '\0'; xptr++ ) { 
    if ( !isdigit(*xptr) ) { go_BYE(-1); }
  }
  int itmp = atoi(cptr); if ( itmp < 2 ) { go_BYE(-1); }
  // need width of 1+1 for nullc
  uint32_t w = (uint32_t)itmp;
BYE:
  if ( status < 0 ) { return 0; } else { return w; }
}
// TODO P2 Add is_load to read_csv and to this function 
// input is a CSV file, output is a QDF dataframe
int
qdf_csv_to_df(
    const char * const infile, // INPUT 
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

  if ( infile == NULL ) { go_BYE(-1); }
  int64_t tempI8 = num_lines(infile);  if ( tempI8 <= 0 ) { go_BYE(-1); }
  uint32_t n_rows = (uint32_t)tempI8;
  if ( is_hdr ) { n_rows--; }
  status = split_str(concat_cols, ",", &cols, &n_cols); cBYE(status);
  status = split_str(concat_qtypes, ",", &str_qtypes, &n_qtypes); cBYE(status);
  if ( n_cols != n_qtypes ) { go_BYE(-1); }
  if ( n_cols <= 0 ) { go_BYE(-1); }

  c_qtypes = malloc(n_cols * sizeof(qtype_t));
  return_if_malloc_failed(c_qtypes);
  //----------------------------------------
  // Allocate space for columns
  vals = malloc(n_cols * sizeof(char *));
  return_if_malloc_failed(vals);
  memset(vals, 0, n_cols * sizeof(char *));

  widths = malloc(n_cols * sizeof(uint32_t));
  return_if_malloc_failed(widths);
  memset(widths, 0, n_cols * sizeof(uint32_t));

  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    qtype_t qtype = Q0; uint32_t width = 0; 
    if ( strncmp(str_qtypes[i], "SC:", strlen("SC:")) == 0 ) { 
      qtype = SC;
      // if we have SC:8, we want width to be 8
      // Note "1234567" can be stored in SC:8 but not "12345678"
      // Need one spot for nullc
      width = extract_width_SC(str_qtypes[i]);
      if ( width == 0 ) { go_BYE(-1); }
      // width needs to be multiple of 8
      width = multiple_n((uint32_t)width, 8);
    }
    else { 
      qtype = get_c_qtype(str_qtypes[i]);
      width = get_width_c_qtype(qtype);
    }
    //----------------------------------
    if ( width == 0 ) { go_BYE(-1); }
    if ( qtype == Q0 ) { 
      fprintf(stderr, "Unknown qtype [%s] \n", str_qtypes[i]);
      go_BYE(-1);  
    }
    //----------------------------------
    uint32_t sz = width * n_rows;
    vals[i] = malloc(sz); return_if_malloc_failed(vals[i]);
    c_qtypes[i] = qtype;
    widths[i] = width;
  }
  status = read_csv(infile, (const char **const)str_qtypes, 
      (void **const)vals, widths, n_rows, n_cols, str_fld_sep, 
      str_fld_delim, str_rec_sep, is_hdr);
  cBYE(status);
  uint32_t sz = 0;
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

  status = make_data_frame((const char **const)cols, n_cols, widths, 
      (const void **)vals, n_rows, sz, c_qtypes, ptr_qdf);
  cBYE(status);

BYE:
  free_if_non_null(c_qtypes);
  free_if_non_null(widths);
  free_2d_array(&str_qtypes, n_qtypes);
  free_2d_array(&cols, n_cols);
  free_2d_array((char ***)&vals, n_cols);
  return status;
}
