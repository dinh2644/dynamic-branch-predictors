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
    void bimodalSingleBit(unsigned int tableSize);
    void bimodal2Bit(unsigned int tableSize);
    // void gshare();

private:
    vector<Branch> branchesVect;
};