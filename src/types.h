#ifndef TYPES_H
#define TYPES_H

#include <cstdlib>
#include <functional>
#include <set>

typedef int NodeKey;

struct EdgeKey {
    int first;
    int second;

    bool operator==(const EdgeKey& other) const;
};

template <>
struct std::hash<EdgeKey> {
    std::size_t operator()(const EdgeKey& p) const;
};


struct NodeEdgeKey {
    NodeKey node;
    EdgeKey edge;

    bool operator==(const NodeEdgeKey& other) const;
};

template <>
struct std::hash<NodeEdgeKey> {
    std::size_t operator()(const NodeEdgeKey& p) const;
};


struct Output {
    std::set<int> collections;
};

#endif
