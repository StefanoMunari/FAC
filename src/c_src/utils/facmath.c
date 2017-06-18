#include "facmath.h"

static long long _gcd(long long x, long long y) {
	return (y != 0)? _gcd(y, x%y) : x;
}

fract_t normalize_fract(fract_t f) {
	long long g = _gcd(f.num, f.den);
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