#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <iostream>
#include <gmp.h>
#include <pthread.h>
#include <algorithm>
#include "pi.hpp"
#include "e.hpp"
/*
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
*/
using namespace std;
using namespace boost;
char* num = 0;
char* rnum = 0;
#define MAGIC 18

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

int _compare_(char* x, char* y) {
	mpz_t xz;
	mpz_init(xz);
	mpz_set_str(xz, x, 10);
	mpz_t yz;
	mpz_init(yz);
	mpz_set_str(yz, y, 10);
	int result = mpz_cmp(xz, yz);
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

bool factorize(char* num, char* rnum, int param, int pk, int repeat, int t) {
	char* ss = (char*) calloc(pk + repeat + 1, sizeof(char));
	ss[pk + repeat] = '\0';
	char* _num_ = 0;
	if (param == 0 && t == 0) {
		strncpy(ss, pi, pk + repeat);
		_num_ = num;
	} else if (param == 0 && t == 1) {
		strncpy(ss, pi, pk + repeat);
		_num_ = rnum;
	} else if (param == 1 && t == 0) {
		strncpy(ss, e, pk + repeat);
		_num_ = num;
	} else if (param == 1 && t == 1) {
		strncpy(ss, e, pk + repeat);
		_num_ = rnum;
	}
	cout << _num_ << endl;
	cout << ss << endl;
	unsigned long long int l = strlen(_num_);
	for ( int i = pk; i < pk + repeat; ++i) {
	cout << ss[i]  << "\t\t"  << _num_[i % l] << "\t\t" << i % l << endl;
		if (ss[i] == _num_[i % l]) {
			free(ss);
			return true;
		}
	}
	cout << endl;
	system("a=1;read a");
	free(ss);
	return false;
}

char* _bin_(unsigned long long int x) {
	std::string binary_string = "";
	while (x > 0) {
		int r = x % 2;
		binary_string += boost::lexical_cast<std::string>(r);
		x /= 2;
	}
	std::reverse(binary_string.begin(), binary_string.end());
	return strdup(binary_string.c_str());
}

int main(int argc, char* argv[]) {
	num = strdup(argv[1]);
	rnum = strrev(num);
	FILE* fp = fopen64("./pi.txt","r");
	FILE* fe = fopen64("./e.txt","r");
	int t = 0;
	int repeat_vector[4] = { 1, 3, 2, 1 };
	unsigned long long int counter = 0;
	char* _num_ = strdup(num);
	unsigned long long int interval_pi = 0;
	unsigned long long int interval_e = 0;
	std::string binary_factor_pi = "";
	std::string binary_factor_e = "";
	unsigned long long int l = strlen(num);
	while (1) {
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
			bool bool_pp = factorize(num, rnum, 1, pk, repeat_vector[i % 4], t);
			bool bool_ee = factorize(num, rnum, 0, ek, repeat_vector[i % 4], 1 - t);
			cout << counter + 1 << "\t" << bool_pp << "\t" << pk << endl;
			cout << counter + 1 << "\t" << bool_ee << "\t" << ek <<  endl;
			bool success = false;
			system("a=1;read a");
			if (bool_pp) {
				binary_factor_pi += ((t == 0)? _bin_(interval_pi):strrev(_bin_(interval_pi)));
				interval_pi = 1;
				success = true;
			} else {
				++interval_pi;
			}
			if (bool_ee) {
				binary_factor_e += ((t == 0)? _bin_(interval_e):strrev(_bin_(interval_e)));
				interval_e = 1;
				success = true;
			} else {
				++interval_e;
			}
			if (success) {
				std::string _binary_factor_e_ = binary_factor_e;
				std::reverse(_binary_factor_e_.begin(), _binary_factor_e_.end());
				char* decimal_factor_pi = _int_(binary_factor_pi);
				char* decimal_factor_e = _int_(_binary_factor_e_);
				char* product = productOf(decimal_factor_pi, decimal_factor_e);
				int result = _compare_(_num_, product);
				if (result < 0) {
					cout << num << " is a prime." << endl;
					exit(0);
				} else if (result == 0) {
					cout << num << " = " << decimal_factor_pi << " X " << decimal_factor_e << endl;
					break;
				} 
			}
			t = 1 - t;
			++counter;
		}
		num += (MAGIC % l);
		cout << _num_ << " is now " << num << endl;
		l = strlen(num);
		if (l == 0) {
			cout << _num_ << " is a prime. " << endl;
			free(_num_);
			exit(0);
		}
		rnum = strrev(num);
	}
	return 0;
}

