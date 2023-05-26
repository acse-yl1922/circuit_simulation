// #include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "GeneticAlgorithm.hpp"
#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"

/**
 * @brief a multidimensional Rosenbrock function used to test how good the 
 *        genetic algorithm is in finding a global optimal solution.
 *        The function has global minimum at x = (1, 1, ..., 1) with f(x) = 0
 * @param x a vector of integers, which can represent the chromosome of the Individual class
 * @return the value of the Rosenbrock function, which can represent the fitness of the Individual class
*/
double Rosenbrock(std::vector<int> x)
{
    double fx = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i)
    {
        double term1 = std::pow(x[i + 1] - std::pow(x[i], 2), 2);
        double term2 = std::pow(1 - x[i], 2);
        fx += 100.0 * term1 + term2;
    }
    return 1.0/(1.0 + fx);  // turn a minimization problem into a maximization problem
}

bool null_validity(const std::vector<int> &chromosome)
{
    return true;
}

struct CircuitOptimizer
{
    int feeder = 4;
    int num_units = 5;
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
    return Evaluate_Circuit(circuit_vector);
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
    // Define the ranges for the hyperparameters
    std::vector<double> PopulationRange = {1300, 1500};
    std::vector<double> MaxGenerationRange = {500, 1000};
    std::vector<double> MutationRateRange = {0.2, 0.01, 0.005};
    std::vector<double> CrossoverRateRange = {0.8, 0.95};
    std::vector<int> BestChromosome;
    CrossoverType type = CrossoverType::ONE_PT;

    double globalBestScore = 0.0;
    double bestPopulation = 0.0;
    double bestMaxGeneration = 0.0;
    double bestMutationRate = 0.0;
    double bestCrossoverRate = 0.0;
    Individual<int> bestIndividual;

    // Perform grid search
    for (const auto& pop_size : PopulationRange)
    {
        for (const auto& max_gen : MaxGenerationRange)
        {
            for (const auto& mutation_rate : MutationRateRange)
            {
                for (const auto& cross_rate : CrossoverRateRange)
                {
                                // Evaluate the model with the current hyperparameters
                                
                                // GA<int> ga(pop_size, 10, 0, 10, max_gen, cross_rate, mutation_rate);
                                GA<int> ga(pop_size, 
                                           2*myCircuit.num_units,
                                           0, 
                                           myCircuit.num_units+2, 
                                           max_gen, 
                                           cross_rate, 
                                           mutation_rate);
                                ga.CONV_GEN = max_gen;
                                ga.setValidity(null_validity);      // ga.setValidity(validity)
                                ga.setFitness(Rosenbrock);          // ga.setFitness(fitness);
                                bestIndividual = ga.optimize(type);
                                ga.toFile("../out/");
                                
                                // Update the best score and hyperparameters if necessary
                                if (bestIndividual.fitness > globalBestScore)
                                {
                                    globalBestScore = bestIndividual.fitness;
                                    bestPopulation = pop_size;
                                    bestMaxGeneration = max_gen;
                                    bestMutationRate = mutation_rate;
                                    bestCrossoverRate = cross_rate;
                                    BestChromosome = bestIndividual.chromosome;
                                }
                }
            }
            
        }
    }

    // Print the best hyperparameters and score
    std::cout << "Best Hyperparameters: \n" << std::endl;
    std::cout << "Population = " << bestPopulation << ", MaxGeneration = " << bestMaxGeneration;
    std::cout << ", MutationRate = " << bestMutationRate << ", CrossoverRate = " << bestCrossoverRate << std::endl;
    std::cout << "\nBest Score: " << globalBestScore << std::endl;
    std::cout << "\nBest Chromosome: " << std::endl;
    for (const auto& i : BestChromosome)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}

/*

g++ -std=c++17 -o ./utils/AE ./utils/AlgorithmEvaluator.cpp ./src/Individual.cpp ./src/Genetic_Algorithm.cpp

*/