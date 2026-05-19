#ifndef GENERATOR_H
#define GENERATOR_H

#include "types.h"
#include "random.h"
#include "util.h"

int generateOutputAFixed(int numOfColors, int numReductionColors, int* combinations, Output* output, long num_of_combs, unsigned long seed);
int generateOutput(int numOfColors, int numReductionColors, bool isAFixed, int* combinations, Output* output, long num_of_combs, unsigned long seed = 42);


#endif