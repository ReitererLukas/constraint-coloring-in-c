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
    int numOfColors = 9;

    clock::time_point start_time = clock::now();
    long num_of_combs = (fak(numOfColors-1) /(fak(3)*fak(numOfColors-4)));
    long size_of_combs_arr = num_of_combs * 3;

    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    for(unsigned long i = 0; i < 1000; i++) {
        int numberOfNodes = generateOutput(numOfColors, 4, true, combinations, output, num_of_combs,i);
        checkAllConstraints(numberOfNodes, output, false);
    }
    
    delete[] combinations;
    delete[] output;
    std::chrono::duration<double> elapsed = clock::now() - start_time;
    std::cout << elapsed.count() << std::endl;
}

void single() {
    int numOfColors = 6;
    
    long num_of_combs = (fak(numOfColors-1) /(fak(3)*fak(numOfColors-4)));
    long size_of_combs_arr = num_of_combs * 3;

    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    unsigned long seed = 15953429712108830733UL;
    int numberOfNodes = generateOutput(numOfColors, 4, true, combinations, output, num_of_combs, seed);

    std::cout << checkAllConstraints(numberOfNodes, output, true) << std::endl;
    delete[] output;
    delete[] combinations;
}

void threadedsingle(int id, int numOfColors, bool aFixed) {
    BRandom rand{};

    int fd = shm_open("seed.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    unsigned long* ptr = (unsigned long*) mmap(NULL, sizeof(unsigned long), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    
    unsigned long iterations = 1000 * 1000 * 10;
    
    long num_of_combs = (fak(numOfColors-1) /(fak(3)*fak(numOfColors-4)));
    long size_of_combs_arr = num_of_combs * 3;
    
    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];
    
    for(unsigned long i = 0; i < iterations; i++) {
        if(*ptr != 0) {
            delete[] output;
            delete[] combinations;
            munmap(ptr, sizeof(unsigned long));
            exit(0);
        }

        unsigned long seed = rand.randLong();
        if(i%100000 == 0) {
            std::cout << id << " " << i << std::endl;
        }
        int numberOfNodes = generateOutput(numOfColors, 4, aFixed, combinations, output, num_of_combs, seed);
        if(checkAllConstraints(numberOfNodes, output, false)) {
            *ptr = seed; // rc is not locked on purpose
        }
    }

    delete[] output;
    delete[] combinations;
    munmap(ptr, sizeof(unsigned long));
}

void loopThreaded(int numOfColors, bool aFixed, char** argv) {
    pid_t processIds[12];
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
            threadedsingle(i+1, numOfColors, aFixed);
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
    if(sol == 0) {
        std::cout << "Restart" << std::endl;
        execv(argv[0], argv);
        std::cout << "Restart Failed" << std::endl;
    } else {
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
        loopThreaded(6, true, argv);
    } else if (mode == 4) {
        loopThreaded(9, true, argv);
    }

    return 0;
}
