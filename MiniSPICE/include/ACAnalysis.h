#pragma once

#include "IAnalysis.h"
#include "ISimulator.h"
#include <vector>
#include <complex>

// One frequency sweep point: the frequency, and the node voltages the
// simulator returned for it.
struct ACResult {
    double frequency;
    std::vector<std::complex<double>> nodeVoltages;
};

// ACAnalysis: sweeps frequency from startFreq to endFreq in steps of
// `step`, calling the simulator once per frequency and storing every
// result. This is the class a future BodeAnalysis/plotting feature
// would read its data from.
class ACAnalysis : public IAnalysis {
    ISimulator& simulator;
    double startFreq, endFreq, step;
    std::vector<ACResult> results;

public:
    ACAnalysis(ISimulator& sim, double start, double end, double stepSize)
        : simulator(sim), startFreq(start), endFreq(end), step(stepSize) {}

    void run(Circuit& ckt) override;

    const std::vector<ACResult>& getResults() const { return results; }
};
