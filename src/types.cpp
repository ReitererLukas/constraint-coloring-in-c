#include "types.h"

bool EdgeKey::operator==(const EdgeKey& other) const {
    return first == other.first && second == other.second;
}

bool NodeEdgeKey::operator==(const NodeEdgeKey& other) const {
    return node == other.node && edge == other.edge;
}

int EdgeKey::toIndex(int numOfColors, int numColorsReduction) const {
    int index = (this->first - numColorsReduction) * (numOfColors - numColorsReduction - 1) + (this->second - numColorsReduction);

    if(this->second > this->first) {
        index -= 1;
    }

    return index;
}

// std::size_t std::hash<EdgeKey>::operator()(const EdgeKey& key) const {
//     return static_cast<std::size_t>(key.second - key.first - 1);
// }

// std::size_t std::hash<NodeEdgeKey>::operator()(const NodeEdgeKey& key) const {
//     if(key.node % 5 == 0) {
//         return static_cast<std::size_t>((key.node/5) * 8) + std::hash<EdgeKey>{}(key.edge);
//     }
//     return static_cast<std::size_t>(((key.node - (key.node % 5)) / 5) * 8 + 4) + std::hash<EdgeKey>{}(key.edge);
// }

int NodeEdgeKey::toIndex(int degree) const {
    int edgeKeyAddition = this->edge.second - this->edge.first - 1;

    if(this->node % (degree+1) == 0) {
        return (this->node / (degree + 1)) * (degree * 2) + edgeKeyAddition;
    }
    return ((this->node - (this->node % (degree + 1))) / (degree + 1)) * (2 * degree) + (degree) + edgeKeyAddition;
    
}