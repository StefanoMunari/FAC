#include "facmath.h"
/** "private" function that computes the gcd of two numbers 
 * @param y
 * @param x
 * @return gcd of x and y
 */
long long gcd(long long x, long long y) {
	return (y != 0)? gcd(y, x%y) : x;
}
/**
 * take as input a fract and returns the normalized fract
 * @param f the fract to normalize
 * @return the normalized fract
 */
fract_t normalizeFract(fract_t f) {
	long long g = gcd(f.num, f.den);
	fract_t ret;
	ret.num = f.num/g;
	ret.den = f.den/g;
	/* Normalize sign in order to have minus only on the numerator */
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

/**
 * Computes -f. It is used in order to implement difference between two fracts.
 * @param f
 * @return -f
 */
fract_t minus(fract_t f) {
	fract_t res;
	res.num = -f.num;
	res.den = f.den;
	return res;
}

/**
 * Computes the reciprocal of the given fractional number fract1 = [a|b]. 
 * @param fract1 in the form [a|b]
 * @return [b|a]
 */
fract_t inverse(fract_t fract1) {
	fract_t res;
	res.num = fract1.den;
	res.den = fract1.num;
	return res;
}

/**
 * Returns the multiplication of fract1 and fract2
 * @param fract1
 * @param fract2
 * @return fract1*fract2
 */
fract_t mult(fract_t fract1, fract_t fract2) {
	fract_t res;
	res.num = fract1.num * fract2.num;
	res.den = fract1.den * fract2.den;
	return normalizeFract(res);
}

/**
 * Returns the sum of fract1 and fract2
 * @param fract1
 * @param fract2
 * @return fract1+fract
 */
fract_t sum(fract_t fract1, fract_t fract2) {
	fract_t res;
	res.num = fract1.num * fract2.den + fract1.den * fract2.num;
	res.den = fract1.den * fract2.den;
	return normalizeFract(res);
}

/**
 * Computes the lcm of two integer numbers
 * @param a
 * @param b
 * @return lcm(a,b)
 */
int lcm(int a, int b) {
	return a*b/gcd(a,b);
}

