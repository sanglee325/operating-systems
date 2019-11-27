#include <stdio.h>

#define F (1 << 14)
//#define INT_MAX((1 << 31) - 1)
//#define INT_MIN(-(1 << 31))

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
	return n*F;
}

/*
int fp_to_int_round(int x) {
	return (int) (x + 0.5);
}

int fp_to_int(int x) {
	return (int) x;
}
*/

int add_fp(int x, int y) {
	return x + y;
}

int add_mixed(int x, int n) {
	return x + n*F;
}

int sub_fp(int x, int y) {
	return x - y;
}

int sub_mixed(int x, int n) {
	return x - n*F;
}

int mult_fp(int x, int y) {
	return x * y;
}

int mult_mixed(int x, int n) {
	int64_t tmp = x;
	tmp = tmp * n / F;;
	return (int)tmp;
}

int div_fp(int x, int y) {
	int64_t tmp = x;
	tmp = (tmp*F) / y;
	return (int)tmp;
}

int div_mixed(int x, int n) {
	return x / n;
}

