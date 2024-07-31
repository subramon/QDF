#include "incs.h"
#include <pthread.h>
#include "rbc_struct.h"
#include "bridge_read_configs.h"
#include "free_configs.h"
#include "read_xconfigs.h"
#include "free_xconfigs.h"
#include "aux_row_producer.h"
#include "row_producer.h"
#include "aux_data_set_producer.h"
#include "data_set_producer.h"

#define MAIN_PGM
#include "row_prod_globals.h"
#include "ds_prod_globals.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C; memset(&C, 0, sizeof(config_t));
  xconfig_t XC; memset(&XC, 0, sizeof(xconfig_t));
  // for row producer
  pthread_t l_row_producer; 
  row_prod_input_t row_prod_info;
  // for data set producer
  pthread_t l_ds_producer; 
  data_set_prod_input_t ds_prod_info;

  g_ds_prod_all_done = 0;
  if ( argc != 2 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  status = bridge_read_configs(config_file, &C); cBYE(status);
  status = read_xconfigs(&C, &XC); cBYE(status);

  // for row producer
  memset(&row_prod_info, 0, sizeof(row_prod_input_t));
  status = row_prod_init(&row_prod_info, &C, 
      &g_row_prod_outq, &g_row_prod_outq_data, 
      &g_row_prod_all_done, &g_row_prod_n_in_q );

  row_prod_info.dump_on_floor = false; // need to pass to data_set_producer

  // for data set producer
  memset(&ds_prod_info, 0, sizeof(data_set_prod_input_t));
  ds_prod_info.C = &C;
  ds_prod_info.XC = &XC;
  status = data_set_prod_init(C.ds_prod_qsz, &g_ds_prod_outq, 
      &g_ds_prod_outq_data, &g_ds_prod_n_in_q);
  cBYE(status);
  ds_prod_info.dump_on_floor = true; // NOTE: Just for testing 
  //-- Spawn threads
  status = pthread_create(&l_row_producer, NULL, 
      &row_producer, &row_prod_info);
  cBYE(status);
  status = pthread_create(&l_ds_producer, NULL, 
      &data_set_producer, &ds_prod_info);
  cBYE(status);
  // wait for threads to terminate
  pthread_join(l_row_producer, NULL);
  pthread_join(l_ds_producer, NULL);
  // check exit status
  if ( row_prod_info.status != 0 ) { go_BYE(-1); }
  if ( ds_prod_info.status != 0 ) { go_BYE(-1); }
  // print stats
  status = row_prod_stats(&row_prod_info); cBYE(status);  
  status = data_set_prod_stats(&ds_prod_info); cBYE(status);  

  if ( row_prod_info.num_rows_read != ds_prod_info.num_consumed ) {
    go_BYE(-1);
  }
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_configs(&C);
  free_xconfigs(&XC);
  data_set_prod_free( g_ds_prod_outq, g_ds_prod_outq_data, 
      C.ds_prod_qsz);
  row_prod_free(g_row_prod_outq, g_row_prod_outq_data); 
  return status;
}
