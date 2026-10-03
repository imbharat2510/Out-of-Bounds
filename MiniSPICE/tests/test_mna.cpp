// Minimal assert-based sanity tests for MNASimulator.
// No test framework dependency — keeps the build simple. Swap in
// Catch2/GoogleTest later if you want nicer reporting.

#include <cassert>
#include <cmath>
#include <iostream>
#include "Circuit.h"
#include "Component.h"
#include "MNASimulator.h"
#include "DCAnalysis.h"

static bool approxEqual(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

static void test_voltage_divider() {
    Circuit c(3);
    c.addComponent(std::make_unique<VoltageSource>("V1", 1, 0, 10.0));
    c.addComponent(std::make_unique<Resistor>("R1", 1, 2, 1000.0));
    c.addComponent(std::make_unique<Resistor>("R2", 2, 0, 1000.0));

    MNASimulator sim;
    DCAnalysis dc(sim);
    dc.run(c);

    const auto& v = dc.getNodeVoltages();
    assert(approxEqual(v[1].real(), 10.0));
    assert(approxEqual(v[2].real(), 5.0));
    std::cout << "[PASS] test_voltage_divider\n";
}

static void test_unequal_divider() {
    // R1=1k, R2=3k -> V(node2) should be 10 * 3000/4000 = 7.5V
    Circuit c(3);
    c.addComponent(std::make_unique<VoltageSource>("V1", 1, 0, 10.0));
    c.addComponent(std::make_unique<Resistor>("R1", 1, 2, 1000.0));
    c.addComponent(std::make_unique<Resistor>("R2", 2, 0, 3000.0));

    MNASimulator sim;
    DCAnalysis dc(sim);
    dc.run(c);

    const auto& v = dc.getNodeVoltages();
    assert(approxEqual(v[2].real(), 7.5));
    std::cout << "[PASS] test_unequal_divider\n";
}

static void test_capacitor_open_at_dc() {
    // A capacitor to ground draws no DC current, so with only a resistor
    // in series and no other path, node2 should float to the source
    // voltage (no current flows, so no drop across R1).
    Circuit c(3);
    c.addComponent(std::make_unique<VoltageSource>("V1", 1, 0, 5.0));
    c.addComponent(std::make_unique<Resistor>("R1", 1, 2, 1000.0));
    c.addComponent(std::make_unique<Capacitor>("C1", 2, 0, 1e-6));

    MNASimulator sim;
    DCAnalysis dc(sim);
    dc.run(c);

    const auto& v = dc.getNodeVoltages();
    assert(approxEqual(v[2].real(), 5.0));
    std::cout << "[PASS] test_capacitor_open_at_dc\n";
}

int main() {
    test_voltage_divider();
    test_unequal_divider();
    test_capacitor_open_at_dc();
    std::cout << "All tests passed.\n";
    return 0;
}
