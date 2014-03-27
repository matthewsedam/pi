/* main.cpp
 
 Copyright 2014 Matthew Sedam
 
 This file is part of Pi.
 
 Pi is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Pi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Pi.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <gmp.h>

#define BITS_PER_DIGIT 3.3219280948873623478703194294893901758648313930245806
#define DIGITS_PER_ITERATION 14.1816474627254776555

int main(int argc, const char *argv[]) {
    mpz_t numberOfDigits;
    int correctArgument;
    if (argc == 2) {
        correctArgument = mpz_init_set_str(numberOfDigits, argv[1], 10);
        if (correctArgument == -1) {
            printf("Usage: pi [number of digits]\n");
            return 0;
        }
//NEED TO ADD A DIGITS < 1 CHECK. DIGITS PROBABLY NEEDS TO BE AN UNSIGNED LONG
    } else {
        printf("Usage: pi [number of digits]\n");
        return 0;
    }
    
    unsigned long numberOfIterations = mpz_get_ui(numberOfDigits) / DIGITS_PER_ITERATION;
    numberOfIterations = numberOfIterations + 1;
    
    unsigned long precision = (BITS_PER_DIGIT * mpz_get_ui(numberOfDigits)) + 1;
    mpf_set_default_prec(precision);
    
    mpf_t numerator, denominator, sNumerator, sDenominator, newDenominator;
    mpf_inits(numerator, denominator, sNumerator, sDenominator, newDenominator, NULL);
    
    mpz_t seriesNumerator, seriesDenominator, a, b, c;
    mpz_inits(seriesNumerator, seriesDenominator, a, b, c, NULL);
    
    mpf_sqrt_ui(numerator, 10005);
    mpf_mul_ui(numerator, numerator, 426880);
    
    for (unsigned long k = 0; k < numberOfIterations; k++) {
        if (k % 2 == 0)
            mpz_set_ui(a, 1);
        else if (k % 2 == 1)
            mpz_set_si(a, -1);
        mpz_fac_ui(b, 6 * k);
        mpz_set_ui(c, 545140134);
        mpz_mul_ui(c, c, k);
        mpz_add_ui(c, c, 13591409);
        mpz_mul(seriesNumerator, a, b);
        mpz_mul(seriesNumerator, seriesNumerator, c);
        
        mpz_fac_ui(a, 3 * k);
        mpz_fac_ui(b, k);
        mpz_pow_ui(b, b, 3);
        mpz_ui_pow_ui(c, 640320, 3 * k);
        mpz_mul(seriesDenominator, a, b);
        mpz_mul(seriesDenominator, seriesDenominator, c);
        
        mpf_set_z(sNumerator, seriesNumerator);
        mpf_set_z(sDenominator, seriesDenominator);
        mpf_div(newDenominator, sNumerator, sDenominator);
        mpf_add(denominator, denominator, newDenominator);
    }
    
    mpf_t pi;
    mpf_init(pi);
    mpf_div(pi, numerator, denominator);
    
    unsigned long digits = mpz_get_ui(numberOfDigits);
    
    mp_exp_t exp;
    char * piString = mpf_get_str(NULL, &exp, 10, digits, pi);
    
    mpf_clears(numerator, denominator, sNumerator, sDenominator, newDenominator, pi, NULL);
    mpz_clears(numberOfDigits, seriesNumerator, seriesDenominator, a, b, c, NULL);
    
    printf("%.1s.%s\n", piString, piString + 1);
    
    free(piString);
    
    return 0;
}