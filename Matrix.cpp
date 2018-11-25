//
// Created by Jan Woźniak on 25.11.2018.
//

#include "Matrix.h"
#include "Node.h"
#include <limits>

void Matrix::BranchAndBound() {
    Node node(*this, 0);
    node.minimizeCost();
    tree<Node> root(node);

    tree<Node>::iterator currentNode = root.begin();

    bool isChildAdded;
    do {
        isChildAdded = false;
        unsigned uBuffer;
        std::vector<bool> visitedNodes = BuildVisited(currentNode, graphSize_);
        std::vector<unsigned> path = BuildPath(currentNode, uBuffer);
        for (unsigned i = 0; i < graphSize_; i++) {
            if (!visitedNodes[i]) {
                Node bufferNode(currentNode->matrix_, i);
                bufferNode.matrix_.SetRow(currentNode->index_);
                bufferNode.matrix_.SetColumn(i);
                for (unsigned j = 0; j < path.size(); j++) {
                    bufferNode.matrix_.SetValue(i, path[j]);
                }
                bufferNode.minimizeCost();
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
            std::cout << std::endl;
        }
    }
    std::cout << "Optimal path cost is equal to: " << downBoundary << std::endl;
}

std::vector<unsigned> Matrix::BuildPath(tree<Node>::iterator currentTree, unsigned &downBoundary) {
    std::vector<unsigned> path;
    tree<Node>::iterator parent;
    if (currentTree != nullptr) {
        downBoundary = currentTree->downBoundary_;
    }
    while (currentTree != nullptr) {
        parent = tree<Node>::parent(currentTree);
        path.insert(path.begin(), currentTree->index_);
        currentTree = parent;
    }
    return path;
}

std::vector<bool> Matrix::BuildVisited(tree<Node>::iterator currentTree, unsigned graphSize) {
    std::vector<bool> visitedNodes(graphSize, false);
    tree<Node>::iterator parent;
    while (currentTree != nullptr) {
        parent = tree<Node>::parent(currentTree);
        visitedNodes[currentTree->index_] = true;
        currentTree = parent;
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
