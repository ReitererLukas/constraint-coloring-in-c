#include "util.h"

#include <iostream>

int fak(int n) {
    int res = 1;
    for(int i = 1; i <= n; i++) {
        res *= i;
    }
    return res;
}

int getNumberOfCombinations(int numOfColors, int degree) {
    return fak(numOfColors-1) / (fak(degree)*fak(numOfColors-1-degree));
}

void getCombinations3(int numOfColors, int wo, int* arr) {
    int index = 0;
    for (int c1 = 0; c1 < numOfColors; c1++) {
        for (int c2 = c1 + 1; c2 < numOfColors; c2++) {
            for (int c3 = c2 + 1; c3 < numOfColors; c3++) {
                if(c1 == wo || c2 == wo || c3 == wo) {
                    continue;
                }
                arr[index] = c1;
                arr[index+1] = c2;
                arr[index+2] = c3;
                index += 3;
            }
        }
    }
}

void getCombinations4(int numOfColors, int wo, int* arr) {
    int index = 0;
    for (int c1 = 0; c1 < numOfColors; c1++) {
        for (int c2 = c1 + 1; c2 < numOfColors; c2++) {
            for (int c3 = c2 + 1; c3 < numOfColors; c3++) {
                for (int c4 = c3 + 1; c4 < numOfColors; c4++) {
                    if(c1 == wo || c2 == wo || c3 == wo || c4 == wo) {
                        continue;
                    }
                    arr[index] = c1;
                    arr[index+1] = c2;
                    arr[index+2] = c3;
                    arr[index+3] = c4;
                    index += 4;
                }
            }
        }
    }
}

void setMinus(int numOfColors, int wo, Collections* collections) {
    std::set<int>& set = collections->at(0).at(0); 
    for(int i = 0; i< numOfColors; i++) {
        if(i != wo) {
            set.insert(i);
        }
    }
}

bool isDisjoint(std::set<int>& set1, std::set<int>& set2) {
    auto it1 = set1.begin();
    auto it2 = set2.begin();

    while (it1 != set1.end() && it2 != set2.end()) {
        if (*it1 == *it2) {
            return false;
        }

        if (*it1 < *it2) {
            it1++;
        } else {
            it2++;
        }
    }

    return true;
}

void getIntersectingSet(std::set<int>& set1, std::set<int>& set2, std::set<int>& res) {
    auto it1 = set1.begin();
    auto it2 = set2.begin();

    while (it1 != set1.end() && it2 != set2.end()) {
        if (*it1 == *it2) {
            res.insert(*it1);
        }

        if (*it1 < *it2) {
            it1++;
        } else {
            it2++;
        }
    }

}

void printSet(std::set<int>& s) {
    std::cout << "{";
    bool first = true;

    for (auto& x : s) {
        if (!first) std::cout << ", ";
        std::cout << x;
        first = false;
    }

    std::cout << "}";
}

void printVectorOfSets(std::vector<std::set<int>>& v) {
    std::cout << "[";

    bool first = true;
    for (auto& s : v) {
        if (!first) std::cout << ", ";
        printSet(s);
        first = false;
    }

    std::cout << "]";
}

void printCollections(std::vector<std::vector<std::set<int>>>& v) {
    std::cout << "[\n";

    for (auto& inner : v) {
        std::cout << "  ";
        printVectorOfSets(inner);
        std::cout << std::endl;
    }

    std::cout << "]" << std::endl;
}

