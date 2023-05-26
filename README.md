# Gormanium rush

## Deadlines

- Code due 12 noon Friday 26th May
- Video Presentation 4pm Friday 26th May

## Introducion
In this project, the focus is on designing an optimal circuit to separate valuable material called gerardium. The circuit consists of multiple units arranged in different configurations. The goal is to maximize gerardium recovery while balancing the purity of the final product.

The circuit produces a concentrate stream containing gerardium and a tailings stream with waste material. The success of the circuit is measured based on the concentrate stream, considering the price paid per kilogram of gerardium and penalties for waste material.

Due to the large number of potential circuit configurations, an optimization algorithm, specifically a genetic algorithm, will be used to find the best circuit design. The objective is to maximize gerardium recovery while considering the trade-off between recovery and purity, ultimately achieving the best economic outcome.

## Structure
- **acs-gerardium-rush-pentlandite**: The root directory of this project.
    - **src**: Contains the source code files.
        - CCircuit.cpp
        - CSimulator.cpp
        - CUnit.cpp
        - DirectedGraph.cpp
        - Genetic_Algorithm.cpp
        - main.cpp
        - CMakeLists.txt

    - **include**: Contains the header files.
        - CCircuit.h
        - CSimulator.h
        - CUnit.h
        - DirectedGraph.h
        - Genetic_Algorithm.h

    - **tests**: Contains the Unittest files.
        - test_CircuitEvaluation.cpp
        - test_CircuitValidity.cpp
        - test_Crossover.cpp
        - test_Cunit.cpp
        - test_DirectedGraph.cpp
        - test_GA.cpp
        - test_RandomCircuit.cpp
        - CMakeLists.txt

    - **docs**: Contains project documentation.
        - Doxyfile

    - **build**: Contains the resulting executable and other generated build files.

    - **.github**:
        - run_tests.yml

    - CMakeLists.txt: The CMake build configuration file for your project.

## Dependencies
- C++11 or later

## Building
1. Clone the repository
2. Ensure you have the required dependencies installed.

## Usage
To use program, follow these steps:
In the terminal, navigate to the root directory of your project and run the following command to do the recompile (optional) and run:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./bin/Circuit_Optimizer
```
or set the number of threads to run:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ OMP_NUM_THREADS=4 ./bin/Circuit_Optimizer
```

## Documentation
Documentation can be accessed by `docs/html/index.html`

Documentation includes: 
- Classes (Circuit, Circuit_Parameters, Stream, CUnit, DirectedGraph, DepthFirstSearch, Individual, Crossover, GA)
- File List

## License
This project is released under the **MIT License**.

## Thank you for using our program ! 

