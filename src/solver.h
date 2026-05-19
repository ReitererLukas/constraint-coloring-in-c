#ifndef SOLVER_H
#define SOLVER_H

#include "types.h"
#include "util.h"

bool checkNodeConstraint(NodeKey& node, Output* output);
bool checkEdgeConstraint(EdgeKey& edge, Output* output);
bool checkAllConstraints(int numberOfNodes, Output* output, bool debug = false);

#endif