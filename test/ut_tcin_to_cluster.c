#include "incs.h"
#include "rbc_struct.h"
#include "rbc_helpers.h"
#include "rbc_xhelpers.h"
#include "rbc_external.h"
#include "aux_R_online.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *cluster = NULL;
  RBC_REC_TYPE meta_rbc; memset(&meta_rbc, 0, sizeof(RBC_REC_TYPE));
  //-------------------
  if ( argc != 3 ) { go_BYE(-1); }
  const char * const infile = argv[1];
  int32_t tcin = atoi(argv[2]); 
  //-------------------
  status = binld(&meta_rbc, infile, false); cBYE(status);
  status = old_tcin_to_cluster(&meta_rbc, 
      "tcin", "cluster_id", tcin, &cluster); 
  cBYE(status);
  printf("FOUND Cluster for %d is %s \n", tcin, cluster);
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_if_non_null(cluster);
  free_rbc(&meta_rbc); 
  return status;
}
