#include <pthread.h>
#include <omp.h>
#include "incs.h"
#include "aux_rcli.h"
#include "num_lines.h"
#include "num_cols.h"
#include "read_csv.h"
#include "get_time_usec.h"
#include "get_named_vec.h"
#include "load_file_as_str.h"
#include "mk_df_str.h"
#include "task_types.h"
#include "load_data.h"
#include "get_bool.h"
#include "set_vec.h"

static void  *
hammer(
    void *X
    )
{
  int status = 0;
  int sock;
  char *key_blob = NULL; int sz_key_blob = 4096; int n_key_blob = 0;
  double *vals = NULL; int sz_vals = 128; int n_vals = 0;
  task_info_t *T = (task_info_t *)X;

  //-- START:  One time setup
  status = rconnect(T->server, T->portnum, &sock); cBYE(status);
  status = exec_str(sock, T->lbrry, NULL, NULL, -1); cBYE(status);
  status = exec_str(sock, T->frmla, NULL, NULL, -1); cBYE(status);
  status = exec_str(sock, "compiled_frmla = as.formula(frmla)", 
      NULL, NULL, -1); cBYE(status);
  status = exec_str(sock, T->fnctn, NULL, NULL, -1); cBYE(status);
  // Allocate space for returned coefficients
  key_blob = malloc(sz_key_blob);
  return_if_malloc_failed(key_blob); 
  vals = malloc(sz_vals * sizeof(double));
  return_if_malloc_failed(vals);
  // STOP  : One time setup
  T->t_start = get_time_usec();
  for ( int iter = 0; iter < T->niters; iter++ ) {
    // Make vectors for each column
    for ( int i = 0; i < T->ncols; i++ ) { 
      if ( strcmp(T->col_types[i], "I1") == 0 ) { go_BYE(-1); } 
      if ( strcmp(T->col_types[i], "XX") == 0 ) { continue; } // ignore
      status = set_vec(sock, 
          T->col_names[i], T->col_types[i], T->X[i], T->nrows); 
      cBYE(status);
    }
    //-- Make data frame
    status = exec_str(sock, T->df_str, NULL, NULL, -1); cBYE(status);
    //-- Now we are ready for the real deal
    status = exec_str(sock, T->callR, NULL, NULL, -1); cBYE(status);
    // Check whether mgcv worked
    bool bval; status = exists1(sock, "coeff", &bval);  cBYE(status);
    if ( bval ) { 
    // Get the coefficients back
    status = get_named_vec_F8(sock, "coeff", 
        key_blob, sz_key_blob, &n_key_blob,
        vals, sz_vals, &n_vals);
    }
    else  {
      fprintf(stderr, "mgcv failed \n"); 
      T->failures++; // TODO Account for this somehow 
    }
  }
  T->t_stop = get_time_usec();
  status = close_conn(sock); cBYE(status);
BYE:
  free_if_non_null(key_blob);
  free_if_non_null(vals);
  if ( status < 0 ) { WHEREAMI; }
  return NULL;
}

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  int portnum = 6311;
  void **X = NULL;
  const char * const server = "127.0.0.1";
  char *df_str = NULL;
  char *frmla = NULL;
  int ncols, nrows; 
  char **col_names = NULL; // [ncols][...]
  char **col_types = NULL; // [ncols][...]
  task_info_t *tinfo = NULL;
  pthread_t *threads = NULL;
#include "for_call_gam_R.c"
  //-------------------------
  if ( argc != 5 ) { go_BYE(-1); }
  const char *  const formula_file   = argv[1];
  const char *  const col_names_file = argv[2];
  const char *  const col_types_file = argv[3];
  const char *  const infile         = argv[4];

  status = load_data(col_names_file, col_types_file, infile,
    &ncols, &nrows, &X, &col_names, &col_types);
  //----------------------------------------
  // make formula 
  frmla = load_file_as_str(formula_file);
  if ( frmla == NULL ) { go_BYE(-1); }
  // make data frame command 
  df_str = mk_df_str(col_names, col_types, ncols); 
  if ( df_str == NULL ) {  go_BYE(-1); }
  //-----------------
  // Create threads and set up information for each thread 
  int niters = 8; // TODO 

  int max_nT = 4; // TODO 
  threads = malloc(max_nT * sizeof(pthread_t));
  return_if_malloc_failed(threads);
  memset(threads, '\0', max_nT * sizeof(pthread_t));

  tinfo = malloc(max_nT * sizeof(task_info_t));
  memset(tinfo, 0, max_nT * sizeof(task_info_t));
  for ( int i = 0; i < max_nT; i++ ) { 
    memset(&(tinfo[i]), 0, sizeof(task_info_t));
    tinfo[i].tid  = i;
    tinfo[i].portnum = portnum;
    tinfo[i].server = server;

    tinfo[i].X = X;
    tinfo[i].col_names = col_names;
    tinfo[i].col_types = col_types;
    tinfo[i].nrows = nrows;
    tinfo[i].ncols = ncols;

    tinfo[i].callR = callR;
    tinfo[i].df_str = df_str;
    tinfo[i].fnctn = fnctn;
    tinfo[i].frmla = frmla;
    tinfo[i].lbrry = lbrry;

    tinfo[i].niters = niters;
  }
  //--------------------------------
  for ( int nT = 4; nT <= max_nT; nT++ ) { 
    printf("Using %d threads \n", nT);
    uint64_t t_start = get_time_usec();
    for ( int tid = 0; tid < nT; tid++ ) { // spawn threads 
      pthread_create(&(threads[tid]), NULL, hammer, (void *)(tinfo+tid));
    }
    // fprintf(stderr, "forked all threads\n");
    for ( int tid = 0; tid < nT; tid++ ) { 
      pthread_join(threads[tid], NULL);
    }
    // fprintf(stderr, "joined all threads\n");
    uint64_t t_stop = get_time_usec();
    double secs = (t_stop - t_start) / 1000000.0;
    fprintf(stderr, "%s,%d,%lf\n", infile, nT, secs);
  }
  //--------------------------------------------
BYE:
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
  free_if_non_null(df_str);
  free_if_non_null(frmla);
  return status;
}
