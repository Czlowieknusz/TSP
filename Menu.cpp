//
// Created by Jan Woźniak on 07.10.2018.
//

#include "Menu.h"
#include "Test.h"

Menu::Menu(std::string fileName) : bruteForce(fileName), heldKarp(fileName) {
    MainMenu();
}

void Menu::PrintMainMenu() {
    std::cout << std::endl << std::endl;
    std::cout << "---------- Main menu ----------" << std::endl;
    std::cout << "1. Print loaded Graph." << std::endl;
    std::cout << "2. Calculate path using Brute Force." << std::endl;
    std::cout << "3. Calculate path using Held-Karp." << std::endl;
    std::cout << "4. Test algorithms." << std::endl;
    std::cout << "0. Finish executing program." << std::endl;
}

void Menu::MainMenu() {
    while (true) {
        PrintMainMenu();
        int userOption = -1;
        while (userOption < 0 || userOption > 4) {
            std::cout << "Your option: ";
            std::cin >> userOption;
        }
        switch (userOption) {
            case 0:
                return;
            case 1:
                bruteForce.PrintGraph();
                break;
            case 2:
                std::cout << "From which vertex do you want to start? ";
                unsigned startVertexBruteForce;
                std::cin >> startVertexBruteForce;
                std::cout << std::endl;
                bruteForce.CalculatePath(startVertexBruteForce);
                break;
            case 3:
                std::cout << "From which vertex do you want to start? ";
                unsigned startVertexHeldKarpProp;
                std::cin >> startVertexHeldKarpProp;
                std::cout << std::endl;
                heldKarp.CalculatePath(startVertexHeldKarpProp);
                break;
            case 4: {
                Test test;
                test.MakeTests();
                break;
            }
            default:
                break;
        }
    }
}
