#include "MNASimulator.h"
#include "LinearSolver.h"

using linalg::Complex;
using linalg::Matrix;
using linalg::Vec;

std::vector<std::complex<double>> MNASimulator::solve(const Circuit& circuit, double frequency) {
    const int numNodes = circuit.getNumNodes();   // includes ground (node 0)
    const int ground    = circuit.getGroundNode();
    const int N          = numNodes - 1;          // unknowns: one per non-ground node

    const auto& comps = circuit.getComponents();

    // Every VoltageSource, and every Inductor when frequency == 0 (DC short),
    // needs its own extra current unknown + constraint equation.
    std::vector<const Component*> branchComponents;
    for (const auto& c : comps) {
        if (c->getType() == "V") {
            branchComponents.push_back(c.get());
        } else if (c->getType() == "L" && frequency == 0.0) {
            branchComponents.push_back(c.get());
        }
    }
    const int M = static_cast<int>(branchComponents.size());
    const int size = N + M;

    // Node index -> matrix row/col (node 0 = ground is never a row/col).
    auto idx = [&](int node) { return node - 1; };

    std::vector<std::complex<double>> result(numNodes, Complex(0.0, 0.0));
    if (size == 0) return result;   // trivial / empty circuit

    Matrix A(size, Vec(size, Complex(0.0, 0.0)));
    Vec b(size, Complex(0.0, 0.0));

    // --- Stamp admittance-based components (R, C, and L when freq > 0) ---
    for (const auto& c : comps) {
        const std::string type = c->getType();
        if (type == "V") continue;
        if (type == "L" && frequency == 0.0) continue;   // handled as a branch below

        Complex Y;
        if (type == "C" && frequency == 0.0) {
            Y = Complex(0.0, 0.0);              // open circuit at DC
        } else {
            Y = Complex(1.0, 0.0) / c->getImpedance(frequency);
        }

        const int a = c->getNodeA();
        const int bN = c->getNodeB();

        if (a != ground)  A[idx(a)][idx(a)]  += Y;
        if (bN != ground) A[idx(bN)][idx(bN)] += Y;
        if (a != ground && bN != ground) {
            A[idx(a)][idx(bN)] -= Y;
            A[idx(bN)][idx(a)] -= Y;
        }
    }

    // --- Stamp branch (voltage-constrained) components ---
    for (int k = 0; k < M; ++k) {
        const Component* c = branchComponents[k];
        const int row = N + k;
        const int a = c->getNodeA();
        const int bN = c->getNodeB();

        // Inductor-as-DC-short is a 0V constraint; a real VoltageSource uses its value.
        const double value = (c->getType() == "L") ? 0.0 : c->getValue();

        if (a != ground)  { A[idx(a)][row]  += 1.0; A[row][idx(a)]  += 1.0; }
        if (bN != ground) { A[idx(bN)][row] -= 1.0; A[row][idx(bN)] -= 1.0; }
        b[row] = Complex(value, 0.0);
    }

    Vec x = linalg::solveLinear(A, b);

    for (int n = 1; n < numNodes; ++n) {
        result[n] = x[idx(n)];
    }
    return result; // result[0] (ground) stays 0
}
