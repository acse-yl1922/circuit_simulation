#pragma once

/**
 * @file GeneticAlgorithm.hpp
 * @brief Header file for the GeneticAlgorithm class.
 *
 * @copyright Copyright (c) 2023
 *
 * Adaptive crossover and mutation rates are implemented according to the following paper:
 * Ref: Lin, W.Y., Lee, W.Y. and Hong, T.P., 2003. Adapting crossover and mutation rates in genetic algorithms. J. Inf. Sci. Eng., 19(5), pp.889-903.
 * This works efficiently for single threaded applications. For parallel applications, the crossover and mutation rates fixed. Can be made for parallel by reduction if time permits
 */

#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <tuple>
#include <functional>
#include <set>
#include <algorithm>
#include <chrono>
#include <omp.h>
#define PARALLEL
// #define SERIAL
// #define ADAPTIVE

// Random number generator
static std::random_device rd;
static std::mt19937 gen(rd()); // Initialize random number generator only once

/**
 * @fn RandomDouble
 * @brief Generates a random double value within the given range.
 *
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @return Random double value within the range.
 */
double RandomDouble(double min, double max);

/**
 * @fn RandomInt
 * @brief Generates a random integer value within the given range.
 *
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @return Random integer value within the range.
 */
int RandomInt(int min, int max);

/**
 * @class Individual
 * @brief Template class representing an individual in the GA population.
 *
 * @details An Individual consists of a chromosome (a vector of a specific type), representing its genetic information,
 * and a fitness value indicating the quality of the individual's solution.
 *
 * @tparam T The type of values in the chromosome.
 */
template <typename T>
class Individual
{
public:
    std::vector<T> chromosome; // Chromosome representing the genetic information
    double fitness;            // Fitness value indicating the quality of the individual's solution

    // overload comparison operator
    /**
     * @brief Default constructor for the Individual class.
     */
    Individual() {}

    /**
     * @brief Constructs an Individual with the given chromosome.
     *
     * @param chromosome The chromosome representing the genetic information.
     */
    Individual(const std::vector<T> &chromosome) : chromosome(chromosome) {}

    // overload comparison operator
    /**
     * @brief Overloads the less-than operator for comparing individuals based on fitness.
     *
     * @param ind The Individual to compare.
     * @return True if this Individual has a lower fitness value than the other Individual, false otherwise.
     */
    bool operator<(const Individual &ind) const
    {
        return fitness < ind.fitness;
    }

    /**
     * @brief Overloads the greater-than operator for comparing individuals based on fitness.
     *
     * @param ind The Individual to compare.
     * @return True if this Individual has a higher fitness value than the other Individual, false otherwise.
     */
    bool operator>(const Individual &ind) const
    {
        return fitness > ind.fitness;
    }

    /**
     * @brief Overloads the equality operator for comparing individuals based on their chromosomes.
     *
     * @param ind The Individual to compare.
     * @return True if the chromosomes of the two Individuals are equal, false otherwise.
     */
    bool operator==(const Individual &ind) const
    {
        return chromosome == ind.chromosome;
    }

    /**
     * @brief Overloads the assignment operator for assigning the values of another Individual to this Individual.
     *
     * @param ind The Individual to assign.
     * @return Reference to the assigned Individual.
     */
    Individual &operator=(const Individual &ind)
    {
        chromosome.resize(ind.chromosome.size());
        chromosome = ind.chromosome;
        fitness = ind.fitness;
        return *this;
    }
};

enum CrossoverType
{
    ONE_PT = 0,
    TWO_PTS = 1,
    UNIFORM = 2,
    MULT_PTS = 3
};

/**
 * @class Crossover
 * @brief Performs crossover operation between two parent individuals.
 *
 * @details  Functions perform crossover between two parent individuals, producing two child individuals.
 * The crossover operation combines genetic information from the parent individuals to create new individuals.
 *
 * @param parent1 The first parent individual.
 * @param parent2 The second parent individual.
 * @param child1 Reference to the first child individual to be created.
 * @param child2 Reference to the second child individual to be created.
 */
template <typename T>
class Crossover
{
public:
    static void one_pt_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2);
    static void two_pts_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2);
    static void uniform_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2);
    static void mult_pts_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2);
    static void crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2, CrossoverType type);
};

/**
 * @class GA
 * @brief Template class representing a Genetic Algorithm (GA).
 *
 * @details The GA class encapsulates the operations and parameters required to perform a GA optimization.
 *          The GA class is completely abstract and can be used for any optimization problem.
 *
 * @tparam T The type of values in the chromosome.
 */
template <typename T>
class GA
{
public:
    // Default constructor
    GA() = default;
    /**
     * @brief Constructor for the GA class.
     *
     * @param populationSize The size of the GA population.
     * @param chromosomeSize The size of the chromosome for each individual.
     * @param chromomin The minimum value of the discrete allele of a chromosome.
     * @param chromomax The maximum value of the discrete allele of a chromosome.
     * @param maxGeneration The maximum number of generations for the GA optimization.
     * @param crossoverRate The crossover rate (probability) for creating offspring.
     * @param mutationRate The mutation rate (probability) for mutating the chromosome.
     */
    GA(int populationSize, int chromosomeSize, T chromomin, T chromomax, int maxGeneration, double crossoverRate, double mutationRate);

