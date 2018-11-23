//
// Created by Jan Woźniak on 24.10.2018.
//

#ifndef PEA_HELDKARP_H
#define PEA_HELDKARP_H

#include "AlgorithmTSP.h"
#include <limits>

struct Path {
    std::vector<unsigned> middleValues_;
    unsigned parentNode_;
    unsigned cost_;
    unsigned successor_;

    Path(unsigned cost, unsigned parent, std::vector<unsigned> &values) : cost_(cost), parentNode_(parent),
                                                                          middleValues_(values) {}

    Path(unsigned parent, unsigned successor, unsigned cost) : cost_(cost), parentNode_(parent),
                                                               successor_(successor) {}
// sprawdzic performance
    Path() = default;

    Path(const Path &permutation)
            : middleValues_(permutation.middleValues_),
              cost_(permutation.cost_),
              parentNode_(permutation.cost_) {}

    friend std::ostream &operator<<(std::ostream &os, const Path &);

    friend bool operator<(const Path &p1, const Path &p2);
};

class HeldKarp : public AlgorithmTSP {
public:
    explicit HeldKarp(std::string);

    double CalculatePath(unsigned) final;

    CalculatedPath CalculatePath(unsigned, CalculatedPath &&);

    unsigned FindIndexOfOptimalPath(const std::vector<CalculatedPath> &);

    std::vector<std::vector<unsigned> > permutations_;

    std::vector<Path> paths_;

    void CreatePermutations(unsigned);

    void PrintPermutations();

    bool CheckIfAllVerticesAreVisited(const std::vector<bool> &);

    double CalculatePathCorrectly(unsigned);

    Path FindMinCostOfPermutation(const unsigned, const std::vector<unsigned> &);

    void FindPredecessorsCost(Path &, unsigned, unsigned);

    void CalculatePathInit();

    Path FindMinPathFromVector(std::vector<Path>);

    void InitVectorOfPermutations(unsigned);

    bool CheckIfCorrectPath(const std::vector<unsigned> &, const std::vector<unsigned> &, unsigned);
};


#endif //PEA_HELDKARP_H
