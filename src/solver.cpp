#include "solver.h"

#include <iostream>

bool checkNodeConstraint(NodeKey& node, Output* output) {
    NodeEdgeKey nodeEdge1{node, EdgeKey{node,node+1}};
    NodeEdgeKey nodeEdge2{node, EdgeKey{node,node+2}};
    NodeEdgeKey nodeEdge3{node, EdgeKey{node,node+3}};

    Output* out1 = output + std::hash<NodeEdgeKey>{}(nodeEdge1);
    Output* out2 = output + std::hash<NodeEdgeKey>{}(nodeEdge2);
    Output* out3 = output + std::hash<NodeEdgeKey>{}(nodeEdge3);

    // should always return false
    // we simplified it, so that there is only one subset
    bool disjoint = isDisjoint(out1->collections, out2->collections);
    disjoint = disjoint || isDisjoint(out1->collections, out3->collections);
    disjoint = disjoint || isDisjoint(out2->collections, out3->collections);

    return !disjoint;
}

bool checkEdgeConstraint(EdgeKey& edge, Output* output) {
    NodeEdgeKey nodeEdge1{edge.first, edge};
    NodeEdgeKey nodeEdge2{edge.second, edge};

    Output* out1 = output + std::hash<NodeEdgeKey>{}(nodeEdge1);
    Output* out2 = output + std::hash<NodeEdgeKey>{}(nodeEdge2);

    return isDisjoint(out1->collections, out2->collections);
}

bool checkAllConstraints(int numberOfNodes, Output* output, bool debug) {
    for(int node = 0; node < numberOfNodes; node += 4) {
        if(!checkNodeConstraint(node, output)) {
            if(debug) {
                std::cout << "Fail at node " << node << std::endl;
            }
            return false;
        }
    }

    for(int node = 0; node < numberOfNodes; node += 4) {
        for(int i = 1; i <= 3; i++) {
            EdgeKey edge{node, node+i};
            if(!checkEdgeConstraint(edge, output)) {
                if(debug) {
                    std::cout << "Fail at edge (" << edge.first << "," << edge.second << ")" << std::endl;
                }
                return false;
            }
        }
    }

    return true;
}

