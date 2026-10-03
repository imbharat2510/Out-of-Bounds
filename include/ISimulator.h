#pragma once

#include <vector>
#include <complex>
#include "Circuit.h"

// ---------------------------------------------------------------------
// ISimulator: answers exactly one question — "given this circuit, what
// are the node voltages at this one frequency?" Nothing above this
// interface (Circuit, IAnalysis) needs to know HOW that's computed.
// ---------------------------------------------------------------------
class ISimulator {
public:
    virtual ~ISimulator() = default;

    // Returns node voltages indexed 0..numNodes-1 (index 0 = ground = 0).
    virtual std::vector<std::complex<double>> solve(const Circuit& circuit, double frequency) = 0;
};
