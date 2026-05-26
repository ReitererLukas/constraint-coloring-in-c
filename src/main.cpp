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
    int numOfColors = 9;
    int degree = 4;
    int numReductionColors = 5;
    
    long num_of_combs = getNumberOfCombinations(numOfColors, degree);
    long size_of_combs_arr = num_of_combs * degree;

    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    int numberOfRandCollections = EdgeKey{numOfColors-1, numOfColors-2}.toIndex(numOfColors,numReductionColors) + 1;
    Collections* givenRandArray = new Collections[numberOfRandCollections];
    
    int numberOfNodes = generateOutput(numOfColors, numReductionColors, true, combinations, output, num_of_combs, givenRandArray, degree);

    unsigned long seed = 12717538143778311477UL;
    randomizeCollections(numReductionColors, givenRandArray, numberOfRandCollections, seed);

    std::cout << checkAllConstraints(numberOfNodes, output, degree, true) << std::endl;

    for(int i = numReductionColors; i< numOfColors; i++) {
        for(int j = numReductionColors; j< numOfColors; j++) {
            if(i == j) {
                continue;
            }
            std::cout << "(" << i << "," << j << ")" << std::endl;
            printCollections(givenRandArray[EdgeKey{i,j}.toIndex(numOfColors, numReductionColors)]);
        }
    }

    delete[] output;
    delete[] combinations;
    delete[] givenRandArray;
}

void threadedsingle(int id, int numOfColors, int numReductionColors, int degree, bool aFixed) {
    BRandom rand{};

    int fd = shm_open("seed.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    SharedSeedData* ptr = (SharedSeedData*) mmap(NULL, sizeof(SharedSeedData), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    
    unsigned long iterations = 1000 * 1000 * 20;
    
    long num_of_combs = getNumberOfCombinations(numOfColors, degree);
    long size_of_combs_arr = num_of_combs * degree;
    
    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    int numberOfRandCollections = EdgeKey{numOfColors-1, numOfColors-2}.toIndex(numOfColors,numReductionColors) + 1;
    Collections* givenRandArray = new Collections[numberOfRandCollections];
    
    int numberOfNodes = generateOutput(numOfColors, numReductionColors, aFixed, combinations, output, num_of_combs, givenRandArray, degree);
    
    for(unsigned long i = 0; i < iterations; i++) {
        if(ptr->seed != 0) {
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
            pthread_mutex_lock(&ptr->mutex);
            if(ptr->seed == 0) {
                ptr->seed = seed;
            }
            pthread_mutex_unlock(&ptr->mutex);
        }
    }

    delete[] output;
    delete[] combinations;
    munmap(ptr, sizeof(SharedSeedData));
}

void initSharedData(SharedSeedData* ptr) {
    ptr->seed = 0; // init with zero

    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr) != 0) {
        throw std::runtime_error("pthread_mutexattr_init failed");
    }

    if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
        pthread_mutexattr_destroy(&attr);
        throw std::runtime_error("pthread_mutexattr_setpshared failed");
    }

    if (pthread_mutex_init(&ptr->mutex, &attr) != 0) {
        pthread_mutexattr_destroy(&attr);
        throw std::runtime_error("pthread_mutex_init failed");
    }

    pthread_mutexattr_destroy(&attr);
}

void loopThreaded(int numOfColors, int numReductionColors, int degree, bool aFixed, bool restart, char** argv) {
    pid_t processIds[11];
    int numberOfProcesses = sizeof(processIds)/sizeof(pid_t);

    int fd = shm_open("/seed.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(ftruncate(fd, sizeof(SharedSeedData)) == -1) {
        return;
    }

    SharedSeedData* ptr = (SharedSeedData*) mmap(NULL, sizeof(SharedSeedData), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
   
    initSharedData(ptr);

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

    unsigned long sol = ptr->seed;
    munmap(ptr, sizeof(SharedSeedData));
    
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
    if(argc == 1 || atoi(argv[1]) < 1 || atoi(argv[1]) > 6) {
        std::cout << "Mode missing" << std::endl;
        std::cout << "1. loop" << std::endl;
        std::cout << "2. single" << std::endl;
        std::cout << "3. loop threaded (6 colors, degree 3)" << std::endl;
        std::cout << "4. loop threaded (9 colors, degree 3)" << std::endl;
        std::cout << "5. loop threaded (9 colors, degree 4)" << std::endl;
        std::cout << "6. loop threaded (10 colors, degree 4)" << std::endl;
        return 1;
    }
    
    int mode = atoi(argv[1]);
    
    if (mode == 1) {
        loop();
    } else if (mode == 2) {
        single();
    } else if (mode == 3) {
        loopThreaded(6, 4, 3, true, false, argv); // valid seed 6414111638862677456
    } else if (mode == 4) {
        loopThreaded(9, 4, 3, true, false, argv); // valid seed 13024669754258439369
    } else if (mode == 5) {
        loopThreaded(9, 5, 4, true, false, argv); // valid seed 12717538143778311477
    } else if (mode == 6) {
        loopThreaded(10, 5, 4, true, false, argv); // valid seed 11042376572657864133
    }

    return 0;
}
