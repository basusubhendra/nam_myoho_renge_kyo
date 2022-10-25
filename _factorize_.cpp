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
	const char state_matrix[MAGIC] = { '1', '1', '1', '1', '0', '0', '1', '1', '1', '0', '0', '1', '1', '0', '0', '1', '0', '0' };
	struct arg* arg1 = (struct arg*) _arg_;
	int param = arg1->t;
	unsigned long long int offset = arg1->offset;
	char* return_string = (char*) calloc(MAGIC + 1, sizeof(char));
	FILE* f = 0;
	if (param == 0) {
		f = fopen64("./pi.txt","r");
	} else {
		f = fopen64("./e.txt","r");
	}
	fseek(f, offset, SEEK_SET);
	unsigned long l = strlen(num);
	for (int i = 0; i < MAGIC; ++i) {
		char pp = 0;
		fscanf(f, "%c", &pp);
		if (pp == num[(i + offset) % l]) {
			return_string[i] = state_matrix[i];
		} else {
			return_string[i] = '*';
		}
	}
	return return_string;
}

void* characterize(char* num, vector<char*>& triplets) {
	unsigned long int ctr = 0;
	unsigned long long int l = strlen(num);
	if (l <= 3) {
		triplets.push_back(strdup(num));
		return 0;
	}
	while (1) {
		char* triplet = (char*) calloc(4, sizeof(char));
		for (int i = ctr; i < ctr + 3; ++i) {
			triplet[i-ctr] = num[i];
		}
		triplets.push_back(triplet);
		if ((ctr + 3) == l) {
			break;
		}
		ctr++;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	num = strdup(argv[1]);
	vector<char*> triplets;
	characterize(num, triplets);
	struct arg* arg1 = (struct arg*) calloc(1, sizeof(struct arg));
	struct arg* arg2 = (struct arg*) calloc(1, sizeof(struct arg));
	arg1->t = 0;
	arg2->t = 1;
	pthread_t thread_id1, thread_id2;
	unsigned long long int ctr = 0;
	unsigned long long int sz = triplets.size();
	while (ctr < sz) {
		arg2->num = arg1->num = triplets[ctr];
		arg1->offset = offset;
		arg2->offset = offset;
		char* ret1 = 0, *ret2 = 0;
                pthread_create(&thread_id1, NULL, factorize, arg1);
		pthread_create(&thread_id2, NULL, factorize, arg2);
		pthread_join(thread_id1, (void**) &ret1);
		pthread_join(thread_id2, (void**) &ret2);
		cout << "ret1: " ;
		for (int i = 0; i < MAGIC; ++i ) {
			if (ret1[i] != '*') {
				cout << ret1[i]  << " , ";
			} else {
				cout  << "  , " ;
			}
		}
		cout << endl;
		cout << "ret2: " ;
		for (int i = 0; i < MAGIC; ++i ) {
			if (ret2[i] != '*') {
				cout << ret2[i] << " , ";
			} else {
				cout << "  , ";
			}
		}
		cout << endl;
                free(ret1);
		free(ret2);
		offset += MAGIC;
		++ctr;
	}
	free(arg1);
	free(arg2);
	return 0;
}

