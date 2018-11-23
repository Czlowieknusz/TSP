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
    os << obj.successor_ << "; " << obj.parentNode_ << "; " << obj.cost_ << std::endl;
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

double HeldKarp::CalculatePathCorrectly(unsigned startVertex) {
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

    for(const auto& i : permutations_) {
        for (const auto j : i) {
            std::cout << j << "; ";
        }
        std::cout << std::endl;
    }

    for (auto node : paths_) {
        std::cout << node.cost_ << std::endl;
    }

    for (const auto &permutation_vec : permutations_) {
        if (permutation_vec.empty()) {
            for (const auto node : listOfEndNodes) {
                std::cout << "Pusty vector" << std::endl;
                Path path(startVertex, node, graph_[startVertex][node]);
                paths_.push_back(path);
            }
        } else {
            for (const auto node : listOfEndNodes) {
                std::cout << "Vector z wartosciami: " << permutation_vec.size() << std::endl;
                if (std::find(permutation_vec.begin(), permutation_vec.end(), node) != permutation_vec.end() && ) {
                    Path path = FindMinCostOfPermutation(node, permutation_vec);
                    paths_.push_back(std::move(path));
                }
            }
        }
    }
    double measured_time = timer.GetCounter();
    std::cout << "Measured time is equal to: " << measured_time << "s." << std::endl;
    /*for(const auto& path : paths_) {
        std::cout << path << std::endl;
    }*/
    return measured_time;
}

/*
 *  Permutations zawiera vector wszystkich permutacji wraz z ich parentem(czyli tutaj niczym, i kosztem czyli tutaj niczym
 *  Tutaj dostajemy permutation.
 *  parent to indeks wierzcholka, z ktorego przeszlismy do nastepnego
 *  dla każdego wierzcholka w jego wnętrzu sprawdzamy koszt dostania sie do niego oraz przejsca z niego do nastepnego
 *  i odpalimy min
 */
Path HeldKarp::FindMinCostOfPermutation(const unsigned successor, const std::vector<unsigned> &permutation_vec) {
    /*if (permutation.middleValues_.empty()) {
        //permutation.cost_ = graph_[startVertex][]
    }*/
    unsigned pathIndex = 0;
    std::vector<Path> paths_vec(permutation_vec.size());
    for (const auto predecessor : permutation_vec) {
        Path path(predecessor, successor, graph_[predecessor][successor]);
        FindPredecessorsCost(path, successor, predecessor);
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

void HeldKarp::FindPredecessorsCost(Path &modifiedPath, unsigned successor, unsigned predecessor) {
    for (const auto &path : paths_) {
        if (path.middleValues_.size() == modifiedPath.middleValues_.size() - 1) {
            if (CheckIfCorrectPath(path.middleValues_, modifiedPath.middleValues_, successor)) {
                modifiedPath.cost_ += path.cost_;
                modifiedPath.parentNode_ = predecessor;
                return;
            }
        }
    }
}

bool HeldKarp::CheckIfCorrectPath(const std::vector<unsigned> &path, const std::vector<unsigned> &modifiedPath,
                                  unsigned successor) {
    auto nodePath = path.begin();
    for (const auto modIterator : modifiedPath) {
        if (modIterator == successor) {
            continue;
        } else {
            if (modIterator != *nodePath) {
                return false;
            }
        }
    }
    return true;
}
