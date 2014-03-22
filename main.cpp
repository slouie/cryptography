#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "impl.h"

using namespace std;

int main(int argc, char *argv[]){
	ostringstream out;
	int num_bits = 128;
	if(argc > 1){
		num_bits = atoi(argv[1]);
		if(num_bits < 0 || num_bits % 2)
			num_bits = 128;
	}

	srand(time(NULL));

	timer t;
	uberzahl p = gen_prime_k(num_bits/2), q = gen_prime_k(num_bits/2);
	uberzahl base, exp;
	uberzahl n = p*q;

	mm_t mm, mm_crt1, mm_crt2;
	crt_t crt, crt_mm;

	int num_trials = 30;
	timer t1, t2, t3, t4;
	
	for(int i = 0; i < num_trials;++i){
		base = rand_n(num_bits-1), exp = rand_n(num_bits-1);
		t1.start();
		modexp(base,exp,n);
		t1.stop(); t1.accumulate();

		t2.start();
		modexp_crt(crt,base,exp,p,q);
		t2.stop(); t2.accumulate();

		t3.start();
		modexp_mm(mm,base,exp,n);
		t3.stop(); t3.accumulate();

		t4.start();
		modexp_mm_crt(mm_crt1,mm_crt2, crt_mm, base, exp, p, q);
		t4.stop(); t4.accumulate();
	}
	out << "average run times over " << num_trials << " trials with modulus of " << num_bits << "-bits\n";
	int old_precision = cout.precision();
	out.precision(10);
	out << "modexp: " << t1.get_time()/double(num_trials) << '\n';
	out << "modexp_crt: " << t2.get_time()/double(num_trials) << '\n';
	out << "modexp_mm: " << t3.get_time()/double(num_trials) << '\n';
	out << "modexp_mm_crt : " <<t4.get_time()/double(num_trials) << '\n';
	out.precision(old_precision);

	cout << out.str();

	return 0;
}
