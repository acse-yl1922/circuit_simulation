/**
 * @file CSimulator.cpp
 * @author acse-sm222
 * @brief Simulator calculations
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <cmath>
#include <cassert>
#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"

/**
 * @brief Calculates the steady state feed rates for the entire circuit
 *
 * @param circuit_vector Circuit vector
 * @param parameters Circuit parameters
 */
void Calculate_Circuit(Circuit &circuit, struct Circuit_Parameters parameters)
{
  // 1. Asssign initial values for F_ger and F_waste in each unit of the circuit
  for (int i = 0; i < circuit.num_units; ++i)
  {
    circuit.units[i].input.ger = parameters.input_Fger;
    circuit.units[i].input.waste = parameters.input_Fwaste;
    circuit.units[i].F = circuit.units[i].input.ger + circuit.units[i].input.waste;
  }

  // 2. Calculate the flow rates for each unit of the circuit
  for (int i = 0; i < circuit.num_units; ++i)
  {
    circuit.units[i].calculateOutputFlowRates(circuit.units[i].input);
  }

  // 3. Set F_old = F
  for (int i = 0; i < circuit.num_units; ++i)
  {
    circuit.units[i].F_old = circuit.units[i].F;
  }

  // Total old feed rates
  std::vector<double> F_old(circuit.num_units, 0.0);
  for (int i = 0; i < circuit.num_units; ++i)
  {
    F_old[i] = circuit.units[i].F_old;
  }

  // New Feed rates
  std::vector<double> F(circuit.num_units, 0.0);

  // Boolean to check if the circuit has reached steady state
  bool terminate = false;

  circuit.it = 0;
  while (!terminate)
  {
    // 4. Calculate the input flow rates for each unit of the circuit
    for (int i = 0; i < circuit.num_units; ++i)
    {
      circuit.units[i].F_old = circuit.units[i].F;
      circuit.units[i].input.ger = 0;
      circuit.units[i].input.waste = 0;
    }

    for (int i = 0; i < circuit.num_units; ++i)
    {
      // First the concentration stream
      if (circuit.units[i].conc_num >= 0)
      {
        circuit.units[circuit.units[i].conc_num].input += circuit.units[i].conc;
      }

      // Then the tails stream
      if (circuit.units[i].tails_num >= 0)
      {
        circuit.units[circuit.units[i].tails_num].input += circuit.units[i].tail;
      }

      if (i == circuit.feeder)
      {
        circuit.units[i].input.ger += parameters.input_Fger;
        circuit.units[i].input.waste += parameters.input_Fwaste;
      }
    }

    // 5. Calculate the output flow rates for each unit of the circuit
    for (int i = 0; i < circuit.num_units; ++i)
    {
      circuit.units[i].calculateOutputFlowRates(circuit.units[i].input);
      F[i] = circuit.units[i].input.ger + circuit.units[i].input.waste;
    }

    // 6. Check for convergence between old and new feed rates of the circuit
    bool converged = true;
    for (int i = 0; i < circuit.num_units; ++i)
    {
      if (fabs(F_old[i] - F[i]) > parameters.tolerance)
      {
        // Circuit not converged yet. Can break and continue
        converged = false;
        break;
      }
    }

    if (converged)
    {
      terminate = true;
    }
    else
    {
      terminate = false;
    }

    ++circuit.it;
    F_old = F;
    // F.resize(circuit.num_units, 0.0);

    if (circuit.it > parameters.max_iterations)
    {
      return;
    }
  }
}

/**
 * @brief Evaluate the circuit with given parameters
 *
 * @param circuit_vector Circuit vector
 * @param parameters Circuit parameters
 * @return double Objective function value
 */
double Evaluate_Circuit(std::vector<int> circuit_vector, struct Circuit_Parameters parameters)
{
  // 1. Create a circuit object
  Circuit circuit(circuit_vector);

  // 2. Calculate the circuit
  Calculate_Circuit(circuit, parameters);

  double objective = parameters.get_money * circuit.units[circuit.num_units - 2].input.ger - parameters.lose_money * circuit.units[circuit.num_units - 2].input.waste;

  return objective;
}

/**
 * @brief Calculate the circuit with default parameters
 *
 * @param circuit_vector Circuit vector
 */
void Calculate_Circuit(Circuit &circuit)
{
  struct Circuit_Parameters default_circuit_parameters;
  Calculate_Circuit(circuit, default_circuit_parameters);
}

/**
 * @brief Evaluate the circuit with default parameters
 *
 * @param circuit_vector Circuit vector
 * @return double Objective function value
 */
double Evaluate_Circuit(std::vector<int> circuit_vector)
{
  struct Circuit_Parameters default_circuit_parameters;
  return Evaluate_Circuit(circuit_vector, default_circuit_parameters);
};
