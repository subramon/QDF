#include "incs.h"
#include "rbc_struct.h"
#include "configs.h"
#include "rbc_helpers.h"
#include "bridge_read_configs.h"
#include "free_configs.h"
#include "db_agg_dataset.h"
#include "db_mark_read.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  memset(&C, 0, sizeof(config_t));
  int64_t break_vals[2]; // TODO P4 hard coded

  if ( argc != 2 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  status = bridge_read_configs(config_file, &C); cBYE(status);

  int i = 0; bool is_eod; int num_is_good = 0;
  for ( i = 0, is_eod = false; !is_eod; i++ ) { 
    uint64_t pk; bool is_good = false;
    RBC_REC_TYPE out_rbc; memset(&out_rbc, 0, sizeof(RBC_REC_TYPE));
    status = db_agg_dataset(&C, &out_rbc, &pk, &is_good,&is_eod,break_vals);
    // printf("%d: pk=%ld, is_good=%s\n", i, pk, is_good ? "true" : "false");
    status = db_mark_read(&C, NULL, NULL, "styl_loc_err", pk); cBYE(status);
    free_rbc(&out_rbc);
    if ( is_good ) { num_is_good++; }
  }
  printf("Found %d aggregates \n", num_is_good); 
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_configs(&C);
  return status;
}
