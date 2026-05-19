#include "random.h"
#include <algorithm>


std::set<int> BRandom::randSubset(int numOfColors, bool allowEmpty) {
    std::vector<int> nums(numOfColors);
    std::iota(nums.begin(), nums.end(), 0); 

    std::shuffle(nums.begin(), nums.end(), this->gen);

    int k = this->randInt(allowEmpty?0:1,numOfColors);
    return std::set<int>(nums.begin(), nums.begin() + k);;
}

int BRandom::randInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(this->gen);
}