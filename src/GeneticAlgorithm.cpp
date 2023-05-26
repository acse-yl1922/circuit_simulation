#include <fstream>
#include <sstream>
#include "GeneticAlgorithm.hpp"

// Generate a random double between min and max, exclusive of max
double RandomDouble(double min, double max)
{
    std::uniform_real_distribution<double> dist(min, max); // Define the range of random numbers
    return dist(gen);                                      // Generate and return a random number
}

// Generate a random integer between min and max, inclusive of max
int RandomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max); // Define the range of random numbers
    return dist(gen);                                  // Generate and return a random number
}

template <typename T>
GA<T>::GA(int populationSize, int chromosomeSize, T chromomin, T chromomax, int maxGeneration, double crossoverRate, double mutationRate)
{
    // GA parameters
    CHROMOMIN = chromomin;            // Minimum value of discrete allele
    CHROMOMAX = chromomax;            // Maximum value of discrete allele
    POP_SIZE = populationSize;        // Population size
    CHROMOSOME_SIZE = chromosomeSize; // Chromosome size
    MAX_GENERATION = maxGeneration;   // Maximum generation
    CROSSOVER_RATE = crossoverRate;   // Crossover rate
    MUTATION_RATE = mutationRate;     // Mutation rate
    ELITISM = 1;                      // Elitism fraction
    CONV_GEN = MAX_GENERATION;        // Convergence generation

    // GA variables
    population.resize(POP_SIZE);

    validity_ = [](const std::vector<T> &chromosome) -> bool
    { return true; };
    fitness_ = [](const std::vector<T> &chromosome) -> double
    { return 0; };
}

template <typename T>
void GA<T>::setValidity(const std::function<bool(const std::vector<T> &)> &validity)
{
    this->validity_ = validity;
}

template <typename T>
void GA<T>::setFitness(const std::function<double(const std::vector<T> &)> &fitness)
{
    this->fitness_ = fitness;
}

template <typename T>
void GA<T>::initPopulation()
{
    fitness.resize(POP_SIZE);

#ifdef PARALLEL
#pragma omp parallel for default(none) shared(std::cout, population, fitness) schedule(static)
#endif
    for (int i = 0; i < POP_SIZE; i++)
    {
        population[i].chromosome.resize(CHROMOSOME_SIZE);
        bool valid = false;
        while (!valid)
        {
            for (int j = 0; j < CHROMOSOME_SIZE; j++)
            {
                population[i].chromosome[j] = CHROMOMIN + (CHROMOMAX - CHROMOMIN) * RandomDouble(0.0, 1.0);
            }
            valid = validity_(population[i].chromosome);
        }
        if (verbose) std::cout << "\rGenerated: " << i << " of " << POP_SIZE << std::flush;
        population[i].fitness = fitness_(population[i].chromosome);
    }
    if (verbose) std::cout << std::endl;

    std::sort(population.begin(), population.end(), std::greater<Individual<T>>());
    for (int i = 0; i < POP_SIZE; i++)
    {
        fitness[i] = population[i].fitness;
    }

    std::sort(population.begin(), population.end(), std::greater<Individual<T>>());
    std::sort(fitness.begin(), fitness.end(), std::greater<double>());
}

