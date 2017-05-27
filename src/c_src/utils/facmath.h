/**
 * @brief file defining the operations on the type fract. @see factype.h
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 */

#ifndef __FAC__MATH__H__
#define __FAC__MATH__H__
#include "../types/factype.h"
/**
 * take as input a fract and returns the normalized fract
 * @param f the fract to normalize
 * @return the normalized fract
 */
fract_t normalizeFract(fract_t f);
/**
 * Computes -f. It is used in order to implement difference between two fracts.
 * @param f
 * @return -f
 */
fract_t minus(fract_t f);
/**
 * Computes the reciprocal of the given fractional number fract1 = [a|b]. 
 * @param fract1 in the form [a|b]
 * @return [b|a]
 */
fract_t reciprocal(fract_t fract1);

/**
 * Returns the multiplication of fract1 and fract2
 * @param fract1 first factor of the multiplication
 * @param fract2 second factor of the multiplication
 * @return fract1*fract2
 */
fract_t mult(fract_t fract1, fract_t fract2);
/**
 * Returns the sum of fract1 and fract2
 * @param fract1 first addend of the sum
 * @param fract2 second addend of the sum
 * @return fract1+fract
 */
fract_t sum(fract_t fract1, fract_t fract2);

#endif

