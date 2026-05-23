#ifndef RANDOM_H
#define RANDOM_H

#include <set>
#include <random>
#include "types.h"

class BRandom {
private:
    std::mt19937 gen;

public:
    BRandom(unsigned long seed) : gen(seed) {}
    BRandom() : gen(std::random_device{}()) {}

    int randInt(int min, int max);
    unsigned long randLong();
    void randSubset(int numOfColors, std::set<int>& set, bool allowEmpty = false);

};



#endif