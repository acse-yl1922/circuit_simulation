#include <iostream>
#include <vector>
#include <cmath>
#include "Unittests.h"

bool validity(const std::vector<int> &chromosome)
{
    return true;
}

double Rosenbrock(const std::vector<int> &x)
{
    double fx = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i)
    {
        double term1 = std::pow(x[i + 1] - std::pow(x[i], 2), 2);
        double term2 = std::pow(1 - x[i], 2);
        fx += 100.0 * term1 + term2;
    }
    return 1.0 / (1.0 + fx); // turn a minimization problem into a maximization problem
}

UnitTestGA::~UnitTestGA(){};

void UnitTestGA::test_mutation() {
    std::cout << "============================Test Mutation============================\n";
    std::vector<int> init_chromosome = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Individual<int> init_individual(init_chromosome);
    Individual<int> result_individual(init_chromosome);
    ga->MUTATION_RATE = 0.5;

    ga->mutate(result_individual);
    assert(init_individual.chromosome != result_individual.chromosome);
    std::cout << "completed" << std::endl;
}

void UnitTestGA::test_select_parent() {
    std::cout << "============================Test Select Parent============================\n";
    int result;
    std::vector<double> comulative_fitness = {-1.0, 1.0, 1.0};

    result = ga->selectParent(comulative_fitness);
    assert(result == 1);
    std::cout << "completed" << std::endl;
}

void UnitTestGA::test_global_optimum() {
    std::cout << "============================Test Global Optimum============================\n";
    // configure GA
    ga->setValidity(validity);
    ga->setFitness(Rosenbrock);
    ga->verbose = false;
    CrossoverType type = CrossoverType::UNIFORM;
    // Known global optimum for Rosenbrock function
    std::vector<int> expected_chromosome = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int success_count = 0;
    int num_runs = 10;

    // Run for 10 times to see if the algorithm can find the global optimum
    // if 50% of the time can find the global optimum, then the test is passed
    for (int i = 0; i < num_runs; i++) {
        auto best = ga->optimize(type);
        if (best.chromosome == expected_chromosome) {
            success_count++;
        }
    }
    assert(success_count >= num_runs / 2);
    std::cout << "successfully found global optimum for Rosenbrock: " << success_count;
    std::cout << "/" << num_runs << " times!" << std::endl;
    std::cout << "completed" << std::endl;
}

void UnitTestGA::run() {
    test_mutation();
    test_select_parent();
    test_global_optimum();
}

