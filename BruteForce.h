//
// Created by Jan Woźniak on 24.10.2018.
//

#ifndef PEA_BRUTEFORCE_H
#define PEA_BRUTEFORCE_H

#include "AlgorithmTSP.h"
#include <vector>

class BruteForce : public AlgorithmTSP {
public:
    explicit BruteForce(std::string);

    double CalculatePath(unsigned) final;
    void CalculatePath(unsigned currentVertex, CalculatedPath &optimalPath, const CalculatedPath &calculatedPath);

    bool SetMinPriceIfLastVertex(CalculatedPath& optimalPath, const CalculatedPath &calculatedPath);
};


#endif //PEA_BRUTEFORCE_H
