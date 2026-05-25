#include "generator.h"

#include <iostream>

int generateOutputAFixed(int numOfColors, int numReductionColors, int* combinations, Output* output, long num_of_combs, Collections* givenRandArray, int degree) {
    int counter = 0;
    for(int centerColor = 0; centerColor < numOfColors; centerColor++) {
        if(degree == 3) {
            getCombinations3(numOfColors, centerColor, combinations);
        } else if(degree == 4) {
            getCombinations4(numOfColors, centerColor, combinations);
        }
        
        for(long pairIndex = 0; pairIndex < num_of_combs; pairIndex++) {
            if(centerColor < numReductionColors)
                continue;

            bool smallerThanRedColors = true;
            for(int i = 0; i < degree; i++) {
               smallerThanRedColors = smallerThanRedColors && combinations[pairIndex*degree+i]; 
            }
            if(smallerThanRedColors) {
                continue;
            }

            int centerNode = counter++;
            
            for(long index = 0; index < degree; index++) {
                int neighborColor = combinations[pairIndex*degree+index];
                int neighborNode = counter++;

                NodeEdgeKey centerKey{centerNode, EdgeKey{centerNode,neighborNode}};
                NodeEdgeKey neighborKey{neighborNode, EdgeKey{centerNode,neighborNode}};
                unsigned long centerIndex = centerKey.toIndex(degree);
                unsigned long neighborIndex = neighborKey.toIndex(degree);

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


int generateOutput(int numOfColors, int numReductionColors, bool isAFixed, int* combinations, Output* output, long num_of_combs, Collections* givenRandArray, int degree) {
    if(isAFixed) {
        return generateOutputAFixed(numOfColors, numReductionColors, combinations, output, num_of_combs, givenRandArray, degree);
    }
    return 0;
}

void randomizeCollections(int numReductionColors, Collections* collections, int numberOfCollections, unsigned long seed) {
    BRandom rand{seed};
    
    int minNumberOfCollections = 2;
    int maxNumberOfCollections = 7;

    int minNumberOfSubsets = 1;
    int maxNumberOfSubsets = 3;

    for(int i = 0; i < numberOfCollections; i++) {
        collections[i].clear();
        
        int numberOfCollectionsinCollection = rand.randInt(minNumberOfCollections, maxNumberOfCollections);
        
        for(int c = 0; c < numberOfCollectionsinCollection; c++) {
            collections[i].push_back({});

            bool correct = false;
            while(!correct) {
                collections[i].at(c).clear();

                int numberOfSubsets = rand.randInt(minNumberOfSubsets, maxNumberOfSubsets);
                for(int s = 0; s < numberOfSubsets; s++) {
                    collections[i].at(c).push_back({});
                    rand.randSubset(numReductionColors, collections[i].at(c).at(s));
                }

                std::set<int> intersectingSet;
                correct = true;
                int s1 = 0;
                while(s1 < numberOfSubsets && correct) {
                    int s2 = s1+1;
                    while(s2 < numberOfSubsets && correct) {
                        intersectingSet.clear();
                        getIntersectingSet(collections[i].at(c).at(s1), collections[i].at(c).at(s2), intersectingSet);
                        if(intersectingSet.size() > 1) {
                            correct = false;
                        }
                        s2++;
                    }
                    s1++;
                }
            }

        }

    }
}