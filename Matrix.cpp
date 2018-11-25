//
// Created by Jan Woźniak on 25.11.2018.
//

#include "Matrix.h"
#include "Node.h"
#include <limits>
#include "tree_util.hh"
#include <time.h>
#include <memory.h>
#include "DataLoader.h"

bool Matrix::seed_ = false;
using kptree::print_tree_bracketed;

Matrix::Matrix(unsigned graphSize) {
    if (!Matrix::seed_) {
        srand((unsigned) time(nullptr));
        Matrix::seed_ = true;
    }
    graphSize_ = 0;
    graph_ = nullptr;
    if (graphSize) {
        ReserveMemoryForGraph(graphSize);
    }
}

Matrix::~Matrix() {
    DeleteGraph();
}

bool Matrix::ReserveMemoryForGraph(unsigned graphSize) {
    if (graphSize) {
        DeleteGraph();
        graphSize_ = graphSize;
        graph_ = new int *[graphSize_];
        int **end = graph_ + graphSize_;
        for (int **i = graph_; i < end; ++i) {
            *i = new int[graphSize];
            memset(*i, 0, sizeof(int) * graphSize);
        }
        return true;
    }
    return false;
}

void Matrix::DeleteGraph() {
    if (graph_) {
        int **end = graph_ + graphSize_;
        for (int **i = graph_; i < end; ++i) {
            if (*i) {
                delete[] *i;
                *i = nullptr;
            }
        }
        delete[] graph_;
        graph_ = nullptr;
    }
    graphSize_ = 0;
}

Matrix &Matrix::operator=(const Matrix &matrix) {
    CloneMatrix(matrix);
    return *this;
}

void Matrix::CloneMatrix(const Matrix &matrix) {
    if (graphSize_ != matrix.graphSize_)
        ReserveMemoryForGraph(matrix.graphSize_);

    for (unsigned i = 0; i < graphSize_; ++i) {
        memcpy(graph_[i], matrix.graph_[i], sizeof(int) * graphSize_);
    }
}

unsigned Matrix::MinimizeCost() {
    unsigned downBoundary = 0;
    for (unsigned row = 0; row < graphSize_; ++row) {
        int buffer = MinRow(row);
        if (buffer > 0) {
            downBoundary += buffer;
            SubtractFromRow(row, buffer);
        }
    }
    for (unsigned column = 0; column < graphSize_; ++column) {
        int buffer = MinColumn(column);
        if (buffer > 0) {
            downBoundary += buffer;
            SubtractFromColumn(column, buffer);
        }
    }
    return downBoundary;
}

void Matrix::ReadFromFile(std::string fileName) {
    DataLoader dataLoader(fileName);
    graphSize_ = dataLoader.GetGraphSizeFromFile();
    std::cout << "graphSize = " << graphSize_ << std::endl;
    if (ReserveMemoryForGraph(graphSize_)) {
        dataLoader.AddEdgesFromFileToGraph(graph_, graphSize_);
    }
}

int Matrix::MinRow(unsigned row) {
    if (row >= graphSize_) {
        return -1;
    } else {
        int min = std::numeric_limits<unsigned>::max();
        for (unsigned column = 0; column < graphSize_; ++column) {
            if (graph_[row][column] < min && graph_[row][column] >= 0)
                min = graph_[row][column];
        }
        if (min == std::numeric_limits<unsigned>::max())
            return -1;
        else
            return min;
    }
}

int Matrix::MinColumn(unsigned column) {
    if (column >= graphSize_) {
        return -1;
    } else {
        int min = std::numeric_limits<unsigned>::max();
        for (unsigned row = 0; row < graphSize_; ++row) {
            if (graph_[row][column] < min && graph_[row][column] >= 0)
                min = graph_[row][column];
        }
        if (min == std::numeric_limits<unsigned>::max())
            return -1;
        else
            return min;
    }
}

