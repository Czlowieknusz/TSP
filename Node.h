//
// Created by Jan Woźniak on 25.11.2018.
//

#ifndef PEA_NODE_H
#define PEA_NODE_H

#include <iostream>
#include "Matrix.h"

class Node {
public:
    Node();

    Node(const Node &);

    Node(Matrix &);

    Node(Matrix &, unsigned);

    void minimizeCost();

    Matrix matrix_;
    unsigned index_, downBoundary_;

    friend std::ostream &operator<<(std::ostream &, const Node &);

};


#endif //PEA_NODE_H