template <typename T>
void GA<T>::updatePopulation(CrossoverType type)
{
    cross_cnt = 0;
    mut_cnt = 0;
    cross_contrib = 0.0;
    mut_contrib = 0.0;

    double minFitness = fitness[POP_SIZE - 1];
    double maxFitness = fitness[0];
    double offset = (minFitness < 0.0) ? -minFitness : 0.0;
    double totalFitness = 0.0;
    for (int i = 0; i < POP_SIZE; i++)
    {
        fitness[i] += offset;
        totalFitness += fitness[i];
    }
    scaledFitness.resize(POP_SIZE, 0.0);
    for (int i = 0; i < POP_SIZE; i++)
    {
        scaledFitness[i] = fitness[i] / totalFitness;
    }
    cumulativeFitness.resize(POP_SIZE, 0.0);
    cumulativeFitness[0] = scaledFitness[0];
    for (int i = 1; i < POP_SIZE; i++)
    {
        cumulativeFitness[i] = cumulativeFitness[i - 1] + scaledFitness[i];
    }

    int remaining = POP_SIZE;
    offspring.resize(POP_SIZE);
    fitness.resize(POP_SIZE);

    // Elitism
    for (int i = 0; i < ELITISM; ++i)
    {
        offspring[i] = population[i];
        fitness[i] = offspring[i].fitness;
        --remaining;
    }

#ifdef PARALLEL
#pragma omp parallel for default(none) shared(population, offspring, fitness, cumulativeFitness, type) reduction(+ : cross_cnt, mut_cnt, cross_contrib, mut_contrib) schedule(static)
    for (int i = ELITISM; i < POP_SIZE; ++i)
    {
        bool validChild1 = false;
        bool validChild2 = false;
        Individual<T> child1;
        child1.chromosome.resize(CHROMOSOME_SIZE);
        Individual<T> child2;
        child2.chromosome.resize(CHROMOSOME_SIZE);

        while (!validChild1 || !validChild2)
        {

            int parent1 = selectParent(cumulativeFitness);
            int parent2 = selectParent(cumulativeFitness);
            // int parent1 = tournamentSelection(10);
            // int parent2 = tournamentSelection(10);

            double crossoverProb = RandomDouble(0.0, 1.0);
            if (crossoverProb > CROSSOVER_RATE)
            {
                Crossover<T>::crossover(population[parent1], population[parent2], child1, child2, type);
#ifdef ADAPTIVE
                cross_cnt++;
                child1.fitness = fitness_(child1.chromosome);
                child2.fitness = fitness_(child2.chromosome);
                cross_contrib += child1.fitness + child2.fitness - population[parent1].fitness - population[parent2].fitness;
#endif
            }
            else
            {
                child1 = population[parent1];
                child2 = population[parent2];
            }
            double fit_before = child1.fitness + child2.fitness;
            mutate(child1);
            mutate(child2);
#ifdef ADAPTIVE
            child1.fitness = fitness_(child1.chromosome);
            child2.fitness = fitness_(child2.chromosome);
            mut_contrib += child1.fitness + child2.fitness - fit_before;
#endif
            validChild1 = validity_(child1.chromosome);
            validChild2 = validity_(child2.chromosome);
        }

        if (validChild1 && !validChild2)
        {
#ifndef ADAPTIVE
            child1.fitness = fitness_(child1.chromosome);
#endif
            offspring[i] = child1;
            fitness[i] = offspring[i].fitness;
        }
        if (validChild2 && !validChild1)
        {
#ifndef ADAPTIVE
            child2.fitness = fitness_(child2.chromosome);
#endif
            offspring[i] = child2;
            fitness[i] = offspring[i].fitness;
        }
        else
        {
#ifndef ADAPTIVE
            child1.fitness = fitness_(child1.chromosome);
            child2.fitness = fitness_(child2.chromosome);
#endif
            if (child1.fitness > child2.fitness)
            {
                offspring[i] = child1;
                fitness[i] = offspring[i].fitness;
            }
            else
            {
                offspring[i] = child2;
                fitness[i] = offspring[i].fitness;
            }
        }
    }

#endif

#ifdef SERIAL
    // Crossover and mutation
    while (remaining > 0)
    {
        bool validChild1 = false;
        Individual<T> child1;
        child1.chromosome.resize(CHROMOSOME_SIZE);

        bool validChild2 = false;
        Individual<T> child2;
        child2.chromosome.resize(CHROMOSOME_SIZE);

        int parent1 = selectParent(cumulativeFitness);
        int parent2 = selectParent(cumulativeFitness);
        // int parent1 = tournamentSelection(10);
        // int parent2 = tournamentSelection(10);

        double crossoverProb = RandomDouble(0.0, 1.0);
        if (crossoverProb > CROSSOVER_RATE)
        {
            Crossover<T>::crossover(population[parent1], population[parent2], child1, child2, type);
#ifdef ADAPTIVE
            cross_cnt++;
            child1.fitness = fitness_(child1.chromosome);
            child2.fitness = fitness_(child2.chromosome);
            cross_contrib += child1.fitness + child2.fitness - population[parent1].fitness - population[parent2].fitness;
#endif
        }
        else
        {
            child1 = population[parent1];
            child2 = population[parent2];
        }
        double fit_before = child1.fitness + child2.fitness;
        mutate(child1);
        mutate(child2);
#ifdef ADAPTIVE
        child1.fitness = fitness_(child1.chromosome);
        child2.fitness = fitness_(child2.chromosome);
        mut_contrib += child1.fitness + child2.fitness - fit_before;
#endif
        validChild1 = validity_(child1.chromosome);
        validChild2 = validity_(child2.chromosome);

        if (validChild1 && remaining > 0)
        {
#ifndef ADAPTIVE
            child1.fitness = fitness_(child1.chromosome);
#endif
            offspring[POP_SIZE - remaining] = child1;
            fitness[POP_SIZE - remaining] = offspring[POP_SIZE - remaining].fitness;
            --remaining;
        }
        if (validChild2 && remaining > 0)
        {
#ifndef ADAPTIVE
            child2.fitness = fitness_(child2.chromosome);
#endif
            offspring[POP_SIZE - remaining] = child2;
            fitness[POP_SIZE - remaining] = offspring[POP_SIZE - remaining].fitness;
            --remaining;
        }
    }
#endif

#ifdef ADAPTIVE
    double cm = cross_contrib / cross_cnt;
    double mm = mut_contrib / mut_cnt;
    double theta = 0.1 * (maxFitness - totalFitness / POP_SIZE) / (maxFitness - totalFitness);
    if (cm > 0.0)
    {
        CROSSOVER_RATE = std::min(1.0, CROSSOVER_RATE + theta);
        MUTATION_RATE = std::max(0.0, MUTATION_RATE - theta);
    }
    else
    {
        CROSSOVER_RATE = std::max(0.0, CROSSOVER_RATE - theta);
        MUTATION_RATE = std::min(1.0, MUTATION_RATE + theta);
    }
#endif
    // Update population
    population = offspring;

    std::sort(population.begin(), population.end(), std::greater<Individual<T>>());
    for (int i = 0; i < POP_SIZE; i++)
    {
        fitness[i] = population[i].fitness;
    }
}

