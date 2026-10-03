#pragma once

#include <vector>
#include <complex>
#include <stdexcept>
#include <cmath>

// ---------------------------------------------------------------------
// Small standalone utility: solves A x = b for complex A, b via Gaussian
// elimination with partial pivoting. Kept separate from MNASimulator so
// the matrix math isn't tangled up with circuit-specific stamping logic.
// ---------------------------------------------------------------------
namespace linalg {

using Complex = std::complex<double>;
using Matrix  = std::vector<std::vector<Complex>>;
using Vec     = std::vector<Complex>;

inline Vec solveLinear(Matrix A, Vec b) {
    const size_t n = A.size();

    for (size_t col = 0; col < n; ++col) {
        // Partial pivoting: swap in the row with the largest magnitude
        // entry in this column, for numerical stability.
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

} // namespace linalg
