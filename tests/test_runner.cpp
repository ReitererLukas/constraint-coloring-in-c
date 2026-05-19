#include <cassert>
#include <iostream>
#include "../src/solver.h"
#include "../src/types.h"
#include "../src/util.h"

void testBasicNodeEdgeConstraints() {
    std::cout << "Testing basicNodeEdgeConstraints..." << std::endl;
    
    const int ARRAY_SIZE = 6;
    Output* outputs = new Output[ARRAY_SIZE];

    NodeEdgeKey nek1{0, EdgeKey{0,1}};
    NodeEdgeKey nek2{0, EdgeKey{0,2}};
    NodeEdgeKey nek3{0, EdgeKey{0,3}};
    NodeEdgeKey nek4{1, EdgeKey{0,1}};
    NodeEdgeKey nek5{2, EdgeKey{0,2}};
    NodeEdgeKey nek6{3, EdgeKey{0,3}};

    outputs[std::hash<NodeEdgeKey>{}(nek1)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek2)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek3)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek4)].collections.insert(1);
    outputs[std::hash<NodeEdgeKey>{}(nek5)].collections.insert(1);
    outputs[std::hash<NodeEdgeKey>{}(nek6)].collections.insert(1);

    bool result = checkAllConstraints(4, outputs);
    assert(result);
    
    delete[] outputs;
    std::cout << "  ✓ basicNodeEdgeConstraints test passed" << std::endl;
}

void testBasicNodeEdgeConstraintsFailNodes() {
    std::cout << "Testing basicNodeEdgeConstraintsFailNodes..." << std::endl;
    
    const int ARRAY_SIZE = 6;
    Output* outputs = new Output[ARRAY_SIZE];

    NodeEdgeKey nek1{0, EdgeKey{0,1}};
    NodeEdgeKey nek2{0, EdgeKey{0,2}};
    NodeEdgeKey nek3{0, EdgeKey{0,3}};
    NodeEdgeKey nek4{1, EdgeKey{0,1}};
    NodeEdgeKey nek5{2, EdgeKey{0,2}};
    NodeEdgeKey nek6{3, EdgeKey{0,3}};

    outputs[std::hash<NodeEdgeKey>{}(nek1)].collections.insert(1);
    outputs[std::hash<NodeEdgeKey>{}(nek2)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek3)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek4)].collections.insert(1);
    outputs[std::hash<NodeEdgeKey>{}(nek5)].collections.insert(1);
    outputs[std::hash<NodeEdgeKey>{}(nek6)].collections.insert(1);

    bool result = checkAllConstraints(4, outputs);
    assert(!result);
    
    delete[] outputs;
    std::cout << "  ✓ basicNodeEdgeConstraintsFailNodes test passed" << std::endl;
}

void testBasicNodeEdgeConstraintsFailEdges() {
    std::cout << "Testing basicNodeEdgeConstraintsFailEdges..." << std::endl;
    
    const int ARRAY_SIZE = 6;
    Output* outputs = new Output[ARRAY_SIZE];

    NodeEdgeKey nek1{0, EdgeKey{0,1}};
    NodeEdgeKey nek2{0, EdgeKey{0,2}};
    NodeEdgeKey nek3{0, EdgeKey{0,3}};
    NodeEdgeKey nek4{1, EdgeKey{0,1}};
    NodeEdgeKey nek5{2, EdgeKey{0,2}};
    NodeEdgeKey nek6{3, EdgeKey{0,3}};

    outputs[std::hash<NodeEdgeKey>{}(nek1)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek2)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek3)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek4)].collections.insert(0);
    outputs[std::hash<NodeEdgeKey>{}(nek5)].collections.insert(1);
    outputs[std::hash<NodeEdgeKey>{}(nek6)].collections.insert(1);

    bool result = checkAllConstraints(4, outputs);
    assert(!result);
    
    delete[] outputs;
    std::cout << "  ✓ basicNodeEdgeConstraintsFailEdges test passed" << std::endl;
}


int main() {
    std::cout << "=== Running Solver Tests ===" << std::endl;
    
    try {
        testBasicNodeEdgeConstraints();
        testBasicNodeEdgeConstraintsFailNodes();
        testBasicNodeEdgeConstraintsFailEdges();
        
        std::cout << "\n=== All tests passed! ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
