#include <vector>
#include <iostream>
#include <cmath>
#include "CSimulator.h"
#include "CCircuit.h"

void test()
{

    std::vector<int> circuit_vector = {0, 4, 6, 2, 4, 3, 4, 5, 4, 1, 0};
    bool validity = Circuit::Check_Validity(circuit_vector);
    Circuit circuit(circuit_vector);
    circuit.toFile();
    Circuit_Parameters parameters;
    parameters.tolerance = 1e-3;
    double value = Evaluate_Circuit(circuit_vector, parameters);
    std::cout << validity << " " << value << std::endl;
}

int main()
{
    test();
    std::vector<int> circuit_vector = {4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3};
    bool validity = Circuit::Check_Validity(circuit_vector);
    if (!validity)
    {
        return 1;
    }
    std::cout << "Circuit is " << validity << std::endl;
    Circuit_Parameters parameters;
    parameters.tolerance = 1e-6;
    double value = Evaluate_Circuit(circuit_vector, parameters);
    std::cout << value << std::endl;
    if (std::fabs(value - 107.204) > 1e-3)
    {
        return 1;
    }

    return 0;
}
