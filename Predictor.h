#pragma once
#include <iostream>
#include <iostream>
#include <string>
#include <vector>

#include "Branch.h"

using namespace std;

class Predictor
{
public:
    void readFile(string fileName);
    void alwaysTaken();
    void alwaysNotTaken();
    void bimodalSingleBit(unsigned int entries);
    void bimodal2Bit(unsigned int entries);
    void gshare(unsigned int entries, unsigned int ghrBitSize);
    void tournament(unsigned int bimodalEntries, unsigned int gshareEntries, unsigned int tournamentEntries, unsigned int ghrBitSize);
    void writeFile(string fileName);

private:
    vector<Branch> branchesVect;
    vector<unsigned long long> result;
};