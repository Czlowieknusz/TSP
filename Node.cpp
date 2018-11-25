//
// Created by Jan Woźniak on 25.11.2018.
//

#include "Node.h"

Node::Node() {

}

Node::Node(const Node &node) {
    matrix_ = node.matrix_;
    index_ = node.index_;
    downBoundary_ = node.downBoundary_;
}

Node::Node(Matrix &matrix) {
    matrix_ = matrix;
}

Node::Node(Matrix &matrix, unsigned index) {
    matrix_ = matrix;
    index_ = index;
}

void Node::minimizeCost() {
    downBoundary_ = matrix_.zminimalizujKoszt();
}

std::ostream &operator<<(std::ostream &os, const Node &node) {
    os << "[#" << node.index_ << ", " << node.downBoundary_ << "]";
    return os;
}