void Matrix::SubtractFromRow(unsigned row, int value) {
    if (value > 0) {
        for (unsigned column = 0; column < graphSize_; ++column) {
            if (graph_[row][column] >= 0)
                graph_[row][column] -= value;
        }
    }
}

void Matrix::SubtractFromColumn(unsigned column, int value) {
    if (value > 0) {
        for (unsigned row = 0; row < graphSize_; ++row) {
            if (graph_[row][column] >= 0)
                graph_[row][column] -= value;
        }
    }
}

void Matrix::BranchAndBound(unsigned startVertex) {
    Node node(*this, startVertex);
    node.MinimizeCost();
    tree<Node> root(node);
    tree<Node>::iterator currentNode = root.begin();
    bool isChildAdded;
    do {
        isChildAdded = false;
        unsigned uBuffer;
        std::vector<bool> visitedNodes = BuildVisited(currentNode, graphSize_);
        std::vector<unsigned> path = BuildPath(currentNode, uBuffer);
        for (unsigned i = 0; i < graphSize_; ++i) {
            if (!visitedNodes[i]) {
                Node bufferNode(currentNode->matrix_, i);
                bufferNode.matrix_.SetRow(currentNode->index_);
                bufferNode.matrix_.SetColumn(i);
                for (unsigned j = 0; j < path.size(); j++) {
                    bufferNode.matrix_.SetValue(i, path[j]);
                }
                bufferNode.MinimizeCost();
                bufferNode.downBoundary_ += currentNode->matrix_.GetValue(currentNode->index_, i);
                bufferNode.downBoundary_ += currentNode->downBoundary_;

                root.append_child(currentNode, bufferNode);
                isChildAdded = true;
            }
        }
        unsigned minimum = std::numeric_limits<unsigned>::max();
        for (tree<Node>::leaf_iterator iter = root.begin_leaf(); iter != root.end_leaf(); ++iter) {
            if (iter->downBoundary_ < minimum) {
                minimum = iter->downBoundary_;
                currentNode = iter;
            }
        }
    } while (isChildAdded);
    unsigned downBoundary;
    std::vector<unsigned> path = BuildPath(currentNode, downBoundary);
    std::cout << "Path: ";
    for (unsigned i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i < path.size() - 1) {
            std::cout << " -> ";
        } else {
            std::cout << " -> " << startVertex << std::endl;
        }
    }
    downBoundary += graph_[*(path.end() - 1)][startVertex];
    std::cout << "Optimal path cost is equal to: " << downBoundary << std::endl;
}

std::vector<unsigned> Matrix::BuildPath(tree<Node>::iterator currentNode, unsigned &downBoundary) {
    std::vector<unsigned> path;
    tree<Node>::iterator parent;
    if (currentNode != nullptr) {
        downBoundary = currentNode->downBoundary_;
    }
    while (currentNode != nullptr) {
        parent = tree<Node>::parent(currentNode);
        path.insert(path.begin(), currentNode->index_);
        currentNode = parent;
    }
    return path;
}

std::vector<bool> Matrix::BuildVisited(tree<Node>::iterator currentNode, unsigned graphSize) {
    std::vector<bool> visitedNodes(graphSize, false);
    tree<Node>::iterator parent;
    while (currentNode != nullptr) {
        parent = tree<Node>::parent(currentNode);
        visitedNodes[currentNode->index_] = true;
        currentNode = parent;
    }
    return visitedNodes;
}

void Matrix::SetRow(unsigned row, int value) {
    for (unsigned column = 0; column < graphSize_; ++column) {
        graph_[row][column] = value;
    }
}

void Matrix::SetColumn(unsigned column, int value) {
    for (unsigned row = 0; row < graphSize_; ++row) {
        graph_[row][column] = value;
    }
}

void Matrix::SetValue(unsigned row, unsigned column, int value) {
    if (row < graphSize_ && column < graphSize_)
        graph_[row][column] = value;
}

int Matrix::GetValue(unsigned row, unsigned column) {
    return graph_[row][column];
}