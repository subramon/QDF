#ifndef UTILS_AUXIL_H
#define UTILS_AUXIL_H
extern void
free_2d_array(
    char ***ptr_X,
    uint32_t n
    );
extern uint64_t
RDTSC(
    void
    );
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
extern pid_t 
get_task_id(
    void
    );
extern int 
get_utime_stime_for_tid(
    pid_t tid,
    uint64_t *ptr_utime,
    uint64_t *ptr_stime
    );
extern int
emptify(
    const char * const file_name
    );
extern int
file_append(
    const char * const file_name,
    const char * const str
    );
extern bool 
is_multiple8(
    uint64_t x
    );
extern uint32_t 
multiple8(
    uint32_t n
    );
extern int
cat_to_buf(
    char **ptr_buf,  // [bufsz]
    uint32_t *ptr_bufsz, 
    uint32_t *ptr_buflen, 
    const char * const str,
    uint32_t str_len
    );
extern bool
approx_equal(
    double x,
    double y,
    double epsilon
    );
uint32_t
extract_width_SC(
    char * const str_qtype
    );
#endif // UTILS_AUXIL_H
