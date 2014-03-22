#ifndef IMPL_H
#define IMPL_H

#include "uberzahl/uberzahl.h"
#include <cassert>
#include <ratio>
#include <chrono>

using namespace std::chrono;
using namespace std;

#define CLASSIC 0
#define MONTGOMERY 1

struct mm_t{
	uberzahl R, Mprime, Rsq, z_init;
	smallType Rbits;
	bool initialized;
	mm_t():initialized(false){}
};

struct crt_t{
	uberzahl p_inverse, q_inverse;
	bool initialized;
	crt_t():initialized(false){}
};

//gcd(2^i,2k+1) = 1
//product of two odd numbers is odd
//(2k+1)(2p+1) = 4kp + 2k + 2p + 1 = 2(2kp + k + p) + 1 = odd

uberzahl modexp(uberzahl base, uberzahl exp, uberzahl n);

//returns next power of 2.
uberzahl next_power(uberzahl n);

uberzahl montgomery_reduction(const uberzahl & T, const uberzahl  & M, const uberzahl  & Mprime, const smallType & Rbits, const uberzahl & R);

//we don't have to bother with making sure that gcd(R,M) == 1 since M is odd.
uberzahl modexp_mm(mm_t & mm, uberzahl base, uberzahl exp, uberzahl M);

uberzahl crt_helper(bool type, mm_t & mm, uberzahl base, uberzahl exp, uberzahl p, uberzahl q, uberzahl q_inverse);

uberzahl modexp_crt(crt_t & crt, uberzahl base,  uberzahl exp, uberzahl p, uberzahl q);

uberzahl modexp_mm_crt(mm_t & mm1, mm_t & mm2, crt_t & crt, uberzahl base, uberzahl exp, uberzahl p, uberzahl q);

//function next_prime is weird..so I made my own prime generator
uberzahl gen_prime_k(mediumType bits, unsigned int accuracy = 50);

//used to exponent and base generation
uberzahl rand_n(mediumType bits);

class timer{
public:
	timer();
	~timer();
	void start();
	void stop();
	void accumulate();
	double get_time();
	void reset();
private:
	bool initialized;
	high_resolution_clock::time_point t1, t2;
	milliseconds total;
};



#endif
