//
// Created by Jan Woźniak on 24.10.2018.
//

#include "BruteForce.h"
#include <limits>
#include "Timer.h"

BruteForce::BruteForce(std::string fileName) : AlgorithmTSP(std::move(fileName)) {
}

double BruteForce::CalculatePath(unsigned startVertex) {
    if (startVertex >= 0 && startVertex < graphSize_) {
        CalculatedPath calculatedPath(0, graphSize_);
        CalculatedPath optimalPath(std::numeric_limits<unsigned>::max(), graphSize_);
        calculatedPath.visitedVertices_[startVertex] = true;
        calculatedPath.path_.push_back(startVertex);
        Timer timer;
        timer.StartCounter();
        CalculatePath(startVertex, optimalPath, calculatedPath);
        optimalPath.price_ += graph_[optimalPath.path_[graphSize_ - 1]][startVertex];
        optimalPath.path_.push_back(startVertex);
        double measured_time = timer.GetCounter();
        std::cout << "Minimal price is equal to: " << optimalPath.price_ << std::endl;
        std::cout << "Measured time is equal to: " << measured_time << "s." << std::endl;
        for (const auto node : optimalPath.path_) {
            std::cout << node << "; ";
        }
        return measured_time;
    } else {
        std::cout << "Vertex is not a part of the graph" << std::endl;
        return 0;
    }
}

void
BruteForce::CalculatePath(unsigned currentVertex, CalculatedPath &optimalPath, const CalculatedPath &calculatedPath) {
    for (unsigned vertex_index = 0; vertex_index < graphSize_; ++vertex_index) {
        if (!calculatedPath.visitedVertices_[vertex_index]) {
            CalculatedPath currentCalculatedPath(calculatedPath);
            currentCalculatedPath.visitedVertices_[vertex_index] = true;
            currentCalculatedPath.price_ += graph_[currentVertex][vertex_index];
            currentCalculatedPath.path_.push_back(vertex_index);
            if (SetMinPriceIfLastVertex(optimalPath, currentCalculatedPath)) {
                return;
            }
            CalculatePath(vertex_index, optimalPath, currentCalculatedPath);
        }
    }
}

bool BruteForce::SetMinPriceIfLastVertex(CalculatedPath &optimalPath, const CalculatedPath &calculatedPath) {
    bool isLastVertex = true;
    for (const auto isVisited : calculatedPath.visitedVertices_) {
        if (!isVisited) {
            isLastVertex = false;
            break;
        }
    }
    if (isLastVertex) {
        if (optimalPath.price_ > calculatedPath.price_) {
            optimalPath = calculatedPath;
        }
    }
    return isLastVertex;
}
