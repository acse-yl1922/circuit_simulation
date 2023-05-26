#pragma once

/**
 * @file CUnit.h
 * @author acse-yl1922, acse-sm222
 * @brief Unit class
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <cstdlib>

/**
 * @class Stream
 * @brief A Stream class to store the stream information
 * @author acse-sm222
 */
class Stream
{
public:
    Stream() /**< Default constructor*/
    {
        ger = 0;
        waste = 0;
    };
    Stream(double GER, double WASTE) : ger(GER), waste(WASTE){}; /**< Constructor taking the GER and WASTE*/
    double ger;                                                  /**< Germanium flow rate*/
    double waste;                                                /**< Waste flow rate*/
    Stream operator+(const Stream &s)                            /**< Overload the + operator*/
    {
        Stream stream;
        stream.ger = this->ger + s.ger;
        stream.waste = this->waste + s.waste;
        return stream;
    }

    void operator+=(const Stream &s) /**< Overload the += operator*/
    {
        this->ger += s.ger;
        this->waste += s.waste;
    }

    ~Stream() {} /**< Destructor*/
};

/**
 * @class CUnit
 * @brief A Unit class to store the unit information and processing
 * @author acse-yl1922, acse-sm222
 *
 */
class CUnit
{
public:
    CUnit();                                    /**< Default constructor*/
    CUnit(int id, int conc_num, int tails_num); /**< Constructor taking the id, conc_num and tails_num*/
    int conc_num;                               /**< index of the unit to which this unit’s concentrate stream is connected*/
    int tails_num;                              /**< index of the unit to which this unit’s tails stream is connected*/

    Stream input;    /**< Input stream*/
    Stream conc;     /**< Concentrate stream*/
    Stream tail;     /**< Tails stream*/
    double F, F_old; /**< Feed values*/

    void calculateOutputFlowRates(const Stream &INP); /**< Function to calculate the output flow rates*/

    ~CUnit() {} /**< Destructor*/

private:
    int id;                  /**< Unit id*/
    double k_ger = 0.005;    /**< Germanium recovery rate*/
    double k_waste = 0.0005; /**< Waste recovery rate*/
    int rho = 3000;          /**< Density of the mineral*/
    double phi = 0.1;        /**< Percentage of solid content in the stream*/
    int V = 10;              /**< Volume of the unit*/
};
