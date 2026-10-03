#include <iostream>
#include <iomanip>
#include <cmath>
#include "Circuit.h"
#include "Component.h"
#include "MNASimulator.h"
#include "DCAnalysis.h"
#include "ACAnalysis.h"

static void printDC(const std::vector<std::complex<double>>& v) {
    for (size_t n = 0; n < v.size(); ++n) {
        std::cout << "  V(node " << n << ") = " << v[n].real() << " V\n";
    }
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    
    std::cout << " Demo 1: DC Voltage Divider \n";
    {
        Circuit divider(3); 
        divider.addComponent(std::make_unique<VoltageSource>("V1", 1, 0, 10.0));
        divider.addComponent(std::make_unique<Resistor>("R1", 1, 2, 1000.0));
        divider.addComponent(std::make_unique<Resistor>("R2", 2, 0, 1000.0));

        divider.listComponents();

        MNASimulator simulator;
        DCAnalysis dc(simulator);
        dc.run(divider);

        std::cout << "DC node voltages:\n";
        printDC(dc.getNodeVoltages());
    }

    
    std::cout << "\n Demo 2: AC RC Low-Pass Filter \n";
    {
        Circuit rc(3);
        rc.addComponent(std::make_unique<VoltageSource>("V1", 1, 0, 1.0));
        rc.addComponent(std::make_unique<Resistor>("R1", 1, 2, 1000.0));
        rc.addComponent(std::make_unique<Capacitor>("C1", 2, 0, 1e-6));

        rc.listComponents();

        MNASimulator simulator;
        ACAnalysis ac(simulator, 50.0,2000.0,250.0);
        ac.run(rc);

        std::cout << "Frequency sweep (output = node 2):\n";
        std::cout << "  " << std::setw(10) << "Freq(Hz)"<< std::setw(14) << "|V(node2)|"<< std::setw(14) << "Phase(deg)\n";
        for (const auto& point : ac.getResults()) {
            std::complex<double> v = point.nodeVoltages[2];
            double mag = std::abs(v);
            double phaseDeg = std::arg(v) * 180.0 / M_PI;
            std::cout << "  " << std::setw(10) << point.frequency<< std::setw(14) << mag<< std::setw(14) << phaseDeg << "\n";
        }
    }

    return 0;
}
