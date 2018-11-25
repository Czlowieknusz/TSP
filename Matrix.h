//
// Created by Jan Woźniak on 25.11.2018.
//

#ifndef PEA_MATRIX_H
#define PEA_MATRIX_H

#include <iostream>
#include <vector>
#include "tree.hh"
#include <fstream>

class Node;

class NodeLIFO;

class Matrix {

public:
    static bool seed_;
    unsigned graphSize_;
    int **graph_;

    explicit Matrix(unsigned graphSize = 0);

    virtual ~Matrix();

    Matrix &operator=(const Matrix &);

    void CloneMatrix(const Matrix &);

    void ReadFromFile(std::string);

    double BranchAndBoundLIFO(unsigned);

    double BranchAndBoundLC(unsigned);

    int GetValue(unsigned, unsigned);

    unsigned MinimizeCost();

    bool ReserveMemoryForGraph(unsigned vNumber = 0);

    void DeleteGraph();

    int MinRow(unsigned);

    int MinColumn(unsigned);

    void SubtractFromRow(unsigned, int);

    void SubtractFromColumn(unsigned, int);

    void SetRow(unsigned row, int value = -1);

    void SetColumn(unsigned column, int value = -1);

    void SetValue(unsigned row, unsigned column, int value = -1);

    static std::vector<bool> BuildVisited(tree<Node>::iterator, unsigned);

    static std::vector<bool> BuildVisited(std::vector<NodeLIFO>::iterator, std::vector<NodeLIFO> &, unsigned);

    static std::vector<unsigned> BuildPath(tree<Node>::iterator, unsigned &);

    static std::vector<unsigned> BuildPath(std::vector<NodeLIFO>::iterator, std::vector<NodeLIFO> &, unsigned &);
};


#endif //PEA_MATRIX_H
