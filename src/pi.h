/* pi.h
 
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

#ifndef PI_H_
#define PI_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>

#define BITS_PER_DIGIT 3.3219280948873623478703194294893901758648313930245806
#define DIGITS_PER_ITERATION 14.1816474627254776555

char * piChudnovsky(unsigned long numberOfDigits);

char * piChudnovskyMultiCore(unsigned long numberOfDigits);

typedef struct {
    mpf_t numerator;
} calculateConstantNumeratorInput;

void * calculateConstantNumerator(void * input);

typedef struct {
    mpz_t seriesDenominator;
    unsigned long k;
} calculateSeriesDenominatorInput;

void * calculateSeriesDenominator(void * input);

#endif
