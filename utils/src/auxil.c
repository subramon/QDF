#include "incs.h"
#include "auxil.h"
#include "get_time_usec.h"

void
free_2d_array(
    char ***ptr_X,
    uint32_t n
    )
{
  char **X = *ptr_X;
  if ( X == NULL ) { return; }
  for ( uint32_t i = 0; i < n; i++ ) {
    free_if_non_null(X[i]);
  }
  free_if_non_null(X);
  *ptr_X = NULL;
}
uint64_t
RDTSC(
    void
    )
{
  return get_time_usec();
  /*
  unsigned int lo, hi;
  asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
  */
}
pid_t 
get_task_id(void) 
{
  pid_t tid = syscall(SYS_gettid);
  return tid;
}

// Parse /proc/self/task/tid/stat to find the execution time of the thread
// File format is the same as that of /proc/[pid]/stat
// https://man7.org/linux/man-pages/man5/proc.5.html
int 
get_utime_stime_for_tid(
    pid_t tid,
    uint64_t *ptr_utime,
    uint64_t *ptr_stime
    ) 
{
#define BUFLEN 63
  int status = 0;
  FILE *fp = NULL;
  char fname[63+1]; // hard coded 
  snprintf(fname, BUFLEN, "/proc/self/task/%d/stat", tid);
  fp = fopen(fname, "r");
  return_if_fopen_failed(fp, fname, "r");

  // utime = user CPU clock ticks
  // stime = system CPU clock ticks
  int nr = fscanf(fp, 
      "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %" PRIu64 " %" PRIu64 "\n", 
        ptr_utime, ptr_stime);
  if ( nr != 2 ) { go_BYE(-1); }
BYE:
  fclose_if_non_null(fp);
  return status;
}
    /*
     * Explanation of utime, stime
              (14) utime  %lu
                     Amount of time that this process has been scheduled
                     in user mode, measured in clock ticks (divide by
                     sysconf(_SC_CLK_TCK)).  This includes guest time,
                     guest_time (time spent running a virtual CPU, see
                     below), so that applications that are not aware of
                     the guest time field do not lose that time from
                     their calculations.

              (15) stime  %lu
                     Amount of time that this process has been scheduled
                     in kernel mode, measured in clock ticks (divide by
                     sysconf(_SC_CLK_TCK)).
                     */

int
emptify(
    const char * const file_name
    )
{
  int status = 0;
  FILE *fp = NULL;
  if ( file_name == NULL ) { go_BYE(-1); }
  fp = fopen(file_name, "w");
  return_if_fopen_failed(fp, file_name, "w");
BYE:
  fclose_if_non_null(fp);
  return status;
}

int
file_append(
    const char * const file_name,
    const char * const str
    )
{
  int status = 0;
  FILE *fp = NULL;

  fp = fopen(file_name, "a");
  return_if_fopen_failed(fp, file_name, "a");
  fprintf(fp, "%s", str);
BYE:
  fclose_if_non_null(fp);
  return status;
}
bool 
is_multiple8(
    uint64_t x
    )
{
  if ( x == 0 ) { return false; }
  if ( ( ( x / 8 ) * 8 ) != x ) { return false; } else { return true; }
}

uint32_t 
multiple8(
    uint32_t n
    )
{
  uint32_t m = (n >> 3) << 3;
  if ( m != n ) { m += 8; }
  return m;
}
#define MIN_TO_ADD 64
// concatenates str to buf 
// If buf is not big enough, it will re-allocate space
int
cat_to_buf(
    char **ptr_buf,  // [bufsz]
    uint32_t *ptr_bufsz, 
    uint32_t *ptr_buflen, 
    const char * const str,
    uint32_t str_len
    )
{
  int status = 0;

  char *buf  = *ptr_buf;
  uint32_t bufsz  = *ptr_bufsz; 
  uint32_t buflen = *ptr_buflen; 

  if ( str == NULL ) { go_BYE(-1); } 
  if ( str_len == 0  ) {  str_len  = strlen(str); }
  if ( str_len == 0  ) {  return status; } // nothing to do 

  if ( *str == '\0' ) { go_BYE(-1); }
  if ( buf == NULL ) { 
    if ( bufsz != 0 ) { go_BYE(-1); } 
    if ( buflen != 0 ) { go_BYE(-1); } 
    buf = malloc(MIN_TO_ADD);
    return_if_malloc_failed(buf);
    memset(buf, 0, MIN_TO_ADD);
    bufsz = MIN_TO_ADD;
  }
  if ( str_len + buflen + 1 >= bufsz ) { // +1 for nullc
    bufsz += mcr_max(MIN_TO_ADD, ((str_len + buflen+1) - bufsz));
    bufsz = multiple8(bufsz);
    char *newbuf = malloc(bufsz);
    return_if_malloc_failed(newbuf);
    memset(newbuf, 0, bufsz); 
    if ( buf != NULL ) { 
      memcpy(newbuf, buf, buflen); 
    }
    free_if_non_null(buf); 
    buf = newbuf;
  }
  memcpy(buf + buflen, str, str_len);
  buflen += str_len;
  if ( buf[buflen] != '\0' ) { go_BYE(-1); } 
  //---------------------------------------
  *ptr_buf    = buf;
  *ptr_bufsz  = bufsz;
  *ptr_buflen = buflen;
BYE:
  return status;
}

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
