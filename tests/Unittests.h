#pragma once
#include <cstdlib>
#include <cassert>
#include "CSimulator.h"
#include "CCircuit.h"
#include "CUnit.h"
#include "GeneticAlgorithm.hpp"


class UnitTestGA
{
public:
    // SetUp
    int POPULATION_SIZE = 100;
    int CHROMOSOME_SIZE = 10;
    int CHROMOMIN = 0;
    int CHROMOMAX = 10;
    int MAX_GENERATION = 2000;
    int CONV_GEN = 2000;
    double CROSSOVER_RATE = 0.8;
    double MUTATION_RATE = 0.15;

    GA<int> *ga = new GA<int>(POPULATION_SIZE,
                              CHROMOSOME_SIZE,
                              CHROMOMIN,
                              CHROMOMAX,
                              MAX_GENERATION,
                              CROSSOVER_RATE,
                              MUTATION_RATE
                            );

    /**
     * @brief  Constructor for the UnitTestGeneticAlgorithm class.
     */
    UnitTestGA(int populationSize, int chromosomeSize, int chromomin, int chromomax,
               int maxGeneration, double crossoverRate, double mutationRate)
              : POPULATION_SIZE(populationSize), CHROMOSOME_SIZE(chromosomeSize), CHROMOMIN(chromomin),
                CHROMOMAX(chromomax), MAX_GENERATION(maxGeneration), CROSSOVER_RATE(crossoverRate), 
                MUTATION_RATE(mutationRate) {};


    /**
     * @brief Destructor for the UnitTestGeneticAlgorithm class..
     */
    ~UnitTestGA();

    void test_mutation();
    void test_select_parent();

    /**
     * @brief Test with Rosenbrock function to check if the algorithm is capable in 
     *        finding the global optimum successfully.
    */
    void test_global_optimum();

    void run();

};

class UnitTestIndividual
{
public:
    /**
     * @brief Default constructor.
     */
    UnitTestIndividual();
    /**
     * @brief Destructor.
     */
    ~UnitTestIndividual();

    /**
     * @brief Test the constructor and getter functions.
     */
    void test_constructor();

    /**
     * @brief Test the calculate fitness value function.
     */
    // void test_calculateFitness();

    /**
     * @brief Test the getter and setter function for fitness.
     */
    void test_Get_Set_fitness();

    /**
     * @brief Test the getter function for num_units.
     */
    void test_Get_numUnits();

    /**
     * @brief Test the getter function for vector_length.
     */
    void test_Get_vectorLength();

protected:
    Individual<int> individual;
};

class UnitTestCrossover
{
public:
    /**
     * @brief Default constructor.
     */
    UnitTestCrossover();
    /**
     * @brief Destructor.
     */
    ~UnitTestCrossover();

    /**
     * @brief .
     */
    void test_one_pt_crossover();

    /**
     * @brief .
     */
    void test_two_pts_crossover();

    /**
     * @brief .
     */
    void test_mult_pts_crossover();

    /**
     * @brief .
     */
    void test_uniform_crossover();
};

class UnitTestCircuit
{
public:
    /**
     * @brief Default constructor.
     */
    UnitTestCircuit();
    /**
     * @brief Destructor.
     */
    ~UnitTestCircuit();

    void test_circuit_evaluation();
    void test_circuit_validity();
    void test_circuit_unit();
    void run();
};
