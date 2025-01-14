#include "incs.h"
#include "qtypes.h"
#include "free_2d_array.h"
#include "prep_args_for_read_csv.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "num_lines.h"
#include "qdf_vv_op.h"
#include "qdf_csv_to_df.h"
#include "qdf_f_to_s.h"
#include "qdf_pr.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  double dtemp; SCLR_REC_TYPE sclr; 
  QDF_REC_TYPE qdf, in1, in2, out_and, out_or;
  memset(&qdf,     0, sizeof(QDF_REC_TYPE));
  memset(&in1,     0, sizeof(QDF_REC_TYPE));
  memset(&in2,     0, sizeof(QDF_REC_TYPE));
  memset(&out_and, 0, sizeof(QDF_REC_TYPE));
  memset(&out_or,  0, sizeof(QDF_REC_TYPE));
  const char * const infile = "../data/test_logical_op_1.csv";
  char * in_cols = strdup("in1,in2,and,or");
  char * in_str_qtypes = strdup("I1,I1,I1,I1");
  bool is_hdr = true;

  char **cols = NULL; uint32_t ncols = 0;
  qtype_t *qtypes = NULL;  // [ncols]
  uint32_t *widths = NULL;  // [ncols]
  char **formats = NULL;  // [ncols]
  bool *has_nulls = NULL; // [ncols]
  bool *is_load = NULL; // [ncols]

  if ( argc != 1 ) { go_BYE(-1); } 
  status = prep_args_for_read_csv(in_cols, in_str_qtypes,
    &qtypes, &widths, &formats, &has_nulls, &is_load , &cols, &ncols);
  status = qdf_csv_to_df(infile, NULL, 0, cols, qtypes,
      widths, formats, has_nulls, is_load, ncols, 
      ",", "\"", "\n", is_hdr, &qdf); 

  status = chk_qdf(&qdf); cBYE(status);
  status = get_key_val(&qdf, -1, "in1", &in1, NULL); cBYE(status);
  status = get_key_val(&qdf, -1, "in2", &in2, NULL); cBYE(status);

  QDF_REC_TYPE str_qdf; memset(&str_qdf, 0, sizeof(str_qdf)); 
  uint32_t len = 0;
  pr_json(&in1, &str_qdf, &len, NULL);
  printf("in1 = %s\n", (char *)str_qdf.data);
  free_qdf(&str_qdf); 

  pr_json(&in2, &str_qdf, &len, NULL);
  printf("in2 = %s\n", (char *)str_qdf.data);
  free_qdf(&str_qdf); 

  //-------------------------------------------------------------
  status = vv_op(&in1, &in2, "logical_and", &out_and); cBYE(status);
  status = chk_qdf(&out_and); cBYE(status);
  
  pr_json(&out_and, &str_qdf, &len, NULL);
  printf("AND = %s\n", (char *)str_qdf.data);
  free_qdf(&str_qdf); 

  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&out_and, "sum", NULL, &sclr); cBYE(status);
  status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status); 
  if ( dtemp != 1 ) { go_BYE(-1); } 

  //-------------------------------------------------------------
  status = vv_op(&in1, &in2, "logical_or", &out_or); cBYE(status);
  status = chk_qdf(&out_or); cBYE(status);

  pr_json(&out_or, &str_qdf, &len, NULL);
  printf("OR  = %s\n", (char *)str_qdf.data);
  free_qdf(&str_qdf); 

  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&out_or, "sum", NULL, &sclr); cBYE(status);
  status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status); 
  if ( dtemp != 3 ) { go_BYE(-1); } 
  //-------------------------------------------------------------
  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);

BYE:
  free_qdf(&qdf);
  free_qdf(&in1);
  free_qdf(&in2);
  free_qdf(&out_or);
  free_qdf(&out_and);
  free_if_non_null(in_cols);
  free_if_non_null(in_str_qtypes);

  free_if_non_null(qtypes);
  free_if_non_null(widths);
  free_2d_array(&cols, ncols);
  free_2d_array(&formats, ncols);
  free_if_non_null(has_nulls);
  free_if_non_null(is_load);
  return status;
}
