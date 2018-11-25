//
// Created by Jan Woźniak on 25.11.2018.
//

#ifndef PEA_NODELIFO_H
#define PEA_NODELIFO_H

#include <iostream>
#include "Matrix.h"

class NodeLIFO {

public:
    NodeLIFO();

    ~NodeLIFO();

    NodeLIFO(const NodeLIFO &);

    NodeLIFO(Matrix &);

    NodeLIFO(Matrix &, unsigned);

    void MinimizeCost();

    Matrix matrix_;
    unsigned index_, downBoundary_;
    NodeLIFO *parent_;

    NodeLIFO &operator=(const NodeLIFO &nodeLIFO);

    friend std::ostream &operator<<(std::ostream &, const Node &);
};


#endif //PEA_NODELIFO_H
