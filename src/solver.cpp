#include "solver.h"

#include <iostream>

bool checkNodeConstraint(NodeKey& node, Output* output, int degree) {
    std::vector<std::pair<int, int>> combinations;
    if(degree == 3) {
        combinations.push_back({1,2});
        combinations.push_back({1,3});
        combinations.push_back({2,3});
    } else if (degree == 4) {
        combinations.push_back({1,2});
        combinations.push_back({1,3});
        combinations.push_back({1,4});
        combinations.push_back({2,3});
        combinations.push_back({2,4});
        combinations.push_back({3,4});
    }
    
    for(auto comb = combinations.begin(); comb != combinations.end(); comb++) {
        Collections* out1 = (output + std::hash<NodeEdgeKey>{}({node, EdgeKey{node, node + (*comb).first}}))->collections;
        Collections* out2 = (output + std::hash<NodeEdgeKey>{}({node, EdgeKey{node, node + (*comb).second}}))->collections;
        
        bool is_disjoint = true;
        auto c1 = out1->begin();
        
        while(c1 != out1->end() && is_disjoint) {
            auto c2 = out2->begin();
            while(c2 != out2->end() && is_disjoint) {
                is_disjoint = checkIfAllAreDisjoint(*c1, *c2);
                c2++;
            }
            c1++;
        }
        
        if(is_disjoint) {
            return false;
        }
    }

    return true;
}

bool checkEdgeConstraint(EdgeKey& edge, Output* output) {
    NodeEdgeKey nodeEdge1{edge.first, edge};
    NodeEdgeKey nodeEdge2{edge.second, edge};

    Collections* out1 = (output + std::hash<NodeEdgeKey>{}(nodeEdge1))->collections;
    Collections* out2 = (output + std::hash<NodeEdgeKey>{}(nodeEdge2))->collections;


    bool is_disjoint = false;
    for(auto c1 = out1->begin(); c1 != out1->end(); c1++) {
        for(auto c2 = out2->begin(); c2 != out2->end(); c2++) {
            is_disjoint = (is_disjoint || checkIfAtLeastOneIsDisjoint(*c1, *c2));
        }

        if(!is_disjoint) {
            return false;
        }
    }

    return true;
}

bool checkIfAtLeastOneIsDisjoint(Collection& coll1, Collection& coll2) {
    for(auto s1 = coll1.begin(); s1 != coll1.end(); s1++) {
        for(auto s2 = coll2.begin(); s2 != coll2.end(); s2++) {
            if(isDisjoint(*s1, *s2)) {
                return true;
            }
        }
    }

    return false;
}

bool checkIfAllAreDisjoint(Collection& coll1, Collection& coll2) {
    for(auto s1 = coll1.begin(); s1 != coll1.end(); s1++) {
        for(auto s2 = coll2.begin(); s2 != coll2.end(); s2++) {
            if(!isDisjoint(*s1, *s2)) {
                return false;
            }
        }
    }

    return true;
}

bool checkAllConstraints(int numberOfNodes, Output* output, int degree, bool debug) {
    for(int node = 0; node < numberOfNodes; node += (degree+1)) {
        if(!checkNodeConstraint(node, output, degree)) {
            if(debug) {
                std::cout << "Fail at node " << node << std::endl;
            }
            return false;
        }
    }
    
    for(int node = 0; node < numberOfNodes; node += (degree+1)) {
        for(int i = 1; i <= degree; i++) {
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

