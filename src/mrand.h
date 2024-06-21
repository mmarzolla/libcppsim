/* Header file "mrand.h" to be included by programs using mrand.c */

#ifndef _MRAND_H
#define _MRAND_H

extern "C" {
double mrand(int stream);
void mrandst(double* seed, int stream);
void mrandgt(double* seed, int stream);
}

#endif
