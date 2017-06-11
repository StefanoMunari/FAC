/**
 * @brief Operations on the type fract.
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

#endif /*__FAC__MATH__H__*/

