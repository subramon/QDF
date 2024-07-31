#include "incs.h"
#include <pthread.h>
#include "rbc_struct.h"
#include "data_set_read_struct.h"
#include "bridge_read_configs.h"
#include "free_configs.h"
#include "aux_data_set_reader.h"
#include "aux_data_set_producer.h"
#include "data_set_prod_struct.h"
#include "data_set_aggregator.h"

#include "row_prod_struct.h"
#define MAIN_PGM
#include "ds_prod_globals.h"
#include "ds_read_globals.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C; memset(&C, 0, sizeof(config_t));
  // for data set reader
  pthread_t l_ds_aggregator; 
  data_set_read_input_t ds_read_info;

  if ( argc != 2 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  status = bridge_read_configs(config_file, &C); cBYE(status);

  // for data set reader
  g_ds_prod_all_done = 1;
  g_ds_read_all_done = 0;
  memset(&ds_read_info, 0, sizeof(data_set_read_input_t));
  ds_read_info.C  = &C;
  ds_read_info.dump_on_floor = true; // NOTE: Just for testing 

  status = data_set_prod_init(C.ds_prod_qsz, &g_ds_prod_outq, 
      &g_ds_prod_outq_data, &g_ds_prod_n_in_q);
  cBYE(status);
  //-- Spawn threads
  status = pthread_create(&l_ds_aggregator, NULL, 
      &data_set_aggregator, &ds_read_info);
  cBYE(status);
  // wait for threads to terminate
  pthread_join(l_ds_aggregator, NULL);
  // check exit status
  if ( ds_read_info.status != 0 ) { go_BYE(-1); }
  // print stats
  status = data_set_read_stats(&ds_read_info); cBYE(status);  

  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_configs(&C);
  data_set_prod_free( g_ds_prod_outq, g_ds_prod_outq_data, 
      C.ds_prod_qsz);
  return status;
}
