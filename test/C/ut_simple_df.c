#include "incs.h"
#include "qtypes.h"
#include "qdf_struct.h"
#include "qdf_checkers.h"
#include "qdf_helpers.h"
#include "qdf_xhelpers.h"
#include "qdf_pr.h"
#include "qdf_test.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  QDF_REC_TYPE qdf; memset(&qdf, 0, sizeof(QDF_REC_TYPE));
  status = qdf_mk_df(&qdf); cBYE(status);
  status = chk_qdf(&qdf); cBYE(status);
  status = pr_df_as_csv(&qdf, NULL, 0, "_df.csv", false);
  fprintf(stdout, "SUCCESS Test %s completed successfully\n", argv[0]);
BYE:
  free_qdf(&qdf); 
  return status;
}
