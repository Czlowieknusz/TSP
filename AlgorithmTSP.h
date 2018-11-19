//
// Created by Jan Woźniak on 24.10.2018.
//

#ifndef PEA_ALGORITHMTSP_H
#define PEA_ALGORITHMTSP_H

#include "DataLoader.h"
#include <iostream>
#include <vector>

struct CalculatedPath {
    std::vector<unsigned> path_;
    std::vector<bool> visitedVertices_;
    unsigned price_;

    explicit CalculatedPath(unsigned price, unsigned graphSize) : visitedVertices_(graphSize, false) {
        price_ = price;
    }

    CalculatedPath(const CalculatedPath &calculatedPath) {
        price_ = calculatedPath.price_;
        visitedVertices_ = calculatedPath.visitedVertices_;
        path_ = calculatedPath.path_;
    }

    CalculatedPath &operator=(const CalculatedPath &other) {
        price_ = other.price_;
        visitedVertices_ = other.visitedVertices_;
        path_ = other.path_;
        return *this;
    }
};

class AlgorithmTSP {
public:
    virtual ~AlgorithmTSP() {
        for (int i = 0; i < graphSize_; ++i) {
            delete[] graph_[i];
        }
        delete[] graph_;
    };

    explicit AlgorithmTSP(std::string fileName) {
        DataLoader dataLoader(fileName);
        graphSize_ = dataLoader.GetGraphSizeFromFile();
        ReserveMemoryForGraph();
        dataLoader.AddEdgesFromFileToGraph(graph_, graphSize_);
    }

    void ReserveMemoryForGraph() {
        graph_ = new unsigned *[graphSize_];
        for (int i = 0; i < graphSize_; ++i) {
            graph_[i] = new unsigned[graphSize_];
        }
    }

    void PrintGraph() {
        for (int i = 0; i < graphSize_; ++i) {
            for (int j = 0; j < graphSize_; ++j) {
                std::cout << graph_[i][j] << "; ";
            }
            std::cout << std::endl;
        }
    }

    virtual double CalculatePath(unsigned) = 0;

    unsigned **graph_;
    unsigned graphSize_;
};

#endif //PEA_ALGORITHMTSP_H


/*
 * Słuchaj Janek, jak do tego usiądziesz to środowy zamysł był taki:
 * 1. Ta klasa to klucz do statycznego albo dynamicznego (obstawiam to pierwsze) polimorfizmu dla Brute i Helda.
 * 2. Ona ma przechowywać graph i jego rozmiar i metody inicjujące go
 * 3. Posługując się danymi w tej klasie Brute i Held mają tylko wykonać odpowiednie algorytmy.
 * 4. W zasadzie to chyba wszystko Maciar ma małego, a Damian kłamie.
 */
