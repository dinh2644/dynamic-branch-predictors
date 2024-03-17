#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>

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
  unsigned long long takenCount = 0;
  unsigned long long totalBranches = 0;

  for (unsigned long long i = 0; i < branchesVect.size(); i++)
  {
    if (branchesVect.at(i).getBehavior() == 1)
    {
      takenCount++;
    }
    totalBranches++;
  }
  cout << takenCount << "," << totalBranches << endl;
}

// Always not taken
void Predictor::alwaysNotTaken()
{
  unsigned long long notTakenCount = 0;
  unsigned long long totalBranches = 0;

  for (int i = 0; i < branchesVect.size(); i++)
  {
    if (branchesVect.at(i).getBehavior() == 0)
    {
      notTakenCount++;
    }
    totalBranches++;
  }
  cout << notTakenCount << "," << totalBranches << endl;
}

// Bimodal Predictor with a single bit of history
void Predictor::bimodalSingleBit(unsigned int tableSize)
{
  unsigned long long correctPred = 0;
  unsigned long long totalBranches = 0;

  // Create table, initially starting w/ Taken
  vector<int> predictorTable(tableSize, 1);

  for (unsigned long long i = 0; i < branchesVect.size(); i++)
  {
    // To hold the prediction of the current program addr
    unsigned int index = branchesVect.at(i).getProgramAddr() % tableSize;

    // If current behavior equals to addr's prediction
    if (branchesVect.at(i).getBehavior() == predictorTable.at(index))
    {
      correctPred++;
    }
    else
    {
      // Update the prediction counter to current behavior
      predictorTable.at(index) = branchesVect.at(i).getBehavior();
    }
    totalBranches++;
  }

  cout << correctPred << "," << totalBranches << endl;
}

// Bimodal Predictor with 2-bit saturating counters
void Predictor::bimodal2Bit(unsigned int tableSize)
{
  unsigned long long correctPred = 0;
  unsigned long long totalBranches = 0;

  // Create table, initially starting w/ Strongly taken
  vector<int> predictorTable(tableSize, 3);

  for (int i = 0; i < branchesVect.size(); i++)
  {
    // Index for current program addr
    unsigned int index = branchesVect.at(i).getProgramAddr() % tableSize;
    // Current line's behavior, 0 or 1
    unsigned int currentBehavior = branchesVect.at(i).getBehavior();
    // Current line's prediction, 0,1,2,3
    int currentState = predictorTable[index];

    // Updating prediction table
    if (currentBehavior == 1)
    {
      // correct for 3 & 2, incorrect for 1 & 0
      if (currentState == 3)
      {
        correctPred++;
      }
      else if (currentState == 2)
      {
        correctPred++;
        predictorTable[index]++;
      }
      else if (currentState == 1)
      {
        predictorTable[index]++;
      }
      else if (currentState == 0)
      {
        predictorTable[index]++;
      }
    }

    if (currentBehavior == 0)
    {
      // correct for 1 & 0, incorrect for 3 & 2
      if (currentState == 3)
      {
        predictorTable[index]--;
      }
      else if (currentState == 2)
      {
        predictorTable[index]--;
      }
      else if (currentState == 1)
      {
        correctPred++;
        predictorTable[index]--;
      }
      else if (currentState == 0)
      {
        correctPred++;
      }
    }

    totalBranches++;
  }

  cout << correctPred << "," << totalBranches << endl;
}

void Predictor::gshare(int ghrSize, unsigned int tableSize)
{
  unsigned long long correctPred = 0;
  unsigned long long totalBranches = 0;
  unsigned long long GHR = 0;

  // Create table, initially starting w/ Strongly taken
  vector<int> predictorTable(tableSize, 3);

  for (int i = 0; i < branchesVect.size(); i++)
  {
    // Index for current program addr
    unsigned int index = (branchesVect.at(i).getProgramAddr() ^ GHR) % tableSize;
    // Current line's behavior, 0 or 1
    unsigned int currentBehavior = branchesVect.at(i).getBehavior();
    // Current line's prediction, 0,1,2,3
    int currentState = predictorTable[index];

    // Updating prediction table
    if (currentBehavior == 1)
    {
      // correct for 3 & 2, incorrect for 1 & 0
      if (currentState == 3)
      {
        correctPred++;
      }
      else if (currentState == 2)
      {
        correctPred++;
        predictorTable[index]++;
      }
      else if (currentState == 1)
      {
        predictorTable[index]++;
      }
      else if (currentState == 0)
      {
        predictorTable[index]++;
      }
    }

    if (currentBehavior == 0)
    {
      // correct for 1 & 0, incorrect for 3 & 2
      if (currentState == 3)
      {
        predictorTable[index]--;
      }
      else if (currentState == 2)
      {
        predictorTable[index]--;
      }
      else if (currentState == 1)
      {
        correctPred++;
        predictorTable[index]--;
      }
      else if (currentState == 0)
      {
        correctPred++;
      }
    }

    // Update GHR
    GHR <<= 1;
    GHR |= currentBehavior;
    // Mask by x size
    GHR &= (1 << ghrSize) - 1;

    totalBranches++;
  }

  cout << correctPred << "," << totalBranches << endl;
}