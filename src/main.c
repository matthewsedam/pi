/* main.c
 
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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gmp.h>

#include "pi.h"

void inputError(void) {
    fprintf(stderr, "Usage: pi [number of digits]\n");
    exit(-1);
}

int main(int argc, const char *argv[]) {
    unsigned long numberOfDigits = 100;
    if (argc == 2) {
        if (argv[1][0] == '-')
            inputError();
        char * endptr;
        numberOfDigits = strtol(argv[1], &endptr, 10);
        if (*endptr != '\0')
            inputError();
    } else
        inputError();
    if (numberOfDigits < 1)
        inputError();
    else if (numberOfDigits == 1) {
        printf("3\n");
        return 0;
    }
    long numberOfCPUs = sysconf(_SC_NPROCESSORS_ONLN);
    char * piString;
    if (numberOfCPUs > 1)
        piString = piChudnovskyMultiCore(numberOfDigits);
    else
        piString = piChudnovsky(numberOfDigits);
    printf("%.1s.%s\n", piString, piString + 1);
    free(piString);
    return 0;
}