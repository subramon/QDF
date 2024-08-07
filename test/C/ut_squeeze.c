#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_checkers.h"
#include "num_lines.h"
#include "qdf_squeeze_where.h"
#include "qdf_f_to_s.h"
#include "qdf_pr.h"
#include "qdf_csv_to_df.h"
#include "qdf_setters.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *cols_b = NULL; char *qtypes_b = NULL;
  // read first data frame that is to be squeezed
  SCLR_REC_TYPE sclr; memset(&sclr, 0, sizeof(sclr));
  QDF_REC_TYPE qdf_a, qdf_b, qdf_u, qdf_v, qdf_w, qdf_x, qdf_op;
  char * cols_a = strdup("x,y");
  char * qtypes_a = strdup("F4,F8");
  bool is_hdr = true;
  uint32_t len_str = 0;
  uint32_t chk_n;

  memset(&qdf_a, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_b, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_u, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_v, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_w, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_x, 0, sizeof(QDF_REC_TYPE));
  memset(&qdf_op, 0, sizeof(QDF_REC_TYPE));
  BUF_SPEC_TYPE buf_spec; memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  if ( argc != 3 ) { go_BYE(-1); } 

  const char * const infile_a = argv[1]; // "../data/test_squeeze_1a.csv"
  const char * const infile_b = argv[2]; // "../data/test_squeeze_1b.csv"

  status = qdf_csv_to_df(infile_a, cols_a, qtypes_a,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf_a); 
  status = chk_qdf(&qdf_a); cBYE(status);
  uint32_t nkeys_a = x_get_obj_len(&qdf_a);
  if ( nkeys_a != 2 ) { go_BYE(-1); }  // => 2 columns in data frame: x, y
  if ( !x_get_is_df(&qdf_a) ) { go_BYE(-1); }  //qdf_as must be data frame
  uint32_t n_rows_a = x_get_obj_arr_len(&qdf_a); 
  if ( n_rows_a != 10 ) { go_BYE(-1); }
  
  // check column x in qdf_a
  status = get_key_val(&qdf_a, -1, "x", &qdf_x, NULL);

  uint32_t n_x = x_get_arr_len(&qdf_x);
  if ( n_x != 10 ) { go_BYE(-1); } 

  qtype_t qtype = x_get_qtype(&qdf_x);
  if ( qtype != F4 ) { go_BYE(-1); }

  SCLR_REC_TYPE sum_sclr; memset(&sum_sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_x, "sum", NULL, &sum_sclr);
  if ( sum_sclr.qtype != F8 ) { go_BYE(-1); } 
  if ( sum_sclr.val.f8 != 55 ) { go_BYE(-1); } 

  SCLR_REC_TYPE min_sclr; memset(&min_sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_x, "min", NULL, &min_sclr);
  if ( min_sclr.qtype != F4 ) { go_BYE(-1); } 
  if ( min_sclr.val.f4 != 1 ) { go_BYE(-1); } 

  SCLR_REC_TYPE max_sclr; memset(&max_sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_x, "max", NULL, &max_sclr);
  if ( max_sclr.qtype != F4 ) { go_BYE(-1); } 
  if ( max_sclr.val.f4 != 10 ) { go_BYE(-1); } 

  // read second data frame that is to be used for squeezing
  cols_b = strdup("w,v,u,x");
  qtypes_b = strdup("I1,I1,I1,I4");

  status = qdf_csv_to_df(infile_b, cols_b, qtypes_b,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf_b); 
  status = chk_qdf(&qdf_b); cBYE(status);
  uint32_t nkeys_b = x_get_obj_len(&qdf_b);
  if ( nkeys_b != 4 ) { go_BYE(-1); }  // => 4 columns in data frame

  // extract where column from qdf_b
  status = get_key_val(&qdf_b, -1, "w", &qdf_w, NULL);
  status = chk_qdf(&qdf_w); cBYE(status);
  uint32_t num_good; 
  status = squeeze_where(&qdf_a, &qdf_w, &num_good); cBYE(status);
  // Note that when squeeze_where succeeds, it returns number of 
  // elemenrs after squeezing
  if ( num_good != 5 ) { go_BYE(-1); } 
  //  check that number after squeezing is correct
  uint32_t sn = x_get_obj_arr_len(&qdf_a); 
  if ( sn != 5 ) { go_BYE(-1); }
  status = pr_json(&qdf_a, &qdf_op, &len_str, NULL); cBYE(status);
  if ( strcmp((char *)qdf_op.data,
        "{\"x\" : [1, 3, 5, 7, 9], \"y\" : [1, 3, 5, 7, 9]}") != 0 ) { 
    go_BYE(-1);
  }
  free_qdf(&qdf_op);
  // check that sum of selected values is correct 
  // = 1 + 3 + 5 + 7 + 9 = 25
  double dtemp; 
  free_qdf(&qdf_x); memset(&qdf_x, 0, sizeof(qdf_x));
  status = get_key_val(&qdf_a, -1, "x", &qdf_x, NULL);
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_x, "sum", NULL, &sclr); cBYE(status);
  status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status); 
  if ( dtemp != 25 ) { go_BYE(-1); }
  // check number of elements
  chk_n = x_get_arr_len(&qdf_x);
  if ( chk_n != 5 ) { go_BYE(-1); } 
  //=============================================================
  // squeeze with v = all 1's (load qdf_a again)
  free_qdf(&qdf_a); memset(&qdf_a, 0, sizeof(qdf_a));
  status = qdf_csv_to_df(infile_a, cols_a, qtypes_a,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf_a); 
  status = get_key_val(&qdf_b, -1, "v", &qdf_v, NULL);
  status = squeeze_where(&qdf_a, &qdf_v, &num_good);  cBYE(status);
  // Note that when squeeze_where succeeds, it returns number of 
  // elemenrs after squeezing
  if ( num_good != 10 ) { go_BYE(-1); } 
  //  check that number after squeezing is correct
  sn  = x_get_obj_arr_len(&qdf_a);
  if ( sn != 10 ) { go_BYE(-1); }
  // check that sum of selected values is correct 
  // = 1 + 2 + .. + 10 = 55 
  free_qdf(&qdf_x); memset(&qdf_x, 0, sizeof(qdf_x));
  status = get_key_val(&qdf_a, -1, "x", &qdf_x, NULL); cBYE(status);
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_x, "sum", NULL, &sclr); cBYE(status);
  status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status); 
  if ( dtemp != 55 ) { go_BYE(-1); }
  // check number of elements
  chk_n = x_get_arr_len(&qdf_x);
  if ( chk_n != 10 ) { go_BYE(-1); } 
  //=============================================================
  // squeeze with u = all 0's, load qdf_a again
  free_qdf(&qdf_a); memset(&qdf_a, 0, sizeof(qdf_a));
  status = qdf_csv_to_df(infile_a, cols_a, qtypes_a,
      ",", "\"", "\n", is_hdr, &buf_spec, &qdf_a); 
  status = get_key_val(&qdf_b, -1, "u", &qdf_u, NULL); cBYE(status);
  status = squeeze_where(&qdf_a, &qdf_u, &num_good); cBYE(status);
  //  check that number after squeezing is correct
  sn  = x_get_obj_arr_len(&qdf_a);
  if ( sn != 0 ) { go_BYE(-1); }
  // check that sum of selected values is correct 
  free_qdf(&qdf_x); memset(&qdf_x, 0, sizeof(qdf_x));
  status = get_key_val(&qdf_a, -1, "x", &qdf_x, NULL); cBYE(status);
  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&qdf_x, "sum", NULL, &sclr); cBYE(status);
  status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status); 
  if ( dtemp != 0 ) { go_BYE(-1); }
  // check number of elements
  chk_n = x_get_arr_len(&qdf_x);
  if ( chk_n != 0 ) { go_BYE(-1); } 
  //--------
  // START DANGEROUS 
  // Squeezing does not delete elements, just moves them around
  // Testing this claim 
  set_obj_arr_len(qdf_a.data, n_rows_a); 
  for ( uint32_t i = 0; i < nkeys_a; i++ ) { 
    free_qdf(&qdf_x); memset(&qdf_x, 0, sizeof(qdf_x));
    status = get_key_val(&qdf_a, (int)i, NULL, &qdf_x, NULL); cBYE(status);
    set_arr_len(qdf_x.data, n_rows_a); 
  }
  // STOP  DANGEROUS 
  sn  = x_get_obj_arr_len(&qdf_a);
  if ( sn != 10 ) { go_BYE(-1); }
  // check number of elements
  chk_n = x_get_arr_len(&qdf_x);
  if ( chk_n != 10 ) { go_BYE(-1); } 

  status = squeeze_where(&qdf_a, &qdf_w, &num_good); cBYE(status);
  if ( num_good != 5 ) { go_BYE(-1); } 

  // check number of elements
  free_qdf(&qdf_x); memset(&qdf_x, 0, sizeof(qdf_x));
  status = get_key_val(&qdf_a, -1, "x", &qdf_x, NULL); cBYE(status);
  chk_n = x_get_arr_len(&qdf_x);
  if ( chk_n != 5 ) { go_BYE(-1); } 

  sn  = x_get_obj_arr_len(&qdf_a);
  if ( sn != 5 ) { go_BYE(-1); }

  // START DANGEROUS 
  // Squeezing does not delete elements, just moves them around
  // Testing this claim 
  set_obj_arr_len(qdf_a.data, n_rows_a); 
  for ( uint32_t i = 0; i < nkeys_a; i++ ) { 
    free_qdf(&qdf_x); memset(&qdf_x, 0, sizeof(qdf_x));
    status = get_key_val(&qdf_a, (int)i, NULL, &qdf_x, NULL); cBYE(status);
    set_arr_len(qdf_x.data, n_rows_a); 
  }
  // STOP  DANGEROUS 
  len_str = 0;
  status = pr_json(&qdf_a, &qdf_op, &len_str, NULL); cBYE(status);
  printf("%s\n", (char *)qdf_op.data);
  if ( strcmp((char *)qdf_op.data, 
        "{\"x\" : [1, 3, 5, 7, 9, 2, 4, 6, 8, 10], "
        "\"y\" : [1, 3, 5, 7, 9, 2, 4, 6, 8, 10]}") != 0 ) { 
    go_BYE(-1);
  }

  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);
BYE:
  free_qdf(&qdf_a);
  free_qdf(&qdf_op);
  free_qdf(&qdf_b);
  free_if_non_null(cols_a);
  free_if_non_null(qtypes_a);
  free_if_non_null(cols_b);
  free_if_non_null(qtypes_b);
  return status;
}
