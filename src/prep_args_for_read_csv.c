#include "incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "split_str.h"
#include "prep_args_for_read_csv.h"

int
prep_args_for_read_csv(
    // input 
    char * const in_cols,
    char * const in_str_qtypes,
    // output 
    qtype_t **ptr_qtypes,
    uint32_t **ptr_widths,
    char ***ptr_formats,
    bool **ptr_has_nulls,
    bool **ptr_is_load,
    char ***ptr_cols,
    uint32_t *ptr_ncols
    )
{
  int status = 0;

  char **cols = NULL; // [ncols]
  uint32_t ncols = 0; 
  char **str_qtypes = NULL; // [nqtypes]
  uint32_t nqtypes = 0; 

  char **formats = NULL;
  uint32_t *widths = NULL;
  qtype_t *qtypes = NULL;
  bool *has_nulls = NULL;
  bool *is_load = NULL;

  status = split_str(in_cols, ",", &cols, &ncols);
  cBYE(status);
  if ( ncols == 0 ) { go_BYE(-1); }

  status = split_str(in_str_qtypes, ",", &str_qtypes, &nqtypes);
  cBYE(status);
  if ( nqtypes != ncols ) { go_BYE(-1); }

  qtypes    = malloc(ncols * sizeof(qtype_t));
  widths    = malloc(ncols * sizeof(uint32_t));
  formats   = malloc(ncols * sizeof(char *));
  has_nulls = malloc(ncols * sizeof(bool));
  is_load   = malloc(ncols * sizeof(bool));

  for ( uint32_t i = 0; i < ncols; i++ ) { 
    has_nulls[i] = false;
    is_load[i] = true;
    qtypes[i] = get_c_qtype(str_qtypes[i]);
    formats[i] = get_format(str_qtypes[i]);
    widths[i] = get_width_c_qtype(qtypes[i]);
  }
  *ptr_qtypes    = qtypes;
  *ptr_widths    = widths;
  *ptr_formats    = formats;
  *ptr_has_nulls = has_nulls;
  *ptr_is_load   = is_load;
  *ptr_cols      = cols;
  *ptr_ncols     = ncols;
BYE:
  free_2d_array(&str_qtypes, ncols);
  return status;
}
