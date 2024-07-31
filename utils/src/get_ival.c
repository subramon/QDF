#include "incs.h"
#include "get_ival.h"
int get_ival(
    const char * const buf,
    long int *ptr_ival
    )
{
  int status = 0;
  char *endptr;
  long int ival = strtol(buf, &endptr, 10);
  if ( *endptr != '\0' ) { 
    double dval = strtod(buf, &endptr);
    if ( *endptr != '\0' ) { 
      go_BYE(-1);
    }
    else {
      if ( ceil(dval) == floor(dval) ) { 
        ival = (long int)dval;
      }
      else {
        go_BYE(-1);
      }
    }
  }
  *ptr_ival = ival;
BYE:
  return status;
}
