#include "ACAnalysis.h"

void ACAnalysis::run(Circuit& ckt) {
    results.clear();
    for (double f = startFreq; f <= endFreq + 1e-9; f += step) {
        results.push_back(ACResult{f, simulator.solve(ckt, f)});
    }
}
