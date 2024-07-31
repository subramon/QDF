#include "incs.h"
#include "qtypes.h"
#include "tm2time.h"
#include "date_str_to_epoch.h"

extern char *strptime(const char *s, const char *format, struct tm *tm);
int
date_str_to_epoch(
    const char * const date_str,
    const char * const format,
    uint32_t *ptr_t_epoch
    )
{
  int status = 0;

  if ( date_str == NULL ) { go_BYE(-1); }
  if ( format == NULL ) { go_BYE(-1); }
  struct tm l_tm; 
  memset(&l_tm, 0, sizeof(struct tm));
  char *rslt = strptime(date_str, format, &l_tm);
  if ( rslt == NULL ) { go_BYE(-1); }
  *ptr_t_epoch = tm2time(&l_tm); 
BYE:
  return status;
}
