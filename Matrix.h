//
// Created by Jan Woźniak on 25.11.2018.
//

#ifndef PEA_MATRIX_H
#define PEA_MATRIX_H

#include <iostream>
#include <vector>
#include "tree.hh"

class Node;

class Matrix {

public:
    Matrix(unsigned lMiast = 0);

    Matrix(const Matrix &);

    Matrix(std::ifstream &);

    virtual ~Matrix();

    Matrix &operator=(const Matrix &);

    void CloneMatrix(const Matrix &);

    void ReadFromFile(std::ifstream &);

    void WriteToFile(std::ofstream &);

    void GenerateGraph(unsigned lMiast, bool symetria = true);

    void GenerateToFile(std::ofstream &wyjscie, unsigned lMiast, double procent,
                        bool nadpisanie = false);

    void PrintGraph(bool noColor = false);

    void BranchAndBound();

    int GetValue(unsigned, unsigned);

    unsigned GetGraphSize();

    unsigned MinimalizeCost();

    bool ReserveMemoryForGraph(unsigned vNumber = 0);

    void DeleteGraph();

    static bool seed_;
    unsigned graphSize_;
    int **graph_;

    unsigned zwrocMatrixMax();

    unsigned zwrocKolumnaMax();

    int minWiersz(unsigned);

    int minKolumna(unsigned);

    int minWiersz(unsigned, int &);

    int minKolumna(unsigned, int &);

    int drugMinWiersz(unsigned);

    int drugMinKolumna(unsigned);

    void odejmijWiersz(unsigned, int);

    void odejmijKolumna(unsigned, int);

    void SetRow(unsigned wiersz, int wartosc = -1);

    void SetColumn(unsigned kolumna, int wartosc = -1);

    void SetValue(unsigned wiersz, unsigned kolumna, int wartosc = -1);

    bool zredukuj(std::vector<Para> &, unsigned &);

    unsigned dodajOstatniaPare(std::vector<Para> &);

    long rysujBezPowt(bool *, unsigned);

    static bool nastepnaPermutacja(unsigned *, unsigned);

    static std::vector<bool> BuildVisited(tree<Node>::iterator, unsigned);

    static std::vector<unsigned> BuildPath(tree<Node>::iterator, unsigned &);
};


#endif //PEA_MATRIX_H
