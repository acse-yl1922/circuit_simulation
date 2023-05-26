#pragma once

/**
 * @file CCircuit.h
 * @author acse-sm222
 * @brief Circuit class
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <vector>
#include "CUnit.h"
#include "DirectedGraph.hpp"

/**
 * @brief A Structure to store the parameters for the circuit simulator
 *
 */
struct Circuit_Parameters
{
  double tolerance = 1e-6;     /**< Tolerance for simulator convergence*/
  int max_iterations = 1000;   /**< Maximum number of iterations*/
  double input_Fger = 10.0;    /**< Input flow rate of the mineral*/
  double input_Fwaste = 100.0; /**< Input flow rate of the waste*/
  double get_money = 100.0;    /**< Money earned per unit of mineral*/
  double lose_money = 500.0;   /**< Money lost per unit of waste*/
};

/**
 * @brief Circuit class
 * @details This class is used to store the circuit information
 * @author acse-sm222
 */
class Circuit
{
public:
  Circuit(std::vector<int> circuit_vector);                           /**< Constructor taking the circuit vector*/
  ~Circuit();                                                         /**< Destructor*/
  void toFile();                                                      /**< Function to write the circuit information to a file*/
  int num_units;                                                      /**< Number of units in the circuit*/
  int feeder;                                                         /**< Feeder unit*/
  std::vector<int> circuit_vector;                                    /**< Circuit vector*/
  std::vector<CUnit> units;                                           /**< Vector of Cunits*/
  static bool Check_Validity(const std::vector<int> &circuit_vector); /**< Function to check the validity of the circuit vector*/
  int it;                                                             /**< Number of iterations*/
};

/**
 * @brief Calculate the ger and waste rates in all streams of the circuit
 *
 * @param circuit_vector
 * @param parameters
 */
void Calculate_Circuit(Circuit &circuit, struct Circuit_Parameters parameters);

/**
 * @brief Overloaded function to calculate the ger and waste rates in all streams of the circuit
 *
 * @param circuit_vector
 */
void Calculate_Circuit(Circuit &circuit);
