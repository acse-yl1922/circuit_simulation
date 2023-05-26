#include <cmath>
#include <iostream>
#include <cassert>
#include "CUnit.h"

int main(int argc, char *argv[])
{

    CUnit test_unit;

    test_unit.input.ger = 20;
    test_unit.input.waste = 80;

    test_unit.calculateOutputFlowRates(test_unit.input);

    double eps = 1e-3;

    bool check1 = fabs(test_unit.conc.ger - 2.6087) < eps;
    bool check2 = fabs(test_unit.conc.waste - 1.18227) < eps;
    bool check3 = fabs(test_unit.tail.ger - 17.3913) < eps;
    bool check4 = fabs(test_unit.tail.waste - 78.8177) < eps;
    if (check1 && check2 && check3 && check4)
    {
        return 0;
    }
    else
    {
        return 1;
    }

    return 0;
}