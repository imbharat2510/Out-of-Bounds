#pragma once

#include "IAnalysis.h"
#include "ISimulator.h"
#include <vector>
#include <complex>

// DCAnalysis: just calls the simulator once, at frequency = 0.
// DC is treated as AC at frequency zero, not as a separate code path.
class DCAnalysis : public IAnalysis {
    ISimulator& simulator;
    std::vector<std::complex<double>> nodeVoltages;

public:
    explicit DCAnalysis(ISimulator& sim) : simulator(sim) {}

    void run(Circuit& ckt) override;

    const std::vector<std::complex<double>>& getNodeVoltages() const { return nodeVoltages; }
};
