//
// Created by Jan Woźniak on 05.11.2018.
//

#ifndef PEA_TESTBF_H
#define PEA_TESTBF_H

#include <string>

class Test {
public:
    Test() = default;
    void MakeTests();
    void CalculateBruteForce(unsigned, std::string);
    void CalculateHeldKarp(unsigned, std::string);
};


#endif //PEA_TESTBF_H
