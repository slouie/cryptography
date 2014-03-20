#include <iostream>
#include <cstdlib>
#include <ctime>
#include "impl.h"

using namespace std;



int main(){

	//srand(time(NULL));

	timer t;
	uberzahl p = gen_prime_k(64), q = gen_prime_k(64);
	uberzahl base = gen_prime_k(15), exp = rand_n(1000);
	uberzahl n = p*q;


	mm_t mm, mm_crt1, mm_crt2;
	crt_t crt, crt_mm;

	int num_trials = 20;

	t.start();
	for(int i = 0; i < num_trials;++i)
		modexp(base,exp,n);
	t.stop();
	cout << t.get_time()/double(num_trials) << endl;
	t.start();
	for(int i = 0; i < num_trials;++i)
		modexp_crt(crt,base,exp,p,q);
	t.stop();
	cout << t.get_time()/double(num_trials) << endl;
	t.start();
	for(int i = 0; i < num_trials;++i)
		modexp_mm(mm,base,exp,n);
	t.stop();
	cout << t.get_time()/double(num_trials) << endl;
	t.start();
	for(int i =0; i < num_trials;++i)
		modexp_mm_crt(mm_crt1, mm_crt2, crt_mm,base,exp,p,q);
	t.stop();
	cout << t.get_time()/double(num_trials) << endl;


	return 0;
}

//some legacy code below
/*
int main(){

	//sage: n=881*883
	//sage: N=IntegerModRing(n)
	//sage: N(13^1023)
	//401718
	//sage: N(13^1024)
	//554796
	//sage: N(13^1025)
	//211041

	uberzahl n(881*883);
	uberzahl base(13);
	uberzahl e1(1023), e2(1024), e3(1025);

	cout << modexp_mm(base,e1,n) << endl;
	cout << modexp_mm(base,e2,n) << endl;
	cout << modexp_mm(base,e3,n) << endl;

	uberzahl m(11413);
	uberzahl b(9726);
	uberzahl e4(3533);

	cout << modexp_mm(b, e4, m) << endl;

 	cout << modexp(base,e1,n) << endl;
	cout << modexp(base,e2,n) << endl;
	cout << modexp(base,e3,n) << endl;
 	cout << modexp(b, e4, m) << endl;

 	uberzahl p(881), q(883);

 	cout << modexp_crt(base,e1,p,q) << endl;
	cout << modexp_crt(base,e2,p,q) << endl;
	cout << modexp_crt(base,e3,p,q) << endl;

	cout << modexp_mm_crt(base,e1,p,q) << endl;
	cout << modexp_mm_crt(base,e2,p,q) << endl;
	cout << modexp_mm_crt(base,e3,p,q) << endl;

	//srand(time(NULL));
	//cout << gen_prime_k(50) << endl;

	return 0;
}*/