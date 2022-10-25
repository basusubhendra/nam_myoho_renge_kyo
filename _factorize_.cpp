#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <iostream>
#include <gmp.h>
/*
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
*/
using namespace std;
using namespace boost;
#define MAGIC 18
unsigned long long int offset = 0;
char* num = 0;
char* rnum = 0;
struct arg {
	char* num;
	int t;
	unsigned long long int offset;
};

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

void* factorize(void* _arg_) {
}

int main(int argc, char* argv[]) {
	num = strdup(argv[1]);
	rnum = strrev(num);
	return 0;
}

