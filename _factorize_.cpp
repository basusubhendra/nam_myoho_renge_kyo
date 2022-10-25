#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <iostream>
#include <gmp.h>
#include <pthread.h>
#include "pi.hpp"
#include "e.hpp"
/*
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
*/
using namespace std;
using namespace boost;
unsigned long long int offset = 0;
char* num = 0;
char* rnum = 0;

char* strrev(char* x) {
	char* y = strdup(x);
	unsigned long l = strlen(y);
	for (int i = 0; i < floor(l/2.0); ++i) {
		char t = y[i];
		y[i] = y[l-1-i];
		y[l-1-i] = t;
	}
	return y;
}

char* productOf(char* x, char* y) {
	mpz_t xz;
	mpz_init(xz);
	mpz_t yz;
	mpz_init(yz);
	mpz_t product;
	mpz_init(product);
	mpz_set_str(xz, x, 10);
	mpz_set_str(yz, y, 10);
	mpz_mul(product, xz, yz);
	char* _product_ = strdup(mpz_get_str(0, 10, product));
	mpz_clear(xz);
	mpz_clear(yz);
	mpz_clear(product);
	return _product_;
}

int _equals_(char* x, char* y) {
	mpz_t xz;
	mpz_init(xz);
	mpz_set_str(xz, x, 10);
	mpz_t yz;
	mpz_init(yz);
	mpz_set_str(yz, y, 10);
	int result = mpz_cmp(yz, xz);
	mpz_clear(xz);
	mpz_clear(yz);
	return result;
}

char* _int_(std::string x) {
	unsigned long int l = x.size();
	mpz_t sum;
	mpz_init(sum);
	mpz_set_si(sum, 0);
	mpz_t prod;
	mpz_init(prod);
	mpz_set_ui(prod, 1);
	mpz_t term;
	mpz_init(term);
	for (int i = l - 1; i >= 0; --i) {
		int sk = x[i] - '0';
		mpz_mul_ui(term, prod, sk);
		mpz_add(sum, sum, term);
		mpz_mul_ui(prod, prod, 2);
	}	
	char* _prod_ = strdup(mpz_get_str(0, 10, prod));
	mpz_clear(sum);
	mpz_clear(prod);
	mpz_clear(term);
	return _prod_;
}

bool factorize(char* num, char* rnum, const char* pp, int pk, int repeat, int t) {
	char* ss = (char*) calloc(pk + repeat + 1, sizeof(char));
	strncpy(ss, pp, pk + repeat);
	ss[pk + repeat + 1] = '\0';
	char* _num_ = 0;
	if (t == 0) {
		_num_ = num;
	} else {
		_num_ = rnum;
	}
	for ( int i = pk; i < pk + repeat; ++i) {
		if (ss[i] == _num_[i]) {
			free(ss);
			return true;
		}
	}
	free(ss);
	return false;
}

int main(int argc, char* argv[]) {
	num = strdup(argv[1]);
	std::string reverse_binary_factor_e = "";
	std::string binary_factor_pi = "";
	FILE* fp = fopen64("./pi.txt","r");
        FILE* fe = fopen64("./e.txt","r");
	unsigned long long int l = strlen(num);
	int t = 0;
	int repeat_vector[4] = { 1, 3, 2, 1 };
	unsigned long long int counter = 0;
	while (1) {
		if (offset > l) {
			num = num + offset;
			rnum = strrev(num);
		} else {
			cout << num << " is a prime..." << endl;
			exit(0);
		}
		for (int i = 0; i < 4; ++i) {
			char pp[3], ee[3];
			fscanf(fp, "%2s", pp);
			fscanf(fe, "%2s", ee);
			pp[2] = ee[2] = '\0';
			int pk = -1, ek = -1;
			if (t == 0) {
				pk = atoi(strrev(pp));
				ek = atoi(ee);
			} else {
				pk = atoi(pp);
				ek = atoi(strrev(ee));
			}
			bool bool_pp = factorize(num, rnum, e, pk, repeat_vector[i % 4], t);
			bool bool_ee = factorize(num, rnum, pi, ek, repeat_vector[i % 4], 1 - t);
			t = 1 - t;
			++counter;
		}
                offset += 2;
	}
	return 0;
}

