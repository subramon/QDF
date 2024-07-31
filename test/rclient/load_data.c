#include "incs.h"
#include "macros.h"
#include "load_file_as_str.h"
#include "num_cols.h"
#include "num_lines.h"
#include "read_csv.h"
#include "load_data.h"

int
load_data(
    const char * const col_names_file,
    const char * const col_types_file,
    const char * const infile,
    int *ptr_ncols,
    int *ptr_nrows,
    void ***ptr_X,
    char ***ptr_col_names,
    char ***ptr_col_types
    )
{
  int status = 0;
  char * tmp = NULL;
  void **X = NULL;
  char **col_names = NULL;
  char **col_types = NULL;
  int ncols, nrows;
  // read column names
  tmp = load_file_as_str(col_names_file);
  if ( tmp == NULL ) { go_BYE(-1); }
  ncols = 0;
  for  ( char *cptr = tmp; *cptr != '\0'; cptr++ ) { 
    if ( *cptr == ',' ) { ncols++; }
  }
  if ( ncols < 1 ) { go_BYE(-1); }
  col_names = malloc(ncols * sizeof(char *));
  col_names[0] = strdup(strtok(tmp, ","));
  for ( int i = 1; i < ncols; i++ ) { 
    col_names[i] = strdup(strtok(NULL, ","));
  }
  //--- read column types
  free_if_non_null(tmp);
  tmp = load_file_as_str(col_types_file);
  if ( tmp == NULL ) { go_BYE(-1); }
  int chk_ncols = 0;
  for  ( char *cptr = tmp; *cptr != '\0'; cptr++ ) { 
    if ( *cptr == ',' ) { chk_ncols++; }
  }
  if ( chk_ncols != ncols  ) { go_BYE(-1); }
  col_types = malloc(ncols * sizeof(char *));
  col_types[0] = strdup(strtok(tmp, ","));
  for ( int i = 1; i < ncols; i++ ) { 
    char *cptr = strdup(strtok(NULL, ","));
    if ( cptr == NULL ) { go_BYE(-1); }
    col_types[i] = strdup(cptr);
  }
  // make some data to send 
  chk_ncols = num_cols(infile);  
  if ( ncols != chk_ncols ) { go_BYE(-1); } 
  nrows = num_lines(infile); if ( nrows <= 1 ) { go_BYE(-1); }
  bool is_hdr = true; // IMPORTANT: Assumption
  nrows--; // because of header 
  X = malloc(ncols * sizeof(void *));
  for ( int i = 0; i < ncols; i++ ) { 
    if ( strcmp(col_types[i], "F8") == 0 ) { 
      X[i] = malloc(nrows * sizeof(double));
    }
    else if ( strcmp(col_types[i], "I4") == 0 ) { 
      X[i] = malloc(nrows * sizeof(int32_t));
    }
    else if ( strcmp(col_types[i], "I1") == 0 ) { 
      X[i] = malloc(nrows * sizeof(int8_t));
    }
    else if ( strcmp(col_types[i], "XX") == 0 ) { 
      X[i] = NULL; // we will not read this in
    }
    else {
      go_BYE(-1);
    }
  }
  status = read_csv(infile, col_types, X, nrows, ncols, ",", "\"", "\n", is_hdr);
  cBYE(status);
  *ptr_X = X;
  *ptr_ncols = ncols;
  *ptr_nrows = nrows;
  *ptr_col_names = col_names;
  *ptr_col_types = col_types;

BYE:
  free_if_non_null(tmp);
  return status;
}
