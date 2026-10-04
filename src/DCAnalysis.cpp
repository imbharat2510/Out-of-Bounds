#include "DCAnalysis.h"

void DCAnalysis::run(Circuit& ckt) {
    nodeVoltages =simulator.solve(ckt,0.0);
}
