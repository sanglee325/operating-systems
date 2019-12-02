#include <stdio.h>

#define F (1 << 14)

int int_to_fp(int n);
int add_fp(int x, int y);
int add_mixed(int x, int n);
int sub_fp(int x, int y);
int sub_mixed(int x, int n);
int mult_fp(int x, int y);
int mult_mixed(int x, int n);
int div_fp(int x, int y);
int div_mixed(int x, int n); 

int int_to_fp(int n) {
	return n * F;
}

int add_fp(int x, int y) {
	return x + y;
}

int add_mixed(int x, int n) {
	return x + n * F;
}

int sub_fp(int x, int y) {
	return x - y;
}

int sub_mixed(int x, int n) {
	return n * F - x;
}

int mult_fp(int x, int y) {
	int64_t tmp = x;
	tmp = tmp * y / F;;
	return (int)tmp;
}

int mult_mixed(int x, int n) {
	return x * n;
}

int div_fp(int x, int y) {
	int64_t tmp = x;
	tmp = (tmp*F) / y;
	return (int)tmp;
}

int div_mixed(int x, int n) {
	return x / n;
}

