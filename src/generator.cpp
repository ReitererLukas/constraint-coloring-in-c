#include "generator.h"

#include <iostream>

int generateOutputAFixed(int numOfColors, int numReductionColors, int* combinations, Output* output, long num_of_combs, Collections* givenRandArray) {
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

                if(centerColor < numReductionColors) {
                    output[centerIndex].collections = new Collections{std::vector<std::set<int>>{std::set<int>{}}};
                    output[centerIndex].collections->at(0).at(0).insert(centerColor);
                } else if (centerColor >= numReductionColors && neighborColor < numReductionColors) {
                    output[centerIndex].collections = new Collections{std::vector<std::set<int>>{std::set<int>{}}};
                    setMinus(numReductionColors, neighborColor, output[centerIndex].collections);
                } else {
                    output[centerIndex].collections = givenRandArray + EdgeKey{centerColor, neighborColor}.toIndex(numOfColors, numReductionColors);
                }
                
                if(neighborColor < numReductionColors) {
                    output[neighborIndex].collections = new Collections{std::vector<std::set<int>>{std::set<int>{}}};
                    output[neighborIndex].collections->at(0).at(0).insert(neighborColor);
                } else if (neighborColor >= numReductionColors && centerColor < numReductionColors) {
                    output[neighborIndex].collections = new Collections{std::vector<std::set<int>>{std::set<int>{}}};
                    setMinus(numReductionColors, centerColor, output[neighborIndex].collections);
                } else {
                    output[neighborIndex].collections = givenRandArray + EdgeKey{neighborColor, centerColor}.toIndex(numOfColors, numReductionColors);
                }
            }
        }

    }

    return counter;
}


int generateOutput(int numOfColors, int numReductionColors, bool isAFixed, int* combinations, Output* output, long num_of_combs, Collections* givenRandArray) {
    if(isAFixed) {
        return generateOutputAFixed(numOfColors, numReductionColors, combinations, output, num_of_combs, givenRandArray);
    }
    return 0;
}

void randomizeCollections(int numReductionColors, Collections* collections, int numberOfCollections, unsigned long seed) {
    BRandom rand{seed};
    
    int minNumberOfCollections = 1;
    int maxNumberOfCollections = 10;

    int minNumberOfSubsets = 1;
    int maxNumberOfSubsets = 5;

    for(int i = 0; i < numberOfCollections; i++) {
        collections[i].clear();
        
        int numberOfCollectionsinCollection = rand.randInt(minNumberOfCollections, maxNumberOfCollections);
        
        for(int c = 0; c < numberOfCollectionsinCollection; c++) {
            collections[i].push_back({});
            int numberOfSubsets = rand.randInt(minNumberOfSubsets, maxNumberOfSubsets);
            for(int s = 0; s < numberOfSubsets; s++) {
                collections[i].at(c).push_back({});
                rand.randSubset(numReductionColors, collections[i].at(c).at(s));
            }
        }
    }
}