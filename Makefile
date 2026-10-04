all:
	g++ -std=c++17 -Wall -O2 -Iinclude src/main.cpp src/ACAnalysis.cpp src/DCAnalysis.cpp src/MNASimulator.cpp -o circuit_sim.exe

run: all
	.\circuit_sim.exe

clean:
	del circuit_sim.exe