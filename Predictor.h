#pragma once
#include <iostream>
#include <iostream>
#include <string>
#include <vector>

#include "Branch.h"

using namespace std;

class Predictor{
    public:
        Predictor() {}
        void readFile(string fileName);
        void alwaysTaken();
        void alwaysNotTaken();

    private:
        vector<Branch> branchLine;
};