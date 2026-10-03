#pragma once
#include "IAnalysis.h"
#include "ISimulator.h"
#include <vector>
#include <complex>

struct ACResult {
    double frequency;
    std::vector<std::complex<double>> nodeVoltages;
};
//ACAnalysis sweeps frequency from startfreq to endFreq in steps of Step.
//Calling the simulator once per frequency and storing every result.
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
