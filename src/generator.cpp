#include "generator.h"

int generateOutputAFixed(int numOfColors, int numReductionColors, int* combinations, Output* output, long num_of_combs, unsigned long seed) {
    BRandom rand{seed};

    int counter = 0;
    for(int centerColor = 0; centerColor < numOfColors; centerColor++) {
        getCombinations(numOfColors, centerColor, combinations);
        
        
        for(long pairIndex = 0; pairIndex < num_of_combs; pairIndex++) {
            if(centerColor < numReductionColors)
                continue;

            if(combinations[pairIndex*3+0] < numReductionColors && combinations[pairIndex*3+1] < numReductionColors && combinations[pairIndex*3+2] < numReductionColors)
                continue;

            int centerNode = counter++;
            
            for(long index = 0; index < 3; index++) {
                int neighborColor = combinations[pairIndex*3+index];
                int neighborNode = counter++;

                NodeEdgeKey centerKey{centerNode, EdgeKey{centerNode,neighborNode}};
                NodeEdgeKey neighborKey{neighborNode, EdgeKey{centerNode,neighborNode}};
                unsigned long centerIndex = std::hash<NodeEdgeKey>{}(centerKey);
                unsigned long neighborIndex = std::hash<NodeEdgeKey>{}(neighborKey);

                output[centerIndex].collections.clear();
                if(centerColor < numReductionColors) {
                    output[centerIndex].collections.insert(centerColor);
                } else if (centerColor >= numReductionColors && neighborColor < numReductionColors) {
                    setMinus(numReductionColors, neighborColor, output[centerIndex].collections);
                } else {
                    rand.randSubset(numReductionColors, output[centerIndex].collections);
                }
                
                output[neighborIndex].collections.clear();
                if(neighborColor < numReductionColors) {
                    output[neighborIndex].collections.insert(neighborColor);
                } else if (neighborColor >= numReductionColors && centerColor < numReductionColors) {
                    setMinus(numReductionColors, centerColor, output[neighborIndex].collections);
                } else {
                    rand.randSubset(numReductionColors, output[neighborIndex].collections, true);
                }
            }
        }

    }

    return counter;
}


int generateOutput(int numOfColors, int numReductionColors, bool isAFixed, int* combinations, Output* output, long num_of_combs, unsigned long seed) {
    if(isAFixed) {
        return generateOutputAFixed(numOfColors, numReductionColors, combinations, output, num_of_combs, seed);
    }
    return 0;
}