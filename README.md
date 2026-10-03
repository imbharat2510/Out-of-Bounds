# MiniSPICE

A small Object-Oriented circuit construction and simulation library in C++.
Model Resistors, Inductors, Capacitors and ideal Voltage Sources as objects,
wire them into a `Circuit` via numbered nodes, and simulate DC and AC
steady-state behaviour.

## Core design idea

Construction and Simulation are deliberately separate responsibilities:

- `Component` / `Resistor` / `Inductor` / `Capacitor` / `VoltageSource` and
  `Circuit` only know how to describe a circuit — they have no idea how to
  solve one.
- `ISimulator` (implemented by `MNASimulator`) only knows how to solve a
  circuit at a single frequency — it doesn't decide what frequencies to try.
- `IAnalysis` (implemented by `DCAnalysis` and `ACAnalysis`) decides what
  question to ask the simulator, and when (once, for DC; many times across a
  sweep, for AC).

See `docs/` for the class diagram and design rationale.

## Why Modified Nodal Analysis (MNA)

Plain nodal analysis can't represent an ideal voltage source directly — a
voltage source *fixes* a node voltage rather than giving a current equation.
`MNASimulator` uses MNA: one extra unknown (branch current) and one extra
equation (the voltage constraint) per voltage source. Inductors get the same
treatment at DC, since `Z_L = jwL = 0` at `w = 0` is a short circuit, which
can't be expressed as a finite admittance either.

## Build

Requires a C++17 compiler.

```bash
mkdir build && cd build
cmake ..
make
./minispice
```

If you don't have CMake available, you can compile directly:

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/MNASimulator.cpp src/DCAnalysis.cpp src/ACAnalysis.cpp -o minispice
./minispice
```

## Run the tests

```bash
g++ -std=c++17 -Iinclude tests/test_mna.cpp src/MNASimulator.cpp src/DCAnalysis.cpp -o test_mna
./test_mna
```

## What the demo does

`src/main.cpp` builds two circuits in code (no file-based netlist — that's
intentionally out of scope for this version):

1. **DC voltage divider** — two 1k resistors and a 10V source; confirms the
   midpoint sits at 5V.
2. **AC RC low-pass filter** — a 1k resistor and 1uF capacitor; sweeps
   frequency and prints the output node's magnitude and phase, showing the
   expected low-pass roll-off.

## Project structure

```
include/     Component.h, Circuit.h, ISimulator.h, MNASimulator.h,
             IAnalysis.h, DCAnalysis.h, ACAnalysis.h, Linalg.h
src/         MNASimulator.cpp, DCAnalysis.cpp, ACAnalysis.cpp, main.cpp
tests/       test_mna.cpp (assert-based sanity checks)
docs/        class diagram, design notes
```

## Team

_Add team member names here._

## Status / next steps

Base DC + AC simulation per the project brief. A Bode plot feature
(magnitude/phase vs. frequency, visualized) is planned as the team's
creative extension.
