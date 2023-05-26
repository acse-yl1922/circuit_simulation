#include <iostream>
#include <vector>
#include <cassert>
#include "GeneticAlgorithm.hpp"
#include "Unittests.h"

// Function for pretty printing a chromosome
template <typename T>
void printChromosome(const std::vector<T> &chromosome)
{
    std::cout << "{ ";
    for (const auto &gene : chromosome)
    {
        std::cout << gene << " ";
    }
    std::cout << "}\n";
}

Crossover<int> crossover;

UnitTestCrossover::UnitTestCrossover()
{
    std::cout << "                                  Test Crossover Start                                 \n";
}
UnitTestCrossover::~UnitTestCrossover() {}

void UnitTestCrossover::test_one_pt_crossover()
{
    std::cout << "============================Test One-Point Crossover method============================\n";

    std::vector<int> chromosome1 = {1, 2, 3, 4, 5};
    std::vector<int> chromosome2 = {6, 7, 8, 9, 10};

    Individual<int> parent1, parent2, child1, child2;

    parent1.chromosome = chromosome1;
    parent2.chromosome = chromosome2;

    child1.chromosome.resize(chromosome1.size());
    child2.chromosome.resize(chromosome2.size());

    crossover.one_pt_crossover(parent1, parent2, child1, child2);

    std::cout << "Parent 1: ";
    printChromosome(parent1.chromosome);

    std::cout << "Parent 2: ";
    printChromosome(parent2.chromosome);

    std::cout << "Child 1: ";
    printChromosome(child1.chromosome);

    std::cout << "Child 2: ";
    printChromosome(child2.chromosome);

    // Check that the chromosomes have been altered
    assert(child1.chromosome != parent1.chromosome);
    assert(child1.chromosome != parent2.chromosome);
    assert(child2.chromosome != parent1.chromosome);
    assert(child2.chromosome != parent2.chromosome);
}

void UnitTestCrossover::test_two_pts_crossover()
{
    std::cout << "============================Test Two-Points Crossover method============================\n";
    
    std::vector<int> chromosome1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> chromosome2 = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

    Individual<int> parent1, parent2, child1, child2;

    parent1.chromosome = chromosome1;
    parent2.chromosome = chromosome2;

    child1.chromosome.resize(chromosome1.size());
    child2.chromosome.resize(chromosome2.size());

    crossover.two_pts_crossover(parent1, parent2, child1, child2);

    std::cout << "Parent 1: ";
    printChromosome(parent1.chromosome);

    std::cout << "Parent 2: ";
    printChromosome(parent2.chromosome);

    std::cout << "Child 1: ";
    printChromosome(child1.chromosome);

    std::cout << "Child 2: ";
    printChromosome(child2.chromosome);

    // Check that the chromosomes have been altered
    assert(child1.chromosome != parent1.chromosome);
    assert(child1.chromosome != parent2.chromosome);
    assert(child2.chromosome != parent1.chromosome);
    assert(child2.chromosome != parent2.chromosome);
}

void UnitTestCrossover::test_mult_pts_crossover()
{
    std::cout << "===========================Test Multi-Points Crossover method===========================\n";
    
    std::vector<int> chromosome1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> chromosome2 = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

    Individual<int> parent1, parent2, child1, child2;

    parent1.chromosome = chromosome1;
    parent2.chromosome = chromosome2;

    child1.chromosome.resize(chromosome1.size());
    child2.chromosome.resize(chromosome2.size());

    crossover.mult_pts_crossover(parent1, parent2, child1, child2);

    std::cout << "Parent 1: ";
    printChromosome(parent1.chromosome);

    std::cout << "Parent 2: ";
    printChromosome(parent2.chromosome);

    std::cout << "Child 1: ";
    printChromosome(child1.chromosome);

    std::cout << "Child 2: ";
    printChromosome(child2.chromosome);

    // Check that the chromosomes have been altered
    assert(child1.chromosome != parent1.chromosome);
    assert(child1.chromosome != parent2.chromosome);
    assert(child2.chromosome != parent1.chromosome);
    assert(child2.chromosome != parent2.chromosome);
}

void UnitTestCrossover::test_uniform_crossover()
{
    std::cout << "=============================Test Uniform Crossover method==============================\n";
    
    std::vector<int> chromosome1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> chromosome2 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    std::vector<int> expected_chromosome1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> expected_chromosome2 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    Individual<int> parent1, parent2, child1, child2;

    parent1.chromosome = chromosome1;
    parent2.chromosome = chromosome2;

    child1.chromosome.resize(chromosome1.size());
    child2.chromosome.resize(chromosome2.size());

    crossover.uniform_crossover(parent1, parent2, child1, child2);

    // Check that the chromosomes have been altered
    assert(child1.chromosome != expected_chromosome1);
    assert(child2.chromosome != expected_chromosome2);
    std::cout << "Results match expected outputs.\n";
}
