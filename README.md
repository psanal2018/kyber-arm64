# Kyber ARM64

This repository contains the ARMv8 implementation of [Kyber](https://www.pq-crystals.org/kyber/). Kyber is a key encapsulation mechanism (KEM) and it is a finalist in [round 3](https://csrc.nist.gov/Projects/post-quantum-cryptography/round-3-submissions) of the [NIST PQC](https://csrc.nist.gov/projects/post-quantum-cryptography) standardization project.

The paper of this implementation ("Kyber on ARM64: Compact Implementations of Kyber on 64-bit ARM Cortex-A Processors") will be published in [`International Workshop on Post-quantum Cryptography for Secure Communications (PQC-SC) workshop 2021`](https://securecomm.eai-conferences.org/2021/pqc-sc/).

## Benchmarking Results

Benchmark results were measured both ARM and Apple chips. The ARM board is on Google Pixel 3 Android smartphone. The processor (Snapdragon 845) on it has 8 cores including 4 of ARM Cortex-A53 (@1.77~GHz) and 4 of ARM Cortex-A75 (@2.8~GHz) based. Performance results are taken by using Cortex-A75 processor. The executable is aarch64 cross-compiled on Linux operating system (Ubuntu 20.04) with gcc-9. The Apple board is on iPad mini 5-th generation. The processor (A12 Bionic) on it has 6 cores including 2 of Vortex (@2.49~GHz) and 4 of Tempest (@1.54~GHz) based. Performance results are taken by using Vortex processor on Apple operating system (iPadOS 14.3). The reference C code is originally obtained from [Kyber GitHub Repository](https://github.com/pq-crystals/kyber).

Comparison of clock cycles for functions of Kyber schemes on 64-bit ARM Cortex-A75@2.8~GHz. (Ref-C: Reference C implementation. Opt: Our optimized implementation.)

| Functions                    | Ref Timing [cc] | Opt Timing [cc] | Ref/Opt |
|:-----------------------------|:---------------:|:---------------:|:-------:|
| **Reduction**                |                 |                 |         |
| poly_tomont (Montgomery Red) | 1,896           | 437             | 4.34    |
| poly_reduce (Barrett Red)    | 2,187           | 294             | 7.44    |
| **NTT**                      |                 |                 |         |
| poly_ntt (NTT+Barrett Red)   | 11,228          | 1,750           | 6.42    |
| poly\_invntt_tomont (InvNTT)  | 17,500          | 2,624           | 6.67    |
| poly\_basemul_montgomery      | 5,396           | 1,168           | 4.62    |

Comparison of clock cycles for Kyber schemes. (Ref-C: Reference C implementation. Opt: Our optimized implementation.)
|               | | | ARM Cortex-A75 @2.8~GHz | | | Apple A12 @2.49~GHz | |
|:--------------|:-------:|:-----:|:-----:|:----:|:-----:|:----:|:-----:|
|               |         | **Ref-C [kc]** | **Opt [kc]** | **Ref-C / Opt** | **Ref-C [kc]** | **Opt [kc]** | **Ref-C / Opt** |
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
