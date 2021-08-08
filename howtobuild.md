
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
