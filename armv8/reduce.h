#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>
#include "params.h"

#define MONT 2285 // 2^16 mod q
#define QINV 62209 // q^-1 mod 2^16

#define montgomery_reduce KYBER_NAMESPACE(_montgomery_reduce)
//int16_t montgomery_reduce(int32_t a);
void tomont_arm(int16_t* a);

#define barrett_reduce KYBER_NAMESPACE(_barrett_reduce)
//int16_t barrett_reduce(int16_t a);
void barret_red_arm(int16_t* a);

// Combination of add/sub and reduce
void sub_reduce_arm(int16_t* r, const int16_t* a, const int16_t* b);
void add_reduce_arm(int16_t* r, const int16_t* a, const int16_t* b);
void add_add_reduce_arm(int16_t* r, const int16_t* a, const int16_t* b, const int16_t* c);

#endif
