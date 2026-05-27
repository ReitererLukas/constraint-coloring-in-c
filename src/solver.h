#ifndef SOLVER_H
#define SOLVER_H

#include "types.h"
#include "util.h"

bool checkNodeConstraint(NodeKey& node, Output* output, int degree);
bool checkNodeConstraintNew(NodeKey& node, Output* output, int degree);
bool checkEdgeConstraint(EdgeKey& edge, Output* output, int degree);
bool checkEdgeConstraintNew(EdgeKey& edge, Output* output, int degree);
bool checkAllConstraints(int numberOfNodes, Output* output, int degree, bool debug = false);
bool checkIfAtLeastOneIsDisjoint(Collection& coll1, Collection& coll2);
bool checkIfAllAreDisjoint(Collection& coll1, Collection& coll2);

#endif