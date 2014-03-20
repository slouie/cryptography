#include "impl.h"

uberzahl modexp(uberzahl base, uberzahl exp, uberzahl n){
	mediumType i = exp.bitLength() - 1;

	uberzahl z((largeType)1);

	while(i >= 0){
		z = (z * z ) % n;
		if(exp.bit(i) == 1)
			z = (z * base) % n;
		if(i == 0)
			break;
		i -= 1;
	}
	
	return z;
}


//returns next power of 2.
uberzahl next_power(uberzahl n){
	return 1 << (n-1).bitLength();
}

uberzahl montgomery_reduction(const uberzahl & T, const uberzahl & M, const uberzahl & Mprime, const smallType & Rbits, const uberzahl & R){
	uberzahl m, t;
	m = (T * Mprime) & (R - 1); //bitwise AND ftw.
	t = (T + m*M) >> (Rbits-1); //forget divisions, use bit-shift since it's power of 2

	if(t >= M)
		t = t -  M;
	return t;
}

//we don't have to bother with making sure that gcd(R,M) == 1 since M is odd.
uberzahl modexp_mm(uberzahl base, uberzahl exp, uberzahl M){
	uberzahl R, Mprime;
	smallType Rbits;

	R = next_power(M);
	Rbits = R.bitLength();
	Mprime = (R-M.inverse(R));

	uberzahl z;

	mediumType i = exp.bitLength() - 1;

	//convert into Montgomery space
	z = R % M;
	base = base * R % M;

	while(i >= 0) {
		z = montgomery_reduction(z * z, M, Mprime, Rbits, R);
		if(exp.bit(i) == 1)
			z = montgomery_reduction(z * base, M, Mprime, Rbits, R);
		if(i == 0)
			break;
		i -= 1;
	}
	return montgomery_reduction(z, M, Mprime, Rbits, R);
}

uberzahl crt_helper(bool type, uberzahl base, uberzahl exp, uberzahl p, uberzahl q){
	if(type == CLASSIC)
		return modexp(base, exp % (p-1), p) * q * q.inverse(p);
	else if(type == MONTGOMERY)
		return modexp_mm(base, exp % (p-1),p) * q * q.inverse(p);

	//Only types allowed are CLASSIC and MONTGOMERY!
	assert(0);
}

uberzahl modexp_crt(uberzahl base, uberzahl exp, uberzahl p, uberzahl q){
	return ( crt_helper(CLASSIC,base,exp,p,q) + crt_helper(CLASSIC, base, exp, q, p) ) % (p * q);
}

uberzahl modexp_mm_crt(uberzahl base, uberzahl exp, uberzahl p, uberzahl q){
	return ( crt_helper(MONTGOMERY,base,exp,p,q) + crt_helper(MONTGOMERY, base, exp, q, p ) ) % (p * q);
}
