#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "kem.h"
#include "kex.h"
#include "params.h"
#include "indcpa.h"
#include "polyvec.h"
#include "poly.h"
#include "cbd.h"
#include "ntt.h"
#include "verify.h"
#include "symmetric.h"
#include "randombytes.h"
#include "cpucycles.h"
#include "speed_print.h"

#define NTESTS 1000
#define KYBER_SYMBYTES 32

uint64_t t[NTESTS];
uint8_t seed[KYBER_SYMBYTES] = {0};

int main()
{
  unsigned int i;
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key[CRYPTO_BYTES];
  polyvec matrix[KYBER_K];
  poly ap;
  
  uint64_t cycles1, cycles2;

	//init();
	
	/* overhead */
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    __asm__ volatile ("");
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("overhead: ", t, NTESTS);





  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    crypto_kem_keypair(pk, sk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("crypto_kem_keypair: ", t, NTESTS);
	
	for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    crypto_kem_enc(ct, key, pk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("crypto_kem_enc: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    crypto_kem_dec(key, ct, sk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("crypto_kem_dec: ", t, NTESTS);
  
  
  
  

	/* GEN MATRIX */
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    gen_matrix(matrix, seed, 0);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("gen_a: ", t, NTESTS);


  /* SAMPLING */
  uint8_t buf1[KYBER_ETA1*KYBER_N/4];
  uint8_t buf2[KYBER_ETA2*KYBER_N/4];
  uint8_t buf1a[2*KYBER_N/4];
  uint8_t buf2a[3*KYBER_N/4];
  
  poly r;

  prf(buf1, sizeof(buf1), seed, 0);
  prf(buf2, sizeof(buf2), seed, 0);
  prf(buf1a, sizeof(buf1a), seed, 0);
  prf(buf2a, sizeof(buf2a), seed, 0);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    cbd2_arm((&r)->coeffs, buf1a);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("cbd2_arm: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    cbd3_arm((&r)->coeffs, buf2a);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("cbd3_arm: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_cbd_eta1(&r, buf1);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_cbd_eta1: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_cbd_eta2(&r, buf2);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_cbd_eta2: ", t, NTESTS);

	for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_getnoise_eta1(&ap, seed, 0);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_getnoise_eta1: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_getnoise_eta2(&ap, seed, 0);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_getnoise_eta2: ", t, NTESTS);
  
  
  
  /* NTT ops*/
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    ntt_arm((&r)->coeffs, zetas);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("ntt: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_ntt(&ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_ntt: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    inv_ntt_arm((&r)->coeffs, zetas_inv);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("invntt: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_invntt_tomont(&ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_invntt_tomont: ", t, NTESTS);



	/* Arithmetic ops */
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_basemul_montgomery(&ap, &r, &r);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_basemul_montgomery: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    polyvec_basemul_acc_montgomery(&ap, &matrix[0], &matrix[1]);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("polyvec_basemul_acc_montgomery: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_add(&ap, &r, &ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_add: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_sub(&ap, &r, &ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_sub: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_reduce(&ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_reduce: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_tomont(&ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_tomont: ", t, NTESTS);
  
  
  
  /* Conversion */
  uint8_t a[KYBER_POLYBYTES];
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_compress(ct,&ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_compress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    polyvec_compress(ct,&matrix[0]);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("polyvec_compress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_decompress(&ap,ct);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_decompress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    polyvec_decompress(&matrix[0],ct);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("polyvec_decompress: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_tobytes(a, &r);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_tobytes: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_frombytes(&r, a);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_frombytes: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_tomsg(ct,&ap);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_tomsg: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    poly_frommsg(&ap,ct);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("poly_frommsg: ", t, NTESTS);
  
  
  
  /* Other & Hashing */
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    hash_h(sk+KYBER_SECRETKEYBYTES-2*KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("hash_h_(pkBytes): ", t, NTESTS);

	uint8_t bufh[2*KYBER_SYMBYTES];
  uint8_t krh[2*KYBER_SYMBYTES];
  randombytes(bufh, KYBER_SYMBYTES);  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    hash_h(bufh, bufh, KYBER_SYMBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("hash_h_(32Bytes): ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    hash_h(krh+KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("hash_h_(ctBytes): ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    hash_g(bufh, bufh, KYBER_SYMBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("hash_g_(32Bytes): ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    hash_g(krh, bufh, 2*KYBER_SYMBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("hash_g_(64Bytes): ", t, NTESTS);

	uint8_t ss[KYBER_SSBYTES];
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    kdf(ss, krh, 2*KYBER_SYMBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("kdf_(64Bytes): ", t, NTESTS);

	uint8_t bufp1[KYBER_ETA1*KYBER_N/4];
  uint8_t bufp2[KYBER_ETA2*KYBER_N/4];
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    prf(bufp1, sizeof(bufp1), seed, 0);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("prf_(eta1): ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    prf(bufp2, sizeof(bufp2), seed, 0);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("prf_(eta2): ", t, NTESTS);

	int fail;  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    fail = verify(ct, ct, KYBER_CIPHERTEXTBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("verify: ", t, NTESTS);
  
	for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    cmov(krh, sk+KYBER_SECRETKEYBYTES-KYBER_SYMBYTES, KYBER_SYMBYTES, fail);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("cmov: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
   	randombytes(bufh, KYBER_SYMBYTES);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("randombytes: ", t, NTESTS);


	/* Main functions */
	for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    indcpa_keypair(pk, sk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("indcpa_keypair: ", t, NTESTS);
	
	for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    indcpa_enc(ct, key, pk, seed);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("indcpa_enc: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    indcpa_dec(key, ct, sk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("indcpa_dec: ", t, NTESTS);
  
   /* GEN MATRIX */ 
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    gen_matrix(matrix, seed, 0);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("gen_a: ", t, NTESTS);
  
  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    crypto_kem_keypair(pk, sk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("crypto_kem_keypair: ", t, NTESTS);
	
	for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    crypto_kem_enc(ct, key, pk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("crypto_kem_enc: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    cycles1 = cpucycles();
    crypto_kem_dec(key, ct, sk);
    cycles2 = cpucycles();
    t[i] = cycles2-cycles1;
  }
  print_results("crypto_kem_dec: ", t, NTESTS);
  
  return 0;
}
