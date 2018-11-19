//
// Created by Jan Woźniak on 24.10.2018.
//

#include "HeldKarp.h"
#include <limits>
#include <algorithm>
#include "Timer.h"

HeldKarp::HeldKarp(std::string fileName) : AlgorithmTSP(std::move(fileName)) {
    CreatePermutations(0);
    PrintPermutations();
}

void HeldKarp::PrintPermutations() {
    for (const auto &list_el : permutations_) {
        for (const auto &vector_el : list_el.middleValues_) {
            std::cout << vector_el << "; ";
        }
        std::cout << std::endl;
    }
}

double HeldKarp::CalculatePath(unsigned startVertex) {
    if (startVertex >= 0 && startVertex < graphSize_) {
        CalculatedPath calculatedPath(0, graphSize_);
        calculatedPath.visitedVertices_[startVertex] = true;
        calculatedPath.path_.push_back(startVertex);

        Timer timer;
        timer.StartCounter();
        auto optimalPath = CalculatePath(startVertex, std::move(calculatedPath));
        optimalPath.price_ += graph_[optimalPath.path_[graphSize_ - 1]][startVertex];
        optimalPath.path_.push_back(startVertex);
        double measured_time = timer.GetCounter();
        std::cout << "Min price is equal to: " << optimalPath.price_ << std::endl;
        std::cout << "Measured time is equal to: " << measured_time << "s." << std::endl;
        for (const auto node : optimalPath.path_) {
            std::cout << node << "; ";
        }
        std::cout << std::endl;
        return measured_time;
    } else {
        std::cout << "Vertex is not a part of the graph" << std::endl;
        return 0;
    }
}

CalculatedPath
HeldKarp::CalculatePath(unsigned startVertex, CalculatedPath &&calculatedPath) {
    if (CheckIfAllVerticesAreVisited(calculatedPath.visitedVertices_)) {
        return calculatedPath;
    }
    std::vector<CalculatedPath> pathsFound;
    for (unsigned i = 0; i < graphSize_; ++i) {
        if (!calculatedPath.visitedVertices_.at(i)) {
            CalculatedPath currentCalculatedPath(calculatedPath);
            currentCalculatedPath.visitedVertices_.at(i) = true;
            currentCalculatedPath.path_.push_back(i);
            CalculatedPath calculatedPathToPushBack = CalculatePath(i, std::move(currentCalculatedPath));
            calculatedPathToPushBack.price_ += graph_[startVertex][i];
            pathsFound.push_back(std::move(calculatedPathToPushBack));
        }
    }
    return pathsFound[FindIndexOfOptimalPath(pathsFound)];
}

unsigned HeldKarp::FindIndexOfOptimalPath(const std::vector<CalculatedPath> &paths) {
    unsigned indexOfOptimalPath = 0;
    unsigned minPrice = std::numeric_limits<unsigned>::max();
    for (unsigned i = 0; i < paths.size(); ++i) {
        if (paths[i].price_ < minPrice) {
            minPrice = paths[i].price_;
            indexOfOptimalPath = i;
        }
    }
    return indexOfOptimalPath;
}

bool HeldKarp::CheckIfAllVerticesAreVisited(const std::vector<bool> &visitedVertices) {
    for (const auto isVisited : visitedVertices) {
        if (!isVisited) {
            return false;
        }
    }
    return true;
}

void HeldKarp::CreatePermutations(unsigned startVertex) {
    if (graphSize_ > 0) {
        unsigned numberOfElements = 1;
        Permutation firstPermutation(startVertex, 0);
        permutations_.push_back(std::move(firstPermutation));
        for (unsigned missingIndex = 0; missingIndex < graphSize_; ++missingIndex) {
            if (missingIndex == startVertex) {
                continue;
            }
            unsigned numberOfNewElements = 0;
            for (unsigned oldElementsIndex = 0; oldElementsIndex < numberOfElements; ++oldElementsIndex) {
                Permutation permutation = permutations_[oldElementsIndex];
                permutation.middleValues_.push_back(missingIndex);
                permutations_.push_back(std::move(permutation));
                ++numberOfNewElements;
            }
            numberOfElements += numberOfNewElements;
        }
    }
}