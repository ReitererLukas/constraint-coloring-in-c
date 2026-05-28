#include <cassert>
#include <iostream>
#include "../src/solver.h"
#include "../src/types.h"
#include "../src/util.h"
#include "../src/types.h"
#include "../src/generator.h"


void testBasicNodeEdgeConstraints() {
    std::cout << "Testing basicNodeEdgeConstraints..." << std::endl;
    int degree = 3;

    const int ARRAY_SIZE = 6;
    Output* outputs = new Output[ARRAY_SIZE];

    NodeEdgeKey nek1{0, EdgeKey{0,1}};
    NodeEdgeKey nek2{0, EdgeKey{0,2}};
    NodeEdgeKey nek3{0, EdgeKey{0,3}};
    NodeEdgeKey nek4{1, EdgeKey{0,1}};
    NodeEdgeKey nek5{2, EdgeKey{0,2}};
    NodeEdgeKey nek6{3, EdgeKey{0,3}};

    outputs[nek1.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek2.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek3.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek4.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{1}}};
    outputs[nek5.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{1}}};
    outputs[nek6.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{1}}};

    bool result = checkAllConstraints(4, outputs, 3, true);
    assert(result);
    
    delete[] outputs;
    std::cout << "  ✓ basicNodeEdgeConstraints test passed" << std::endl;
}

void testBasicNodeEdgeConstraintsFailNodes() {
    std::cout << "Testing basicNodeEdgeConstraintsFailNodes..." << std::endl;
    int degree = 3;
    
    const int ARRAY_SIZE = 6;
    Output* outputs = new Output[ARRAY_SIZE];

    NodeEdgeKey nek1{0, EdgeKey{0,1}};
    NodeEdgeKey nek2{0, EdgeKey{0,2}};
    NodeEdgeKey nek3{0, EdgeKey{0,3}};
    NodeEdgeKey nek4{1, EdgeKey{0,1}};
    NodeEdgeKey nek5{2, EdgeKey{0,2}};
    NodeEdgeKey nek6{3, EdgeKey{0,3}};

    outputs[nek1.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{1}}};
    outputs[nek2.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek3.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek4.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek5.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek6.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};

    bool result = checkAllConstraints(4, outputs, 3);
    assert(!result);
    
    delete[] outputs;
    std::cout << "  ✓ basicNodeEdgeConstraintsFailNodes test passed" << std::endl;
}

void testBasicNodeEdgeConstraintsFailEdges() {
    std::cout << "Testing basicNodeEdgeConstraintsFailEdges..." << std::endl;
    int degree = 3;
    
    const int ARRAY_SIZE = 6;
    Output* outputs = new Output[ARRAY_SIZE];

    NodeEdgeKey nek1{0, EdgeKey{0,1}};
    NodeEdgeKey nek2{0, EdgeKey{0,2}};
    NodeEdgeKey nek3{0, EdgeKey{0,3}};
    NodeEdgeKey nek4{1, EdgeKey{0,1}};
    NodeEdgeKey nek5{2, EdgeKey{0,2}};
    NodeEdgeKey nek6{3, EdgeKey{0,3}};

    outputs[nek1.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek2.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek3.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek4.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{0}}};
    outputs[nek5.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{1}}};
    outputs[nek6.toIndex(degree)].collections = new Collections{std::vector<std::set<int>>{std::set<int>{1}}};

    bool result = checkAllConstraints(4, outputs, 3);
    assert(!result);
    
    delete[] outputs;
    std::cout << "  ✓ basicNodeEdgeConstraintsFailEdges test passed" << std::endl;
}


// remove
// {{1,2}, {1,3}, {2,3}}
// {{1,2}, {1,4}, {2,4}}
// {{1,3}, {1,4}, {3,4}}
// {{2,3}, {2,4}, {3,4}}
// {{0,3}, {0,4}, {3,4}}


