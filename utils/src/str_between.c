#include "incs.h"
#include "str_between.h"
char *
str_between(
    char * in_str,
    const char * const start,
    const char * const stop
    )
{
  int status = 0;
  char *str = NULL;

  if ( in_str == NULL ) { go_BYE(-1); } 
  if ( start  == NULL ) { go_BYE(-1); } 
  if ( stop   == NULL ) { go_BYE(-1); } 
  char *cptr_start = strstr(in_str, start); 
  if ( cptr_start == NULL ) { return NULL; } // start not found
  char *haystack = in_str + (cptr_start - in_str) + strlen(start);

  char *cptr_stop = strstr(haystack, stop); 
  if ( cptr_stop != NULL ) { // stop found 
    uint32_t len = (cptr_stop - cptr_start) + 1; 
    str = malloc(len); memset(str, 0, len);
    strncpy(str, cptr_start, len-1); 
  }
  else { // everything till nullc is what we return 
    uint32_t len = strlen(haystack) + 1;
    str = malloc(len); memset(str, 0, len);
    strncpy(str, haystack, len-1); 
  } 

BYE:
  if ( status < 0 ) { return NULL; } else { return str; }
}
