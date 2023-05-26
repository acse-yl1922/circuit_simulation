/**
 * @file CUnit.cpp
 * @author your name (you@domain.com)
 * @brief CUnit class implementation
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "CUnit.h"

/**
 * @brief Construct a new CUnit object
 *
 *
 */

CUnit::CUnit()
{
    this->id = 0;
    this->conc_num = -1;
    this->tails_num = -1;
}

/**
 * @brief Construct a new CUnit object
 *
 * @param id Unit ID
 * @param conc_num Concentrate stream number
 * @param tails_num Tails stream number
 */
CUnit::CUnit(int id, int conc_num, int tails_num)
{
    this->id = id;
    this->conc_num = conc_num;
    this->tails_num = tails_num;
}

/**
 * @brief Calculate the output flow rates
 *
 * @param INP Input stream
 *
 */
void CUnit::calculateOutputFlowRates(const Stream &INP)
{

    // Calculate Tau
    double sumFlowRate = INP.ger + INP.waste;
    if (std::abs(sumFlowRate) < 1e-10)
    {
        sumFlowRate = 1e-10; // Set sumFlowRate to the minimum flow rate
    }
    double Tau = phi * V * rho / sumFlowRate;

    // Calculate R
    double R_ger = (k_ger * Tau) / (1 + (k_ger * Tau));
    double R_waste = (k_waste * Tau) / (1 + (k_waste * Tau));

    // Calculate C
    this->conc.ger = INP.ger * R_ger;
    this->conc.waste = INP.waste * R_waste;

    // Calculate T
    this->tail.ger = INP.ger * (1 - R_ger);
    this->tail.waste = INP.waste * (1 - R_waste);
}
