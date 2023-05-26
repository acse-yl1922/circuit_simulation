#include <vector>

/**
 * @file CCircuit.cpp
 * @author acse-yl1922, acse-sm222
 * @brief Circuit class implementation
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <CCircuit.h>

class Units
{
public:
  Units(){};
  Units(int conc_num, int tails_num) : conc_num(conc_num), tails_num(tails_num){};
  int conc_num;
  int tails_num;
  bool marked;
};

void markunits(std::vector<Units> &units, int unit_num)
{
  if (units[unit_num].marked == true)
  {
    return;
  }
  else
  {
    units[unit_num].marked = true;
  }

  if (units[unit_num].conc_num < units.size())
  {
    markunits(units, units[unit_num].conc_num);
  }
  else
  {
    return;
  }

  if (units[unit_num].tails_num < units.size())
  {
    markunits(units, units[unit_num].tails_num);
  }
  else
  {
    return;
  }
}

/**
 * @brief Construct a new Circuit:: Circuit object
 * @details This constructor takes the circuit vector and initializes the circuit
 * The circuit vector is of the form:
 * Feeder, C0, T0, C1, T1, C2, T2, ... , Cn, Tn
 * Two extra units are added to the circuit, one for the concentrate and one for the tails
 * Each unit is stored in the units vector
 * @param circuit_vector
 */

Circuit::Circuit(std::vector<int> circuit_vector)
{
  this->num_units = (circuit_vector.size() - 1) / 2 + 2;
  this->units.resize(this->num_units);
  this->circuit_vector = circuit_vector;
  this->feeder = circuit_vector[0];
  this->it = 0;

  // Initialize the units with unit ID and destinations
  for (int i = 0; i < this->num_units - 2; ++i)
  {
    this->units[i] = CUnit(i, circuit_vector[2 * i + 1], circuit_vector[2 * i + 2]);
  }
}

/**
 * @brief To write the circuit information to a file
 * @details This function writes the circuit information to a file. It creates a DirectedGraph object for the concentrate and tails
 * and writes the graph to a file
 */
void Circuit::toFile()
{
  DirectedGraph Conc(num_units);
  DirectedGraph Tail(num_units);
  for (int i = 0; i < num_units - 2; ++i)
  {
    Conc.addEdge(i, circuit_vector[2 * i + 1]);
    Tail.addEdge(i, circuit_vector[2 * i + 2]);
  }

  Conc.addEdge(num_units - 2, num_units + 1);
  Conc.addEdge(num_units - 1, num_units + 1);
  Tail.addEdge(num_units - 2, num_units + 1);
  Tail.addEdge(num_units - 1, num_units + 1);

  Circuit_Parameters params;
  Calculate_Circuit(*this, params);

  std::fstream ConcFile;
  std::fstream TailFile;
  std::fstream DataFile;
  ConcFile.open("C.txt", std::ios::out);
  TailFile.open("T.txt", std::ios::out);
  DataFile.open("D.txt", std::ios::out);

  ConcFile << Conc;
  ConcFile << feeder << std::endl;
  TailFile << Tail;
  TailFile << feeder << std::endl;

  for (int i = 0; i < num_units - 2; ++i)
  {
    DataFile << i << " " << units[i].conc.ger << " " << units[i].conc.waste << " " << units[i].tail.ger << " " << units[i].tail.waste << std::endl;
  }

  ConcFile.close();
  TailFile.close();
  DataFile.close();
}

/**
 * @brief Destroy the Circuit object
 *
 */
Circuit::~Circuit()
{
}

/**
 * @brief Function to check the validity of the circuit vector
 *
 * @param circuit_vector
 * @return true
 * @return false
 * TODO: Add more validity checks
 */
bool Circuit::Check_Validity(const std::vector<int> &circuit_vector)
{
  if (circuit_vector.size() % 2 != 1)
  {
    return false;
  }
  else
  {
    int feeder = circuit_vector[0];
    int num_units = (circuit_vector.size() - 1) / 2;

    // 0. No units conc should go directly to tails and vice versa
    for (int i = 0; i < num_units; ++i)
    {
      if (circuit_vector[2 * i + 1] == num_units + 1 || circuit_vector[2 * i + 2] == num_units)
      {
        return false;
      }
    }

    // 1. No self loops
    for (int i = 0; i < num_units; ++i)
    {
      if (circuit_vector[2 * i + 1] == i || circuit_vector[2 * i + 2] == i)
      {
        return false;
      }
    }

    // 2. Talilings and Concentrations don't have same destination
    for (int i = 0; i < num_units; ++i)
    {
      if (circuit_vector[2 * i + 1] == circuit_vector[2 * i + 2])
      {
        return false;
      }
    }

    // 3. Everynode has a feed - Each node must lie on atleast one of the graphs from the feeder
    std::vector<int> hasfeed(num_units + 2, 0);
    hasfeed[feeder] += 1;
    for (int i = 0; i < num_units; ++i)
    {
      hasfeed[circuit_vector[2 * i + 1]] += 1;
      hasfeed[circuit_vector[2 * i + 2]] += 1;
    }

    for (int i = 0; i < num_units + 2; ++i)
    {
      if (hasfeed[i] == 0)
      {
        return false;
      }
    }

    // 4. Every unit has a route forward to leaf nodes on BOTH graphs
    for (int i = 0; i < num_units; ++i)
    {
      std::vector<Units> myunits(num_units + 2);
      for (int j = 0; j < num_units; ++j)
      {
        myunits[j].conc_num = circuit_vector[2 * j + 1];
        myunits[j].tails_num = circuit_vector[2 * j + 2];
      }
      myunits[num_units].conc_num = 1e5;
      myunits[num_units + 1].tails_num = 1e5;
      markunits(myunits, i);
      if (myunits[num_units].marked == false || myunits[num_units + 1].marked == false)
      {
        return false;
      }
    }
    return true;
  }
}