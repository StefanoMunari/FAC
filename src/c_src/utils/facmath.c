#include "facmath.h"

long long gcd(long long x, long long y) {
	return (y != 0)? gcd(y, x%y) : x;
}

fract_t normalizeFract(fract_t f) {
	long long g = gcd(f.num, f.den);
	fract_t ret;
	ret.num = f.num/g;
	ret.den = f.den/g;
	/* Normalize sign in order to have negative only on the numerator */
	if(ret.num < 0 && ret.den < 0){
		ret.num = -ret.num;
		ret.den = -ret.den;
	}
	else if(ret.num >= 0 && ret.den < 0) {
		ret.num = -ret.num;
		ret.den = -ret.den;
	}
	return ret;
}

fract_t reciprocal(fract_t fract1) {
	fract_t res;
	res.num = fract1.den;
	res.den = fract1.num;
	return res;
}

fract_t mult(fract_t fract1, fract_t fract2) {
	fract_t res;
	res.num = fract1.num * fract2.num;
	res.den = fract1.den * fract2.den;
	return normalizeFract(res);
}

fract_t sum(fract_t fract1, fract_t fract2) {
	fract_t res;
	res.num = fract1.num * fract2.den + fract1.den * fract2.num;
	res.den = fract1.den * fract2.den;
	return normalizeFract(res);
}

int lcm(int a, int b) {
	return a*b/gcd(a,b);
}

