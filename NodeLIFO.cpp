//
// Created by Jan Woźniak on 25.11.2018.
//

#include "NodeLIFO.h"

NodeLIFO::NodeLIFO() {

}

NodeLIFO::NodeLIFO(const NodeLIFO &node) {
    matrix_ = node.matrix_;
    index_ = node.index_;
    downBoundary_ = node.downBoundary_;
    parent_ = node.parent_;
}

NodeLIFO::NodeLIFO(Matrix &matrix) {
    matrix_ = matrix;
}

NodeLIFO::NodeLIFO(Matrix &matrix, unsigned index) {
    matrix_ = matrix;
    index_ = index;
}

void NodeLIFO::MinimizeCost() {
    downBoundary_ = matrix_.MinimizeCost();
}

std::ostream &operator<<(std::ostream &os, const NodeLIFO &node) {
    os << "[#" << node.index_ << ", " << node.downBoundary_ << "]";
    return os;
}

NodeLIFO::~NodeLIFO() {
    delete parent_;
}

NodeLIFO &NodeLIFO::operator=(const NodeLIFO &node) {
    matrix_ = node.matrix_;
    index_ = node.index_;
    downBoundary_ = node.downBoundary_;
    parent_ = node.parent_;
    return *this;
}