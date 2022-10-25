#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <iostream>
#include <gmp.h>
using namespace std;
using namespace boost;
#define MAGIC 18
unsigned long long int offset = 0;

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
	const int state_matrix[MAGIC] = { 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0 };
	struct arg* arg1 = (struct arg*) _arg_;
	char* num = arg1->num;
	int param = arg1->t;
	unsigned long long int offset = arg1->offset;
	std::string ss = "";
	FILE* f = 0;
	if (param == 0) {
		f = fopen64("./pi.txt","r");
	} else {
		f = fopen64("./e.txt","r");
	}
	fseek(f, offset, SEEK_SET);
	for (int i = 0; i < MAGIC; ++i) {
		char pp = 0;
		fscanf(f, "%c", &pp);
		if (pp == num[i]) {
			ss += boost::lexical_cast<std::string>(state_matrix[i]);
		}
	}
	char* return_string = strdup(ss.c_str());
	return return_string;
}

int main(int argc, char* argv[]) {
	char* num = strdup(argv[1]);
	char* rnum = strrev(num);
	char* decimal_factor1 = 0;
	char* decimal_factor2 = 0;
	std::string binary_factor1 = "";
	std::string binary_factor2 = "";
	struct arg* arg1 = (struct arg*) calloc(1, sizeof(struct arg));
	struct arg* arg2 = (struct arg*) calloc(1, sizeof(struct arg));
	arg1->num = num;
	arg2->num = rnum;
	arg1->t = 0;
	arg2->t = 1;
	pthread_t thread_id1, thread_id2;
	bool success = false;
	while (1) {
		arg1->offset = offset;
		arg2->offset = offset;
		char* ret1 = 0, *ret2 = 0;
                pthread_create(&thread_id1, NULL, factorize, arg1);
		pthread_create(&thread_id2, NULL, factorize, arg2);
		pthread_join(thread_id1, (void**) &ret1);
		pthread_join(thread_id2, (void**) &ret2);
                char* binary_snippet1 = (char*) ret1;
		char* binary_snippet2 = (char*) ret2;
		if (binary_snippet1 && strlen(binary_snippet1) > 0) {
			binary_factor1 += binary_snippet1;
		}
		if (binary_snippet2 && strlen(binary_snippet2) > 0) {
			binary_factor2 += strrev(binary_snippet2);
		}
		decimal_factor1 = _int_(binary_factor1);
		decimal_factor2 = _int_(binary_factor2);
	        char* _product_ = productOf(decimal_factor1, decimal_factor2);
		int result = _equals_(num, _product_);
		if (result == 0) {
			success = true;
			break;
		} else if (result > 0) {
			success = false;
			break;
		} 
		offset += MAGIC;
	}
	free(arg1);
	free(arg2);
	return 0;
}

