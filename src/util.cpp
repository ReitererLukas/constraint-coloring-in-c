#include "util.h"

long fak(int n) {
    long res = 1;
    for(long i = 1; i <= n; i++) {
        res *= i;
    }
    return res;
}

void getCombinations(int numOfColors, int wo, int* arr) {
    int index = 0;
    for (int c1 = 0; c1 < numOfColors; c1++) {
        for (int c2 = c1+1; c2 < numOfColors; c2++) {
            for (int c3 = c2+1; c3 < numOfColors; c3++) {
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

std::set<int> setMinus(int numOfColors, int wo) {
    std::set<int> set;
    for(int i = 0; i< numOfColors; i++) {
        if(i != wo) {
            set.insert(i);
        }
    }

    return set;
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

