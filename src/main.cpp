#include <iostream>
#include <chrono>
#include <omp.h>
#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"
#include "GeneticAlgorithm.hpp"

static Circuit_Parameters parameter;

struct CircuitOptimizer

{
    int feeder = 0;
    int num_units = 40;
};

CircuitOptimizer myCircuit;

double fitness(const std::vector<int> &x)
{
    std::vector<int> circuit_vector(x.size() + 1);
    circuit_vector[0] = myCircuit.feeder;
    for (int i = 0; i < x.size(); ++i)
    {
        circuit_vector[i + 1] = x[i];
    }
    return Evaluate_Circuit(circuit_vector, parameter);
}

bool validity(const std::vector<int> &x)
{
    std::vector<int> circuit_vector(x.size() + 1);
    circuit_vector[0] = myCircuit.feeder;
    for (int i = 0; i < x.size(); ++i)
    {
        circuit_vector[i + 1] = x[i];
    }

    return Circuit::Check_Validity(circuit_vector);
}

int main()
{
    /****************************  Analysis Settings ******************************/
    // Setup the circuit parameters
    parameter.input_Fger = 10.0;    /**< Input flow rate of the mineral*/
    parameter.input_Fwaste = 50.0; /**< Input flow rate of the waste*/
    parameter.get_money = 100.0;    /**< Money earned per unit of mineral*/
    parameter.lose_money = 500.0;   /**< Money lost per unit of waste*/
    parameter.tolerance = 1e-6;

    // Setup the GA
    int POP_SIZE = 400;
    int MAX_GENERATION = 2000;
    double CROSSOVER_RATE = 0.9;
    double MUTATION_RATE = 0.01;
    int CONV_GEN = MAX_GENERATION;
    int ELITE = POP_SIZE * 0.1;
    CrossoverType type = CrossoverType::UNIFORM;
    /*********************************************************************************/

    // Setup the cicuit optimiser
    CircuitOptimizer myCircuit;

    // Don't change these
    int CHROMOSOME_SIZE = 2 * myCircuit.num_units;
    int CHROMOMIN = 0;
    int CHROMOMAX = myCircuit.num_units + 2;

    GA<int> ga(POP_SIZE, CHROMOSOME_SIZE, CHROMOMIN, CHROMOMAX, MAX_GENERATION, CROSSOVER_RATE, MUTATION_RATE);
    ga.CONV_GEN = CONV_GEN;
    ga.ELITISM = ELITE;
    ga.setFitness(fitness);
    ga.setValidity(validity);

    double wtime = omp_get_wtime();
    auto bestIndividual = ga.optimize(type);
    wtime = omp_get_wtime() - wtime;
    std::cout << "Time taken: " << wtime << "s\n";

    ga.toFile("../out/");

    std::vector<int> bestCircuitVector = bestIndividual.chromosome;
    bestCircuitVector.insert(bestCircuitVector.begin(), myCircuit.feeder);

    Circuit bestCircuit(bestCircuitVector);

    bestCircuit.toFile();

    return 0;
}