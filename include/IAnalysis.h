#pragma once

#include "Circuit.h"

// ---------------------------------------------------------------------
// IAnalysis: decides WHAT question(s) to ask the simulator, and when.
// Sits one layer above ISimulator — it doesn't do any matrix math
// itself, it just calls solve() with the right frequency, once or many
// times, and holds onto the results.
// ---------------------------------------------------------------------
class IAnalysis {
public:
    virtual ~IAnalysis() = default;
    virtual void run(Circuit& ckt) = 0;
};
