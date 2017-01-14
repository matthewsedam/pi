/* pi.c
 
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

#include "pi.h"

char * piChudnovsky(unsigned long numberOfDigits) {
    unsigned long numberOfIterations = (numberOfDigits / DIGITS_PER_ITERATION) + 1;
    unsigned long precision = (BITS_PER_DIGIT * numberOfDigits) + 1;
    mpf_set_default_prec(precision);
    
    mpz_t seriesNumerator, seriesDenominator, a, b, c;
    mpz_inits(seriesNumerator, seriesDenominator, a, b, c, NULL);
    mpf_t numerator, denominator, seriesFloatNumerator, seriesFloatDenominator, newDenominator;
    mpf_inits(numerator, denominator, seriesFloatNumerator, seriesFloatDenominator, newDenominator, NULL);
    
    mpf_sqrt_ui(numerator, 10005);
    mpf_mul_ui(numerator, numerator, 426880);
    
    for (unsigned long k = 0; k < numberOfIterations; k++) {
        mpz_fac_ui(a, 6 * k);
        mpz_set_ui(b, 545140134);
        mpz_mul_ui(b, b, k);
        mpz_add_ui(b, b, 13591409);
        mpz_mul(seriesNumerator, a, b);
        if (k % 2 == 1)
            mpz_neg(seriesNumerator, seriesNumerator);
        
        mpz_fac_ui(a, 3 * k);
        mpz_fac_ui(b, k);
        mpz_pow_ui(b, b, 3);
        mpz_ui_pow_ui(c, 640320, 3 * k);
        mpz_mul(seriesDenominator, a, b);
        mpz_mul(seriesDenominator, seriesDenominator, c);
        
        mpf_set_z(seriesFloatNumerator, seriesNumerator);
        mpf_set_z(seriesFloatDenominator, seriesDenominator);
        mpf_div(newDenominator, seriesFloatNumerator, seriesFloatDenominator);
        mpf_add(denominator, denominator, newDenominator);
    }
    
    mpf_t pi;
    mpf_init(pi);
    mpf_div(pi, numerator, denominator);
    mp_exp_t exp;
    char * piString = mpf_get_str(NULL, &exp, 10, numberOfDigits, pi);
    
    mpf_clears(numerator, denominator, seriesFloatNumerator, seriesFloatDenominator, newDenominator, pi, NULL);
    mpz_clears(seriesNumerator, seriesDenominator, a, b, c, NULL);
    
    return piString;
}

char * piChudnovskyMultiCore(unsigned long numberOfDigits) {
    unsigned long numberOfIterations = (numberOfDigits / DIGITS_PER_ITERATION) + 1;
    unsigned long precision = (BITS_PER_DIGIT * numberOfDigits) + 1;
    mpf_set_default_prec(precision);
    
    calculateSeriesDenominatorInput inputToCalculateSeriesDenominatorThread;
    calculateConstantNumeratorInput inputToCalculateConstantNumeratorThread;
    
    mpz_t seriesNumerator, a, b, c;
    mpz_inits(seriesNumerator, inputToCalculateSeriesDenominatorThread.seriesDenominator, a, b, c, NULL);
    mpf_t denominator, seriesFloatNumerator, seriesFloatDenominator, newDenominator;
    mpf_inits(inputToCalculateConstantNumeratorThread.numerator, denominator, seriesFloatNumerator, seriesFloatDenominator, newDenominator, NULL);
    
    pthread_t numeratorThread;
    int rc = pthread_create(&numeratorThread, NULL, calculateConstantNumerator, (void *) &inputToCalculateConstantNumeratorThread);
    if (rc) {
        fprintf(stderr, "Error: pthread_create() returned %d\n", rc);
        exit(-1);
    }
    
    pthread_t seriesDenominatorThread;
    
    for (unsigned long k = 0; k < numberOfIterations; k++) {
        inputToCalculateSeriesDenominatorThread.k = k;
        rc = pthread_create(&seriesDenominatorThread, NULL, calculateSeriesDenominator, (void *) &inputToCalculateSeriesDenominatorThread);
        if (rc) {
            fprintf(stderr, "Error: pthread_create() returned %d\n", rc);
            exit(-1);
        }
        
        mpz_fac_ui(a, 6 * k);
        mpz_set_ui(b, 545140134);
        mpz_mul_ui(b, b, k);
        mpz_add_ui(b, b, 13591409);
        mpz_mul(seriesNumerator, a, b);
        if (k % 2 == 1)
            mpz_neg(seriesNumerator, seriesNumerator);
        
        pthread_join(seriesDenominatorThread, NULL);
        
        mpf_set_z(seriesFloatNumerator, seriesNumerator);
        mpf_set_z(seriesFloatDenominator, inputToCalculateSeriesDenominatorThread.seriesDenominator);
        mpf_div(newDenominator, seriesFloatNumerator, seriesFloatDenominator);
        mpf_add(denominator, denominator, newDenominator);
    }
    
    pthread_join(numeratorThread, NULL);
    
    mpf_t pi;
    mpf_init(pi);
    mpf_div(pi, inputToCalculateConstantNumeratorThread.numerator, denominator);
    mp_exp_t exp;
    char * piString = mpf_get_str(NULL, &exp, 10, numberOfDigits, pi);
    
    mpf_clears(inputToCalculateConstantNumeratorThread.numerator, denominator, seriesFloatNumerator, seriesFloatDenominator, newDenominator, pi, NULL);
    mpz_clears(seriesNumerator, inputToCalculateSeriesDenominatorThread.seriesDenominator, a, b, c, NULL);
    
    return piString;
}

void * calculateConstantNumerator(void * input) {
    calculateConstantNumeratorInput * numeratorStruct = (calculateConstantNumeratorInput *) input;
    mpf_sqrt_ui(numeratorStruct->numerator, 10005);
    mpf_mul_ui(numeratorStruct->numerator, numeratorStruct->numerator, 426880);
    pthread_exit(0);
}

void * calculateSeriesDenominator(void * input) {
    calculateSeriesDenominatorInput * inputStruct = (calculateSeriesDenominatorInput *) input;
    mpz_t a, b, c;
    mpz_inits(a, b, c, NULL);
    mpz_fac_ui(a, 3 * inputStruct->k);
    mpz_fac_ui(b, inputStruct->k);
    mpz_pow_ui(b, b, 3);
    mpz_ui_pow_ui(c, 640320, 3 * inputStruct->k);
    mpz_mul(inputStruct->seriesDenominator, a, b);
    mpz_mul(inputStruct->seriesDenominator, inputStruct->seriesDenominator, c);
    mpz_clears(a, b, c, NULL);
    pthread_exit(0);
}