template <typename T>
int GA<T>::tournamentSelection(int k)
{
    std::vector<int> candidates(k);
    for (int i = 0; i < k; ++i)
    {
        candidates[i] = RandomInt(0, POP_SIZE - 1);
    }
    Individual<T> best = population[candidates[0]];
    int bestIndex = candidates[0];
    for (int i = 1; i < k; ++i)
    {
        if (population[candidates[i]].fitness > best.fitness)
        {
            best = population[candidates[i]];
            bestIndex = candidates[i];
        }
    }

    return bestIndex;
}

template <typename T>
int GA<T>::selectParent(const std::vector<double> &cf)
{
    double p = RandomDouble(0.0, 1.0);
    for (int i = 0; i < POP_SIZE; i++)
    {
        if (p <= cf[i])
        {
            return i;
        }
    }
    return POP_SIZE - 1;
}

template <typename T>
void GA<T>::mutate(Individual<T> &child)
{
    for (int i = 0; i < CHROMOSOME_SIZE; i++)
    {
        double p = RandomDouble(0.0, 1.0);
        if (p <= MUTATION_RATE)
        {
            mut_cnt += 1;
            T randomGene;
            while ((randomGene = CHROMOMIN + (CHROMOMAX - CHROMOMIN) * RandomDouble(0.0, 1.0)) == child.chromosome[i])
                ;
            child.chromosome[i] = randomGene;
        }
    }
}

template <typename T>
Individual<T> GA<T>::optimize(CrossoverType type)
{
    Individual<T> bestIndividual = population[0];
    Individual<T> oldBestIndividual = bestIndividual;
    double bestFitness = bestIndividual.fitness;
    if (verbose) std::cout << "Initialising population..." << std::endl;
    initPopulation();
    int conv = 0;
    if (verbose) std::cout << "Starting evolution..." << std::endl;
    for (int i = 0; i < MAX_GENERATION; i++)
    {
        updatePopulation(type);
        bestIndividual = population[0];
        bestFitness = bestIndividual.fitness;
        if (verbose)
        {
        std::cout << "Generation: " << i << std::endl;
        std::cout << "Best chromosome: " << std::endl;
        
        for (int k = 0; k < CHROMOSOME_SIZE; ++k)
        {
            std::cout << bestIndividual.chromosome[k] << " ";
        }
        std::cout << " => " << bestIndividual.fitness << std::endl;
        std::cout << std::endl;
        std::cout << "Worst chromosome: " << std::endl;
        for (int k = 0; k < CHROMOSOME_SIZE; ++k)
        {
            std::cout << population[POP_SIZE - 1].chromosome[k] << " ";
        }
        std::cout << " => " << population[POP_SIZE - 1].fitness << std::endl;
        std::cout << std::endl;

        std::cout << "Adaptive Crossover Rate: " << CROSSOVER_RATE << std::endl;
        std::cout << "Adaptive Mutation Rate: " << MUTATION_RATE << std::endl;

        std::cout << "--------------------------" << std::endl;
        }

        if (oldBestIndividual == bestIndividual)
        {
            ++conv;
        }
        bestFitnessHistory.push_back(bestFitness);
        
        if (conv > CONV_GEN)
        {
            break; // early stopping criteria
        }
        oldBestIndividual = bestIndividual;
    }

    if (verbose) std::cout << "Finshed.." << std::endl;
    return bestIndividual;
}

