#include "CCircuit.h"

bool testValidity()
{
    // std::vector<int> test = {0, 3, 1, 0, 2, 1, 4};
    // std::vector<int> test = {0, 2, 1, 0, 4, 3, 0};
    // std::vector<int> test = {0, 2, 1, 3, 2, 3, 4};
    // std::vector<int> test = {2, 10, 6, 8, 4, 8, 7, 8, 1, 8, 11, 6, 2, 0, 8, 8, 3, 6, 9, 6, 5};
    // std::vector<int> test = {0, 6, 7, 2, 6, 10, 1, 1, 8, 6, 5, 6, 9, 1, 3, 6, 4, 1, 0, 6, 8};
    std::vector<int> test = {0, 8, 3, 7, 2, 7, 4, 8, 5, 10, 7, 8, 9, 8, 11, 4, 8, 7, 1, 8, 6};
    Circuit c(test);
    c.toFile();
    bool check = Circuit::Check_Validity(test);
    return check;
}

int main()
{
    // auto val = testValidity();
    // std::cout << val << std::endl;
    // Case 1
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
    if (!check1 && !check2 && !check3 && !check4)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    return 0;
}