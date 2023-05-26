#include "CSimulator.h"
#include "CCircuit.h"
#include <iostream>
#include <vector>
#include <random>

std::vector<int> generateRandomVector(int size, int minRange, int maxRange)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(minRange, maxRange);

    std::vector<int> randomVector(size);

    for (int i = 0; i < size; i++)
    {
        randomVector[i] = dist(rng);
    }

    return randomVector;
}

int main()
{
    struct Circuit_Parameters default_circuit_parameters;
    bool validity = false;
    int feeder = 0;
    int num_units = 5;
    std::vector<int> circuit_vector;
    circuit_vector.push_back(feeder);
    while (!validity)
    {
        std::vector<int> cvec = generateRandomVector(2 * num_units, 0, num_units + 1);
        for (const auto &i : cvec)
        {
            circuit_vector.push_back(i);
        }
        validity = Circuit::Check_Validity(circuit_vector);
        if (!validity)
        {
            circuit_vector.clear();
            circuit_vector.push_back(feeder);
        }
    }
    double value = Evaluate_Circuit(circuit_vector, default_circuit_parameters);
    Circuit circuit(circuit_vector);
    circuit.toFile();
    std::cout << "Given test value:" << value << std::endl;
}