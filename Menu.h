//
// Created by Jan Woźniak on 07.10.2018.
//

#ifndef PEA_MENU_H
#define PEA_MENU_H

#include "BruteForce.h"
#include "HeldKarp.h"

class Menu {
public:
    void MainMenu();

    void PrintMainMenu();

    explicit Menu(std::string);

    BruteForce bruteForce;
    HeldKarp heldKarp;
};


#endif //PEA_MENU_H
