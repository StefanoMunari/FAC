/**
 * @brief file defining the operations on the type fract.
 * @see factype.h
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 */

#ifndef __FAC__MATH__H__
#define __FAC__MATH__H__
#include "../types/factype.h"
/**
 * Normalize the fract taken as input:
 * - apply GCD to numerator and denominator;
 * - normalize the sign, keep the negative sign only for the numerator.
 * @param f - the fract to normalize
 * @return the normalized fract
 */
fract_t normalizeFract(fract_t f);
/**
 * Computes the reciprocal of the given fractional number.
 * @param fract1 in the form [a|b]
 * @return [b|a]
 */
fract_t reciprocal(fract_t fract1);

/**
 * Computes the product of fract1 and fract2
 * @param fract1 - first factor
 * @param fract2 - second factor
 * @return fract1*fract2
 */
fract_t mult(fract_t fract1, fract_t fract2);
/**
 * Computes the sum of fract1 and fract2
 * @param fract1 - first addend
 * @param fract2 - second addend
 * @return fract1+fract
 */
fract_t sum(fract_t fract1, fract_t fract2);

#endif

