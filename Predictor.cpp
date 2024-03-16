#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Predictor.h"

using namespace std;

void Predictor::readFile(string fileName)
{
  // Temporary variables
  unsigned long long addr;
  string behavior, line;
  unsigned long long target;

  // Open file for reading
  ifstream infile(fileName);
  if (!infile)
  {
    cout << "No file found!" << endl;
    exit(1);
  }

  // The following loop will read a line at a time
  while (getline(infile, line))
  {
    // Now we have to parse the line into it's two pieces
    stringstream s(line);
    s >> std::hex >> addr >> behavior >> std::hex >> target;
    // Now we can output it
    if (behavior == "T")
    {
      Branch branchLine(addr, 1, target);
      branchesVect.push_back(branchLine);
    }
    else if (behavior == "NT")
    {
      Branch branchLine(addr, 0, target);
      branchesVect.push_back(branchLine);
    }
    else
    {
      cout << "Unidentifiable behavior" << endl;
      exit(1);
    }
  }
}

// Always taken
void Predictor::alwaysTaken()
{
  unsigned int takenCount = 0;
  unsigned int total = 0;

  for (int i = 0; i < branchesVect.size(); i++)
  {
    if (branchesVect.at(i).getBehavior() == 1)
    {
      takenCount++;
    }
    total++;
  }
  cout << takenCount << "," << total << endl;
}

// Always not taken
void Predictor::alwaysNotTaken()
{
  unsigned notTakenCount = 0;
  unsigned int total = 0;

  for (int i = 0; i < branchesVect.size(); i++)
  {
    if (branchesVect.at(i).getBehavior() == 0)
    {
      notTakenCount++;
    }
    total++;
  }
  cout << notTakenCount << "," << total << endl;
}

// Bimodal Predictor with a single bit of history
void Predictor::bimodalSingleBit(unsigned int tableSize)
{
  unsigned int correctCount = 0;
  unsigned int totalBranches = 0;

  // Create table
  vector<int> predCounter(tableSize, 1);

  for (int i = 0; i < branchesVect.size(); i++)
  {
    // To hold the prediction of the current program addr
    unsigned int index = branchesVect.at(i).getProgramAddr() % tableSize;
    // If current behavior equals to addr's prediction
    if (branchesVect.at(i).getBehavior() == predCounter.at(index))
    {
      correctCount++;
    }
    else
    {
      // Update the prediction counter to current behavior
      predCounter.at(index) = branchesVect.at(i).getBehavior();
    }
    totalBranches++;
  }

  cout << correctCount << "," << totalBranches << endl;
}

// Bimodal Predictor with a single bit of history
void Predictor::bimodal2Bit(unsigned int tableSize)
{
  // implement code
}