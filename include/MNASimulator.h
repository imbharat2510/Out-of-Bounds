#pragma once

#include "ISimulator.h"

class MNASimulator : public ISimulator {
public:
    std::vector<std::complex<double>> solve(const Circuit& circuit, double frequency) override;
};
