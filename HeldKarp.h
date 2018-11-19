//
// Created by Jan Woźniak on 24.10.2018.
//

#ifndef PEA_HELDKARP_H
#define PEA_HELDKARP_H

#include "AlgorithmTSP.h"
#include <vector>
#include <limits>

struct Permutation {
    std::vector<unsigned> middleValues_;
    unsigned parentNode_;
    unsigned cost_;

    Permutation(unsigned cost, unsigned parent, std::vector<unsigned> &values) : cost_(cost), parentNode_(parent),
                                                                               middleValues_(values) {}

    Permutation(unsigned cost, unsigned parent) : cost_(cost), parentNode_(parent) {}
};

class HeldKarp : public AlgorithmTSP {
public:
    explicit HeldKarp(std::string);

    double CalculatePath(unsigned) final;

    CalculatedPath CalculatePath(unsigned, CalculatedPath &&);

    unsigned FindIndexOfOptimalPath(const std::vector<CalculatedPath> &);

    std::vector<Permutation> permutations_;

    void CreatePermutations(unsigned);

    void PrintPermutations();

    bool CheckIfAllVerticesAreVisited(const std::vector<bool> &);
};


#endif //PEA_HELDKARP_H
