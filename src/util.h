#ifndef UTIL_H
#define UTIL_H

#include <set>
#include <random>

long fak(int n);
void getCombinations(int numOfColors, int wo, int* arr);
void setMinus(int numOfColors, int wo, std::set<int>& set);
bool isDisjoint(std::set<int>& set1, std::set<int>& set2);

#endif
