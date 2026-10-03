#include "MNASimulator.h"
#include "LinearSolver.h"
#include <cmath>
#include <stdexcept>
#include <utility>

using linalg::Complex;
using linalg::Matrix;
using linalg::Vec;

Vec solveLinear(Matrix A, Vec b) {
    const size_t n = A.size();

    for (size_t col = 0; col < n; ++col) {
        size_t pivotRow = col;
        double best = std::abs(A[col][col]);
        for (size_t r = col + 1; r < n; ++r) {
            double mag = std::abs(A[r][col]);
            if (mag > best) { best = mag; pivotRow = r; }
        }

        if (best < 1e-12) {
            throw std::runtime_error(
                "Singular matrix: the circuit is ill-formed (a floating node, "
                "a missing ground reference, or a loop of only voltage sources).");
        }

        std::swap(A[col], A[pivotRow]);
        std::swap(b[col], b[pivotRow]);

        for (size_t r = col + 1; r < n; ++r) {
            Complex factor = A[r][col] / A[col][col];
            for (size_t c = col; c < n; ++c) {
                A[r][c] -= factor * A[col][c];
            }
            b[r] -= factor * b[col];
        }
    }

    // Back substitution
    Vec x(n);
    for (size_t ri = n; ri-- > 0; ) {
        Complex sum = b[ri];
        for (size_t c = ri + 1; c < n; ++c) {
            sum -= A[ri][c] * x[c];
        }
        x[ri] = sum / A[ri][ri];
    }
    return x;
}

std::vector<std::complex<double>> MNASimulator::solve(const Circuit& circuit, double frequency) {
    const int numNodes = circuit.getNumNodes();   
    const int ground    = circuit.getGroundNode();
    const int N          = numNodes - 1;         

    const auto& comps = circuit.getComponents();

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

    auto idx = [&](int node) { return node - 1; };

    std::vector<std::complex<double>> result(numNodes, Complex(0.0, 0.0));
    if (size == 0) return result; 

    Matrix A(size, Vec(size, Complex(0.0, 0.0)));
    Vec b(size, Complex(0.0, 0.0));

    //*Stamping admittance
    for (const auto& c : comps) {
        const std::string type = c->getType();
        if (type == "V") continue;
        if (type == "L" && frequency == 0.0) continue;

        Complex Y;
        if (type == "C" && frequency == 0.0) {
            Y = Complex(0.0, 0.0);              
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

    //*Stamping branch voltage components
    for (int k = 0; k < M; ++k) {
        const Component* c = branchComponents[k];
        const int row = N + k;
        const int a = c->getNodeA();
        const int bN = c->getNodeB();

        const double value = (c->getType() == "L") ? 0.0 : c->getValue();

        if (a != ground)  { A[idx(a)][row]  += 1.0; A[row][idx(a)]  += 1.0; }
        if (bN != ground) { A[idx(bN)][row] -= 1.0; A[row][idx(bN)] -= 1.0; }
        b[row] = Complex(value, 0.0);
    }

    Vec x = solveLinear(A, b);

    for (int n = 1; n < numNodes; ++n) {
        result[n] = x[idx(n)];
    }
    return result; 
}