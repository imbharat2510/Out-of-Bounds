#pragma once

#include <string>
#include <complex>
#include <cmath>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



class Component {
protected:
    std::string name;
    int nodeA, nodeB;   

public:
    Component(std::string n, int a, int b)
        : name(std::move(n)), nodeA(a), nodeB(b) {}

    virtual ~Component() = default;

    
    virtual std::complex<double> getImpedance(double frequency) const = 0;

    
    virtual double getValue() const = 0;

    
    virtual std::string getType() const = 0;

    std::string getName() const { return name; }
    int getNodeA() const { return nodeA; }
    int getNodeB() const { return nodeB; }
};


class Resistor : public Component {
    double resistance; 
public:
    Resistor(std::string n, int a, int b, double r)
        : Component(std::move(n), a, b), resistance(r) {}

    std::complex<double> getImpedance(double) const override {
        return {resistance, 0.0};              
    }
    double getValue() const override { return resistance; }
    std::string getType() const override { return "R"; }
};


class Inductor : public Component {
    double inductance; 
public:
    Inductor(std::string n, int a, int b, double l): Component(std::move(n), a, b), inductance(l) {}

    std::complex<double> getImpedance(double frequency) const override {
        return {0.0, 2.0 * M_PI * frequency * inductance};  
    }
    double getValue() const override { return inductance; }
    std::string getType() const override { return "L"; }
};


class Capacitor : public Component {
    double capacitance; 
public:
    Capacitor(std::string n, int a, int b, double c): Component(std::move(n), a, b), capacitance(c) {}

    std::complex<double> getImpedance(double frequency) const override {
        if (frequency == 0.0) {
            
            return {std::numeric_limits<double>::infinity(), 0.0};
        }
        return {0.0, -1.0 / (2.0 * M_PI * frequency * capacitance)}; 
    }
    double getValue() const override { return capacitance; }
    std::string getType() const override { return "C"; }
};


class VoltageSource : public Component {
    double voltage; 
    double frequency;
public:
    VoltageSource(std::string n, int a, int b, double v, double f = 0.0)
        : Component(std::move(n), a, b), voltage(v), frequency(f) {}

    std::complex<double> getImpedance(double) const override {
        return {0.0, 0.0};   
    }
    double getValue() const override { return voltage; }
    std::string getType() const override { return "V"; }
    double getFrequency() const { return frequency; }
};
