#ifndef GENERATOR_H
#define GENERATOR_H

#include "types.h"
#include "random.h"
#include "util.h"

int generateOutputAFixed(int numOfColors, int numReductionColors, int* combinations, Output* output, long num_of_combs, Collections* givenRandArray);
int generateOutput(int numOfColors, int numReductionColors, bool isAFixed, int* combinations, Output* output, long num_of_combs,  Collections* givenRandArray);
void randomizeCollections(int numReductionColors, Collections* collections, int numberOfCollections, unsigned long seed = 42);

#endif