//
// Created by Jan Woźniak on 24.10.2018.
//

#include "HeldKarp.h"
#include <limits>
#include <algorithm>
#include <numeric>
#include <list>
#include "Timer.h"

bool operator<(const Path &p1, const Path &p2) {
    return p1.middleValues_.size() < p2.middleValues_.size();
}

std::ostream &operator<<(std::ostream &os, const Path &obj) {
    os << "Parent: " << obj.parentNode_ << "; Successor: " << obj.successor_ << "; Koszt: " << obj.cost_ << std::endl;
    os << "Vector: ";
    for (auto i : obj.middleValues_) {
        os << i << "; ";
    }
    os << std::endl;
    return os;
}

HeldKarp::HeldKarp(std::string fileName) : AlgorithmTSP(std::move(fileName)) {
    PrintPermutations();
}

void HeldKarp::PrintPermutations() {
    for (const auto &innerVector : permutations_) {
        for (const auto vector_el : innerVector) {
            std::cout << vector_el << "; ";
        }
        std::cout << std::endl;
    }
}

void HeldKarp::CreatePermutations(unsigned startVertex) {
    if (graphSize_ > 0) {
        unsigned numberOfElements = 1;
        std::vector<unsigned> firstPermutation = {};
        permutations_.push_back(std::move(firstPermutation));
        for (unsigned missingIndex = 0; missingIndex < graphSize_; ++missingIndex) {
            if (missingIndex == startVertex) {
                continue;
            }
            unsigned numberOfNewElements = 0;
            for (unsigned oldElementsIndex = 0; oldElementsIndex < numberOfElements; ++oldElementsIndex) {
                std::vector<unsigned> permutation = permutations_[oldElementsIndex];
                permutation.push_back(missingIndex);
                permutations_.push_back(std::move(permutation));
                ++numberOfNewElements;
            }
            numberOfElements += numberOfNewElements;
        }
    }
}

void HeldKarp::InitVectorOfPermutations(unsigned startVertex) {
    if (permutations_.empty()) {
        CreatePermutations(startVertex);
        std::sort(permutations_.begin(), permutations_.end(),
                  [](const std::vector<unsigned> &vec1, const std::vector<unsigned> &vec2) {
                      return vec1.size() < vec2.size();
                  });
    }
}

double HeldKarp::CalculatePath(unsigned startVertex) {
    if (startVertex < 0 || startVertex >= graphSize_) {
        std::cout << "Vertex is not a part of graph" << std::endl;
        return 0.0f;
    }
    Timer timer;
    timer.StartCounter();
    InitVectorOfPermutations(startVertex);
    std::list<unsigned> listOfEndNodes;
    for (unsigned i = 0; i < graphSize_; ++i) {
        if (i != startVertex) {
            listOfEndNodes.push_back(i);
        }
    }
    unsigned vectorSize = 0;
    for (const auto &permutation_vec : permutations_) {
        if (permutation_vec.empty()) {
            indexesVec.push_back(0);
            for (const auto node : listOfEndNodes) {
                Path path(startVertex, node, graph_[startVertex][node], permutation_vec);
                paths_.push_back(std::move(path));
            }
        } else if (permutation_vec.size() == graphSize_ - 1) {
            indexesVec.push_back(paths_.size() - 1);
            ++vectorSize;
            Path path = FindMinCostOfPermutation(startVertex, permutation_vec);
            paths_.push_back(std::move(path));
        } else {
            if (permutation_vec.size() != vectorSize) {
                indexesVec.push_back(paths_.size() - 1);
                ++vectorSize;
            }
            for (const auto successor : listOfEndNodes) {
                if (std::find(permutation_vec.begin(), permutation_vec.end(), successor) == permutation_vec.end()) {
                    Path path = FindMinCostOfPermutation(successor, permutation_vec);
                    paths_.push_back(std::move(path));
                }
            }
        }
    }
    double measured_time = timer.GetCounter();
    std::cout << "Measured time is equal to: " << measured_time << "s." << std::endl;
    std::cout << "Measured cost is equal to: " << (paths_.end() - 1)->cost_ << std::endl;
    std::cout << startVertex << "; ";
    PrintOptimalPath(*(paths_.end() - 1), indexesVec.size() - 1);
    return measured_time;
}

Path HeldKarp::FindMinCostOfPermutation(const unsigned successor, const std::vector<unsigned> &permutation_vec) {
    unsigned pathIndex = 0;
    std::vector<Path> paths_vec(permutation_vec.size());
    for (const auto predecessor : permutation_vec) {
        Path path(predecessor, successor, graph_[predecessor][successor], permutation_vec);
        FindPredecessorsCost(path, predecessor, successor);
        paths_vec[pathIndex] = path;
        ++pathIndex;
    }
    return FindMinPathFromVector(std::move(paths_vec));
}

Path HeldKarp::FindMinPathFromVector(std::vector<Path> paths_vec) {
    unsigned minCost = std::numeric_limits<unsigned>::max();
    unsigned minPathIndex = 0;
    for (unsigned index = 0; index < paths_vec.size(); ++index) {
        if (paths_vec[index].cost_ < minCost) {
            minCost = paths_vec[index].cost_;
            minPathIndex = index;
        }
    }
    return paths_vec[minPathIndex];
}

void HeldKarp::FindPredecessorsCost(Path &modifiedPath, unsigned predecessor, unsigned successor) {
    for (const auto &path : paths_) {
        if (path.middleValues_.size() == modifiedPath.middleValues_.size() - 1) {
            if (CheckIfCorrectPath(path.middleValues_, modifiedPath.middleValues_, predecessor)) {
                if (path.successor_ == modifiedPath.parentNode_) {
                    modifiedPath.cost_ += path.cost_;
                    return;
                }
            }
        }
    }
}

bool HeldKarp::CheckIfCorrectPath(const std::vector<unsigned> &path, const std::vector<unsigned> &modifiedPath,
                                  unsigned predecessor) {
    auto nodePath = path.begin();
    for (const auto modIterator : modifiedPath) {
        if (modIterator == predecessor) {
            continue;
        } else {
            if (modIterator != *nodePath) {
                return false;
            } else {
                std::advance(nodePath, 1);
            }
        }
    }
    return true;
}

void HeldKarp::PrintOptimalPath(Path &postPath, unsigned indexOfIndexesVec) {
    if (indexOfIndexesVec == indexesVec.size() - 1) {
        std::cout << postPath.parentNode_ << "; ";
        PrintOptimalPath(postPath, indexOfIndexesVec - 1);
    } else if (indexOfIndexesVec == 0) {
        return;
    } else {
         //   std::cout << "postPath: " << postPath << std::endl;
        for (unsigned index = indexesVec[indexOfIndexesVec] + 1; index < indexesVec[indexOfIndexesVec + 1]; ++index) {
           //           std::cout << "Path: " << paths_[index] << std::endl;
            if (CheckIfCorrectPath(paths_[index].middleValues_, postPath.middleValues_, paths_[index].successor_)) {
             //   std::cout << "Jak to?" << std::endl;
                if (paths_[index].successor_ == postPath.parentNode_) {
                    //        std::cout << "postPath: " << postPath << std::endl << "Path: " << paths_[index] << std::endl;
                    std::cout << paths_[index].parentNode_ << "; ";
                    PrintOptimalPath(paths_[index], indexOfIndexesVec - 1);
                    return;
                }
            }
        }
    }
}
