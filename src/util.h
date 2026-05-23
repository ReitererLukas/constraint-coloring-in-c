#ifndef UTIL_H
#define UTIL_H

#include <set>
#include <random>
#include "types.h"

long fak(int n);
void getCombinations(int numOfColors, int wo, int* arr);
void setMinus(int numOfColors, int wo, Collections* collections);
bool isDisjoint(std::set<int>& set1, std::set<int>& set2);

#endif
