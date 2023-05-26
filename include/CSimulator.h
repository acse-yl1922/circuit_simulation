#pragma once

/**
 * @file CSimulator.h
 * @author acse-sm222
 * @brief Simulator class
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <vector>
#include <float.h>
#include "CCircuit.h"

/**
 * @brief Function to evaluate the objective function
 *
 * @param circuit_vector Vector representing the separator circuit
 * @param parameters Structure containing the parameters for the circuit simulator
 * @return double Objective function value
 */
double Evaluate_Circuit(std::vector<int> circuit_vector, struct Circuit_Parameters parameters);

/**
 * @brief Overloaded function to evaluate the objective function
 *
 * @param circuit_vector Vector representing the separator circuit
 * @return double Objective function value
 */
double Evaluate_Circuit(std::vector<int> circuit_vector);
