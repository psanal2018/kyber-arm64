#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

#define zetas KYBER_NAMESPACE(_zetas)
extern const int16_t zetas[128];
extern const int16_t zetas_inv[128];

// zetas for ntt_arm
extern const int16_t zetas_arm[287];
extern const int16_t zetas_inv_arm[287];

#define ntt KYBER_NAMESPACE(_ntt)
//void ntt(int16_t poly[256]);
void ntt_arm(int16_t *poly, const int16_t* zetas);

#define invntt KYBER_NAMESPACE(_invntt)
//void invntt(int16_t poly[256]);
void inv_ntt_arm(int16_t *poly, const int16_t* zetas_inv);

#define basemul KYBER_NAMESPACE(_basemul)
//void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);
void basemul_arm(int16_t *poly, const int16_t *a, const int16_t *b, const int16_t* zetas);

#endif