void advancedTest() {
    std::cout << "Testing basicNodeEdgeConstraintsFailEdges..." << std::endl;

    int numOfColors = 9;
    int numReductionColors = 5;
    int degree = 4;
    
    long num_of_combs = getNumberOfCombinations(numOfColors, degree);
    long size_of_combs_arr = num_of_combs * degree;

    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    int numberOfRandCollections = EdgeKey{numOfColors-1, numOfColors-2}.toIndex(numOfColors,numReductionColors) + 1;
    Collections* givenRandArray = new Collections[numberOfRandCollections];
    
    givenRandArray[EdgeKey{5,6}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{0}},
        {{1}, {2}},
        {{1}, {3}},
        {{1}, {4}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        // {{1,2}, {1,3}, {2,3}},
        // {{1,2}, {1,4}, {2,4}},
        // {{1,3}, {1,4}, {3,4}},
        // {{2,3}, {2,4}, {3,4}},
        {{1,3}, {2,3,4}}
    };
    
    givenRandArray[EdgeKey{5,7}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{0}},
        {{1}, {2}},
        {{1}, {3}},
        {{1}, {4}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        // {{1,2}, {1,3}, {2,3}},
        // {{1,2}, {1,4}, {2,4}},
        // {{1,3}, {1,4}, {3,4}},
        // {{2,3}, {2,4}, {3,4}},
        {{1,2,3}, {1,3,4}, {1,2,4}}
    };
    
    givenRandArray[EdgeKey{5,8}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{0}},
        {{1}, {2}},
        {{1}, {3}},
        {{1}, {4}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        // {{1,2}, {1,3}, {2,3}},
        // {{1,2}, {1,4}, {2,4}},
        // {{1,3}, {1,4}, {3,4}},
        // {{2,3}, {2,4}, {3,4}},
        {{1,2,3}, {1,3,4}, {1,2,4}}
    };
    
    givenRandArray[EdgeKey{6,5}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{1}},
        {{0}, {2}},
        {{0}, {3}},
        {{0}, {4}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        {{0,2}, {0,3}, {2,3}},
        {{0,2}, {0,4}, {2,4}},
        // {{0,3}, {0,4}, {3,4}},
        // {{2,3}, {2,4}, {3,4}},
        {{0,2,3}, {0,2,4}, {2,3,4}}
    };
    
    givenRandArray[EdgeKey{6,7}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{1}},
        {{0}, {2}},
        {{0}, {3}},
        {{0}, {4}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        {{0,2}, {0,3}, {2,3}},
        {{0,2}, {0,4}, {2,4}},
        // {{0,3}, {0,4}, {3,4}},
        // {{2,3}, {2,4}, {3,4}},
        {{2,3}, {0,3,4}}
    };
    
    givenRandArray[EdgeKey{6,8}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{1}},
        {{0}, {2}},
        {{0}, {3}},
        {{0}, {4}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        {{0,2}, {0,3}, {2,3}},
        {{0,2}, {0,4}, {2,4}},
        // {{0,3}, {0,4}, {3,4}},
        // {{2,3}, {2,4}, {3,4}},
        {{0,2,3}, {0,2,4}, {2,3,4}}
    };
    
    givenRandArray[EdgeKey{7,5}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{2}},
        {{0}, {1}},
        {{0}, {3}},
        {{0}, {4}},
        {{1}, {3}},
        {{1}, {4}},
        {{3}, {4}},
        {{0,1}, {0,3}, {1,3}},
        {{0,1}, {0,4}, {1,4}},
        // {{0,3}, {0,4}, {3,4}},
        // {{1,3}, {1,4}, {3,4}},
        {{0,3}, {1,3,4}}
    };
    
    givenRandArray[EdgeKey{7,6}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{2}},
        {{0}, {1}},
        {{0}, {3}},
        {{0}, {4}},
        {{1}, {3}},
        {{1}, {4}},
        {{3}, {4}},
        {{0,1}, {0,3}, {1,3}},
        {{0,1}, {0,4}, {1,4}},
        // {{0,3}, {0,4}, {3,4}},
        // {{1,3}, {1,4}, {3,4}},
        {{0,1,3}, {0,3,4}, {0,1,4}}
    };
    
    givenRandArray[EdgeKey{7,8}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{2}},
        {{0}, {1}},
        {{0}, {3}},
        {{0}, {4}},
        {{1}, {3}},
        {{1}, {4}},
        {{3}, {4}},
        {{0,1}, {0,3}, {1,3}},
        {{0,1}, {0,4}, {1,4}},
        // {{0,3}, {0,4}, {3,4}},
        // {{1,3}, {1,4}, {3,4}},
        {{0,1,3}, {0,3,4}, {0,1,4}}
    };
    
    givenRandArray[EdgeKey{8,5}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{3}},
        {{4}},
        {{0}, {1}},
        {{1}, {2}},
        {{0}, {2}},
        {{0,1}, {1,2}, {0,2}}
    };
    
    givenRandArray[EdgeKey{8,6}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{3}},
        {{4}},
        {{0}, {1}},
        {{1}, {2}},
        {{0}, {2}},
        {{0,1}, {1,2}, {0,2}}
    };
    
    givenRandArray[EdgeKey{8,7}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{3}},
        {{4}},
        {{0}, {1}},
        {{1}, {2}},
        {{0}, {2}},
        {{0,1}, {1,2}, {0,2}}
    };
    
    
    int numberOfNodes = generateOutput(numOfColors, numReductionColors, true, combinations, output, num_of_combs, givenRandArray, degree);
    
    bool res = checkAllConstraints(numberOfNodes, output, degree, true);

    assert(res);

    std::cout << "  ✓ basicNodeEdgeConstraintsFailEdges test passed" << std::endl;
}

