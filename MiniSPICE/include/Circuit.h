#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include "Component.h"


class Circuit {
private:
    std::vector<std::unique_ptr<Component>> components;
    int numNodes;     
    int groundNode;

public:
    explicit Circuit(int nodes) : numNodes(nodes), groundNode(0) {}

    void addComponent(std::unique_ptr<Component> comp) {
        components.push_back(std::move(comp));
    }

    void listComponents() const {
        std::cout << "Circuit: " << numNodes << " nodes (node 0 = ground)\n";
        for (const auto& c : components) {
            std::cout << "  " << std::left << std::setw(2) << c->getType()
                      << " " << std::setw(6) << c->getName()
                      << " nodes(" << c->getNodeA() << "," << c->getNodeB() << ")"
                      << "  value=" << c->getValue() << "\n";
        }
    }

    int getNumNodes() const { return numNodes; }
    int getGroundNode() const { return groundNode; }
    const std::vector<std::unique_ptr<Component>>& getComponents() const { return components; }
};
