#ifndef UTIL_H
#define UTIL_H

#include <set>
#include <random>
#include <iostream>
#include "types.h"

int fak(int n);
void getCombinations3(int numOfColors, int wo, int* arr);
void getCombinations4(int numOfColors, int wo, int* arr);
void setMinus(int numOfColors, int wo, Collections* collections);
bool isDisjoint(std::set<int>& set1, std::set<int>& set2);
int getNumberOfCombinations(int numOfColors, int degree);

void printSet(std::set<int>& s);
void printVectorOfSets(std::vector<std::set<int>>& v);
void printCollections(std::vector<std::vector<std::set<int>>>& v);


#endif
