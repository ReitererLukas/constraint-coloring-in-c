#include <iostream>
#include "types.h"
#include "util.h"
#include "random.h"
#include "generator.h"
#include "solver.h"
#include <chrono>


int main() {
    using clock = std::chrono::steady_clock;
    int numOfColors = 9;

    clock::time_point start_time = clock::now();
    long num_of_combs = (fak(numOfColors-1) /(fak(3)*fak(numOfColors-4)));
    long size_of_combs_arr = num_of_combs * 3;

    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    for(unsigned long i = 0; i < 1000; i++) {
        int numberOfNodes = generateOutput(numOfColors, 4, true, combinations, output, num_of_combs,i);
        checkAllConstraints(numberOfNodes, output, true);
    }
    
    delete[] combinations;
    delete[] output;
    std::chrono::duration<double> elapsed = clock::now() - start_time;
    std::cout << elapsed.count() << std::endl;
    return 0;
}
