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
	uberzahl retval(1);
	return retval << (n-1).bitLength();
}

uberzahl montgomery_reduction(const uberzahl & T, const uberzahl  & M, const uberzahl  & Mprime, const smallType & Rbits, const uberzahl & R){
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

	uberzahl z("1");
	uberzahl t("2");
	uberzahl Rsq = modexp(R,t,M);

	//convert into Montgomery space

	z = R % M;
	//assert(base * Rsq < M*R);

	//According to Piazza post we don't even need to calculate the residues with mod
	base = montgomery_reduction(base * Rsq, M, Mprime, Rbits, R);
	//base = base * R % M;

	mediumType i = exp.bitLength() - 1;


	while(i >= 0) {

		z = montgomery_reduction(z * z, M, Mprime, Rbits, R);
		if(exp.bit(i) == 1){
			z = montgomery_reduction(z * base , M, Mprime, Rbits, R);
		}
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

uberzahl gen_prime_k(mediumType bits, unsigned int accuracy){
	uberzahl retval;
	do{
		retval.random(bits);
	}while(!rabinmiller(retval,accuracy));
	
	return retval;
}

uberzahl rand_n(mediumType bits){
	uberzahl retval;
	retval.random(bits);
	return retval;
}

timer::timer(){
	initialized = false;
}

timer::~timer(){
}

void timer::start(){
	initialized = true;
	t1  = high_resolution_clock::now();
}

void timer::stop(){
	t2 = high_resolution_clock::now();
}

void timer::reset(){
	initialized = false;
}

double timer::get_time(){
	if(!initialized)
		return 0;
	return (duration_cast<duration< double > > (t2 - t1)).count();
}
