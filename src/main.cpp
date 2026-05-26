#include <iostream>
#include "types.h"
#include "util.h"
#include "random.h"
#include "generator.h"
#include "solver.h"
#include <chrono>
#include <fstream>

// fork specific imports
#include <unistd.h>
#include <sys/wait.h>

// mmap
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

void loop() {
    using clock = std::chrono::steady_clock;
    int numOfColors = 6;
    int degree = 3;
    int numReductionColors = 4;
    
    clock::time_point start_time = clock::now();
    long num_of_combs = getNumberOfCombinations(numOfColors, degree);
    long size_of_combs_arr = num_of_combs * degree;
    
    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    int numberOfRandCollections = EdgeKey{numOfColors-1, numOfColors-2}.toIndex(numOfColors,numReductionColors) + 1;
    Collections* givenRandArray = new Collections[numberOfRandCollections];
    
    int numberOfNodes = generateOutput(numOfColors, numReductionColors, true, combinations, output, num_of_combs, givenRandArray, degree);
    
    BRandom rand;
    for(unsigned long i = 0; i < 3; i++) {
        unsigned long seed = rand.randLong();
        randomizeCollections(numReductionColors, givenRandArray, numberOfRandCollections, seed);
        if(checkAllConstraints(numberOfNodes, output, degree, false)) {
            std::cout << "seed: " << seed << std::endl;
        }
    }
    
    delete[] combinations;
    delete[] output;
    std::chrono::duration<double> elapsed = clock::now() - start_time;
    std::cout << elapsed.count() << std::endl;
}

void single() {
    int numOfColors = 6;
    int numReductionColors = 4;
    int degree = 3;
    
    long num_of_combs = getNumberOfCombinations(numOfColors, degree);
    long size_of_combs_arr = num_of_combs * degree;

    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    int numberOfRandCollections = EdgeKey{numOfColors-1, numOfColors-2}.toIndex(numOfColors,numReductionColors) + 1;
    Collections* givenRandArray = new Collections[numberOfRandCollections];
    
    int numberOfNodes = generateOutput(numOfColors, numReductionColors, true, combinations, output, num_of_combs, givenRandArray, degree);

    randomizeCollections(numReductionColors, givenRandArray, numberOfRandCollections);

    std::cout << checkAllConstraints(numberOfNodes, output, 3, true) << std::endl;

    delete[] output;
    delete[] combinations;
    delete[] givenRandArray;
}

void threadedsingle(int id, int numOfColors, int numReductionColors, int degree, bool aFixed) {
    BRandom rand{};

    int fd = shm_open("seed.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    unsigned long* ptr = (unsigned long*) mmap(NULL, sizeof(unsigned long), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    
    unsigned long iterations = 1000 * 1000 * 20;
    
    long num_of_combs = getNumberOfCombinations(numOfColors, degree);
    long size_of_combs_arr = num_of_combs * degree;
    
    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    int numberOfRandCollections = EdgeKey{numOfColors-1, numOfColors-2}.toIndex(numOfColors,numReductionColors) + 1;
    Collections* givenRandArray = new Collections[numberOfRandCollections];
    
    int numberOfNodes = generateOutput(numOfColors, numReductionColors, aFixed, combinations, output, num_of_combs, nullptr, degree);
    
    for(unsigned long i = 0; i < iterations; i++) {
        if(*ptr != 0) {
            delete[] output;
            delete[] combinations;
            munmap(ptr, sizeof(unsigned long));
            exit(0);
        }

        unsigned long seed = rand.randLong();
        if(i%1000000 == 0) {
            std::cout << id << " " << i << " " << seed << std::endl;
        }
        randomizeCollections(numReductionColors, givenRandArray, numberOfRandCollections, seed);
        if(checkAllConstraints(numberOfNodes, output, degree)) {
            *ptr = seed; // rc is not locked on purpose
        }
    }

    delete[] output;
    delete[] combinations;
    munmap(ptr, sizeof(unsigned long));
}

void loopThreaded(int numOfColors, int numReductionColors, int degree, bool aFixed, bool restart, char** argv) {
    pid_t processIds[11];
    int numberOfProcesses = sizeof(processIds)/sizeof(pid_t);

    int fd = shm_open("/seed.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(ftruncate(fd, sizeof(unsigned long)) == -1) {
        return;
    }
    unsigned long* ptr = (unsigned long*) mmap(NULL, sizeof(unsigned long), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    (*ptr) = 0; // init with zero

    for(int i = 0; i < numberOfProcesses; i++) {
        pid_t pid = fork();
        if(pid == 0) {
            threadedsingle(i+1, numOfColors, numReductionColors, degree, aFixed);
            exit(0);
        }
        processIds[i] = pid;
    }
    
    for(int i = 0; i < numberOfProcesses; i++) {
        int status = 0;
        waitpid(processIds[i], &status, 0);
    }

    unsigned long sol = *ptr;
    munmap(ptr, sizeof(unsigned long));
    
    std::cout << "Found seed: " << sol << std::endl;
    if(sol == 0 && restart) {
        std::cout << "Restart" << std::endl;
        execv(argv[0], argv);
        std::cout << "Restart Failed" << std::endl;
    } else if(sol != 0) {
        std::string mode(argv[1]);
        std::string fileName = "seed_for_" + std::to_string(numOfColors) + "_colors.txt";
        if(aFixed) {
            fileName = "seed_for_" + std::to_string(numOfColors) + "_colors_and_fixed_a.txt";
        }
        std::ofstream file(fileName);
        if(!file) {
            std::cout << "Cannot open file" << std::endl;
            return;
        }

        file << sol << std::endl;
        file.close();
    }
}


int main(int argc, char* argv[]) {
    if(argc == 1 || atoi(argv[1]) < 1 || atoi(argv[1]) > 4) {
        std::cout << "Mode missing" << std::endl;
        std::cout << "1. loop" << std::endl;
        std::cout << "2. single" << std::endl;
        std::cout << "3. loop threaded (6 colors)" << std::endl;
        std::cout << "4. loop threaded (9 colors)" << std::endl;
        return 1;
    }
    
    int mode = atoi(argv[1]);
    
    if (mode == 1) {
        loop();
    } else if (mode == 2) {
        single();
    } else if (mode == 3) {
        loopThreaded(6, 4, 3, true, true, argv);
    } else if (mode == 4) {
        loopThreaded(9, 4, 3, true, false, argv);
    }

    return 0;
}