void advancedTest2() {
    std::cout << "Testing advancedTests2..." << std::endl;

    int numOfColors = 8;
    int numReductionColors = 5;
    int degree = 4;

    long num_of_combs = getNumberOfCombinations(numOfColors, degree);
    long size_of_combs_arr = num_of_combs * degree;

    Output* output = new Output[numOfColors*size_of_combs_arr*2];
    int* combinations = new int[size_of_combs_arr];

    int numberOfRandCollections = EdgeKey{numOfColors-1, numOfColors-2}.toIndex(numOfColors, numReductionColors) + 1;
    Collections* givenRandArray = new Collections[numberOfRandCollections];

    givenRandArray[EdgeKey{5,6}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{0}},
        {{1}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        {{2,3}, {3,4}, {2,4}}
    };

    givenRandArray[EdgeKey{5,7}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{0}},
        {{1}},
        {{2}, {3}},
        {{2}, {4}},
        {{3}, {4}},
        {{2,3}, {3,4}, {2,4}}
    };

    givenRandArray[EdgeKey{6,5}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{2}},
        {{3}},
        {{0}, {1}},
        {{0}, {4}},
        {{1}, {4}},
        {{0,1}, {0,4}, {1,4}}
    };

    givenRandArray[EdgeKey{6,7}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{2}},
        {{3}},
        {{0}, {1}},
        {{0}, {4}},
        {{1}, {4}},
        {{0,1}, {0,4}, {1,4}}
    };

    givenRandArray[EdgeKey{7,5}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{4}},
        {{0}, {1}},
        {{0}, {2}},
        {{0}, {3}},
        {{1}, {2}},
        {{1}, {3}},
        {{2}, {3}},
        {{0,1}, {0,2}, {1,2}},
        {{0,1}, {0,3}, {1,3}},
        {{0,2}, {0,3}, {2,3}},
        {{1,2}, {2,3}, {1,3}},
        {{0,1,2}, {0,1,3}, {0,2,3}, {1,2,3}}
    };

    givenRandArray[EdgeKey{7,6}.toIndex(numOfColors, numReductionColors)] = Collections{
        {{4}},
        {{0}, {1}},
        {{0}, {2}},
        {{0}, {3}},
        {{1}, {2}},
        {{1}, {3}},
        {{2}, {3}},
        {{0,1}, {0,2}, {1,2}},
        {{0,1}, {0,3}, {1,3}},
        {{0,2}, {0,3}, {2,3}},
        {{1,2}, {2,3}, {1,3}},
        {{0,1,2}, {0,1,3}, {0,2,3}, {1,2,3}}
    };

    int numberOfNodes = generateOutput(numOfColors, numReductionColors, true, combinations, output, num_of_combs, givenRandArray, degree);

    bool res = checkAllConstraints(numberOfNodes, output, degree, true);

    assert(res);

    std::cout << "  ✓ advancedTests2 test passed" << std::endl;
}

int main() {
    std::cout << "=== Running Solver Tests ===" << std::endl;
    
    try {
        // testBasicNodeEdgeConstraints();
        // testBasicNodeEdgeConstraintsFailNodes();
        // testBasicNodeEdgeConstraintsFailEdges();
        // advancedTest();
        advancedTest2();
        
        std::cout << "\n=== All tests passed! ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
