# Out-of-Bounds

## Circuit Simulator

Out-of-Bounds is a C++ based circuit simulator developed as a group project. The project simulates electrical circuits by converting the circuit into a system of equations and solving them using **Modified Nodal Analysis (MNA)**.

The simulator supports both **DC and AC circuit analysis** and uses matrix-based methods to calculate the voltages and other circuit quantities.

## Features

- DC circuit analysis
- AC circuit analysis
- Modified Nodal Analysis (MNA)
- Matrix-based circuit solving
- Support for complex-valued calculations in AC analysis
- Separate modules for DC and AC analysis
- Command-line based interface
- Built using C++17

## How It Works

For a given circuit, the program first processes the circuit elements and their connections. It then constructs the required equations using Modified Nodal Analysis.

These equations are represented as:

```text
[A][x] = [b]
```

where:

- `A` is the circuit/system matrix
- `x` contains the unknown node voltages and currents
- `b` contains the known source values

The resulting system of equations is solved to obtain the circuit solution.

## Analysis Types

### DC Analysis

DC analysis is used to determine the steady-state behaviour of a circuit.

The simulator constructs the MNA system using the circuit components and solves for the node voltages.

### AC Analysis

AC analysis is used to analyse circuits containing AC sources and frequency-dependent components.

The circuit equations are represented using complex numbers, allowing the simulator to calculate both magnitude and phase of the circuit quantities.

## Project Structure

```text
Out-of-Bounds/
│
├── include/
│   └── Header files
│
├── src/
│   ├── main.cpp
│   ├── ACAnalysis.cpp
│   ├── DCAnalysis.cpp
│   └── MNASimulator.cpp
│
├── Makefile
├── circuit_sim.exe
└── README.md
```

### Source Files

**`main.cpp`**

Contains the main program and handles the execution of the simulator.

**`DCAnalysis.cpp`**

Contains the implementation related to DC circuit analysis.

**`ACAnalysis.cpp`**

Contains the implementation related to AC circuit analysis and complex-valued circuit calculations.

**`MNASimulator.cpp`**

Contains the core Modified Nodal Analysis implementation used to construct and solve the circuit equations.

**`include/`**

Contains the header files used by the different parts of the simulator.

## Requirements

- C++17 compatible compiler
- GNU Make

## Building the Project

Clone the repository:

```bash
git clone https://github.com/imbharat2510/Out-of-Bounds.git
```

Move into the project directory:

```bash
cd Out-of-Bounds
```

Build the project:

```bash
make
```

## Running

After building the project:

```bash
make run
```

Alternatively:

```bash
.\circuit_sim.exe
```

## Cleaning the Build

```bash
make clean
```

## Contributors

**Out-of-Bounds — C++ Group Project**
Aarya IE2025004 - AC and DC analysis, output handling ,testing 
Aditya BE2025001 - Logic, matrix solving, documentation 
Bharat BE2025007 - Logic, Forming equations and creating matrix, debugging
Mayank BE2025018 - Input handling and formatting, menu structure

Developed as a collaborative project to implement and understand the fundamentals of circuit simulation and Modified Nodal Analysis.