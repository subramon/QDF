#include "incs.h"
#include "auxil.h"
#include "get_time_usec.h"
#include "q_helpers.h"

int 
find_a_spot_write(
    struct drand48_data *ptr_rbuf,
    uint32_t qsz,
    int *Q,
    int expected,
    int desired,
    uint64_t *ptr_t_sleep
    )
{
  int qidx = -1;

  *ptr_t_sleep = 0; 
  for ( bool found_a_spot = false; !found_a_spot; ) {
    // start looking from spot j
    // int j; lrand48_r(ptr_rbuf, &j); j = j % qsz; 
    uint32_t j = (uint32_t)(RDTSC() % qsz); // TODO P1 
    for ( uint32_t k = 0; ((k < qsz) && (!found_a_spot)); k++,j++) {
      int l_expected = expected; // TODO P3 Why is this needed?
      int l_desired  = desired;
      // look at every spot but break as soon as you find an empty spot
      if ( j == qsz ) { j = 0; } // wrap around scanning
      bool rslt = __atomic_compare_exchange(
          Q+j, &l_expected, &l_desired, false, 
          __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
      // if spot is busy, then skip to next spot
      if ( !rslt ) { continue; }
      // control here => you have found a free spot 
      qidx = (int)j; found_a_spot = true; break;
    }
    if ( found_a_spot ) { break; }
    // Control comes here => you looked everywhere and drew a blank
    // so take a short nap
    // wait a bit for an empty spot 
    uint64_t t1 = get_time_usec();
    struct timespec  tmspec = { .tv_sec = 0, .tv_nsec = 10 * 1000000 };
    nanosleep(&tmspec, NULL);
    uint64_t t2 = get_time_usec();
    *ptr_t_sleep += (t2-t1);
  }
  return qidx;
}
int 
find_a_spot_read(
    struct drand48_data *ptr_rbuf,
    uint32_t qsz,
    int *Q,
    int expected,
    int desired,
    uint64_t *ptr_t_sleep
    )
{
  int qidx = -1;
  *ptr_t_sleep = 0;

  // start looking from spot j
  // int j; lrand48_r(ptr_rbuf, &j); j = j % qsz; 
  uint32_t j = (uint32_t)(RDTSC() % qsz); // TODO P1 
  for ( uint32_t k = 0; k < qsz; k++,j++) {
    int l_expected = expected; // TODO P3 Why is this needed?
    int l_desired  = desired;
    // look at every spot but break as soon as you find an empty spot
    if ( j == qsz ) { j = 0; } // wrap around scanning
    bool rslt = __atomic_compare_exchange(
        Q+j, &l_expected, &l_desired, false, 
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    // if spot is busy, then skip to next spot
    if ( !rslt ) { continue; }
    // control here => you have found a free spot 
    qidx = (int)j; break;
  }
  if ( qidx < 0 ) { 
    uint64_t t1 = get_time_usec();
    struct timespec  tmspec = { .tv_sec = 0, .tv_nsec = 10 * 1000000 };
    nanosleep(&tmspec, NULL);
    uint64_t t2 = get_time_usec();
    *ptr_t_sleep += (t2-t1);
  }
  return qidx;
}
// NOTE: When the exchange failed, I found that expected was getting
// set to 0. This led to wrong behavior. Making copies seemed to work
// but I have no idea why
