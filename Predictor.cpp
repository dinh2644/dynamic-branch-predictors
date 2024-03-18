#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>

#include "Predictor.h"

using namespace std;

unsigned long long totalBranches;
// for Branch Target Buffer
unsigned long long totalAttemptedPred;

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
    totalBranches++;
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

  for (unsigned long long i = 0; i < branchesVect.size(); i++)
  {
    if (branchesVect.at(i).getBehavior() == 1)
    {
      takenCount++;
    }
  }
  result.push_back(takenCount);
}

// Always not taken
void Predictor::alwaysNotTaken()
{
  unsigned long long notTakenCount = 0;

  for (int i = 0; i < branchesVect.size(); i++)
  {
    if (branchesVect.at(i).getBehavior() == 0)
    {
      notTakenCount++;
    }
  }
  result.push_back(notTakenCount);
}

// Bimodal Predictor with a single bit of history
void Predictor::bimodalSingleBit(unsigned int entries)
{
  unsigned long long correctPred = 0;

  // Create table, initially starting w/ Taken
  vector<int> predictorTable(entries, 1);

  for (unsigned long long i = 0; i < branchesVect.size(); i++)
  {
    // To hold the prediction of the current program addr
    unsigned int index = branchesVect.at(i).getProgramAddr() % entries;

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
  }

  result.push_back(correctPred);
}

// Bimodal Predictor with 2-bit saturating counters
void Predictor::bimodal2Bit(unsigned int entries)
{
  unsigned long long correctPred = 0;

  // Create table, initially starting w/ Strongly taken
  vector<int> predictorTable(entries, 3);

  for (int i = 0; i < branchesVect.size(); i++)
  {
    // Get index of curr addr by masking table size
    unsigned int index = branchesVect.at(i).getProgramAddr() % entries;
    // Current line's behavior, 0 or 1
    unsigned int currentBehavior = branchesVect.at(i).getBehavior();
    // Current line's prediction, 0,1,2,3 (00,01,10,11)
    int indexPrediction = predictorTable[index];

    // Updating prediction table
    if (currentBehavior == 1)
    {
      // correct for 3 & 2, incorrect for 1 & 0
      if (indexPrediction == 3)
      {
        correctPred++;
      }
      else if (indexPrediction == 2)
      {
        correctPred++;
        predictorTable[index]++;
      }
      else if (indexPrediction == 1)
      {
        predictorTable[index]++;
      }
      else if (indexPrediction == 0)
      {
        predictorTable[index]++;
      }
    }

    if (currentBehavior == 0)
    {
      // correct for 1 & 0, incorrect for 3 & 2
      if (indexPrediction == 3)
      {
        predictorTable[index]--;
      }
      else if (indexPrediction == 2)
      {
        predictorTable[index]--;
      }
      else if (indexPrediction == 1)
      {
        correctPred++;
        predictorTable[index]--;
      }
      else if (indexPrediction == 0)
      {
        correctPred++;
      }
    }
  }

  result.push_back(correctPred);
}

void Predictor::gshare(unsigned int entries, unsigned int ghrBitSize)
{
  unsigned long long correctPred = 0;
  unsigned long long GHR = 0;

  // Create table, initially starting w/ Strongly taken
  vector<int> predictorTable(entries, 3);

  for (int i = 0; i < branchesVect.size(); i++)
  {
    // Get index, behavior of current branch (line)
    unsigned int index = (branchesVect.at(i).getProgramAddr() ^ GHR) % entries;
    unsigned int currentBehavior = branchesVect.at(i).getBehavior();
    // Get prediction(s) of that branch, 0,1,2,3 (00,01,10,11)
    int indexPrediction = predictorTable[index];

    // Updating prediction table
    if (currentBehavior == 1)
    {
      // correct for 3 & 2, incorrect for 1 & 0
      if (indexPrediction == 3)
      {
        correctPred++;
      }
      else if (indexPrediction == 2)
      {
        correctPred++;
        predictorTable[index]++;
      }
      else if (indexPrediction == 1)
      {
        predictorTable[index]++;
      }
      else if (indexPrediction == 0)
      {
        predictorTable[index]++;
      }
    }

    if (currentBehavior == 0)
    {
      // correct for 1 & 0, incorrect for 3 & 2
      if (indexPrediction == 3)
      {
        predictorTable[index]--;
      }
      else if (indexPrediction == 2)
      {
        predictorTable[index]--;
      }
      else if (indexPrediction == 1)
      {
        correctPred++;
        predictorTable[index]--;
      }
      else if (indexPrediction == 0)
      {
        correctPred++;
      }
    }

    // Update GHR
    GHR <<= 1;
    GHR |= currentBehavior;
    // Mask by x size
    GHR &= (1 << ghrBitSize) - 1;
  }

  result.push_back(correctPred);
}