// Generate a output file to store the results for each generation
template <typename T>
void GA<T>::toFile(std::string fileDir)
{
    std::stringstream fname;
    std::fstream f1;
    fname << fileDir << "GA_p" << std::to_string(POP_SIZE) << "_n" << std::to_string(CHROMOSOME_SIZE) << "_g" << std::to_string(MAX_GENERATION) << "_c" << std::to_string(CROSSOVER_RATE) << "_m" << std::to_string(MUTATION_RATE) << ".dat";
    std::cout << "Saving output to " << fname.str() << std::endl;
    f1.open(fname.str().c_str(), std::ios_base::out);

    // handle the case when the file cannot be opened
    if (f1.fail())
    {
        std::cout << "Error opening file! with name: " << std::endl;
        std::cout.flush();
        exit(0);
    }
    for (int it = 0; it < bestFitnessHistory.size(); ++it)
        f1 << it << "\t" << bestFitnessHistory[it] << std::endl;
    f1.close();
}

template <typename T>
void Crossover<T>::one_pt_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2)
{
    std::uniform_int_distribution<> distr(1, parent1.chromosome.size() - 1);

    int crossoverPoint = distr(gen);
    for (int i = 0; i < parent1.chromosome.size(); ++i)
    {
        if (i < crossoverPoint)
        {
            child1.chromosome[i] = parent1.chromosome[i];
            child2.chromosome[i] = parent2.chromosome[i];
        }
        else
        {
            child1.chromosome[i] = parent2.chromosome[i];
            child2.chromosome[i] = parent1.chromosome[i];
        }
    }
}

template <typename T>
void Crossover<T>::two_pts_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2)
{
    // Define two crossover points
    std::uniform_int_distribution<> distr(0, parent1.chromosome.size() - 1);

    int crossoverPoint1 = distr(gen);
    int crossoverPoint2 = distr(gen);

    while (crossoverPoint1 == crossoverPoint2 ||
           (crossoverPoint1 == 0 && crossoverPoint2 == parent1.chromosome.size() - 1) ||
           (crossoverPoint2 == 0 && crossoverPoint1 == parent1.chromosome.size() - 1))
    {
        crossoverPoint2 = distr(gen);
    };

    if (crossoverPoint1 > crossoverPoint2)
        std::swap(crossoverPoint1, crossoverPoint2);

    for (int i = 0; i < parent1.chromosome.size(); ++i)
    {
        if (i < crossoverPoint1 || i > crossoverPoint2)
        {
            child1.chromosome[i] = parent1.chromosome[i];
            child2.chromosome[i] = parent2.chromosome[i];
        }
        else
        {
            child1.chromosome[i] = parent2.chromosome[i];
            child2.chromosome[i] = parent1.chromosome[i];
        }
    }
};

template <typename T>
void Crossover<T>::mult_pts_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2)
{
    int chromosomeLength = parent1.chromosome.size();
    std::uniform_int_distribution<> distr(0, parent1.chromosome.size() - 1);

    // Generate a number of crossover points
    int numCrossoverPoints = 4;

    // Generate crossover points
    std::set<int> crossoverPoints;
    while (crossoverPoints.size() < numCrossoverPoints)
    {
        crossoverPoints.insert(distr(gen));
    }
    crossoverPoints.insert(chromosomeLength);

    // Perform crossover
    bool sw = false;
    auto startIter = crossoverPoints.begin();
    for (int i = 0; i < chromosomeLength; ++i)
    {
        // If i is in crossoverPoints, switch genes
        if (i == *startIter)
        {
            sw = !sw;
            ++startIter;
        }
        if (!sw)
        {
            child1.chromosome[i] = parent1.chromosome[i];
            child2.chromosome[i] = parent2.chromosome[i];
        }
        else
        {
            child1.chromosome[i] = parent2.chromosome[i];
            child2.chromosome[i] = parent1.chromosome[i];
        }
    }
};

template <typename T>
void Crossover<T>::uniform_crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2)
{
    double crossoverRate = 0.5;
    double r;
    for (int i = 0; i < parent1.chromosome.size(); ++i)
    {
        r = RandomDouble(0, 1);
        if (r < crossoverRate)
        {
            child1.chromosome[i] = parent1.chromosome[i];
            child2.chromosome[i] = parent2.chromosome[i];
        }
        else
        {
            child1.chromosome[i] = parent2.chromosome[i];
            child2.chromosome[i] = parent1.chromosome[i];
        }
    }
};

template <typename T>
void Crossover<T>::crossover(Individual<T> &parent1, Individual<T> &parent2, Individual<T> &child1, Individual<T> &child2, CrossoverType type)
{
    switch (type)
    {

    case CrossoverType::ONE_PT:
        one_pt_crossover(parent1, parent2, child1, child2);
        break;

    case CrossoverType::TWO_PTS:
        two_pts_crossover(parent1, parent2, child1, child2);
        break;

    case CrossoverType::MULT_PTS:
        mult_pts_crossover(parent1, parent2, child1, child2);
        break;

    case CrossoverType::UNIFORM:
        uniform_crossover(parent1, parent2, child1, child2);
        break;
    default:
        one_pt_crossover(parent1, parent2, child1, child2);
    };
}
