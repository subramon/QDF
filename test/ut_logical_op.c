#include "incs.h"
#include "rbc_struct.h"
#include "auxil.h"
#include "rbc_helpers.h"
#include "rbc_checkers.h"
#include "num_lines.h"
#include "rbc_vv_op.h"
#include "rbc_csv_to_df.h"
#include "rbc_f_to_s.h"
#include "rbc_pr.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  double dtemp; RBC_SCLR_TYPE sclr; 
  RBC_REC_TYPE rbc, in1, in2, out_and, out_or;
  memset(&rbc,     0, sizeof(RBC_REC_TYPE));
  memset(&in1,     0, sizeof(RBC_REC_TYPE));
  memset(&in2,     0, sizeof(RBC_REC_TYPE));
  memset(&out_and, 0, sizeof(RBC_REC_TYPE));
  memset(&out_or,  0, sizeof(RBC_REC_TYPE));
  const char * const infile = "../data/test_logical_op_1.csv";
  char * cols = strdup("in1,in2,and,or");
  char * qtypes = strdup("I1,I1,I1,I1");
  bool is_hdr = true;
  BUF_SPEC_TYPE buf_spec; 
  memset(&buf_spec, 0, sizeof(BUF_SPEC_TYPE));
  if ( argc != 1 ) { go_BYE(-1); } 

  status = rbc_csv_to_df(infile, cols, qtypes,
      ",", "\"", "\n", is_hdr, &buf_spec, &rbc); 
  status = chk_rbc(&rbc); cBYE(status);
  status = get_key_val(&rbc, -1, "in1", &in1, NULL); cBYE(status);
  status = get_key_val(&rbc, -1, "in2", &in2, NULL); cBYE(status);

  RBC_REC_TYPE str_rbc; memset(&str_rbc, 0, sizeof(str_rbc)); 
  pr_json(&in1, &(str_rbc.data), &(str_rbc.size), NULL);
  printf("in1 = %s\n", (char *)str_rbc.data);
  free_rbc(&str_rbc); 

  pr_json(&in2, &(str_rbc.data), &(str_rbc.size), NULL);
  printf("in2 = %s\n", (char *)str_rbc.data);
  free_rbc(&str_rbc); 

  //-------------------------------------------------------------
  status = vv_op(&in1, &in2, "logical_and", &out_and); cBYE(status);
  status = chk_rbc(&out_and); cBYE(status);
  
  pr_json(&out_and, &(str_rbc.data), &(str_rbc.size), NULL);
  printf("AND = %s\n", (char *)str_rbc.data);
  free_rbc(&str_rbc); 

  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&out_and, "sum", NULL, &sclr); cBYE(status);
  status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status); 
  if ( dtemp != 1 ) { go_BYE(-1); } 

  //-------------------------------------------------------------
  status = vv_op(&in1, &in2, "logical_or", &out_or); cBYE(status);
  status = chk_rbc(&out_or); cBYE(status);

  pr_json(&out_or, &(str_rbc.data), &(str_rbc.size), NULL);
  printf("OR  = %s\n", (char *)str_rbc.data);
  free_rbc(&str_rbc); 

  memset(&sclr, 0, sizeof(sclr));
  status = f_to_s(&out_or, "sum", NULL, &sclr); cBYE(status);
  status = get_sclr_as_F8(&sclr, &dtemp); cBYE(status); 
  if ( dtemp != 3 ) { go_BYE(-1); } 
  //-------------------------------------------------------------
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);

BYE:
  free_rbc(&rbc);
  free_rbc(&in1);
  free_rbc(&in2);
  free_rbc(&out_or);
  free_rbc(&out_and);
  free_if_non_null(cols);
  free_if_non_null(qtypes);
  return status;
}
