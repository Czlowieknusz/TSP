cmake_minimum_required(VERSION 3.12)
project(PEA)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXE_LINKER_FLAGS=-static)

add_executable(PEA main.cpp DataLoader.cpp DataLoader.h Menu.cpp Menu.h HeldKarp.cpp HeldKarp.h AlgorithmTSP.h BruteForce.cpp BruteForce.h Test.cpp Test.h Timer.cpp Timer.h Node.cpp Node.h Matrix.cpp Matrix.h NodeLIFO.cpp NodeLIFO.h)