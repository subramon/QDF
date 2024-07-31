#include "incs.h"
#include "rbc_struct.h"
#include "bridge_read_configs.h"
#include "free_configs.h"

int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  config_t C;
  memset(&C, 0, sizeof(config_t));

  if ( argc != 2 ) { go_BYE(-1); }
  const char * const config_file = argv[1];
  status = bridge_read_configs(config_file, &C); cBYE(status);
  free_configs(&C);
  fprintf(stdout, "Test %s completed successfully\n", argv[0]);
BYE:
  return status;
}
