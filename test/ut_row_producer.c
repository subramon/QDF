#include "incs.h"
#include <pthread.h>
#include "rbc_struct.h"
#include "bridge_read_configs.h"
#include "free_configs.h"
#include "aux_row_producer.h"
#include "row_producer.h"

#define MAIN_PGM
#include "row_prod_globals.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  memset(&C, 0, sizeof(config_t));
  pthread_t l_row_producer; 

  if ( argc != 2 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  status = bridge_read_configs(config_file, &C); cBYE(status);

  row_prod_input_t row_prod_info;
  status = row_prod_init(&row_prod_info, &C, 
      &g_row_prod_outq, &g_row_prod_outq_data, 
      &g_row_prod_all_done, &g_row_prod_n_in_q );
  row_prod_info.dump_on_floor = true; // NOTE: Just for testing 
  //-----------------------------
  status = pthread_create(&l_row_producer, NULL, 
      &row_producer, &row_prod_info);
  cBYE(status);
  pthread_join(l_row_producer, NULL);
  if ( row_prod_info.status != 0 ) { go_BYE(-1); }
  status = row_prod_stats(&row_prod_info); cBYE(status);  
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  free_configs(&C);
  row_prod_free(g_row_prod_outq, g_row_prod_outq_data); 
  return status;
}
