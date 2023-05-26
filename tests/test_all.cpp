#include "Unittests.h"
#include "test_GeneticAlgorithm.cpp"
#include "test_Crossover.cpp"
#include "test_Circuit.cpp"


int main() {
    // set a random seed for consistent results
    srand(42); 

    int POPULATION_SIZE = 100;
    int CHROMOSOME_SIZE = 10;
    int CHROMOMIN = 0;
    int CHROMOMAX = 10;
    int MAX_GENERATION = 1500;
    int CONV_GEN = 1500;
    double CROSSOVER_RATE = 0.8;
    double MUTATION_RATE = 0.5;

    UnitTestGA test_ga(POPULATION_SIZE,
        CHROMOSOME_SIZE,
        CHROMOMIN,
        CHROMOMAX,
        MAX_GENERATION,
        CROSSOVER_RATE,
        MUTATION_RATE
    );
    test_ga.run();

    UnitTestCrossover x;
    x.test_one_pt_crossover();
    x.test_two_pts_crossover();
    x.test_mult_pts_crossover();
    x.test_uniform_crossover();

    UnitTestCircuit circuit;
    circuit.run();

    
}