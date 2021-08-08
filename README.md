# Kyber ARMv8

This repository contains the ARMv8 implementation of the [Kyber](https://www.pq-crystals.org/kyber/) key encapsulation mechanism. Kyber is a finalist in [round 3](https://csrc.nist.gov/Projects/post-quantum-cryptography/round-3-submissions) of the [NIST PQC](https://csrc.nist.gov/projects/post-quantum-cryptography) standardization project.

"Kyber on ARM64: Compact Implementations of Kyber on 64-bit ARM Cortex-A Processors". The preprint version is available [`here`](https://eprint.iacr.org/2021/561.pdf). 

## Build instructions

The implementations contain several test and benchmarking programs and a Makefile to facilitate compilation.

### Prerequisites

Some of the test programs require [OpenSSL](https://openssl.org). If the OpenSSL header files and/or shared libraries do not lie in one of the standard locations on your system, it is necessary to specify their location via compiler and linker flags in the environment variables `CFLAGS`, `NISTFLAGS`, and `LDFLAGS`.

For example, on macOS you can install OpenSSL via [Homebrew](https://brew.sh) by running
```sh
brew install openssl
```
Then, run
```sh
export CFLAGS="-I/usr/local/opt/openssl@1.1/include"
export NISTFLAGS="-I/usr/local/opt/openssl@1.1/include"
export LDFLAGS="-L/usr/local/opt/openssl@1.1/lib"
```
before compilation to add the OpenSSL header and library locations to the respective search paths.

### Cross Compilation for ARMv8 on Linux
ARMv8 executables can be generated using cross-compilation on Linux. There are different methods for cross-compilation. An easy approach is to install `gcc-aarch64-linux-gnu` package by executing:
```sh
$  sudo apt-get install gcc-aarch64-linux-gnu
```
After installation, simply use the following command to generate the ARMv8 executables:
```sh
$ make
```
Now, the generated binaries can be run on ARMv8-A cores. 

### Test programs

To compile the test programs, run
```sh
make
```
This produces the executables
```sh
test_kyber$ALG
test_kex$ALG
test_vectors$ALG
```
where `$ALG` ranges over the parameter sets 512, 768, 1024, 512-90s, 768-90s, 1024-90s.

* `test_kyber$ALG` tests 1000 times to generate keys, encapsulate a random key and correctly decapsulate it again. Also, the program tests that the keys cannot correctly be decapsulated using a random secret key or a ciphertext where a single random byte was randomly distorted in order to test for trivial failures of the CCA security. The program will abort with an error message and return 1 if there was an error. Otherwise it will output the key and ciphertext sizes and return 0.
* `test_kex$ALG` tests the authenticated key exchange schemes derived from the Kyber KEM
* `test_vectors$ALG` generates 10000 sets of test vectors containing keys, ciphertexts and shared secrets whose byte-strings are output in hexadecimal. The required random bytes come from a simple deterministic expansion of a fixed seed defined in `test_vectors.c`.

### Benchmarking programs

For benchmarking the implementations, run
```sh
make speed
```
This produces the executables
```sh
test_speed$ALG
```
for all parameter sets `$ALG` as above. The programs report the median and average timing results (in nanoseconds) of 1000 executions of various internal functions and the API functions for key generation, encapsulation and decapsulation.

### KAT tests

To run the KEM implementations against the KATs, execute:

```sh
$ make PQCgenKAT_kem$ALG(_90s)
```

## Shared libraries

All implementations can be compiled into shared libraries by running
```sh
make shared
```
For example in the directory `armv8/` of the reference implementation, this produces the libraries
```sh
libpqcrystals_kyber$ALG_armv8.so
```
for all parameter sets `$ALG`, and the required symmetric crypto libraries
```
libpqcrystals_aes256ctr_armv8.so
libpqcrystals_fips202_armv8.so
```
All global symbols in the libraries lie in the namespaces `pqcrystals_kyber$ALG_armv8`, `libpqcrystals_aes256ctr_armv8` and `libpqcrystals_fips202_armv8`. Hence it is possible to link a program against all libraries simultaneously and obtain access to all implementations for all parameter sets. The corresponding API header file is `armv8/api.h`, which contains prototypes for all API functions and preprocessor defines for the key and signature lengths.

## Benchmarking Results

| Functions                    | Ref Timing [cc] | Opt Timing [cc] | Ref/Opt |
|:-----------------------------|:---------------:|:---------------:|:-------:|
| **Reduction**                |                 |                 |         |
| poly_tomont (Montgomery Red) | 1,896           | 437             | 4.34    |
| poly_reduce (Barrett Red)    | 2,187           | 294             | 7.44    |
| **NTT**                      |                 |                 |         |
| poly_ntt (NTT+Barrett Red)   | 11,228          | 1,750           | 6.42    |
| poly_invntt_tomont (InvNTT)  | 17,500          | 2,624           | 6.67    |
| poly_basemul_montgomery      | 5,396           | 1,168           | 4.62    |


|               | | | ARM Cortex-A75 @2.8~GHz | | | Apple A12 @2.49~GHz | |
|:--------------|:-------:|:-----:|:-----:|:----:|:-----:|:----:|:-----:|
|               |         | Ref-C | Opt | Ref-C / Opt | Ref-C | Opt | Ref-C / Opt |
| Kyber512      |  K      | 145.8 | 81.7  | 1.79 | 60.4  | 34.9 | 1.78  |
| Kyber512      |  E      | 205.2 | 104.9 | 1.96 | 77.7  | 37.7 | 2.06  |
| Kyber512      |  D      | 248.5 | 101.9 | 2.44 | 94.6  | 37.2 | 2.53  |
|               |         |       |       |      |       |      |       |
| Kyber768      |  K      | 247.5 | 138   | 1.79 | 106   | 62.2 | 1.7   |
| Kyber768      |  E      | 327.8 | 173.4 | 1.89 | 131.9 | 60.8 | 2.16  |
| Kyber768      |  D      | 383   | 168.6 | 2.27 | 146.7 | 60   | 2.44  |
|               |         |       |       |      |       |      |       |
| Kyber1024     |  K      | 385.1 | 222.7 | 1.73 | 171.2 | 95.2 | 1.79  |
| Kyber1024     |  E      | 476.7 | 262.8 | 1.81 | 182.2 | 93   | 1.95  |
| Kyber1024     |  D      | 546   | 257.7 | 2.12 | 209.1 | 91   | 2.29  |
|               |         |       |       |      |       |      |       |
| Kyber512-90s  |  K      | 270.5 | 205.6 | 1.32 | 279.7 | 32.6 | 8.57  |
| Kyber512-90s  |  E      | 334.5 | 236.7 | 1.41 | 292.7 | 42.1 | 6.94  |
| Kyber512-90s  |  D      | 375.1 | 230.7 | 1.63 | 305.5 | 37   | 8.26  |
|               |         |       |       |      |       |      |       |
| Kyber768-90s  |  K      | 491.7 | 379.2 | 1.3  | 554.2 | 56.4 | 9.82  |
| Kyber768-90s  |  E      | 581.4 | 426.1 | 1.36 | 576   | 64.5 | 8.92  |
| Kyber768-90s  |  D      | 632.9 | 417.1 | 1.52 | 590.7 | 57   | 10.35 |
|               |         |       |       |      |       |      |       |
| Kyber1024-90s |  K      | 790.3 | 625.8 | 1.26 | 941.9 | 87.1 | 10.8  |
| Kyber1024-90s |  E      | 897.3 | 680.5 | 1.32 | 964.8 | 93.8 | 10.28 |
| Kyber1024-90s |  D      | 959.6 | 669.4 | 1.43 | 983   | 83.5 | 11.76 |

## Contributors
* Pakize Sanal, Florida Atlantic University, Boca Raton, USA: (psanal2018@fau.edu)
* Emrah Karagoz, Florida Atlantic University, Boca Raton, USA: (ekaragoz2017@fau.edu)
* Reza Azarderakhsh, Florida Atlantic University, Boca Raton, USA: (razarderakhsh@fau.edu)
* Hwajeong Seo, Hansung University, Seoul, South Korea: (hwajeong84@gmail.com)
* Mehran Mozaffari-Kermani, University of South Florida, Tampa, USA: (mehran2@usf.edu)
