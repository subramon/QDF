#include "incs.h"
#include "aux_rcli.h"
#include "num_lines.h"
#include "num_cols.h"
#include "read_csv.h"
#include "get_time_usec.h"
#include "get_named_vec.h"
#include "load_file_as_str.h"
#include "get_bool.h"
#include "set_vec.h"
#include "mk_df_str.h"


int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  int portnum = 6311;
  int sock;
  void **X = NULL;
  const char * const server = "127.0.0.1";
  char *df_str = NULL;
  char *key_blob = NULL; int sz_key_blob = 4096; int n_key_blob = 0;
  double *vals = NULL; int sz_vals = 128; int n_vals = 0;
  char *frmla = NULL;
  int ncols; char *tmp = NULL;
  char **col_names = NULL; // [ncols][...]
  char **col_types = NULL; // [ncols][...]
  //-------------------------

#include "for_call_gam_R.c"

  if ( argc != 5 ) { go_BYE(-1); }
  const char *  const formula_file   = argv[1];
  const char *  const col_names_file = argv[2];
  const char *  const col_types_file = argv[3];
  const char *  const infile         = argv[4];

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
  //----------------------------------------
  // make formula 
  frmla = load_file_as_str(formula_file);
  if ( frmla == NULL ) { go_BYE(-1); }
  // make some data to send 
  chk_ncols = num_cols(infile);  
  if ( ncols != chk_ncols ) { go_BYE(-1); } 
  int nrows = num_lines(infile); if ( nrows <= 1 ) { go_BYE(-1); }
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
  //-----------------
  status = rconnect(server, portnum, &sock); cBYE(status);
  // printf("Established connection\n");
  //-- START:  One time setup
  status = exec_str(sock, lbrry, NULL, NULL, -1); cBYE(status);
  status = exec_str(sock, frmla, NULL, NULL, -1); cBYE(status);

  status = exec_str(sock, "compiled_frmla = as.formula(frmla)", 
      NULL, NULL, -1); cBYE(status);
  status = exec_str(sock, fnctn, NULL, NULL, -1); cBYE(status);
  // make data frame command 
  df_str = mk_df_str(col_names, col_types, ncols); 
  if ( df_str == NULL ) {  go_BYE(-1); }
  // Allocate space for returned coefficients
  key_blob = malloc(sz_key_blob);
  return_if_malloc_failed(key_blob); 
  vals = malloc(sz_vals * sizeof(double));
  return_if_malloc_failed(vals);
  // STOP  : One time setup
  //
  uint64_t t_start = get_time_usec();
  int niters = 6;
  for ( int iter = 0; iter < niters; iter++ ) {
    // Make vectors for each column
    for ( int i = 0; i < ncols; i++ ) { 
      if ( strcmp(col_types[i], "I1") == 0 ) { go_BYE(-1); } //not supported
      if ( strcmp(col_types[i], "XX") == 0 ) { continue; } // ignore
      status = set_vec(sock, col_names[i], col_types[i], X[i], nrows); 
      cBYE(status);
    }
    //-- Make data frame
    status = exec_str(sock, df_str, NULL, NULL, -1); cBYE(status);
    //-- Now we are ready for the real deal
    status = exec_str(sock, callR, NULL, NULL, -1); cBYE(status);
    // Check whether mgcv worked
    bool bval;
    status = exists1(sock, "coeff", &bval); cBYE(status);
    if ( bval ) { 
      // Get the coefficients back
      status = get_named_vec_F8(sock, "coeff", 
          key_blob, sz_key_blob, &n_key_blob,
          vals, sz_vals, &n_vals);
      cBYE(status);
    }
    else {
      fprintf(stderr, "Iter %d mgcv failed on ", iter);
      for ( int i = 1; i < argc; i++ ) { 
        fprintf(stderr, " %s ", argv[i]);
      }
      fprintf(stderr, "\n"); 
    }
    //--------------------------------------------
  }
  uint64_t t_stop = get_time_usec();
  fprintf(stdout, "Iters=%d,Time_per=%lf\n", niters,
      ((double)(t_stop-t_start)/(niters * 1000000.0)));
  //-------------------------
  status = close_conn(sock); cBYE(status);
  // printf("Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(key_blob);
  free_if_non_null(vals);
  if ( X != NULL ) { 
    for ( int i  = 0; i < ncols; i++ ) { 
      free_if_non_null(X[i]);
    }
    free_if_non_null(X);
  }
  if ( col_names != NULL ) { 
    for ( int i  = 0; i < ncols; i++ ) { 
      free_if_non_null(col_names[i]);
    }
    free_if_non_null(col_names);
  }
  if ( col_types != NULL ) { 
    for ( int i  = 0; i < ncols; i++ ) { 
      free_if_non_null(col_types[i]);
    }
    free_if_non_null(col_types);
  }
  free_if_non_null(tmp);
  free_if_non_null(df_str);
  free_if_non_null(frmla);
  return status;
}
