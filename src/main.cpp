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

#include <iostream>
#include <iomanip>
#include <string>
#include <gmp.h>
#include <gmpxx.h>

int main(int argc, const char *argv[]) {
    std::cout << std::setprecision(50);
    mpz_class k = 1;
    mpf_class pi = mpf_class(0.0, 10485760);
    mpf_class piIteration = mpf_class(0.0, 10485760);
    while (true) {
        if (k % 2 == 1)
            piIteration = mpf_class(4.0, 10485760);
        else
            piIteration = mpf_class(-4.0, 10485760);
        piIteration = piIteration / mpf_class(mpz_class(2*k) - mpz_class(1), 10485760);
        pi = pi + piIteration;
        piIteration = mpf_class(0.0, 10485760);
        std::cout << pi << std::endl;
        k = k + 1;
    }
    return 0;
}