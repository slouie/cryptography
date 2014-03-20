#ifndef IMPL_H
#define IMPL_H

#include "uberzahl/uberzahl.h"
#include <cassert>

using namespace std;

#define CLASSIC 0
#define MONTGOMERY 1

uberzahl modexp(uberzahl base, uberzahl exp, uberzahl n);

//returns next power of 2.
uberzahl next_power(uberzahl n);

uberzahl montgomery_reduction(const uberzahl & T, const uberzahl & M, const uberzahl & Mprime, const smallType & Rbits, const uberzahl & R);

//we don't have to bother with making sure that gcd(R,M) == 1 since M is odd.
uberzahl modexp_mm(uberzahl base, uberzahl exp, uberzahl M);

uberzahl crt_helper(bool type, uberzahl base, uberzahl exp, uberzahl p, uberzahl q);

uberzahl modexp_crt(uberzahl base, uberzahl exp, uberzahl p, uberzahl q);

uberzahl modexp_mm_crt(uberzahl base, uberzahl exp, uberzahl p, uberzahl q);

#endif