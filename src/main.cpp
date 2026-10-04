#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include "ACAnalysis.h"
#include "Circuit.h"
#include "Component.h"
#include "DCAnalysis.h"
#include "MNASimulator.h"

static bool readInteger(const std::string& prompt, int& value) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!(std::cin >> input)) {
            return false;
        }

        std::istringstream parser(input);
        int parsedValue;
        char trailingCharacter;
        if ((parser >> parsedValue) && !(parser >> trailingCharacter)) {
            value = parsedValue;
            return true;
        }
        std::cout << "Please enter a whole number.\n";
    }
}

static bool readNode(const std::string& prompt, int numNodes, int otherNode, int& node) {
    while (true) {
        if (!readInteger(prompt, node)) {
            return false;
        }
        if (node < 0 || node >= numNodes) {
            std::cout << "Node must be between 0 and " << numNodes - 1 << ".\n";
        } else if (node == otherNode) {
            std::cout << "A component must connect two different nodes.\n";
        } else {
            return true;
        }
    }
}

static bool readFiniteValue(const std::string& prompt, double& value) {
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            if (std::cin.eof()) {
                return false;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number.\n";
        } else if (!std::isfinite(value)) {
            std::cout << "Value must be finite.\n";
        } else {
            return true;
        }
    }
}

static bool readPositiveValue(const std::string& prompt, double& value) {
    while (true) {
        if (!readFiniteValue(prompt, value)) {
            return false;
        }
        if (value > 0.0) {
            return true;
        }
        std::cout << "Value must be greater than zero.\n";
    }
}

static bool readNonNegativeValue(const std::string& prompt, double& value) {
    while (true) {
        if (!readFiniteValue(prompt, value)) {
            return false;
        }
        if (value >= 0.0) {
            return true;
        }
        std::cout << "Value must be zero or greater.\n";
    }
}

static std::string nextComponentName(const Circuit& circuit, const std::string& type) {
    int count = 0;
    for (const auto& component : circuit.getComponents()) {
        if (component->getType() == type) {
            ++count;
        }
    }
    return type + std::to_string(count + 1);
}

static bool addComponent(Circuit& circuit, int choice) {
    const std::string type = choice == 1 ? "R" : choice == 2 ? "L" :
                             choice == 3 ? "C" : "V";
    const std::string name = nextComponentName(circuit, type);
    int nodeA;
    int nodeB;
    double value = 0.0;

    std::cout << "\nAdding " << (choice == 1 ? "resistor" : choice == 2 ? "inductor" :
                                  choice == 3 ? "capacitor" : "voltage source")
              << " " << name << '\n';
    if (!readNode("Node A: ", circuit.getNumNodes(), -1, nodeA) ||
        !readNode("Node B: ", circuit.getNumNodes(), nodeA, nodeB)) {
        return false;
    }

    if (choice == 1) {
        if (!readPositiveValue("Resistance in ohms: ", value)) return false;
        circuit.addComponent(std::make_unique<Resistor>(name, nodeA, nodeB, value));
    } else if (choice == 2) {
        if (!readPositiveValue("Inductance in henries: ", value)) return false;
        circuit.addComponent(std::make_unique<Inductor>(name, nodeA, nodeB, value));
    } else if (choice == 3) {
        if (!readPositiveValue("Capacitance in farads: ", value)) return false;
        circuit.addComponent(std::make_unique<Capacitor>(name, nodeA, nodeB, value));
    } else {
        double frequency;
        if (!readFiniteValue("Voltage in volts: ", value) ||
            !readNonNegativeValue("Source frequency in Hz (0 for DC): ", frequency)) {
            return false;
        }
        circuit.addComponent(std::make_unique<VoltageSource>(name, nodeA, nodeB, value, frequency));
    }

    std::cout << "Added " << name << ".\n";
    return true;
}

static bool hasACVoltageSource(const Circuit& circuit) {
    for (const auto& component : circuit.getComponents()) {
        if (component->getType() == "V") {
            const auto* source = dynamic_cast<const VoltageSource*>(component.get());
            if (source->getFrequency() > 0.0) {
                return true;
            }
        }
    }
    return false;
}

static bool runAnalysis(Circuit& circuit) {
    MNASimulator simulator;
    std::cout << std::fixed << std::setprecision(4);
    try {
        if (!hasACVoltageSource(circuit)) {
            DCAnalysis analysis(simulator);
            analysis.run(circuit);
            std::cout << "\nDC node voltages:\n";
            const auto& voltages = analysis.getNodeVoltages();
            for (std::size_t node = 0; node < voltages.size(); ++node) {
                std::cout << "  V(node " << node << ") = " << voltages[node].real() << " V\n";
            }
            return true;
        }

        double startFrequency;
        double endFrequency;
        double step;
        if (!readNonNegativeValue("\nAC sweep start frequency in Hz: ", startFrequency) ||
            !readNonNegativeValue("AC sweep end frequency in Hz: ", endFrequency) ||
            !readPositiveValue("AC sweep step in Hz: ", step)) {
            return false;
        }
        while (endFrequency < startFrequency) {
            std::cout << "Sweep end frequency must be greater than or equal to the start frequency.\n";
            if (!readNonNegativeValue("AC sweep end frequency in Hz: ", endFrequency)) {
                return false;
            }
        }

        ACAnalysis analysis(simulator, startFrequency, endFrequency, step);
        analysis.run(circuit);
        std::cout << "\nAC frequency sweep (node voltage magnitude and phase):\n";
        for (const auto& point : analysis.getResults()) {
            std::cout << "  Frequency: " << point.frequency << " Hz\n";
            for (std::size_t node = 0; node < point.nodeVoltages.size(); ++node) {
                const auto& voltage = point.nodeVoltages[node];
                std::cout << "    V(node " << node << ") = " << std::abs(voltage)
                          << " V, phase = " << std::arg(voltage) * 180.0 / M_PI << " deg\n";
            }
        }
        return true;
    } catch (const std::exception& error) {
        std::cerr << "Analysis failed: " << error.what() << '\n';
        return false;
    }
}

int main() {
    int numNodes;
    if (!readInteger("Number of nodes (including ground node 0): ", numNodes)) {
        return 0;
    }
    while (numNodes < 2) {
        std::cout << "Enter at least 2 nodes: ground (0) and one circuit node.\n";
        if (!readInteger("Number of nodes (including ground node 0): ", numNodes)) {
            return 0;
        }
    }

    Circuit circuit(numNodes);
    bool running = true;
    while (running) {
        std::cout << "\nCircuit Builder\n"
                  << "  1. Add resistor\n"
                  << "  2. Add inductor\n"
                  << "  3. Add capacitor\n"
                  << "  4. Add voltage source\n"
                  << "  5. List components\n"
                  << "  0. Finish\n";

        int choice;
        if (!readInteger("Choose an option: ", choice)) {
            break;
        }

        switch (choice) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (!addComponent(circuit, choice)) {
                running = false;
            }
            break;
        case 5:
            circuit.listComponents();
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "Choose 0, 1, 2, 3, 4, or 5.\n";
            break;
        }
    }

    std::cout << "\nFinal circuit:\n";
    circuit.listComponents();
    return runAnalysis(circuit) ? 0 : 1;
}
