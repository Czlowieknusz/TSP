//
// Created by Jan Woźniak on 05.11.2018.
//

#ifndef PEA_TIMER_H
#define PEA_TIMER_H

#include <windows.h>

class Timer {
public:
    void StartCounter();
    double GetCounter();

    double PCFreq = 0.0;
    __int64 CounterStart = 0;
};

#endif //PEA_TIMER_H
