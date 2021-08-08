#include <stdint.h>
#include "params.h"
#include "cbd.h"

void poly_cbd_eta1(poly *r, const uint8_t buf[KYBER_ETA1*KYBER_N/4])
{
#if KYBER_ETA1 == 2
  cbd2_arm(r->coeffs, buf);
#elif KYBER_ETA1 == 3
  cbd3_arm(r->coeffs, buf);
#else
#error "This implementation requires eta1 in {2,3}"
#endif
}

void poly_cbd_eta2(poly *r, const uint8_t buf[KYBER_ETA2*KYBER_N/4])
{
#if KYBER_ETA2 == 2
  cbd2_arm(r->coeffs, buf);
#else
#error "This implementation requires eta2 = 2"
#endif
}