void Predictor::tournament(unsigned int bimodalEntries, unsigned int gshareEntries, unsigned int selectorEntries, unsigned int ghrBitSize)
{
  unsigned long long correctPred = 0;
  unsigned long long GHR = 0;

  // Create table, initially starting w/ Strongly taken
  vector<int> bimodalTable(bimodalEntries, 3);
  vector<int> gshareTable(gshareEntries, 3);
  // initially starting with prefer Gshare (00)
  vector<int> selectorTable(selectorEntries, 0);

  for (int i = 0; i < branchesVect.size(); i++)
  {
    // Get index, behavior of current branch (line)
    unsigned int bimodalIndex = branchesVect.at(i).getProgramAddr() % bimodalEntries;
    unsigned int gshareIndex = (branchesVect.at(i).getProgramAddr() ^ GHR) % gshareEntries;
    unsigned int currentBehavior = branchesVect.at(i).getBehavior();
    // Get prediction(s) of that branch, 0,1,2,3 (00,01,10,11)
    int bimodalPrediction = bimodalTable[bimodalIndex];
    int gsharePrediction = gshareTable[gshareIndex];
    int selectorPrediction = selectorTable[bimodalIndex]; // same index as bimodal

    // Update bimodal & gshare table as usual
    if (currentBehavior == 1)
    {
      // Bimodal
      if (bimodalPrediction == 3)
      {
        // if at max 11 then do nothing
      }
      else if (bimodalPrediction == 2)
      {
        bimodalTable[bimodalIndex]++;
      }
      else if (bimodalPrediction == 1)
      {
        bimodalTable[bimodalIndex]++;
      }
      else if (bimodalPrediction == 0)
      {
        bimodalTable[bimodalIndex]++;
      }
      // Gshare
      if (gsharePrediction == 3)
      {
        // if at max 11 then do nothing
      }
      else if (gsharePrediction == 2)
      {
        gshareTable[gshareIndex]++;
      }
      else if (gsharePrediction == 1)
      {
        gshareTable[gshareIndex]++;
      }
      else if (gsharePrediction == 0)
      {
        gshareTable[gshareIndex]++;
      }
    }

    if (currentBehavior == 0)
    {
      // bimodal
      if (bimodalPrediction == 3)
      {
        bimodalTable[bimodalIndex]--;
      }
      else if (bimodalPrediction == 2)
      {
        bimodalTable[bimodalIndex]--;
      }
      else if (bimodalPrediction == 1)
      {
        bimodalTable[bimodalIndex]--;
      }
      else if (bimodalPrediction == 0)
      {
        // if at min 00 then do nothing
      }

      // Gshare
      if (gsharePrediction == 3)
      {
        gshareTable[gshareIndex]--;
      }
      else if (gsharePrediction == 2)
      {
        gshareTable[gshareIndex]--;
      }
      else if (gsharePrediction == 1)
      {
        gshareTable[gshareIndex]--;
      }
      else if (gsharePrediction == 0)
      {
        // if at 00 then do nothing
      }
    }

    // Update GHR
    GHR <<= 1;
    GHR |= currentBehavior;
    // Mask by x size
    GHR &= (1 << ghrBitSize) - 1;

    // Tournament predicting
    // 00 - gshare
    // 01 - gshare
    // 10 - bimodal
    // 11 - bimodal

    // Get the 2nd LSB (all we need for comparison)
    int bimodalSecondLSB = (bimodalPrediction >> 1) & 1;
    int gshareSecondLSB = (gsharePrediction >> 1) & 1;
    int selectorSecondLSB = (selectorPrediction >> 1) & 1;

    // If both predictions' LSB are the same AND matches actual behavior
    if (bimodalSecondLSB == gshareSecondLSB)
    {
      if (currentBehavior == bimodalSecondLSB || currentBehavior == gshareSecondLSB)
      {
        correctPred++;
      }
    }
    else
    {
      // If selector's 2nd LSB == 1, CHOOSE BIMODAL
      if (selectorSecondLSB == 1)
      {
        if (bimodalSecondLSB == 1 == currentBehavior == 1)
        {
          correctPred++;
          // Update towards 11 if not already there
          if (selectorPrediction != 3)
          {
            selectorTable[bimodalIndex]++;
          }
        }
        else
        {
          // If bimodal is wrong, decrement to gshare
          if (selectorPrediction != 0)
          {
            selectorTable[bimodalIndex]--;
          }
        }
      }
      // If selector's 2nd LSB == 0, CHOOSE GSHARE
      if (selectorSecondLSB == 0)
      {
        if (gshareSecondLSB == 0 == currentBehavior == 0)
        {
          correctPred++;
          // Update towards 00 if not already there
          if (selectorPrediction != 0)
          {
            selectorTable[bimodalIndex]--;
          }
        }
        else
        {
          // If gshare is wrong, increment to bimodal
          if (selectorPrediction != 3)
          {
            selectorTable[bimodalIndex]++;
          }
        }
      }
    }
  }

  result.push_back(correctPred);
}

