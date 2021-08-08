#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "cpucycles.h"
#include "speed_print.h"

static int cmp_uint64(const void *a, const void *b) {
  if(*(uint64_t *)a < *(uint64_t *)b) return -1;
  if(*(uint64_t *)a > *(uint64_t *)b) return 1;
  return 0;
}

static uint64_t median(uint64_t *l, size_t llen) {
  //qsort(l,llen,sizeof(uint64_t),cmp_uint64);

  if(llen%2) return l[llen/2];
  else return (l[llen/2-1]+l[llen/2])/2;
}

static uint64_t average(uint64_t *t, size_t tlen) {
  size_t i;
  uint64_t acc=0;

  for(i=0;i<tlen;i++)
    acc += t[i];

  return acc/tlen;
}

static void best3(uint64_t *t, size_t tlen, uint64_t best[3], size_t occ[3]) {
  size_t i;
    
  for(i=0;i<3;i++){ best[i] = 0; occ[i] = 0; }
  
  uint64_t tmp;
  size_t ctr;
  
  i = 0;
  while(i < tlen){
    tmp = t[i];
    ctr = 0;
    
		while((t[i] == tmp) && (i < tlen)){ ctr++; i++; }
		
		if(ctr > occ[0]){
				best[2] = best[1]; occ[2] = occ[1];
				best[1] = best[0]; occ[1] = occ[0];
				best[0] = tmp; occ[0] = ctr;				
		}
		else if (ctr > occ[1]){
				best[2] = best[1]; occ[2] = occ[1];
				best[1] = tmp; occ[1] = ctr;		
		}
		else if (ctr > occ[2]){
				best[2] = tmp; occ[2] = ctr;		
		}		
 }
}


void print_results(const char *s, uint64_t *t, size_t tlen) {
  
  qsort(t,tlen,sizeof(uint64_t),cmp_uint64);
  uint64_t med = median(t, tlen);
  uint64_t ave = average(t, tlen);
  uint64_t best[3];
  size_t occ[3];
  best3(t, tlen, best, occ);
  

  printf("%s %llu %llu \n", s, (unsigned long long)med, (unsigned long long)ave);
	
}
