#pragma once
#include "IAnalysis.h"
#include "ISimulator.h"
#include <vector>
#include <complex>
//DC analysis just calls simulator with freq=0 because DC is AC with frequency as zero 
class DCAnalysis : public IAnalysis {
    ISimulator& simulator;
    std::vector<std::complex<double>>nodeVoltages;
public:
    explicit DCAnalysis(ISimulator& sim) :simulator(sim) {}
    void run(Circuit& ckt) override;
    const std::vector<std::complex<double>>& getNodeVoltages() const { return nodeVoltages; }
};
