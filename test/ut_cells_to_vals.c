#include "incs.h"
#include "auxil.h"
#include "rbc_struct.h"
#include "rbc_struct.h"
#include "holiday.h"
#include "cells_to_vals.h"

// For DFE data set hash map
#include "DFEDS_HMAP/inc/dfeds_rs_hmap_struct.h"
rs_hmap_config_t g_dfeds_hmap_config; // for DFE data set hashmap
dfeds_rs_hmap_t  g_dfeds_hmap;        // for DFE data set hashmap
#include "row_prod_struct.h"

// used for communication between row producer and data set producer
int *g_row_prod_outq;
row_prod_output_t *g_row_prod_outq_data;
// used for external communiciation through atomics
int g_row_prod_all_done;
int g_row_prod_n_in_q; // number of valid entries in Q
int
main(
    int argc,
    char **argv
    )
{
  int  status = 0;
  char **cells = NULL; 
  uint32_t n_rows = 2; // hard coded
  uint32_t n_cols = 8;
  void **vals = NULL;
  bool **nn = NULL;
  char **str_qtypes = NULL;
  bool *is_load = NULL;
  bool *has_nulls = NULL;
  uint32_t *widths = NULL;

  if ( argc != 1 ) { go_BYE(-1); }

  is_load = malloc(n_cols * sizeof(bool));
  for ( uint32_t i = 0; i < n_cols; i++ ) { is_load[i] = true; }

  has_nulls = malloc(n_cols * sizeof(bool));
  for ( uint32_t i = 0; i < n_cols; i++ ) { has_nulls[i] = false; }

  str_qtypes = malloc(n_cols * sizeof(char *));
  memset(str_qtypes, 0,  n_cols * sizeof(char *));
  str_qtypes[0] = strdup("I1");
  str_qtypes[1] = strdup("I2");
  str_qtypes[2] = strdup("I4");
  str_qtypes[3] = strdup("I8");
  str_qtypes[4] = strdup("F4");
  str_qtypes[5] = strdup("F8");
  str_qtypes[6] = strdup("TM:%Y-%m-%d");
  str_qtypes[7] = strdup("HL");

  vals = malloc(n_cols * sizeof(void **));
  memset(vals, 0,  n_cols * sizeof(void **));

  nn = malloc(n_cols * sizeof(bool *));
  memset(nn, 0,  n_cols * sizeof(bool *));

  widths = malloc(n_cols * sizeof(int));
  memset(widths, 0,  n_cols * sizeof(int));
  for ( uint32_t c = 0; c < n_cols; c++ ) { 
    uint32_t w;
    switch ( c ) { 
      case 0 : w = sizeof(int8_t); break; 
      case 1 : w = sizeof(int16_t); break; 
      case 2 : w = sizeof(int32_t); break; 
      case 3 : w = sizeof(int64_t); break; 
      case 4 : w = sizeof(float); break; 
      case 5 : w = sizeof(double); break; 
      case 6 : w = sizeof(tm_t); break; 
      case 7 : w = sizeof(holiday_bmask_t); break; 
      default : go_BYE(-1); break;
    }
    widths[c] = w;
    vals[c] = malloc(n_rows * w);
    memset(vals[c], 0, n_rows * w);
  }
  //------------------------------
  cells = malloc(n_cols * sizeof(char *));
  memset(cells, 0,  n_cols * sizeof(char *));
  cells[0] = strdup("1"); // I1
  cells[1] = strdup("2"); // I2
  cells[2] = strdup("3"); // I4
  cells[3] = strdup("4"); // I8
  cells[4] = strdup("5.6"); // F4
  cells[5] = strdup("7.89"); // F8
  cells[6] = strdup("2021-02-03"); // TM:%Y-%m-%d
  cells[7] = strdup("memorialday.minus1+memorialday+NA"); // HL

  uint32_t cellsz = 64; // TODO P4 fix hard coded 
  // here  is the call
  bool is_null, is_err;
  for ( uint32_t r = 0; r < n_rows; r++ ) { 
    status = cells_to_vals(cells, n_cols, cellsz, str_qtypes, widths, 
        is_load, has_nulls, r, &n_rows, &vals, &nn, &is_null,  &is_err); 
    cBYE(status);
  }
  //-----------------
  for ( uint32_t r = 0; r < n_rows; r++ ) {
    if ( ((int8_t *)vals[0])[r] != 1 ) { go_BYE(-1); }
    if ( ((int16_t *)vals[1])[r] != 2 ) { go_BYE(-1); }
    if ( ((int32_t *)vals[2])[r] != 3 ) { go_BYE(-1); }
    if ( ((int64_t *)vals[3])[r] != 4 ) { go_BYE(-1); }

    float eps = (float)0.01;
    float fval = ((float *)vals[4])[r];
    if ( ( fval > 5.6+eps) || ( fval < 5.6-eps) ) { go_BYE(-1); }

    double dval = ((double *)vals[5])[r];
    if ( ( dval > 7.89+eps) || ( dval < 7.89-eps) ) { go_BYE(-1); }

    if ( ((tm_t *)vals[6])[r].tm_year != 121  ) { go_BYE(-1); }

    holiday_bmask_t hlval = ((holiday_bmask_t *)vals[7])[0];
    if  ( hlval != (512+256) ) { go_BYE(-1); }
  }
  // Post-processing on any columns of type HL
  // Need to post-process holidays so that foo and foo_minus
  // are both not present in same row 
  for ( uint32_t i = 0; i < n_cols; i++ ) { 
    if ( strcmp(str_qtypes[i], "HL") != 0 ) { continue; }
    status = post_process_holiday_bmask((holiday_bmask_t *)vals[i],n_rows); 
    cBYE(status);
  }
  for ( uint32_t r = 0; r < n_rows; r++ ) {
    holiday_bmask_t hlval = ((holiday_bmask_t *)vals[7])[0];
    if  ( hlval != 512 ) { go_BYE(-1); }
  }

  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(nn);
  free_2d_array((char ***)&vals, n_cols);
  free_2d_array(&cells, n_cols);
  free_2d_array(&str_qtypes, n_cols);
  free(is_load);
  free(has_nulls);
  free(widths);
  return status;
}
