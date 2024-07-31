// ./ut2 data/in1.csv
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "macros.h"
#include "aux_rcli.h"
#include "num_lines.h"
#include "mk_df_str.h"
#include "num_cols.h"
#include "read_csv.h"
#include "get_time_usec.h"
#include "get_named_vec.h"

#include "cols.c" // Defines column names and types and NCOLS


int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  int portnum = 6311;
  int sock;
  double *tmpF8 = NULL; int tmpn;
  char *exec_out = NULL; size_t len_out;
  void **X = NULL;
  const char * const server = "127.0.0.1";
  char *df_str = NULL;
  char *key_blob = NULL; int sz_key_blob = 4096; int n_key_blob = 0;
  double *vals = NULL; int sz_vals = 128; int n_vals = 0;
  //-------------------------

  const char * const lbrry = "library(mgcv)";
  const char * const frmla = 
    " frmla = \"sls_unit_q ~ s(co_loc_i, bs = 're') + "
    " columbusday + goodfriday + easter + fathersday.minus + fathersday + "
    " laborday.minus + laborday + mardigras + memorialday.minus + "
    " memorialday + mlkday + mothersday.minus + mothersday + presidentsday + "
    " superbowl.minus + superbowl + thanksgiving.minus + thanksgiving + "
    " cyber.monday + valentines.minus + valentines + stpatricks + "
    " cincodemayo + julyfourth.minus + julyfourth + halloween.minus + "
    " halloween + veteransday + christmas.minus + christmas + "
    " newyearsday.minus + newyearsday + s(t_o_y, bs = 'cc', k = 25) + "
    " s(time_band, bs = 're') "
  const char * const fnctn = 
    " callgam <- function(rsdf) { "
    " rsdf$co_loc_i <- as.factor(rsdf$co_loc_i); "
    " rsdf$time_band <- as.factor(rsdf$time_band); " 
    " args <- list( "
    "   data =  rsdf, " 
    "   family = nb,  "
    "   formula = compiled_frmla, 
    " m <- do.call(gam, args); "
    " coeff <- m$coefficients; "
    " return(coeff); "
    " } ";
  const char * const callR = "coeff <- callgam(rsdf) ";

  if ( argc != 2 ) { go_BYE(-1); }
  const char *  const infile = argv[1];
  // make some data to send 
  int ncols = num_cols(infile);  
  if ( ncols != NCOLS ) { go_BYE(-1); } 
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
    else {
      go_BYE(-1);
    }
  }
  status = read_csv(infile, col_types, X, nrows, ncols, ",", "\"", "\n", is_hdr);
  cBYE(status);
  //-----------------
  status = rconnect(server, portnum, &sock); cBYE(status);
  printf("Established connection\n");
  // Make vectors for each column
  for ( int i = 0; i < ncols; i++ ) { 
    status = set_vec(sock, col_names[i], col_types[i], X[i], nrows); 
    cBYE(status);
    if ( strcmp(col_types[i], "F8") == 0 ) { 
      free_if_non_null(tmpF8);
#define CHECK_SET
#ifdef CHECK_SET
      status = get_vec_F8(sock, col_names[i], &tmpF8, &tmpn); cBYE(status);
      if ( tmpn != nrows ) { go_BYE(-1); }
      double *dptr = (double *)X[i];
      for ( int j = 0; j < nrows; j++ ) { 
        if ( tmpF8[j] != dptr[j] ) { 
          go_BYE(-1); 
        }
      }
      free_if_non_null(exec_out); len_out = 0;
      char getlen[1024];
      sprintf(getlen, "length(%s)", col_names[i]);
      // TODO status = exec_str(sock, getlen, &exec_out, &len_out, -1); 

#endif
    }
  }
  //-- START:  One time setup
  status = exec_str(sock, lbrry, NULL, NULL, -1); cBYE(status);
  status = exec_str(sock, frmla, NULL, NULL, -1); cBYE(status);
  status = exec_str(sock, "compiled_frmla = as.formula(frmla)", 
      NULL, NULL, -1); cBYE(status);

  free_if_non_null(exec_out); len_out = 0;
  status = exec_str(sock, "frmla", &exec_out, &len_out, XT_ARRAY_STR); 
  if ( exec_out == NULL ) { go_BYE(-1); }
  status = exec_str(sock, fnctn, NULL, NULL, -1); cBYE(status);
  free_if_non_null(exec_out); len_out = 0;
  status = exec_str(sock, "class(callgam)", &exec_out, &len_out, XT_ARRAY_STR); 
  if ( exec_out == NULL ) { go_BYE(-1); }
  if ( strcmp(exec_out, "function") != 0 ) { go_BYE(-1); }
  // Allocate space for returned coefficients
  key_blob = malloc(sz_key_blob);
  return_if_malloc_failed(key_blob); 
  vals = malloc(sz_vals * sizeof(double));
  return_if_malloc_failed(vals);
  // STOP  : One time setup
  //
  uint64_t t_start = get_time_usec();
  int niters = 1;
  for ( int iter = 0; iter < niters; iter++ ) {
    //-- Make data frame
    df_str = mk_df_str(col_names, NULL, ncols); 
    if ( df_str == NULL ) {  go_BYE(-1); }
    status = exec_str(sock, df_str, NULL, NULL, -1); cBYE(status);
#ifdef DEBUG
    // Check that data frame created
    status = exec_str(sock, "class(rsdf)", &exec_out, &len_out,XT_ARRAY_STR); 
    if ( len_out != 12 ) { go_BYE(-1); }
    if ( exec_out == NULL ) { go_BYE(-1); }
    if ( strcmp(exec_out, "data.frame") != 0 )  { go_BYE(-1); }
#endif
    //-- Now we are ready for the real deal
    status = exec_str(sock, callR, NULL, NULL, -1); cBYE(status);
    // Get the coefficients back
    bool get_unnamed = false;
    if ( get_unnamed ) { 
      free_if_non_null(tmpF8);
      status = get_vec_F8(sock, "coeff", &tmpF8, &n_vals); cBYE(status);
    }
    else {
      status = get_named_vec_F8(sock, "coeff", 
          key_blob, sz_key_blob, &n_key_blob,
          vals, sz_vals, &n_vals);
      cBYE(status);
    }
#ifdef SHORT_FORMULA
    if ( n_vals != 11 ) { go_BYE(-1); }
#else
    if ( n_vals != 76 ) { go_BYE(-1); }
#endif
  }
  uint64_t t_stop = get_time_usec();
  fprintf(stderr, "Iterations = %d, Time per  %lf \n", niters,
      ((double)(t_stop-t_start)/(niters * 1000000.0)));
  //-------------------------

  //-------------------------
  status = close_conn(sock); cBYE(status);
  printf("Closed connection\n");
  printf("Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(key_blob);
  free_if_non_null(vals);
  free_if_non_null(tmpF8);
  if ( X != NULL ) { 
    for ( int i  = 0; i < ncols; i++ ) { 
      free_if_non_null(X[i]);
    }
    free_if_non_null(X);
  }
  free_if_non_null(df_str);
  return status;
}
