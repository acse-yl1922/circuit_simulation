#include <iostream>
#include <cassert>
#include <cmath>

#include "test_GeneticAlgorithm.h"

UnitTestIndividual::UnitTestIndividual() {
    std::cout << "Test Individual begins" << std::endl;
}
UnitTestIndividual::~UnitTestIndividual() {
    std::cout << "Test Individual completed" << std::endl;
}

void UnitTestIndividual::test_constructor() {
    int chromosomeLength = 10;
    individual = Individual(chromosomeLength, 0, 10);

    assert(individual.Get_vectorLength() == 10);
    assert(individual.chromosome.size() == 10);
}

// void UnitTestIndividual::test_calculateFitness(){
//     // set seed for randon
//     srand(42);
//     double expectedFitness = 119;
//     assert(individual.calculateFitness() == expectedFitness);
// }

void UnitTestIndividual::test_Get_Set_fitness() {
    std::vector<int> fitness_list = {10, 20, 30, 40, 50};

    for (int i = 0; i < fitness_list.size(); i++){
        individual.Set_fitness(fitness_list[i]);
        assert(individual.Get_fitness() == fitness_list[i]);
    }
}

void UnitTestIndividual::test_Get_vectorLength() {
    std::vector<int> vectorLength_list = {10, 100, 1000};

    for (int item : vectorLength_list){
        individual = Individual(item, 0, 10);
         assert(individual.Get_vectorLength() == item);
    }
}

int main (){
    UnitTestIndividual u;
    u.test_constructor();
    // u.test_calculateFitness();
    u.test_Get_Set_fitness();
    u.test_Get_vectorLength();
}
