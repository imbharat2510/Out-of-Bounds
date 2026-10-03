#pragma once

#include "ISimulator.h"

// ---------------------------------------------------------------------
// MNASimulator: Modified Nodal Analysis.
//
// Plain nodal analysis (KCL at every node) cannot represent an ideal
// voltage source directly, because a voltage source doesn't give you a
// current equation — it FIXES a node voltage. MNA fixes this by adding
// one extra unknown (the current through each voltage source) and one
// extra equation (the voltage constraint) per voltage source.
//
// Inductors need the same trick at DC: Z_L = jwL = 0 at w = 0, which is
// a short circuit (0V source) — not representable as a finite
// admittance, so it's handled as a branch equation too, exactly like a
// voltage source with value 0.
// ---------------------------------------------------------------------
class MNASimulator : public ISimulator {
public:
    std::vector<std::complex<double>> solve(const Circuit& circuit, double frequency) override;
};
