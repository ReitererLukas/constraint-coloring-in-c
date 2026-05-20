#include "random.h"
#include <algorithm>
#include <climits>


void BRandom::randSubset(int numOfColors, std::set<int>& set, bool allowEmpty) {
    std::vector<int> nums(numOfColors);
    std::iota(nums.begin(), nums.end(), 0); 

    std::shuffle(nums.begin(), nums.end(), this->gen);

    int k = this->randInt(allowEmpty?0:1,numOfColors);
    int index = 0;
    auto iter = nums.begin();
    while(index < k && iter != nums.end()) {
        set.insert(*iter);
        iter++;
        index++;
    }
}

int BRandom::randInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(this->gen);
}

unsigned long BRandom::randLong() {
    std::uniform_int_distribution<unsigned long> dist(0, ULONG_MAX);
    return dist(this->gen);
}