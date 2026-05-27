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
        int index1 = NodeEdgeKey{node, EdgeKey{node, node + (*comb).first}}.toIndex(degree);
        Collections* out1 = (output + index1)->collections;
        int index2 = NodeEdgeKey{node, EdgeKey{node, node + (*comb).second}}.toIndex(degree);
        Collections* out2 = (output + index2)->collections;
        
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

bool checkNodeConstraintNew(NodeKey& node, Output* output, int degree) {
    if(degree == 3) // TODO:
        return false;

    
    int index1 = NodeEdgeKey{node, EdgeKey{node, node + 1}}.toIndex(degree);
    int index2 = NodeEdgeKey{node, EdgeKey{node, node + 2}}.toIndex(degree);
    int index3 = NodeEdgeKey{node, EdgeKey{node, node + 3}}.toIndex(degree);
    int index4 = NodeEdgeKey{node, EdgeKey{node, node + 4}}.toIndex(degree);

    for (auto coll1 = output[index1].collections->begin(); coll1 != output[index1].collections->end(); coll1++) {
        for (auto coll2 = output[index2].collections->begin(); coll2 != output[index2].collections->end(); coll2++) {
            for (auto coll3 = output[index3].collections->begin(); coll3 != output[index3].collections->end(); coll3++) {
                for (auto coll4 = output[index4].collections->begin(); coll4 != output[index4].collections->end(); coll4++) {
                
                    bool isNotDisjoint = true;
                    for(auto set1 = (*coll1).begin(); set1 != (*coll1).end(); set1++) {
                        for(auto set2 = (*coll2).begin(); set2 != (*coll2).end(); set2++) {
                            for(auto set3 = (*coll3).begin(); set3 != (*coll3).end(); set3++) {
                                for(auto set4 = (*coll4).begin(); set4 != (*coll4).end(); set4++) {
                                    if(!isNotDisjoint) {
                                        continue;
                                    }

                                    std::set<int> intersectingSet1;
                                    std::set<int> intersectingSet2;
                                    std::set<int> intersectingSetFinal;
                                    getIntersectingSet(*set1, *set2, intersectingSet1);
                                    getIntersectingSet(*set3, *set4, intersectingSet2);
                                    getIntersectingSet(intersectingSet1, intersectingSet2, intersectingSetFinal);
                                    if(intersectingSetFinal.empty()) {
                                        isNotDisjoint = false;
                                    }
                                }
                            }
                        }
                    }

                    if(isNotDisjoint) {
                        return true;
                    }
                
                }
            }
        }
    }
    return false;
}


bool checkEdgeConstraint(EdgeKey& edge, Output* output, int degree) {
    NodeEdgeKey nodeEdge1{edge.first, edge};
    NodeEdgeKey nodeEdge2{edge.second, edge};

    Collections* out1 = (output + nodeEdge1.toIndex(degree))->collections;
    Collections* out2 = (output + nodeEdge2.toIndex(degree))->collections;

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

bool checkEdgeConstraintNew(EdgeKey& edge, Output* output, int degree) {
    // std::cout << "================" << edge.first << " " << edge.second << std::endl;
    NodeEdgeKey nodeEdge1{edge.first, edge};
    NodeEdgeKey nodeEdge2{edge.second, edge};

    Collections* out1 = (output + nodeEdge1.toIndex(degree))->collections;
    Collections* out2 = (output + nodeEdge2.toIndex(degree))->collections;

    for(auto c1 = out1->begin(); c1 != out1->end(); c1++) {
        for(auto c2 = out2->begin(); c2 != out2->end(); c2++) {
            if(!checkIfAtLeastOneIsDisjoint(*c1, *c2)) {
                return false;
            }
        }
    }

    return true;
}

bool checkIfAtLeastOneIsDisjoint(Collection& coll1, Collection& coll2) {
    for(auto s1 = coll1.begin(); s1 != coll1.end(); s1++) {
        for(auto s2 = coll2.begin(); s2 != coll2.end(); s2++) {
            if(isDisjoint(*s1, *s2)) {
                // printSet(*s1);
                // std::cout << std::endl;
                // printSet(*s2);
                // std::cout << std::endl;
                // std::cout << "=============" << std::endl;
                return true;
            }
        }
    }
    // printVectorOfSets(coll1);
    // std::cout << std::endl;
    // printVectorOfSets(coll2);
    // std::cout << std::endl;

    return false;
}

bool checkIfAllAreDisjoint(Collection& coll1, Collection& coll2) {
    for(auto s1 = coll1.begin();
     s1 != coll1.end(); s1++) {
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
        if(!checkNodeConstraintNew(node, output, degree)) {
            if(debug) {
                std::cout << "Fail at node " << node << std::endl;
            }
            return false;
        }
    }

    for(int node = 0; node < numberOfNodes; node += (degree+1)) {
        for(int i = 1; i <= degree; i++) {
            EdgeKey edge{node, node+i};
            if(!checkEdgeConstraint(edge, output, degree)) {
                if(debug) {
                    std::cout << "Fail at edge (" << edge.first << "," << edge.second << ")" << std::endl;
                }
                return false;
            }
        }
    }

    return true;
}
