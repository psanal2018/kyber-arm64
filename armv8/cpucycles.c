#include <stdint.h>
#include "cpucycles.h"

uint64_t cpucycles(void) {
  struct timespec time;

  //clock_gettime(CLOCK_REALTIME, &time);
  clock_gettime(CLOCK_MONOTONIC, &time);
      
  return (uint64_t)(time.tv_sec*1e9 + time.tv_nsec);
}

