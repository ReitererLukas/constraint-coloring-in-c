#include "types.h"

bool EdgeKey::operator==(const EdgeKey& other) const {
    return first == other.first && second == other.second;
}

bool NodeEdgeKey::operator==(const NodeEdgeKey& other) const {
    return node == other.node && edge == other.edge;
}

std::size_t std::hash<EdgeKey>::operator()(const EdgeKey& key) const {
    return key.second - key.first - 1;
}

std::size_t std::hash<NodeEdgeKey>::operator()(const NodeEdgeKey& key) const {
    if(key.node % 4 == 0) {
        return (key.node/4) * 6 + std::hash<EdgeKey>{}(key.edge);
    }
    return ((key.node - (key.node % 4)) / 4) * 6 + std::hash<EdgeKey>{}(key.edge) + 3;
}