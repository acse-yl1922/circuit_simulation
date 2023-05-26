#include <vector>
#include <iostream>
#include <cmath>
#include "Unittests.h"

UnitTestCircuit::UnitTestCircuit() {}
UnitTestCircuit::~UnitTestCircuit() {}

void UnitTestCircuit::test_circuit_evaluation(){
    std::cout << "============================Test Circuit Evaluation============================\n";
    
    std::vector<int> circuit_vector = {4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3};
    bool validity = Circuit::Check_Validity(circuit_vector);
    assert(validity==true);
    
    Circuit_Parameters parameters;
    parameters.tolerance = 1e-6;
    
    double value = Evaluate_Circuit(circuit_vector, parameters);
    std::cout << value << std::endl;
    assert(std::fabs(value - 107.204) <= 1e-3);
    std::cout << "completed" << std::endl;
}

void UnitTestCircuit::test_circuit_validity(){
    std::cout << "============================Test Circuit Validity============================\n";
    
    // Cases when there are 3 units
    // Case 1: self-recycle
    std::vector<int> circuit_vector1 = {0, 1, 2, 0, 2, 1, 0};
    // Case 2: points to itself
    std::vector<int> circuit_vector2 = {0, 1, 2, 1, 2, 3, 4};
    // Case 3: there are units not ending up in 2 destinations
    std::vector<int> circuit_vector3 = {0, 1, 1, 3, 2, 1, 4};
    // Case 4: there is a unit not accessible from the feed
    std::vector<int> circuit_vector4 = {0, 1, 4, 3, 4, 3, 0};
    bool check1 = Circuit::Check_Validity(circuit_vector1);
    bool check2 = Circuit::Check_Validity(circuit_vector2);
    bool check3 = Circuit::Check_Validity(circuit_vector3);
    bool check4 = Circuit::Check_Validity(circuit_vector4);
    assert(check1==false);
    assert(check2==false);
    assert(check3==false);
    assert(check4==false);
    std::cout << "completed" << std::endl;
}

void UnitTestCircuit::test_circuit_unit() {
    std::cout << "============================Test Circuit Unit============================\n";
    
    CUnit test_unit;

    test_unit.input.ger = 20;
    test_unit.input.waste = 80;

    test_unit.calculateOutputFlowRates(test_unit.input);

    double eps = 1e-3;

    bool check1 = fabs(test_unit.conc.ger - 2.6087) < eps;
    bool check2 = fabs(test_unit.conc.waste - 1.18227) < eps;
    bool check3 = fabs(test_unit.tail.ger - 17.3913) < eps;
    bool check4 = fabs(test_unit.tail.waste - 78.8177) < eps;

    assert(check1==true);
    assert(check2==true);
    assert(check3==true);
    assert(check4==true);
    std::cout << "completed" << std::endl;
}

void UnitTestCircuit::run(){
    test_circuit_evaluation();
    test_circuit_validity();
    test_circuit_unit();
}

