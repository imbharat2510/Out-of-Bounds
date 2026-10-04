#pragma once
#include "Circuit.h"

class IAnalysis {
public:
    virtual ~IAnalysis() =default;
    virtual void run(Circuit& ckt)= 0;
};