    /**
     * @fn setValidity
     * @brief Sets a validity checker function for ensuring the validity of individuals.
     *
     * @details The validity checker function should take a const reference to the chromosome vector
     * and return true if the chromosome is valid, and false otherwise.
     *
     * @param validity The validity checker function.
     */
    void setValidity(const std::function<bool(const std::vector<T> &)> &validity);

    /**
     * @fn setFitness
     * @brief Sets a fitness function for evaluating the fitness of individuals.
     *
     * @details The fitness function should take a const reference to the chromosome vector
     * and return a fitness value (double) indicating the quality of the individual.
     *
     * @param fitness The fitness function.
     */
    void setFitness(const std::function<double(const std::vector<T> &)> &fitness);

    /**
     * @fn optimize
     * @brief Runs the GA optimization and returns the best individual found.
     *
     * @return The best individual (solution) found by the GA optimization.
     */
    Individual<T> optimize(CrossoverType type);

    /**
     * @brief Destructor for the GA class.
     */
    ~GA(){};

    T CHROMOMIN;           // Minimum value of discrete allele                                                                         // chromosome min
    T CHROMOMAX;           // Maximum value of discrete allele                                                                         // chromosome max
    int POP_SIZE;          // Population size
    int CHROMOSOME_SIZE;   // Chromosome size
    int MAX_GENERATION;    // Maximum generation
    double CROSSOVER_RATE; // Crossover rate
    double MUTATION_RATE;  // Mutation rate
    int ELITISM;           // Number of elite individuals
    int CONV_GEN;          // Number of generations for convergence
    bool verbose = false;  // Print progress to console

    std::vector<Individual<T>> population; // Population of individuals
    /**
     * @fn toFile
     * @brief Write the best individual to a file.
     *
     * @details This function grabs the best individual and write its fitness score to an output file.
     */
    void toFile(std::string fileDir);

    /**
     * @fn mutate
     * @brief Performs mutation operation on an individual.
     *
     * @details This function performs mutation on the given individual's chromosome.
     * Mutation introduces small random changes in the chromosome to explore new solutions.
     *
     * @param child The individual to mutate.
     */
    void mutate(Individual<T> &child);

    /**
     * @fn selectParent
     * @brief Selects a parent based on the cumulative fitness values.
     *
     * @details This function selects a parent individual from the population based on the cumulative fitness values.
     * The probability of selection is proportional to the fitness value of each individual.
     *
     * @param cf The cumulative fitness values of the population.
     * @return The index of the selected parent individual.
     */
    int selectParent(const std::vector<double> &cf);


private:
    // GA variables
    std::vector<Individual<T>> offspring;  // Offspring population
    std::vector<double> fitness;           // Fitness values of individuals
    std::vector<double> scaledFitness;     // Scaled fitness values of individuals
    std::vector<double> cumulativeFitness; // Cumulative fitness values of individuals
    int cross_cnt, mut_cnt;                // Crossover and mutation counters
    double cross_contrib, mut_contrib;     // Crossover and mutation contribution to the next generation
    std::vector<double> bestFitnessHistory;// Best fitness values of each generation

    // GA functions
    /**
     * @fn initPopulation
     * @brief Initializes the population with random individuals.
     *
     * @details This function creates the initial population for the GA optimization,
     * where each individual's chromosome is randomly generated within the specified range.
     */
    void initPopulation();

    /**
     * @fn updatePopulation
     * @brief Updates the population for the next generation.
     *
     * @details This function creates the population for the next generation of the GA optimization,
     * using the current population and the generated offspring.
     * It performs selection, crossover, and mutation operations to create new individuals.
     */
    void updatePopulation(CrossoverType type);

    /**
     * @brief
     *
     * @param k
     * @return int
     */
    int tournamentSelection(int k);




    /**
     * @brief Function object used for checking the validity of individuals in the Genetic Algorithm (GA).
     *
     * @details The validity function is a user-defined function that determines whether an individual's chromosome is valid or not.
     * It takes a const reference to the chromosome vector and returns a boolean value: true if the chromosome is valid,
     * and false otherwise.
     *
     * @tparam T The type of values in the chromosome.
     * @param chromosome The chromosome vector to be checked for validity.
     * @return True if the chromosome is valid, false otherwise.
     */
    std::function<bool(const std::vector<T> &)> validity_;

    /**
     * @brief Function object used for evaluating the fitness of individuals in the Genetic Algorithm (GA).
     *
     * @details The fitness function is a user-defined function that evaluates the fitness of an individual's chromosome,
     * indicating the quality of the individual's solution. It takes a const reference to the chromosome vector
     * and returns a fitness value (double).
     *
     * @tparam T The type of values in the chromosome.
     * @param chromosome The chromosome vector for which to evaluate the fitness.
     * @return The fitness value of the chromosome.
     */
    std::function<double(const std::vector<T> &)> fitness_;
};

// Declare the template class
template class GA<double>;
template class GA<int>;
template class Crossover<double>;
template class Crossover<int>;
