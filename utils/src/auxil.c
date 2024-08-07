#include "incs.h"
#include "auxil.h"
#include "get_time_usec.h"

bool
approx_equal(
    double x,
    double y,
    double epsilon
    )
{
  if ( fabs((x-y)/(x+y) ) < epsilon ) {
    return true;
  }
  else {
    return false;
  }
}

uint32_t
extract_width_SC(
    char * const str_qtype
    )
{
  int status = 0;
  if ( str_qtype == NULL ) { go_BYE(-1); }
  if ( strlen(str_qtype) < 4 ) { go_BYE(-1); } 
  char *cptr = str_qtype + strlen("SC:");
  if ( *cptr == '\0' ) { go_BYE(-1); } 
  for ( char *xptr = cptr; *xptr != '\0'; xptr++ ) { 
    if ( !isdigit(*xptr) ) { go_BYE(-1); }
  }
  int itmp = atoi(cptr); if ( itmp < 2 ) { go_BYE(-1); }
  // need width of 1+1 for nullc
  uint32_t w = (uint32_t)itmp;
BYE:
  if ( status < 0 ) { return 0; } else { return w; }
}
