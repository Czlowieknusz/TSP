//
// Created by Jan Woźniak on 07.10.2018.
//

#include "DataLoader.h"
#include <fstream>
#include <iostream>
#include <string>


DataLoader::DataLoader(std::string fileName) : myFile_(fileName) {
}

DataLoader::~DataLoader() {
    if (myFile_.is_open())
        myFile_.close();
}

unsigned DataLoader::GetGraphSizeFromFile() {
    if (myFile_.is_open()) {
        std::getline(myFile_, line_of_file_, ' ');
        return static_cast<unsigned>(std::stoi(line_of_file_));
    }
    return 0;
}

void DataLoader::AddEdgesFromFileToGraph(unsigned **graph, const unsigned graphSize) {
    if (myFile_.is_open()) {
        for (int i = 0; i < graphSize; ++i) {
            for (int j = 0; j < graphSize; ++j) {
                graph[i][j] = GetUnsignedFromFile();
            }
        }
    } else {
        std::cout << "Couldn't load data." << std::endl;
    }
}

void DataLoader::AddEdgesFromFileToGraph(int **graph, const unsigned graphSize) {
    if (myFile_.is_open()) {
        for (int i = 0; i < graphSize; ++i) {
            for (int j = 0; j < graphSize; ++j) {
                graph[i][j] = GetUnsignedFromFile();
            }
        }
    } else {
        std::cout << "Couldn't load data." << std::endl;
    }
}

unsigned DataLoader::GetUnsignedFromFile() {
    std::getline(myFile_, line_of_file_, ' ');
    return static_cast<unsigned>(std::stoi(line_of_file_));
}