#pragma once

#include <vector>
#include <complex>

namespace linalg {
    using Complex = std::complex<double>;
    using Matrix  = std::vector<std::vector<Complex>>;
    using Vec     = std::vector<Complex>;
}