void Predictor::branchTargetBuffer(unsigned int entries)
{
  unsigned long long correctPred = 0;
  vector<int> bimodalTable(entries, 1);
  // each entry contains the predicted branch target but we did not predict anything yet so initiliaze to -1 for now
  vector<int> btbTable(entries, -1);

  for (unsigned int i = 0; i < branchesVect.size(); i++)
  {

    unsigned int index = branchesVect.at(i).getProgramAddr() % entries;
    // We dont know yet, so set to 0
    int btbTargetAddressPrediction = 0;
    int bimodalPrediction = bimodalTable[index];
    int actualTarget = branchesVect.at(i).getTargetAddr();
    int actualBehavior = branchesVect.at(i).getBehavior();

    // If a prediction is “Taken”, then the predicted target address is read from the BTB table
    if (bimodalPrediction == 1)
    {
      btbTargetAddressPrediction = btbTable[index];
      totalAttemptedPred++;
    }

    // If actual behavior is Taken, then update BTB with correct branch target
    if (actualBehavior == 1)
    {
      btbTable[index] = actualTarget;
    }

    // Count a BTB prediction to be correct if the predicted target equals the actual target, regardless of the actual branch outcome
    if (btbTargetAddressPrediction == actualTarget)
    {
      correctPred++;
    }

    // Perform bimodal prediction check as usual
    if (bimodalPrediction != actualBehavior)
    {
      bimodalTable[index] = actualBehavior;
    }
  }

  result.push_back(correctPred);
}

void Predictor::writeFile(string fileName)
{
  ofstream file(fileName);

  for (int i = 0; i < result.size(); i++)
  {
    if (i >= 2 && i <= 7)
    {
      file << result.at(i) << "," << totalBranches << "; ";
    }
    else if (i >= 9 && i <= 14)
    {
      file << result.at(i) << "," << totalBranches << "; ";
    }
    else if (i >= 16 && i <= 23)
    {
      file << result.at(i) << "," << totalBranches << "; ";
    }
    else
    {
      // if the result value equals the last one, print it with total attempted predictions
      if (result.at(i) == result.back())
      {
        file << result.at(i) << "," << totalAttemptedPred << ";" << endl;
        return;
      }
      else
      {
        file << result.at(i) << "," << totalBranches << ";" << endl;
      }
    }
  }

  file.close();
}