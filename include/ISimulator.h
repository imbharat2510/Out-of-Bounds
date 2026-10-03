#pragma once

#include <vector>
#include <complex>
#include "Circuit.h"

class ISimulator {
public:
    virtual ~ISimulator() = default;
    virtual std::vector<std::complex<double>> solve(const Circuit& circuit, double frequency) = 0;
};
